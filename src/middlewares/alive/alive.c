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
#include "bsp/bsp_config.h"
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
        HAL_GPIO_TogglePin(BSP_CONFIG_LD3_GPIO_Port, BSP_CONFIG_LD3_Pin);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
