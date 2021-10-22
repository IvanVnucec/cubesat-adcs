#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "queue.h"
#include "usart.h"

#include <cstdint>

// TODO [ivan vnucec]: guard variables by disabling interrupts or use some other solution
static std::uint8_t data_received;

QueueHandle_t bufferIn = xQueueCreate(100, sizeof(std::uint8_t));
static std::uint8_t bufferOut[100];

void startReceivingFromUart()
{
    HAL_UART_Receive_IT(&huart2, &data_received, 1);
}

// TODO [ivan vnucec]: test this
void getDataFromUart(std::uint8_t *data, unsigned len)
{
    for (unsigned i = 0; i < len; i++)
        if (xQueueReceive(bufferIn, data, portMAX_DELAY) == errQUEUE_EMPTY) {
            // TODO: handle when empty
            break;
        }
}

// TODO [ivan vnucec]: test this
void sendDataWithUart(std::uint8_t *data, unsigned len)
{
    for (unsigned i = 0; i < len; i++)
        bufferOut[i] = data[i];

    HAL_UART_Transmit_IT(&huart2, bufferOut, len);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle->Instance == USART2) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        BaseType_t result =
            xQueueSendFromISR(bufferIn, &data_received, &xHigherPriorityTaskWoken);

        if (result == errQUEUE_FULL) {
            // TODO [ivan vnucec]: add error handling here
        }

        if (xHigherPriorityTaskWoken) {
            // TODO [ivan vnucec]: add support for taskYIELD_FROM_ISR function
            //taskYIELD_FROM_ISR();
        }
    }

    HAL_UART_Receive_IT(handle, &data_received, 1);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *handle)
{
}
