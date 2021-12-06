#include "fault_handling.hpp"

#include "FreeRTOS.h"
#include "adcs_tasks.hpp"
#include "cmsis_os.h"
#include "main.h"
#include "parser.hpp"
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
    static bool fault_not_sended = true;

    for (;;) {
        State state = getFaultState();
        std::string fault_str("Fault: ");

        switch (state) {
            case State::NO_FAULT: {
                break;
            }
            case State::I2C_DRIVER_FAULT: {
                fault_str += "I2C_DRIVER_FAULT";
                break;
            }
            case State::MPU9250_FAULT: {
                fault_str += "MPU9250_FAULT";
                break;
            }
            case State::IMU_FAULT: {
                fault_str += "IMU_FAULT";
                break;
            }
            case State::UART_DRIVER_FAULT: {
                fault_str += "UART_DRIVER_FAULT";
                break;
            }
            case State::PARSER_FAULT: {
                fault_str += "PARSER_FAULT";
                break;
            }
            case State::REACTION_WHEEL_FAULT: {
                fault_str += "REACTION_WHEEL_FAULT";
                break;
            }
            case State::GENERIC_FAULT:
            default: {
                fault_str += "GENERIC_FAULT";
                break;
            }
        }

        if (state == State::NO_FAULT) {
            HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
        } else {
            if (fault_not_sended) {    // transmit Fault state only once
                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
                Parser::sendString(fault_str + '\n');
                fault_not_sended = false;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

}    // namespace Fault
