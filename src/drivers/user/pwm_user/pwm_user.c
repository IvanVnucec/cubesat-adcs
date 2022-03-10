#if 0
#include "pwm_user.hpp"

#include "core/tim.h"

namespace Pwm_User {

static bool pwm_driver_initialized = false;

Pwm_User::Pwm_User()
{
    private_assert(pwm_driver_initialized == false);

    startPWM();

    pwm_driver_initialized = true;
}

Pwm_User::~Pwm_User()
{
    stopPWM();
}

/**
 * Sets the PWM by setting the Timer Compare register.
 * @param pwm_value - 0 to MAX_PWM_VALUE
 * @warning This function depends on MCU peripherals.
 */
void Pwm_User::setPWM(pwm_value pwm)
{
    private_assert(pwm <= MAX_PWM_VALUE);
    m_pwm = pwm;

    // convert pwm range to timer compare
    uint32_t tim_compare = convertPwmToTimCompare(m_pwm);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, tim_compare);
}

uint32_t Pwm_User::convertPwmToTimCompare(pwm_value pwm)
{
    return (uint32_t)(pwm / MAX_PWM_VALUE) * MAX_TIM_COMPARE_REG_VAL;
}

/**
 * Starts the Timer and PWM.
 * @warning This function depends on MCU peripherals.
 */
void Pwm_User::startPWM()
{
    HAL_StatusTypeDef hal_status;

    // start Timer and PWM
    hal_status = HAL_TIM_Base_Start(&htim1);
    private_assert(hal_status == HAL_OK);

    hal_status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    private_assert(hal_status == HAL_OK);

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
}

/**
 * Stops the Timer and PWM.
 * @warning This function depends on MCU peripherals.
 */
void Pwm_User::stopPWM()
{
    HAL_StatusTypeDef hal_status;

    // stop PWM and Timer
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);

    hal_status = HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    private_assert(hal_status == HAL_OK);

    hal_status = HAL_TIM_Base_Stop(&htim1);
    private_assert(hal_status == HAL_OK);
}

void Pwm_User::private_assert(bool condition)
{
    if (not condition)
        pwmDriverErrorHandle();
}

void Pwm_User::pwmDriverErrorHandle()
{
}

}    // namespace Pwm_User
#endif