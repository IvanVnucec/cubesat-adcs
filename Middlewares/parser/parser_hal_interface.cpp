#include "buffer.hpp"
#include "usart.h"

#include <cstdint>

// TODO [ivan vnucec]: guard variables by disabling interrupts or use some other solution
static std::uint8_t data_to_send;
static std::uint8_t data_received;

static Buffer<std::uint8_t> bufferIn(3);
static Buffer<std::uint8_t> bufferOut(3);

extern "C" void startReceivingFromUart(void)
{
    HAL_UART_Receive_IT(&huart2, &data_received, 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle->Instance == USART2) {
        if (bufferOut.is_empty() == false) {
            data_to_send = bufferOut.pop();

            HAL_UART_Transmit_IT(handle, &data_to_send, 1);
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle->Instance == USART2) {
        if (bufferIn.is_full() == false) {
            bufferIn.push(data_received);
        }

        HAL_UART_Receive_IT(handle, &data_received, 1);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *handle)
{
}
