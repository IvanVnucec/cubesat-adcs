/**
  ******************************************************************************
  * @file           : bsp.c
  * @brief          : Source file.
  *                   This file contains the common code for the BSP layer.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#include "led/bsp_led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void BSP_init(void)
{
    BSP_LED_init();
}

void BSP_deinit(void)
{
    BSP_LED_deinit();
}
