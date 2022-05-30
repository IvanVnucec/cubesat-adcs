/**
 ******************************************************************************
 * @file           : mcu_gpio.c
 * @brief          : Source file.
 *                   This file contains the common code of the application.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "mcu_gpio.h"

#include "bsp/bsp_config.h"
#include "bsp/led/bsp_led_config.h"
#include "stm32l4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void MCU_GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    /* Init LEDs */
    HAL_GPIO_WritePin(BSP_LED_CONFIG_LED_INDEX_0_GPIO_Port,
                      BSP_LED_CONFIG_LED_INDEX_0_Pin,
                      GPIO_PIN_RESET);

    HAL_GPIO_WritePin(BSP_LED_CONFIG_LED_INDEX_1_GPIO_Port,
                      BSP_LED_CONFIG_LED_INDEX_1_Pin,
                      GPIO_PIN_RESET);

    HAL_GPIO_WritePin(BSP_LED_CONFIG_LED_INDEX_2_GPIO_Port,
                      BSP_LED_CONFIG_LED_INDEX_2_Pin,
                      GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(BSP_CONFIG_REACTION_WHEEL_DIR_GPIO_Port,
                      BSP_CONFIG_REACTION_WHEEL_DIR_Pin,
                      GPIO_PIN_RESET);

    /* Encoder pins */
    /*Configure GPIO pin Input Level */
    HAL_GPIO_WritePin(BSP_CONFIG_ENCODER_CHA_GPIO_Port,
                      BSP_CONFIG_ENCODER_CHA_Pin,
                      GPIO_PIN_RESET);

    /*Configure GPIO pin Input Level */
    HAL_GPIO_WritePin(BSP_CONFIG_ENCODER_CHB_GPIO_Port,
                      BSP_CONFIG_ENCODER_CHB_Pin,
                      GPIO_PIN_RESET);

    /*Configure GPIO pin : PtPin */
    /* Init LEDs */
    GPIO_InitStruct.Pin   = BSP_LED_CONFIG_LED_INDEX_0_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BSP_LED_CONFIG_LED_INDEX_0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = BSP_LED_CONFIG_LED_INDEX_1_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BSP_LED_CONFIG_LED_INDEX_1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = BSP_LED_CONFIG_LED_INDEX_2_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BSP_LED_CONFIG_LED_INDEX_2_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = BSP_CONFIG_REACTION_WHEEL_DIR_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(BSP_CONFIG_REACTION_WHEEL_DIR_GPIO_Port, &GPIO_InitStruct);

    /* Encoder pins */
    GPIO_InitStruct.Pin  = BSP_CONFIG_ENCODER_CHA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_CONFIG_ENCODER_CHA_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = BSP_CONFIG_ENCODER_CHB_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BSP_CONFIG_ENCODER_CHB_GPIO_Port, &GPIO_InitStruct);

    /* For encoder pins interrupts (both channel A and B) */
    HAL_NVIC_SetPriority(BSP_CONFIG_ENCODER_CHA_EXTI_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(BSP_CONFIG_ENCODER_CHA_EXTI_IRQn);

    HAL_NVIC_SetPriority(BSP_CONFIG_ENCODER_CHB_EXTI_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(BSP_CONFIG_ENCODER_CHB_EXTI_IRQn);
}
