/**
  ******************************************************************************
  * @file           : error.c
  * @brief          : Source file.
  *                   This file contains the common code of the error handling.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "error.h"

#include "bsp/bsp_config.h"
#include "cmsis_os.h"
#include "stm32l4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

void ERROR_signalFaultCondition(void)
{
    HAL_GPIO_WritePin(BSP_CONFIG_LD3_GPIO_Port, BSP_CONFIG_LD3_Pin, GPIO_PIN_SET);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void ERROR_signalFaultConditionAndWaitIndefinitely(void)
{
    ERROR_signalFaultCondition();
    __disable_irq();
    while (1) {
    }
}

/* Stack overflow hook
https://www.freertos.org/Stacks-and-stack-overflow-checking.html
*/
#ifdef DEBUG
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    __disable_irq();
    while (1) {
    }
}
#endif /* DEBUG */
