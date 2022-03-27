/**
  ******************************************************************************
  * @file           : middlewares.c
  * @brief          : Source file.
  *                   This file contains the common code of the application.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "middlewares.h"

#include "cmsis_os.h"
#include "middlewares/adcs/adcs.h"
#include "middlewares/alive/alive.h"
#include "middlewares/communication/comm.h"
#include "utils/error/error.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static osThreadId_t aliveThreadHandle             = NULL;
static const osThreadAttr_t aliveThreadAttributes = {
    .name       = "aliveThread",
    .stack_size = MIDDLEWARES_ALIVE_THREAD_STACK_SIZE,
    .priority   = (osPriority_t)osPriorityRealtime,
};

static osThreadId_t adcsThreadHandle             = NULL;
static const osThreadAttr_t adcsThreadAttributes = {
    .name       = "adcsThread",
    .stack_size = MIDDLEWARES_ADCS_THREAD_STACK_SIZE,
    .priority   = (osPriority_t)osPriorityLow,
};

static osThreadId_t commThreadHandle             = NULL;
static const osThreadAttr_t commThreadAttributes = {
    .name       = "commThread",
    .stack_size = MIDDLEWARES_COMM_THREAD_STACK_SIZE,
    .priority   = (osPriority_t)osPriorityRealtime1,
};

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void MIDDLEWARES_initThreads(void)
{
    aliveThreadHandle = osThreadNew(ALIVE_thread, NULL, &aliveThreadAttributes);
    if (aliveThreadHandle == NULL) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }

    adcsThreadHandle = osThreadNew(ADCS_thread, NULL, &adcsThreadAttributes);
    if (adcsThreadHandle == NULL) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }

    commThreadHandle = osThreadNew(COMM_thread, NULL, &commThreadAttributes);
    if (commThreadHandle == NULL) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }
}
