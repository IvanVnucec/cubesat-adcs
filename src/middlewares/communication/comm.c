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

#include "FreeRTOS.h"
#include "task.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void COMM_thread(void *argument)
{
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
