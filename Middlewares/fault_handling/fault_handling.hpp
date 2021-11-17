#ifndef FAULT_HANDLING_FAULT_HANDLING_HPP_
#define FAULT_HANDLING_FAULT_HANDLING_HPP_

#include <cstdint>

namespace Fault {

using namespace std;

enum class State : uint32_t {
    NO_FAULT,
    GENERIC_FAULT,
    I2C_DRIVER_FAULT,
    UART_DRIVER_FAULT,
    MPU9250_FAULT,
    IMU_FAULT,
    PARSER_FAULT,
    REACTION_WHEEL_FAULT
};

void setFaultState(State state = State::GENERIC_FAULT);
State getFaultState();
void assertAndRaiseFault(bool condition, State fault_state = State::GENERIC_FAULT);
void faultHandlingThread(void *argument);

}    // namespace Fault

#endif /* FAULT_HANDLING_FAULT_HANDLING_HPP_ */
