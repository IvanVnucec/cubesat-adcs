/*
 * reaction_wheel.hpp
 *
 *  Created on: 20. ruj 2021.
 *      Author: ivnucec
 */

#ifndef INC_REACTION_WHEEL_HPP_
#define INC_REACTION_WHEEL_HPP_

#include <climits>
#include <cstdint>

using namespace std;

class ReactionWheel {
  private:
    using pwm_value = uint32_t;

    enum ReactionWheelDirection { CLOCKWISE, ANTICLOCKWISE };

    static constexpr pwm_value MAX_PWM_VALUE          = UINT32_MAX;
    static constexpr uint32_t MAX_TIM_COMPARE_REG_VAL = UINT32_MAX;
    float m_ang_vel_rad_p_sec;    // rad/s
    uint32_t m_pwm;               // range 0 - UINT32_MAX
    ReactionWheelDirection m_dir;

    void setDirection(ReactionWheelDirection dir);
    void startPWM();
    void stopPWM();
    void setPWM(pwm_value pwm);
    pwm_value convertAbsAngVelRadPSecToPwm(float ang_vel_rad_p_sec);
    uint32_t convertPwmToTimCompare(pwm_value pwm);

  public:
    static constexpr float MAX_ANG_VEL_RAD_P_SEC =
        2.0f;    //	rad/s TODO: determine this for max pwm

    ReactionWheel();
    ~ReactionWheel();
    void setAngularVelocity(float ang_vel_rad_p_sec);
};

#endif /* INC_REACTION_WHEEL_HPP_ */
