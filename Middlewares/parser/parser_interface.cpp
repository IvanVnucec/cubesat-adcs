#include "usart.h"

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
    //static uint8_t data[] = "hello world\n";
    //HAL_UART_Transmit_IT(handle, data, sizeof(data) / sizeof(data[0]));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    //static uint8_t data[1];
    //HAL_UART_Receive_IT(handle, data, 1);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *handle)
{
}
