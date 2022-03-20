/**
  ******************************************************************************
  * @file    mcu_i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mcu_i2c.h"

#include "stm32l4xx_hal.h"
#include "utils/error/error.h"

I2C_HandleTypeDef hi2c3;

/* I2C3 init function */
void MCU_I2C_init(void)
{
    hi2c3.Instance              = I2C3;
    hi2c3.Init.Timing           = 0x00303D5B;
    hi2c3.Init.OwnAddress1      = 0;
    hi2c3.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c3.Init.OwnAddress2      = 0;
    hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c3.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c3.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c3) != HAL_OK) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }
    /** Configure Analogue filter
  */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }
    /** Configure Digital filter
  */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK) {
        ERROR_signalFaultConditionAndWaitIndefinitely();
    }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *i2cHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct       = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if (i2cHandle->Instance == I2C3) {
        /** Initializes the peripherals clock
  */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C3;
        PeriphClkInit.I2c3ClockSelection   = RCC_I2C3CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            ERROR_signalFaultConditionAndWaitIndefinitely();
        }

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C3 GPIO Configuration
            PA7     ------> I2C3_SCL
            PB4 (NJTRST)     ------> I2C3_SDA
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_7;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_4;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* I2C3 clock enable */
        __HAL_RCC_I2C3_CLK_ENABLE();

        /* I2C3 interrupt Init */
        HAL_NVIC_SetPriority(I2C3_EV_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
        HAL_NVIC_SetPriority(I2C3_ER_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *i2cHandle)
{
    if (i2cHandle->Instance == I2C3) {
        /* Peripheral clock disable */
        __HAL_RCC_I2C3_CLK_DISABLE();

        /**I2C3 GPIO Configuration
            PA7     ------> I2C3_SCL
            PB4 (NJTRST)     ------> I2C3_SDA
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);

        /* I2C3 interrupt Deinit */
        HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
    }
}
