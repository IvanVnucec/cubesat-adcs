#ifndef FAULT_HANDLING_FAULT_HANDLING_HPP_
#define FAULT_HANDLING_FAULT_HANDLING_HPP_

namespace Fault {

enum State { NO_FAULT, IMU_FAULT, PARSER_FAULT, REACTION_WHEEL_FAULT };

void sendFaultStateFromIRQ(State state);
void sendFaultState(State state);

class Fault {
  private:
    State m_state = NO_FAULT;
    friend void sendFaultStateFromIRS(State state);
    friend void sendFaultState(State state);

  public:
    Fault();
    ~Fault();
    State getFaultState();
    void signalFault();
    void signalNotFault();
};

}    // namespace Fault

#endif /* FAULT_HANDLING_FAULT_HANDLING_HPP_ */
