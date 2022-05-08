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
#include "stm32l4xx_hal.h"
#include "utils/error/error.h"

#include <string.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define COMM_UART_TX_QUEUE_LEN (10)
#define COMM_UART_RX_QUEUE_LEN (10)
#define COMM_CALLBACK_FUNCTIONS_LEN (1)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static osMessageQueueId_t COMM_uart_tx_queue = NULL;
static osMessageQueueId_t COMM_uart_rx_queue = NULL;
static uint8_t data_received[COMM_MESSAGE_MAX_BUFF_LEN];    // for uart rx
static int data_received_ptr = 0;                           // for uart rx
static int COMM_UART_tx_free = 0;                           // for uart tx
static COMM_CallbackFunction COMM_callback_functions[COMM_CALLBACK_FUNCTIONS_LEN] = {
    {"stop", osThreadExit},
};

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

            COMM_sendMessageOverBluetooth(&msg, &status);
            ERROR_assert(status == COMM_STATUS_OK);
        }

        if (COMM_haveMessages(COMM_uart_rx_queue)) {
            COMM_getMessage(&msg, COMM_uart_rx_queue, &status);
            ERROR_assert(status == COMM_STATUS_OK);

            COMM_parse(&msg, &status);
            ERROR_assert(status == COMM_STATUS_OK);
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

    for (int i = 0; i < COMM_CALLBACK_FUNCTIONS_LEN; i++) {
        if (strncmp((const char *)msg->buffer,
                    COMM_callback_functions[i].name,
                    msg->msg_len)
            == 0) {
            // call callback function
            COMM_callback_functions[i].fun_ptr();

            local_status = COMM_STATUS_OK;
            break;
        }
    }

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

    ERROR_assert(0);
}
