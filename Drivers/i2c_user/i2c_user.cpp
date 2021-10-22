/*
 * i2c_user.cpp
 *
 *  Created on: Oct 21, 2021
 *      Author: ivnucec
 */

#include "i2c_user.hpp"

#include "FreeRTOS.h"
#include "i2c.h"
#include "semphr.h"
#include "stm32l4xx.h"

#include <cstdint>

using namespace std;

static SemaphoreHandle_t i2cDmaSemaphore = NULL;

I2C_User::I2C_User(I2C_HandleTypeDef *hi2c)
{
    i2cDmaSemaphore = xSemaphoreCreateBinary();
    assert(i2cDmaSemaphore != NULL);
    xSemaphoreGive(i2cDmaSemaphore);
}

I2C_User::~I2C_User()
{
}

void I2C_User::sendOverDMA(uint16_t dev_address,
                           uint16_t mem_address,
                           uint8_t *data,
                           unsigned data_len)
{
    HAL_StatusTypeDef retval = HAL_ERROR;

    // TODO: Add virtual functions for taking and sending over DMA

    // get i2c mutex (dma callback releases mutex)
    if (xSemaphoreTake(i2cDmaSemaphore, portMAX_DELAY) == pdTRUE) {
        // send data over dma to i2c
        retval = HAL_I2C_Mem_Write_DMA(&hi2c1,
                                       dev_address,
                                       mem_address,
                                       I2C_MEMADD_SIZE_8BIT,
                                       data,
                                       data_len);
    }

    // TODO: implement error handling
    (void)retval;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(i2cDmaSemaphore, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
