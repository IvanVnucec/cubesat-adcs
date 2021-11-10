#include "fault_handling.hpp"

#include "FreeRTOS.h"
#include "adcs_tasks.hpp"
#include "cmsis_os.h"
#include "main.h"
#include "portmacro.h"
#include "task.h"

namespace Fault {

void setFaultState(State state)
{
    BaseType_t rtos_status = pdFAIL;

    assert(faultHandlingHandle);

    bool inISR = xPortIsInsideInterrupt();
    if (inISR) {
        BaseType_t woken          = pdFALSE;
        uint32_t prevNotification = 0u;

        rtos_status = xTaskGenericNotifyFromISR(
            static_cast<TaskHandle_t>(
                faultHandlingHandle),        // safe conversion, see "task.h" file
            static_cast<uint32_t>(state),    // safe conversion,
            eSetValueWithOverwrite,
            &prevNotification,
            &woken);

        // TODO: see what to do with prevNotification value
        (void)prevNotification;

        portYIELD_FROM_ISR(woken);

    } else {
        rtos_status =
            xTaskNotify(static_cast<TaskHandle_t>(
                            faultHandlingHandle),    // safe conversion, see "task.h" file
                        static_cast<uint32_t>(state),    // safe conversion
                        eSetValueWithOverwrite);
    }

    assert(rtos_status == pdPASS);
}

void assertAndRaiseFault(bool condition, State fault_state)
{
    if (not condition)
        setFaultState(fault_state);
}

void faultHandlingThread(void *argument)
{
    for (;;) {
        uint32_t notification;
        BaseType_t rtos_status = xTaskNotifyWait(0u, 0u, &notification, portMAX_DELAY);
        assert(rtos_status == pdPASS);
        State state = static_cast<State>(notification);    // TODO: test this

        // TODO: fix switch case because if IMU_FAULT triggers, it will also suspend parser handle
        // TODO: if fault occurs, osThreadSuspend will be called on every fault handling thread pass
        switch (state) {
            case State::NO_FAULT: {
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
                break;
            }
            case State::I2C_DRIVER_FAULT:
            case State::MPU9250_FAULT:
            case State::IMU_FAULT: {
                // TODO: check retval from rtos, also below
                osThreadSuspend(inertialMeasUnitHandle);
                // intentionally fall through case
            }
            case State::UART_DRIVER_FAULT:
            case State::PARSER_FAULT: {
                osThreadSuspend(parserHandle);
                // intentionally fall through case
            }
            case State::REACTION_WHEEL_FAULT: {
                // intentionally fall through case
            }
            case State::GENERIC_FAULT:
            default: {
                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

}    // namespace Fault
