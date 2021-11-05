#ifndef PWM_USER_PWM_USER_HPP_
#define PWM_USER_PWM_USER_HPP_

#include <cstdint>

namespace Pwm_User {

using pwm_value = uint32_t;

class Pwm_User {
  private:
    uint32_t m_pwm;
    uint32_t convertPwmToTimCompare(pwm_value pwm);
    void startPWM();
    void stopPWM();
    void private_assert(bool condition);

  protected:
    static constexpr pwm_value MAX_PWM_VALUE          = UINT32_MAX;
    static constexpr uint32_t MAX_TIM_COMPARE_REG_VAL = UINT32_MAX;

    Pwm_User();
    ~Pwm_User();
    void setPWM(pwm_value pwm);
    virtual void pwmDriverErrorHandle();
};

}    // namespace Pwm_User

#endif /* PWM_USER_PWM_USER_HPP_ */
