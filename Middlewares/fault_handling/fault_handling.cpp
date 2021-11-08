#include "fault_handling.hpp"

#include "FreeRTOS.h"
#include "adcs_tasks.hpp"
#include "cmsis_os.h"
#include "main.h"
#include "semphr.h"
#include "task.h"

namespace Fault {

void setFaultState(State state)
{
    assert(faultHandlingHandle);
    BaseType_t rtos_status =
        xTaskNotify(static_cast<TaskHandle_t>(
                        faultHandlingHandle),        // safe conversion, see "task.h" file
                    static_cast<uint32_t>(state),    // safe conversion
                    eSetValueWithOverwrite);
    assert(rtos_status == pdPASS);
}

void setFaultStateFromISR(State state)
{
    BaseType_t woken          = pdFALSE;
    uint32_t prevNotification = 0u;

    assert(faultHandlingHandle);
    BaseType_t rtos_status = xTaskGenericNotifyFromISR(
        static_cast<TaskHandle_t>(
            faultHandlingHandle),        // safe conversion, see "task.h" file
        static_cast<uint32_t>(state),    // safe conversion,
        eSetValueWithOverwrite,
        &prevNotification,
        &woken);
    assert(rtos_status == pdPASS);

    // TODO: see what to do with prevNotification value
    (void)prevNotification;

    portYIELD_FROM_ISR(woken);
}

void faultHandlingThread(void *argument)
{
    for (;;) {
        uint32_t notification;
        BaseType_t rtos_status = xTaskNotifyWait(0u, 0u, &notification, portMAX_DELAY);
        assert(rtos_status == pdPASS);
        State state = static_cast<State>(notification);    // TODO: test this

        switch (state) {
            case State::NO_FAULT: {
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
                break;
            }
            case State::IMU_FAULT:
            case State::PARSER_FAULT:
            case State::REACTION_WHEEL_FAULT:
            default: {
                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

}    // namespace Fault
