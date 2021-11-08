// TODO: Port code in this file

#include "reaction_wheel.hpp"

#include "fault_handling.hpp"
#include "main.h"

#include <cmath>
#include <cstdint>

namespace ReactionWheel {

using namespace std;
using namespace Pwm_User;

/**
 * Initialize MCU peripherals and set reaction wheel Angular velocity to zero.
 *
 */
ReactionWheel::ReactionWheel() : Pwm_User()
{
    setAngularVelocity(0.0f);
}

/**
 * Set reaction wheel angular velocity to zero and deinitializes MCU peripherals.
 */
ReactionWheel::~ReactionWheel()
{
    setAngularVelocity(0.0f);
}

/**
 * Set direction of rotation and the PWM based on angular velocity.
 * @param angular_vel - in rad/s. Values above MAX_ANG_VEL_RAD_P_SEC
 * are limited to that value
 */
void ReactionWheel::setAngularVelocity(float ang_vel_rad_p_sec)
{
    m_ang_vel_rad_p_sec = ang_vel_rad_p_sec;

    if (m_ang_vel_rad_p_sec > 0.0f) {
        setDirection(CLOCKWISE);
    } else {
        setDirection(ANTICLOCKWISE);
    }

    float abs_ang_vel = abs(ang_vel_rad_p_sec);

    if (abs_ang_vel > MAX_ANG_VEL_RAD_P_SEC)
        abs_ang_vel = MAX_ANG_VEL_RAD_P_SEC;

    pwm_value pwm = convertAbsAngVelRadPSecToPwm(abs_ang_vel);
    setPWM(pwm);
}

/**
 * Set the GPIO Direction Pin based on direction value.
 * @param dir - CLOCKWISE or ANTICLOCKWISE
 * @warning This function depends on the hardware.
 */
void ReactionWheel::setDirection(ReactionWheelDirection dir)
{
    private_assert(dir == CLOCKWISE || dir == ANTICLOCKWISE);
    m_dir = dir;

    if (dir == CLOCKWISE) {
        // TODO: Figure out digital pin state for clockwise direction
        HAL_GPIO_WritePin(REACTION_WHEEL_DIR_GPIO_Port,
                          REACTION_WHEEL_DIR_Pin,
                          GPIO_PIN_RESET);
    } else {
        // TODO: Figure out digital pin state for anticlockwise direction
        HAL_GPIO_WritePin(REACTION_WHEEL_DIR_GPIO_Port,
                          REACTION_WHEEL_DIR_Pin,
                          GPIO_PIN_SET);
    }
}

pwm_value ReactionWheel::convertAbsAngVelRadPSecToPwm(float abs_ang_vel_rad_p_sec)
{
    pwm_value pwm =
        (pwm_value)(abs_ang_vel_rad_p_sec / MAX_ANG_VEL_RAD_P_SEC) * MAX_PWM_VALUE;

    return pwm;
}

void ReactionWheel::private_assert(bool condition)
{
    if (not condition)
        errorHandle();
}

void ReactionWheel::pwmDriverErrorHandle()
{
    errorHandle();
}

void ReactionWheel::errorHandle()
{
    Fault::setFaultState(Fault::State::REACTION_WHEEL_FAULT);
}

}    // namespace ReactionWheel
