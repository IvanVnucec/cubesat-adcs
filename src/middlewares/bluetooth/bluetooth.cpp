#include "bluetooth.hpp"
#include "FreeRTOS.h"
#include "semphr.h"
#include "core/usart.h"

namespace Bluetooth
{


#define ms_to_os_ticks(ms) pdMS_TO_TICKS(ms)


static constexpr unsigned RX_TIMEOUT_MS = 1000u; // ms
static constexpr unsigned TX_TIMEOUT_MS = 1000u; // ms


static constexpr UART_HandleTypeDef* uart_hal = &huart1;
static bool initialized = false;
static SemaphoreHandle_t uartTxSemaphore = NULL;
static SemaphoreHandle_t uartRxSemaphore = NULL;
static std::uint8_t data_received = 0x00;


static void start_receiving();
static void stop_receiving();
static void private_assert(bool condition);


bool init()
{
    uartTxSemaphore = xSemaphoreCreateBinary();
    if (uartTxSemaphore == NULL)
        goto return_failure;

    uartRxSemaphore = xSemaphoreCreateBinary();
    if (uartRxSemaphore == NULL)
        goto return_failure;

    initialized = true;

    return true;

return_failure:
    return false;
}

bool transmit(const std::uint8_t* data, unsigned len, unsigned timeout_in_ms)
{
    if (not initialized)
        return false;

    // TODO: use dma not interrupt
    HAL_StatusTypeDef hal_status = HAL_UART_Transmit_IT(uart_hal, (std::uint8_t*)data, len);
    if (hal_status != HAL_OK)
        return false;

    BaseType_t rtos_status =
        xSemaphoreTake(uartTxSemaphore, ms_to_os_ticks(timeout_in_ms));
    if (rtos_status != pdPASS)
        return false;

    return true;
}


bool receive_until(char c, std::uint8_t* data, unsigned max_len, unsigned timeout_in_ms)
{
    unsigned i = 0u;

    while (i < max_len) {
        // TODO: fix this fixed timeout, timeout can accumulate with this 
        std::uint8_t received;
        bool success = receive_byte(&received, RX_TIMEOUT_MS);
        if (not success)
            return false;

        data[i++] = received;

        if (received == c)
            break;
    }

    return true;
}


bool receive(std::uint8_t* data, unsigned len, unsigned timeout_in_ms)
{
    if (not initialized)
        return false;

    for (unsigned i = 0u; i < len; i++)
    {
        std::uint8_t c;
        // TODO: fix this fixed timeout, timeout can accumulate with this 
        bool success = receive_byte(&c, RX_TIMEOUT_MS);
        if (not success)
            return false; 
        data[i] = c;
    }

    return true;
}


bool receive_byte(std::uint8_t* received, unsigned timeout_in_ms)
{
    start_receiving();

    BaseType_t rtos_status = 
        xSemaphoreTake(uartRxSemaphore, ms_to_os_ticks(timeout_in_ms));
    if (rtos_status != pdPASS)
        
    stop_receiving();

    return data_received;
}


static void start_receiving()
{
    HAL_StatusTypeDef hal_status = HAL_UART_Receive_IT(uart_hal, &data_received, 1);
    private_assert(hal_status == HAL_OK);
}


static void stop_receiving()
{
    HAL_StatusTypeDef hal_status = HAL_UART_AbortReceive_IT(uart_hal);
    private_assert(hal_status == HAL_OK);
}


static void private_assert(bool condition)
{
    // TODO: add code here

#if DEBUG
    assert(0);
#endif
}


extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
    if (initialized)
    {
        if (handle->Instance == uart_hal->Instance) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;

            BaseType_t rtos_status =
                xSemaphoreGiveFromISR(uartTxSemaphore, &xHigherPriorityTaskWoken);
            private_assert(rtos_status == pdPASS);

            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}


extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    if (initialized)
    {
        if (handle->Instance == uart_hal->Instance) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;

            BaseType_t rtos_status =
                xSemaphoreGiveFromISR(uartRxSemaphore, &xHigherPriorityTaskWoken);
            private_assert(rtos_status == pdPASS);

            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}


extern "C" void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
    // called after HAL_UART_AbortReceive_IT
}


extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *handle)
{
    uint32_t error_code = handle->ErrorCode;
    (void)error_code;

    private_assert(0);
}


} // namespace Bluetooth
