/**
  ******************************************************************************
  * @file           : bsp_led.c
  * @brief          : Source file.
  *                   This file contains the common code for LED on the board.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_led.h"

#include "bsp_led_config.h"
#include "stm32l4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GPIO_TypeDef *const BSP_LED_Index_To_GPIO_TypeDef[BSP_LED_INDEX_LENGTH] = {
    BSP_LED_CONFIG_LED_INDEX_0_GPIO_Port,
};

static const uint16_t BSP_LED_Index_To_GPIO_Pin[BSP_LED_INDEX_LENGTH] = {
    BSP_LED_CONFIG_LED_INDEX_0_Pin,
};

static const GPIO_PinState BSP_LED_Index_To_GPIO_PinState[BSP_LED_STATE_LENGTH] = {
    GPIO_PIN_RESET,
    GPIO_PIN_SET,
};

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void BSP_LED_init(void)
{
    for (BSP_LED_Index i = BSP_LED_INDEX_0; i < BSP_LED_INDEX_LENGTH; i++)
        BSP_LED_setState(i, BSP_LED_STATE_OFF);
}

void BSP_LED_deinit(void)
{
    for (BSP_LED_Index i = BSP_LED_INDEX_0; i < BSP_LED_INDEX_LENGTH; i++)
        BSP_LED_setState(i, BSP_LED_STATE_OFF);
}

void BSP_LED_setState(BSP_LED_Index index, BSP_LED_State state)
{
    if (index < BSP_LED_INDEX_LENGTH) {
        if (state < BSP_LED_STATE_LENGTH) {
            GPIO_TypeDef *gpio = (GPIO_TypeDef *)BSP_LED_Index_To_GPIO_TypeDef[index];
            uint16_t pin       = BSP_LED_Index_To_GPIO_Pin[index];
            GPIO_PinState pin_state = BSP_LED_Index_To_GPIO_PinState[state];

            HAL_GPIO_WritePin(gpio, pin, pin_state);
        }
    }
}

void BSP_LED_toggle(BSP_LED_Index index)
{
    if (index < BSP_LED_INDEX_LENGTH) {
        GPIO_TypeDef *gpio = (GPIO_TypeDef *)BSP_LED_Index_To_GPIO_TypeDef[index];
        uint16_t pin       = BSP_LED_Index_To_GPIO_Pin[index];

        HAL_GPIO_TogglePin(gpio, pin);
    }
}
