#include "uart_user.hpp"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "queue.h"
#include "semphr.h"
#include "usart.h"

#include <cassert>
#include <cstdint>
#include <sys/_stdint.h>

namespace Uart_User {

using namespace std;

static constexpr UART_HandleTypeDef *hal_uart_handle_ptr = &huart1;
static constexpr unsigned buffer_out_len                 = 100u;

static uint8_t data_received;
static uint8_t bufferOut[buffer_out_len];

static SemaphoreHandle_t uartTxSemaphore = NULL;
static SemaphoreHandle_t uartRxSemaphore = NULL;

UART_User::UART_User()
{
    m_hal_uart_ptr = hal_uart_handle_ptr;
    private_assert(m_hal_uart_ptr != NULL);

    uartTxSemaphore = xSemaphoreCreateBinary();
    private_assert(uartTxSemaphore != NULL);

    uartRxSemaphore = xSemaphoreCreateBinary();
    private_assert(uartRxSemaphore != NULL);
}

UART_User::~UART_User()
{
}

void UART_User::startReceiving()
{
    HAL_StatusTypeDef hal_status = HAL_UART_Receive_IT(m_hal_uart_ptr, &data_received, 1);
    private_assert(hal_status == HAL_OK);
}

void UART_User::stopReceiving()
{
    HAL_StatusTypeDef hal_status = HAL_UART_AbortReceive_IT(m_hal_uart_ptr);
    private_assert(hal_status == HAL_OK);
}

uint8_t UART_User::readByteAsync()
{
    startReceiving();

    BaseType_t rtos_status = xSemaphoreTake(uartRxSemaphore, portMAX_DELAY);
    private_assert(rtos_status == pdPASS);

    stopReceiving();

    return data_received;
}

void UART_User::readDataAsync(uint8_t *data, unsigned len)
{
    for (unsigned i = 0u; i < len; i++) {
        data[i] = readByteAsync();
    }
}

unsigned UART_User::readDataAsyncUntilChar(uint8_t *data, char c, unsigned max_len)
{
    unsigned i = 0u;

    while (i < max_len) {
        data[i] = readByteAsync();
        if (data[i++] == c)
            break;
    }

    return i;
}

/**
 * @brief Sends data asyncronusly. Data is being copied into internal buffer atomically.
 * 
 * @param data Data to send. Data is copied into internal buffer atomically.
 * @param len 
 */
void UART_User::writeDataAsync(const uint8_t *data, unsigned len)
{
    private_assert(len <= buffer_out_len);

    __disable_irq();
    for (unsigned i = 0; i < len; i++)
        bufferOut[i] = data[i];
    __enable_irq();

    HAL_StatusTypeDef hal_status = HAL_UART_Transmit_IT(m_hal_uart_ptr, bufferOut, len);
    private_assert(hal_status == HAL_OK);

    BaseType_t rtos_status = xSemaphoreTake(uartTxSemaphore, portMAX_DELAY);
    private_assert(rtos_status == pdPASS);
}

void UART_User::private_assert(bool condition)
{
    if (not condition)
        uartDriverErrorHandle();
}

void UART_User::uartDriverErrorHandle()
{
#ifdef DEBUG
    while (true)
        ;
#endif
}

}    // namespace Uart_User

// ***************************** HAL Callback functions *****************************
extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
    using namespace Uart_User;

    if (handle->Instance == hal_uart_handle_ptr->Instance) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        BaseType_t rtos_status =
            xSemaphoreGiveFromISR(uartTxSemaphore, &xHigherPriorityTaskWoken);
        assert(rtos_status == pdPASS);    // TODO: Replace assert() with private_assert()

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    using namespace Uart_User;

    if (handle->Instance == hal_uart_handle_ptr->Instance) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        BaseType_t rtos_status =
            xSemaphoreGiveFromISR(uartRxSemaphore, &xHigherPriorityTaskWoken);
        assert(rtos_status == pdPASS);    // TODO: Replace assert() with private_assert()

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
    using namespace Uart_User;

    // called after HAL_UART_AbortReceive_IT
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *handle)
{
    using namespace Uart_User;

    assert(0);    // TODO: Replace assert() with private_assert()
}
