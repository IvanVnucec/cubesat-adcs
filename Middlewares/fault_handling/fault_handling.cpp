#include "fault_handling.hpp"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "semphr.h"

namespace Fault {

static State m_state = NO_FAULT;

Fault::Fault()
{
}

Fault::~Fault()
{
}

State Fault::getFaultState()
{
    __disable_irq();
    State retval = m_state;
    __enable_irq();

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

void sendFaultState(State state)
{
    __disable_irq();
    m_state = state;
    __enable_irq();
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
