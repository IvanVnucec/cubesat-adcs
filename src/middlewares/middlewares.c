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
#include "middlewares/alive/alive.h"
#include "middlewares/mpu/mpu.h"
#include "middlewares/communication/comm.h"
#include "utils/error/error.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static osThreadId_t aliveThreadHandle             = NULL;
static const osThreadAttr_t aliveThreadAttributes = {
    .name       = "aliveThread",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

static osThreadId_t mpuThreadHandle             = NULL;
static const osThreadAttr_t mpuThreadAttributes = {
    .name       = "mpuThread",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityRealtime,
};

static osThreadId_t commThreadHandle             = NULL;
static const osThreadAttr_t commThreadAttributes = {
    .name       = "commThread",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityLow1,
};

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void MIDDLEWARES_initThreads(void)
{
    aliveThreadHandle = osThreadNew(ALIVE_thread, NULL, &aliveThreadAttributes);
    if (aliveThreadHandle == NULL) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }

    mpuThreadHandle = osThreadNew(MPU_thread, NULL, &mpuThreadAttributes);
    if (mpuThreadHandle == NULL) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }

    commThreadHandle = osThreadNew(COMM_thread, NULL, &commThreadAttributes);
    if (commThreadHandle == NULL) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }
}
