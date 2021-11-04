// TODO: Port code in this file

#include "reaction_wheel.hpp"

#include "main.h"
#include "tim.h"

#include <cmath>
#include <cstdint>

using namespace std;

/**
 * Initialize MCU peripherals and set reaction wheel Angular velocity to zero.
 *
 */
ReactionWheel::ReactionWheel()
{
    startPWM();
    setAngularVelocity(0.0f);
}

/**
 * Set reaction wheel angular velocity to zero and deinitializes MCU peripherals.
 */
ReactionWheel::~ReactionWheel()
{
    setAngularVelocity(0.0f);
    stopPWM();
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
    assert(dir == CLOCKWISE || dir == ANTICLOCKWISE);
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
    HAL_StatusTypeDef hal_status;

    // start Timer and PWM
    hal_status = HAL_TIM_Base_Start(&htim1);
    assert(hal_status == HAL_OK);

    hal_status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    assert(hal_status == HAL_OK);

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
}

/**
 * Stops the Timer and PWM.
 * @warning This function depends on MCU peripherals.
 */
void ReactionWheel::stopPWM()
{
    HAL_StatusTypeDef hal_status;

    // stop PWM and Timer
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);

    hal_status = HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    assert(hal_status == HAL_OK);

    hal_status = HAL_TIM_Base_Stop(&htim1);
    assert(hal_status == HAL_OK);
}

/**
 * Sets the PWM by setting the Timer Compare register.
 * @param pwm_value - 0 to MAX_PWM_VALUE
 * @warning This function depends on MCU peripherals.
 */
void ReactionWheel::setPWM(pwm_value pwm)
{
    assert(pwm <= MAX_PWM_VALUE);
    m_pwm = pwm;

    // convert pwm range to timer compare
    uint32_t tim_compare = convertPwmToTimCompare(m_pwm);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, tim_compare);
}

ReactionWheel::pwm_value
    ReactionWheel::convertAbsAngVelRadPSecToPwm(float abs_ang_vel_rad_p_sec)
{
    pwm_value pwm =
        (pwm_value)(abs_ang_vel_rad_p_sec / MAX_ANG_VEL_RAD_P_SEC) * MAX_PWM_VALUE;

    return pwm;
}

uint32_t ReactionWheel::convertPwmToTimCompare(pwm_value pwm)
{
    return (uint32_t)(pwm / MAX_PWM_VALUE) * MAX_TIM_COMPARE_REG_VAL;
}
