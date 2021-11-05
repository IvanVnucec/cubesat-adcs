/*
 * reaction_wheel.hpp
 *
 *  Created on: 20. ruj 2021.
 *      Author: ivnucec
 */

#ifndef INC_REACTION_WHEEL_HPP_
#define INC_REACTION_WHEEL_HPP_

#include "pwm_user.hpp"

#include <climits>

namespace ReactionWheel {

using namespace Pwm_User;

class ReactionWheel : public Pwm_User {
  private:
    enum ReactionWheelDirection { CLOCKWISE, ANTICLOCKWISE };

    float m_ang_vel_rad_p_sec;    // rad/s
    ReactionWheelDirection m_dir;

    void setDirection(ReactionWheelDirection dir);
    pwm_value convertAbsAngVelRadPSecToPwm(float ang_vel_rad_p_sec);

  public:
    static constexpr float MAX_ANG_VEL_RAD_P_SEC =
        2.0f;    //	rad/s TODO: determine this for max pwm

    ReactionWheel();
    ~ReactionWheel();
    void errorHandle();
    void setAngularVelocity(float ang_vel_rad_p_sec);
    void pwmDriverErrorHandle() override;
};

}    // namespace ReactionWheel

#endif /* INC_REACTION_WHEEL_HPP_ */
