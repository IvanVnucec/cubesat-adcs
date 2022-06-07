/**
  ******************************************************************************
  * @file           : comm.c
  * @brief          : Source file.
  *                   This file contains the common code of the application
  *                   which gets and sends commands over bluetooth module.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "comm.h"

#include "cmsis_os2.h"
#include "mcu/uart/mcu_uart.h"
#include "middlewares/adcs/adcs.h"
#include "stm32l4xx_hal.h"
#include "utils/error/error.h"

#include <stdlib.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define COMM_UART_TX_QUEUE_LEN (10)
#define COMM_UART_RX_QUEUE_LEN (10)
#define COMM_CALLBACK_FUNCTIONS_LEN (5)

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void COMM_init(COMM_Status *status);
static void COMM_parse(const COMM_Message *msg, COMM_Status *status);
static void
    COMM_getMessage(COMM_Message *msg, osMessageQueueId_t queue, COMM_Status *status);
static int COMM_haveMessages(osMessageQueueId_t queue);
static void COMM_sendMessageOverBluetooth(const COMM_Message *const msg,
                                          COMM_Status *status);
static void COMM_UART_startReceiving(uint8_t *save_to, COMM_Status *status);
static void COMM_UART_stopReceiving(COMM_Status *status);
static void COMM_echo(const char *msg);
static void COMM_help(const char *msg);
static void COMM_setSending(const char *msg);
static void COMM_setRegulationModeAngVel(const char *msg);
static void COMM_setRegulationModeAttitude(const char *msg);

/* Private variables ---------------------------------------------------------*/
static osMessageQueueId_t COMM_uart_tx_queue = NULL;
static osMessageQueueId_t COMM_uart_rx_queue = NULL;
static uint8_t data_received[COMM_MESSAGE_MAX_BUFF_LEN];    // for uart rx
static int data_received_ptr = 0;                           // for uart rx
static int COMM_UART_tx_free = 0;                           // for uart tx
static COMM_CallbackFunction COMM_callback_functions[COMM_CALLBACK_FUNCTIONS_LEN] = {
    {"echo", COMM_echo},
    {"help", COMM_help},
    {"set_sending", COMM_setSending},
    {"set_reg_angvel", COMM_setRegulationModeAngVel},
    {"set_reg_attitude", COMM_setRegulationModeAttitude},
};
static int COMM_sending_enabled = 1;

/* Private user code ---------------------------------------------------------*/
void COMM_thread(void *argument)
{
    static COMM_Status status;
    static COMM_Message msg;

    COMM_init(&status);
    ERROR_assert(status == COMM_STATUS_OK);

    for (;;) {
        if (COMM_haveMessages(COMM_uart_tx_queue) && COMM_UART_tx_free) {
            COMM_getMessage(&msg, COMM_uart_tx_queue, &status);
            ERROR_assert(status == COMM_STATUS_OK);

            if (COMM_sending_enabled) {
                COMM_sendMessageOverBluetooth(&msg, &status);
            }
            ERROR_assert(status == COMM_STATUS_OK);
        }

        if (COMM_haveMessages(COMM_uart_rx_queue)) {
            COMM_getMessage(&msg, COMM_uart_rx_queue, &status);
            ERROR_assert(status == COMM_STATUS_OK);

            COMM_parse(&msg, &status);
            ERROR_assert(status == COMM_STATUS_OK
                         || status == COMM_STATUS_NO_CALLBACK_FUNCTION
                         || status == COMM_STATUS_PARSING_ERROR);
        }

        osThreadYield();
    }
}

void COMM_sendMessage(const COMM_Message *const msg, COMM_Status *status)
{
    COMM_Status comm_status = COMM_STATUS_ERROR;
    osStatus_t os_status;

    if (msg != NULL && COMM_uart_tx_queue != NULL) {
        do {
            os_status = osMessageQueuePut(COMM_uart_tx_queue, msg, 0U, osWaitForever);
        } while (os_status == osErrorTimeout);

        if (os_status == osOK) {
            comm_status = COMM_STATUS_OK;
        }
    }

    if (status != NULL) {
        *status = comm_status;
    }
}

static void COMM_parse(const COMM_Message *msg, COMM_Status *status)
{
    COMM_Status local_status = COMM_STATUS_NO_CALLBACK_FUNCTION;
    char fun[100];
    char arg[100];

    const char *ob = strchr((const char *)msg->buffer, '(');
    if (ob == NULL) {
        local_status = COMM_STATUS_PARSING_ERROR;
        goto exit;
    }

    const char *cb = strchr((const char *)msg->buffer, ')');
    if (cb == NULL) {
        local_status = COMM_STATUS_PARSING_ERROR;
        goto exit;
    }

    int fun_len = ob - (const char *)msg->buffer;
    int arg_len = cb - ob - 1;

    strncpy(fun, (const char *)msg->buffer, fun_len);
    fun[fun_len] = '\0';

    strncpy(arg, ob + 1, arg_len);
    arg[arg_len] = '\0';

    // call function with argument
    for (int i = 0; i < COMM_CALLBACK_FUNCTIONS_LEN; i++) {
        if (strncmp(fun, COMM_callback_functions[i].name, fun_len) == 0) {
            // call callback function
            COMM_callback_functions[i].fun_ptr(arg);
            local_status = COMM_STATUS_OK;
            break;
        }
    }

exit:
    if (status != NULL) {
        *status = local_status;
    }
}

static int COMM_haveMessages(osMessageQueueId_t queue)
{
    return osMessageQueueGetCount(queue) != 0;
}

static void COMM_init(COMM_Status *status)
{
    COMM_Status local_status = COMM_STATUS_OK;

    if (local_status == COMM_STATUS_OK) {
        COMM_uart_tx_queue =
            osMessageQueueNew(COMM_UART_TX_QUEUE_LEN, sizeof(COMM_Message), NULL);
        if (COMM_uart_tx_queue == NULL) {
            local_status = COMM_STATUS_ERROR;
        }
    }

    if (local_status == COMM_STATUS_OK) {
        COMM_uart_rx_queue =
            osMessageQueueNew(COMM_UART_RX_QUEUE_LEN, sizeof(COMM_Message), NULL);
        if (COMM_uart_rx_queue == NULL) {
            local_status = COMM_STATUS_ERROR;
        }
    }

    if (local_status == COMM_STATUS_OK) {
        // set uart tx as free
        COMM_UART_tx_free = 1;

        // enable uart rx interrupt
        data_received_ptr = 0;
        COMM_UART_startReceiving(&data_received[data_received_ptr], &local_status);
    }

    if (status != NULL) {
        *status = local_status;
    }
}

static void
    COMM_getMessage(COMM_Message *msg, osMessageQueueId_t queue, COMM_Status *status)
{
    osStatus_t os_status;
    COMM_Status local_status = COMM_STATUS_ERROR;

    os_status = osMessageQueueGet(queue, msg, NULL, 0);
    if (os_status == osOK) {
        local_status = COMM_STATUS_OK;
    }

    if (status != NULL) {
        *status = local_status;
    }
}

// TODO: use zs040 lib
static void COMM_sendMessageOverBluetooth(const COMM_Message *const msg,
                                          COMM_Status *status)
{
    COMM_Status priv_status = COMM_STATUS_ERROR;

    if (msg != NULL) {
        if (msg->buffer != NULL && msg->msg_len <= COMM_MESSAGE_MAX_BUFF_LEN) {
            COMM_UART_tx_free = 0;
            HAL_StatusTypeDef hal_status =
                HAL_UART_Transmit_IT(&huart1, (uint8_t *)msg->buffer, msg->msg_len);
            if (hal_status == HAL_OK) {
                priv_status = COMM_STATUS_OK;
            }
        }
    }

    if (status != NULL) {
        *status = priv_status;
    }
}

static void COMM_UART_startReceiving(uint8_t *save_to, COMM_Status *status)
{
    COMM_Status local_status     = COMM_STATUS_ERROR;
    HAL_StatusTypeDef hal_status = HAL_ERROR;

    hal_status = HAL_UART_Receive_IT(&huart1, save_to, 1);
    if (hal_status == HAL_OK) {
        local_status = COMM_STATUS_OK;
    }

    if (status != NULL) {
        *status = local_status;
    }
}

__attribute__((unused)) static void COMM_UART_stopReceiving(COMM_Status *status)
{
    COMM_Status local_status     = COMM_STATUS_ERROR;
    HAL_StatusTypeDef hal_status = HAL_ERROR;

    hal_status = HAL_UART_AbortReceive_IT(&huart1);
    if (hal_status == HAL_OK) {
        local_status = COMM_STATUS_OK;
    }

    if (status != NULL) {
        *status = local_status;
    }
}

static void COMM_echo(const char *msg)
{
    COMM_Message msg_to_send;
    COMM_Status status;

    int msg_len = strlen(msg);
    strncpy((char *)msg_to_send.buffer, msg, msg_len);
    msg_to_send.msg_len = msg_len;

    COMM_sendMessage(&msg_to_send, &status);
}

static void COMM_help(const char *msg)
{
    COMM_Message msg_to_send;
    COMM_Status status;
    int msg_len;

    for (int i = 0; i < COMM_CALLBACK_FUNCTIONS_LEN; i++) {
        const char *fun_name = COMM_callback_functions[i].name;

        msg_len = strlen(fun_name);
        strncpy((char *)msg_to_send.buffer, fun_name, msg_len);
        // add newline
        msg_to_send.buffer[msg_len] = '\n';
        msg_len += 1;
        msg_to_send.buffer[msg_len] = '\0';

        msg_to_send.msg_len = msg_len;

        COMM_sendMessage(&msg_to_send, &status);
    }
}

static void COMM_setSending(const char *msg)
{
    if (msg[0] == '1')
        COMM_sending_enabled = 1;
    else
        COMM_sending_enabled = 0;
}

static void COMM_setRegulationModeAngVel(const char *msg)
{
    ADCS_setRegulationMode(ADCS_REGULATIOM_MODE_ANGULAR_VELOCITY);
}

static void COMM_setRegulationModeAttitude(const char *msg)
{
    ADCS_setRegulationMode(ADCS_REGULATION_MODE_ATTITUDE);
}

// TODO: move UART code from comm to uart (bsp or mcu)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle->Instance == huart1.Instance) {
        __disable_irq();
        if (data_received[data_received_ptr] == '\n') {
            COMM_Message msg;

            msg.msg_len = data_received_ptr;
            strncpy((char *)msg.buffer, (const char *)data_received, msg.msg_len);

            osStatus_t os_status = osMessageQueuePut(COMM_uart_rx_queue, &msg, 0, 0);
            ERROR_assert(os_status == osOK);

            data_received_ptr = 0;
        } else {
            data_received_ptr++;
            if (data_received_ptr >= COMM_MESSAGE_MAX_BUFF_LEN) {
                // TODO: signal to user that the buffered chars were cleared
                data_received_ptr = 0;
            }
        }

        // enable receiving again
        COMM_Status comm_status = COMM_STATUS_ERROR;
        COMM_UART_startReceiving(&data_received[data_received_ptr], &comm_status);
        ERROR_assert(comm_status == COMM_STATUS_OK);

        __enable_irq();
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle->Instance == huart1.Instance) {
        COMM_UART_tx_free = 1;
    }
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
    // called after HAL_UART_AbortReceive_IT
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *handle)
{
    uint32_t error_code = handle->ErrorCode;
    (void)error_code;
}
