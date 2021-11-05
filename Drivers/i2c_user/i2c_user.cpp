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

static bool i2c_driver_initialized = false;

static SemaphoreHandle_t i2cTxSemaphore = NULL;
static SemaphoreHandle_t i2cRxSemaphore = NULL;

I2C_User::I2C_User()
{
    assert(i2c_driver_initialized == false);

    m_hi2c_ptr = hal_i2c_handle_ptr;
    assert(m_hi2c_ptr != NULL);

    i2cTxSemaphore = xSemaphoreCreateBinary();
    assert(i2cTxSemaphore != NULL);

    i2cRxSemaphore = xSemaphoreCreateBinary();
    assert(i2cRxSemaphore != NULL);

    i2c_driver_initialized = true;
}

I2C_User::~I2C_User()
{
    HAL_StatusTypeDef hal_status = HAL_I2C_DeInit(m_hi2c_ptr);
    assert(hal_status == HAL_OK);
}

// TODO: fix this function. After sending, reading returns with HAL busy retval
void I2C_User::WriteMemAsync(uint16_t dev_address,
                             uint16_t mem_address,
                             uint8_t *data,
                             uint16_t data_len)
{
    // TODO: See can we use HAL_I2C_Master_Transmit and not HAL_I2C_Mem_Write
    // TODO: Use HAL_I2C_Mem_Write_DMA
    HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Write_IT(m_hi2c_ptr,
                                                        dev_address,
                                                        mem_address,
                                                        I2C_MEMADD_SIZE_8BIT,
                                                        data,
                                                        data_len);
    assert(hal_status == HAL_OK);

    BaseType_t rtos_status = xSemaphoreTake(i2cTxSemaphore, portMAX_DELAY);
    assert(rtos_status == pdPASS);
}

void I2C_User::ReadMemAsync(uint16_t dev_address,
                            uint16_t mem_address,
                            uint8_t *data,
                            uint16_t data_len)
{
    // TODO: Use HAL_I2C_Mem_Read_DMA
    HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Read_IT(m_hi2c_ptr,
                                                       dev_address,
                                                       mem_address,
                                                       I2C_MEMADD_SIZE_8BIT,
                                                       data,
                                                       data_len);
    assert(hal_status == HAL_OK);

    BaseType_t rtos_status = xSemaphoreTake(i2cRxSemaphore, portMAX_DELAY);
    assert(rtos_status == pdPASS);
}

extern "C" void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == hal_i2c_handle_ptr->Instance) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        BaseType_t rtos_status =
            xSemaphoreGiveFromISR(i2cTxSemaphore, &xHigherPriorityTaskWoken);
        assert(rtos_status == pdPASS);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

extern "C" void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == hal_i2c_handle_ptr->Instance) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        BaseType_t rtos_status =
            xSemaphoreGiveFromISR(i2cRxSemaphore, &xHigherPriorityTaskWoken);
        assert(rtos_status == pdPASS);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

extern "C" void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    assert(0);
}
