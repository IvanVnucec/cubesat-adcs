#ifndef FAULT_HANDLING_FAULT_HANDLING_HPP_
#define FAULT_HANDLING_FAULT_HANDLING_HPP_

#include <cstdint>

namespace Fault {

using namespace std;

enum class State : uint32_t {
    NO_FAULT             = 0u,
    IMU_FAULT            = 1u,
    PARSER_FAULT         = 2u,
    REACTION_WHEEL_FAULT = 3u
};

void setFaultState(State state);
void faultHandlingThread(void *argument);

class Fault {
  private:
  public:
    Fault();
    ~Fault();
    State getFaultState();
};

}    // namespace Fault

#endif /* FAULT_HANDLING_FAULT_HANDLING_HPP_ */
