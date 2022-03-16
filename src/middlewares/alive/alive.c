/**
  ******************************************************************************
  * @file           : alive.c
  * @brief          : Source file.
  *                   This file contains the common code of the application
  *                   which signals that everything is executing.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "alive.h"

#include "FreeRTOS.h"
#include "bsp/led/bsp_led.h"
#include "task.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void ALIVE_thread(void *argument)
{
    for (;;) {
        BSP_LED_toggle(BSP_LED_INDEX_0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
