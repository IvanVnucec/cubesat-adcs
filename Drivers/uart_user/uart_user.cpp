#include "uart_user.hpp"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "queue.h"
#include "usart.h"

#include <cassert>
#include <cstdint>

using namespace std;

static constexpr UART_HandleTypeDef *hal_uart_handle_ptr = &huart1;
static constexpr unsigned buffer_out_len                 = 100u;
static constexpr unsigned buffer_in_len                  = 100u;

static uint8_t data_received;
static uint8_t bufferOut[buffer_out_len];
static QueueHandle_t bufferIn = NULL;

UART_User::UART_User()
{
    m_hal_uart_ptr = hal_uart_handle_ptr;
    assert(m_hal_uart_ptr != NULL);

    // TODO: init uart dma binary semaphore

    bufferIn = xQueueCreate(buffer_in_len, sizeof(uint8_t));
    assert(bufferIn != NULL);
}

UART_User::~UART_User()
{
}

void UART_User::startReceiving()
{
    HAL_StatusTypeDef hal_status = HAL_UART_Receive_IT(m_hal_uart_ptr, &data_received, 1);
    assert(hal_status == HAL_OK);
}

void UART_User::stopReceiving()
{
    HAL_StatusTypeDef hal_status = HAL_UART_AbortReceive_IT(m_hal_uart_ptr);
    assert(hal_status == HAL_OK);
}

// TODO [ivan vnucec]: implement this
void UART_User::readDataAsync()
{
}

/**
 * @brief Sends data asyncronusly. Data is being copied into internal buffer atomically.
 * 
 * @param data Data to send. Data is copied into internal buffer atomically.
 * @param len 
 */
void UART_User::writeDataAsync(uint8_t *data, unsigned len)
{
    assert(len <= buffer_out_len);

    __disable_irq();
    for (unsigned i = 0; i < len; i++)
        bufferOut[i] = data[i];
    __enable_irq();

    // TODO: take uart dma binary semaphore
    HAL_StatusTypeDef hal_status = HAL_UART_Transmit_DMA(m_hal_uart_ptr, bufferOut, len);
    assert(hal_status == HAL_OK);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle->Instance == hal_uart_handle_ptr->Instance) {
        // TODO: give uart dma binary semaphore
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle->Instance == hal_uart_handle_ptr->Instance) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        BaseType_t result =
            xQueueSendFromISR(bufferIn, &data_received, &xHigherPriorityTaskWoken);

        if (result == errQUEUE_FULL) {
            // TODO [ivan vnucec]: add error handling here
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        HAL_StatusTypeDef hal_status = HAL_UART_Receive_IT(handle, &data_received, 1);
        assert(hal_status == HAL_OK);
    }
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
    // called after HAL_UART_AbortReceive_IT
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *handle)
{
}
