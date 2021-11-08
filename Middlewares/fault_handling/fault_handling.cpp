#include "fault_handling.hpp"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "semphr.h"

namespace Fault {

static State private_state = NO_FAULT;

Fault::Fault()
{
}

Fault::~Fault()
{
}

State Fault::getFaultState()
{
    __disable_irq();
    State state = private_state;
    __enable_irq();

    return state;
}

void setFaultState(State state)
{
    // TODO: implement task wait if the state is already set
    __disable_irq();
    private_state = state;
    __enable_irq();
}

void setFaultStateFromISR(State state)
{
    // TODO: See if we need to use FromISR function or not
    setFaultState(state);
}

void faultHandlingThread(void *argument)
{
    Fault fault_handler;

    for (;;) {
        State state = fault_handler.getFaultState();

        switch (state) {
            case NO_FAULT: {
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
                break;
            }
            case IMU_FAULT:
            case PARSER_FAULT:
            case REACTION_WHEEL_FAULT:
            default: {
                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

}    // namespace Fault
