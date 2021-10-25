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

static constexpr I2C_HandleTypeDef *hal_i2c_handle_ptr = &hi2c1;

static SemaphoreHandle_t i2cDmaSemaphore = NULL;

I2C_User::I2C_User()
{
    m_hi2c_ptr = hal_i2c_handle_ptr;
    assert(m_hi2c_ptr != NULL);

    i2cDmaSemaphore = xSemaphoreCreateBinary();
    assert(i2cDmaSemaphore != NULL);

    BaseType_t rtos_status = xSemaphoreGive(i2cDmaSemaphore);
    assert(rtos_status == pdPASS);
}

I2C_User::~I2C_User()
{
    HAL_I2C_DeInit(m_hi2c_ptr);
}

void I2C_User::WriteMemAsync(uint16_t dev_address,
                             uint16_t mem_address,
                             uint8_t *data,
                             uint16_t data_len)
{
    BaseType_t rtos_status = xSemaphoreTake(i2cDmaSemaphore, portMAX_DELAY);
    assert(rtos_status == pdPASS);

    HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Write_DMA(m_hi2c_ptr,
                                                         dev_address,
                                                         mem_address,
                                                         I2C_MEMADD_SIZE_8BIT,
                                                         data,
                                                         data_len);

    assert(hal_status == HAL_OK);
}

void I2C_User::ReadMemAsync()
{
    // TODO: Implement async reading
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    BaseType_t rtos_status =
        xSemaphoreGiveFromISR(i2cDmaSemaphore, &xHigherPriorityTaskWoken);
    assert(rtos_status == pdPASS);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
