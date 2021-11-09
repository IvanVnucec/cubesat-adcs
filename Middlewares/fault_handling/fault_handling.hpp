#ifndef FAULT_HANDLING_FAULT_HANDLING_HPP_
#define FAULT_HANDLING_FAULT_HANDLING_HPP_

#include <cstdint>

namespace Fault {

using namespace std;

enum class State : uint32_t {
    NO_FAULT,
    GENERIC_FAULT,
    IMU_FAULT,
    PARSER_FAULT,
    REACTION_WHEEL_FAULT
};

void setFaultState(State state);
void faultHandlingThread(void *argument);
void assertAndRaiseFault(bool condition, State fault_state);

}    // namespace Fault

#endif /* FAULT_HANDLING_FAULT_HANDLING_HPP_ */
