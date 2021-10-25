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

#include <cassert>
#include <cstdint>

using namespace std;

static SemaphoreHandle_t i2cDmaSemaphore = NULL;

I2C_User::I2C_User(I2C_HandleTypeDef *hi2c)
{
    // TODO [Ivan vnucec]: check if HAL driver is initialized
    i2cDmaSemaphore = xSemaphoreCreateBinary();
    assert(i2cDmaSemaphore != NULL);
    xSemaphoreGive(i2cDmaSemaphore);
    m_initialized = true;
}

I2C_User::~I2C_User()
{
    // TODO: call hal deinit functions  or something
    m_initialized = false;
}

void I2C_User::WriteMemAsync(uint16_t dev_address,
                             uint16_t mem_address,
                             uint8_t *data,
                             unsigned data_len)
{
    HAL_StatusTypeDef hal_status = HAL_ERROR;
    BaseType_t rtos_status       = pdFAIL;

    assert(m_initialized == true);

    // get i2c mutex (dma callback releases mutex)
    rtos_status = xSemaphoreTake(i2cDmaSemaphore, portMAX_DELAY);
    if (rtos_status == pdPASS) {
        // send data over dma to i2c
        hal_status = HAL_I2C_Mem_Write_DMA(&hi2c1,
                                           dev_address,
                                           mem_address,
                                           I2C_MEMADD_SIZE_8BIT,
                                           data,
                                           data_len);
    }

    // TODO: implement error handling
    (void)rtos_status;
    (void)hal_status;
}

// TODO: Implement async reading
void I2C_User::ReadMemAsync()
{
    assert(m_initialized == true);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    assert(i2cDmaSemaphore != NULL);
    xSemaphoreGiveFromISR(i2cDmaSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
