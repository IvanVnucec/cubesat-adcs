/*
 * reaction_wheel.hpp
 *
 *  Created on: 20. ruj 2021.
 *      Author: ivnucec
 */

#ifndef INC_REACTION_WHEEL_HPP_
#define INC_REACTION_WHEEL_HPP_

enum ReactionWheelDirection { CLOCKWISE, ANTICLOCKWISE };

class ReactionWheel {
  private:
    int m_ang_vel;    // TODO: define units, write range
    float m_pwm;      // TODO: define units, write range
    ReactionWheelDirection m_dir;

    void setDirection(ReactionWheelDirection dir);
    void startPWM();
    void stopPWM();
    void setPWM(float pwm);

  public:
    ReactionWheel();
    ~ReactionWheel();
    void setAngularVelocity(int angular_vel);
};

#endif /* INC_REACTION_WHEEL_HPP_ */
