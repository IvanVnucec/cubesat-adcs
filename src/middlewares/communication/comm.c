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
#include "utils/error/error.h"
#include "zs040/zs040.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define MSGQUEUE_OBJECTS 10

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static osMessageQueueId_t COMM_msgQueueId = NULL;

/* Private function prototypes -----------------------------------------------*/
static void COMM_init(COMM_Status *status);
static void COMM_sendMessageOverBluetooth(const COMM_Message *const msg,
                                          COMM_Status *status);
static void COMM_getMessageFromThreadsBlocking(COMM_Message *msg, COMM_Status *status);
static void COMM_getMessageFromBluetoothNonBlocking(COMM_Message *msg,
                                                    COMM_Status *status);

/* Private user code ---------------------------------------------------------*/
void COMM_thread(void *argument)
{
    static COMM_Status status;
    static COMM_Message msg;

    COMM_init(&status);
    ERROR_assert(status == COMM_STATUS_OK);

    for (;;) {
        COMM_getMessageFromThreadsBlocking(&msg, &status);
        ERROR_assert(status == COMM_STATUS_OK);

        COMM_sendMessageOverBluetooth(&msg, &status);
        ERROR_assert(status == COMM_STATUS_OK);

        COMM_getMessageFromBluetoothNonBlocking(&msg, &status);
        if (status == COMM_STATUS_AVAILABLE_MESSAGE) {
            // echo message back
            COMM_sendMessageOverBluetooth(&msg, &status);
            ERROR_assert(status == COMM_STATUS_OK);
        }
    }
}

void COMM_sendMessage(const COMM_Message *const msg, COMM_Status *status)
{
    COMM_Status comm_status = COMM_STATUS_ERROR;
    osStatus_t os_status;

    if (msg != NULL && COMM_msgQueueId != NULL) {
        do {
            os_status = osMessageQueuePut(COMM_msgQueueId, msg, 0U, osWaitForever);
        } while (os_status == osErrorTimeout);

        if (os_status == osOK) {
            comm_status = COMM_STATUS_OK;
        }
    }

    if (status != NULL) {
        *status = comm_status;
    }
}

static void COMM_init(COMM_Status *status)
{
    *status = COMM_STATUS_ERROR;

    COMM_msgQueueId = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(COMM_Message), NULL);
    if (COMM_msgQueueId != NULL) {
        *status = COMM_STATUS_OK;
    }

    ZS040_init();
}

static void COMM_getMessageFromThreadsBlocking(COMM_Message *msg, COMM_Status *status)
{
    osStatus_t os_status;
    *status = COMM_STATUS_ERROR;

    do {
        os_status = osMessageQueueGet(COMM_msgQueueId, msg, NULL, osWaitForever);
    } while (os_status == osErrorTimeout);

    if (os_status == osOK) {
        *status = COMM_STATUS_OK;
    }
}

static void COMM_sendMessageOverBluetooth(const COMM_Message *const msg,
                                          COMM_Status *status)
{
    COMM_Status priv_status = COMM_STATUS_ERROR;

    if (msg != NULL) {
        if (msg->buffer != NULL && msg->msg_len <= COMM_MESSAGE_MAX_BUFF_LEN) {
            ZS040_send(msg->buffer, msg->msg_len);
            priv_status = COMM_STATUS_OK;
        }
    }

    if (status != NULL) {
        *status = priv_status;
    }
}

static void COMM_getMessageFromBluetoothNonBlocking(COMM_Message *msg,
                                                    COMM_Status *status)
{
    COMM_Status priv_status = COMM_STATUS_ERROR;

    if (msg != NULL) {
        if (msg->buffer != NULL) {
            // TODO: add receive code here
            priv_status = COMM_STATUS_NO_AVAILABLE_MESSAGE;
        }
    }

    if (status != NULL) {
        *status = priv_status;
    }
}