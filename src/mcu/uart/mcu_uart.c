/**
  ******************************************************************************
  * @file    mcu_uart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mcu_uart.h"

#include "utils/error/error.h"

UART_HandleTypeDef huart1;

void MCU_UART_init(void)
{
    huart1.Instance                    = USART1;
    huart1.Init.BaudRate               = 9600;
    huart1.Init.WordLength             = UART_WORDLENGTH_8B;
    huart1.Init.StopBits               = UART_STOPBITS_1;
    huart1.Init.Parity                 = UART_PARITY_NONE;
    huart1.Init.Mode                   = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct       = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if (uartHandle->Instance == USART1) {
        /** Initializes the peripherals clock */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
        PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            ERROR_signalFaultConditionAndWaitIndefinitely();
        }

        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
            PA9     ------> USART1_TX
            PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{
    if (uartHandle->Instance == USART1) {
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
            PA9     ------> USART1_TX
            PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
}
