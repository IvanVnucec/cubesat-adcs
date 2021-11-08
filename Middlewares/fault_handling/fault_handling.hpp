#ifndef FAULT_HANDLING_FAULT_HANDLING_HPP_
#define FAULT_HANDLING_FAULT_HANDLING_HPP_

namespace Fault {

enum State { NO_FAULT, IMU_FAULT, PARSER_FAULT, REACTION_WHEEL_FAULT };

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
