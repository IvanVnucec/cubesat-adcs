#include "fault_handling.hpp"

#include "FreeRTOS.h"
#include "adcs_tasks.hpp"
#include "cmsis_os.h"
#include "main.h"
#include "portmacro.h"
#include "task.h"

namespace Fault {

static State private_state = State::NO_FAULT;

void setFaultState(State state)
{
    __disable_irq();
    private_state = state;
    __enable_irq();
}

State getFaultState()
{
    State retval;

    __disable_irq();
    retval = private_state;
    __enable_irq();

    return retval;
}

void assertAndRaiseFault(bool condition, State fault_state)
{
    if (not condition)
        setFaultState(fault_state);
}

void faultHandlingThread(void *argument)
{
    for (;;) {
        State state = getFaultState();

        switch (state) {
            case State::NO_FAULT: {
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
                break;
            }
            case State::I2C_DRIVER_FAULT:
            case State::MPU9250_FAULT:
            case State::IMU_FAULT:
            case State::UART_DRIVER_FAULT:
            case State::PARSER_FAULT:
            case State::REACTION_WHEEL_FAULT:
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
