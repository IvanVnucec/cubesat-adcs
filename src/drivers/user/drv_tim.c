/**
 ******************************************************************************
 * @file           : drv_tim.c
 * @brief          : Source file.
 *                   This file contains the common defines for the timer 
 *                   operations.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "drv_tim.h"

#include "mcu/tim/mcu_tim.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static uint32_t DRV_TIM_pwmDutyCycleToPwmCompare(DRV_TIM_PwmDutyCycle duty_cycle);

/* Private user code ---------------------------------------------------------*/
void DRV_TIM_init(void)
{
}

void DRV_TIM_deInit(void)
{
}

void DRV_TIM_pwmStart(void)
{
    MCU_TIM_pwmStart();
}

void DRV_TIM_pwmStop(void)
{
    MCU_TIM_pwmStop();
}

void DRV_TIM_pwmSetDutyCycle(DRV_TIM_PwmDutyCycle duty_cycle)
{
    if (duty_cycle >= DRV_TIM_PWM_DUTY_CYCLE_ZERO
        && duty_cycle <= DRV_TIM_PWM_DUTY_CYCLE_MAX) {
        uint32_t compare = DRV_TIM_pwmDutyCycleToPwmCompare(duty_cycle);
        MCU_TIM_pwmSetCompare(compare);
    }
}

static uint32_t DRV_TIM_pwmDutyCycleToPwmCompare(DRV_TIM_PwmDutyCycle duty_cycle)
{
    // in our case, compare reg should go from [0 to 99] so:
    return duty_cycle;
}
