// TODO: Port code in this file

#include "reaction_wheel.hpp"

#include "main.h"
#include "tim.h"

#include <cmath>
#include <limits.h>

/**
 * Initialize MCU peripherals.
 * Set reaction wheel Angular velocity to zero.
 * @param angular_vel - Default 0 // TODO: add units and ranges here
 *
 */
ReactionWheel::ReactionWheel(int ang_vel)
{
    startPWM();
    setAngularVelocity(ang_vel);
}

/**
 * Set reaction wheel angular velocity to zero and deinitializes MCU peripherals.
 */
ReactionWheel::~ReactionWheel()
{
    setAngularVelocity(0);
    stopPWM();
}

/**
 * Set direction of rotation and the PWM based on angular velocity.
 * @param angular_vel - // TODO: add units and ranges here
 */
void ReactionWheel::setAngularVelocity(int angular_vel)
{
    // TODO: add assert here
    m_ang_vel = angular_vel;

    if (m_ang_vel > 0) {
        // TODO: determine experimentally which direction we should set here
        setDirection(CLOCKWISE);
    } else {
        // TODO: determine experimentally which direction we should set here
        setDirection(ANTICLOCKWISE);
    }

    // TODO: convert angular_vel to PWM value (this implementation is for example only!!!)
    float pwm = std::abs(m_ang_vel) / INT_MAX;
    setPWM(pwm);
}

/**
 * Set the GPIO Direction Pin based on direction value.
 * @param dir - CLOCKWISE or ANTICLOCKWISE
 * @warning This function depends on the hardware.
 */
void ReactionWheel::setDirection(ReactionWheelDirection dir)
{
    // TODO: add assert here. See how to assert Enums
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

/**
 * Starts the Timer and PWM.
 * @warning This function depends on MCU peripherals.
 */
void ReactionWheel::startPWM()
{
    // start Timer and PWM
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
}

/**
 * Stops the Timer and PWM.
 * @warning This function depends on MCU peripherals.
 */
void ReactionWheel::stopPWM()
{
    // stop PWM and Timer
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_Base_Stop(&htim1);
}

/**
 * Sets the PWM by setting the Timer Compare register.
 * @param pwm - // TODO: determine range of pwm variable.
 * @warning This function depends on MCU peripherals.
 */
void ReactionWheel::setPWM(float pwm)
{
    // TODO: add assert here
    m_pwm = pwm;

    // TODO: convert pwm to pulse_width
    uint32_t pulse_width = 0u;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse_width);
}
