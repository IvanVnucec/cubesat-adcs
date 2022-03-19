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

#include "bsp/led/bsp_led.h"
#include "cmsis_os.h"
#include "stm32l4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

void ERROR_assert(int condition)
{
    if (!condition)
        ERROR_signalFaultConditionAndWaitIndefinitely();
}

void ERROR_signalFaultCondition(void)
{
    BSP_LED_setState(BSP_LED_INDEX_0, BSP_LED_STATE_ON);
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
    ERROR_signalFaultConditionAndWaitIndefinitely();
}
#endif /* DEBUG */
