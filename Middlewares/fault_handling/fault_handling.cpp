#include "fault_handling.hpp"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "semphr.h"

namespace Fault {

static Fault *private_this           = nullptr;
static SemaphoreHandle_t fault_mutex = NULL;

Fault::Fault()
{
    fault_mutex = xSemaphoreCreateBinary();
    assert(fault_mutex);

    BaseType_t rtos_status = xSemaphoreGive(fault_mutex);
    assert(rtos_status == pdPASS);

    private_this = this;
}

Fault::~Fault()
{
}

State Fault::getFaultState()
{
    BaseType_t rtos_status = xSemaphoreTake(fault_mutex, portMAX_DELAY);
    assert(rtos_status == pdPASS);

    State retval = m_state;

    rtos_status = xSemaphoreGive(fault_mutex);
    assert(rtos_status == pdPASS);

    return retval;
}

void Fault::signalFault()
{
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

void Fault::signalNotFault()
{
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

void sendFaultStateFromISR(State state)
{
    assert(private_this);

    BaseType_t woken       = pdFALSE;
    BaseType_t rtos_status = xSemaphoreTakeFromISR(fault_mutex, &woken);
    assert(rtos_status == pdPASS);
    portYIELD_FROM_ISR(woken);

    private_this->m_state = state;

    rtos_status = xSemaphoreGiveFromISR(fault_mutex, &woken);
    assert(rtos_status == pdPASS);
    portYIELD_FROM_ISR(woken);
}

void sendFaultState(State state)
{
    assert(private_this);

    BaseType_t rtos_status = xSemaphoreTake(fault_mutex, portMAX_DELAY);
    assert(rtos_status == pdPASS);

    private_this->m_state = state;

    rtos_status = xSemaphoreGive(fault_mutex);
    assert(rtos_status == pdPASS);
}

void faultHandlingThread(void *argument)
{
    Fault fault_handler;

    for (;;) {
        State state = fault_handler.getFaultState();

        switch (state) {
            case NO_FAULT: {
                fault_handler.signalNotFault();
                break;
            }
            case IMU_FAULT:
            case PARSER_FAULT:
            case REACTION_WHEEL_FAULT:
            default: {
                fault_handler.signalFault();
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

}    // namespace Fault
