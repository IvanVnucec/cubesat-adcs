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

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void MIDDLEWARES_initThreads(void)
{
    aliveThreadHandle = osThreadNew(ALIVE_thread, NULL, &aliveThreadAttributes);
    if (aliveThreadHandle == NULL) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }
}
