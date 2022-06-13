/**
 ******************************************************************************
 * @file           : adcs_rw.c
 * @brief          : Source file.
 *                   This file contains the common code of the application
 *                   defining Reaction Wheel API.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "adcs_rw.h"

#include "bsp/bsp_config.h"
#include "drivers/user/drv_tim.h"
#include "utils/lerp/lerp.h"

#include <stddef.h>
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void ADCS_RW_initPwm(ADCS_RW_Status *status);
static void
    ADCS_RW_convertRwDutyCycleToPwmDutyCycle(DRV_TIM_PwmDutyCycle *drv_duty_cycle,
                                             const ADCS_RW_DutyCycle rw_duty_cycle);

/* Private user code ---------------------------------------------------------*/
void ADCS_RW_init(ADCS_RW_Handle *handle, ADCS_RW_Status *status)
{
    ADCS_RW_Status local_status = ADCS_RW_STATUS_ERROR;

    if (handle != NULL) {
        // init PWM
        ADCS_RW_initPwm(&local_status);

        if (local_status == ADCS_RW_STATUS_OK) {
            // set PWM to 0% and direction to CW
            ADCS_RW_setPwmDutyCycle(handle, ADCS_RW_DUTY_CYCLE_MIN, &local_status);
            if (local_status == ADCS_RW_STATUS_OK) {
                ADCS_RW_setDirection(handle, ADCS_RW_DIRECTION_CW, &local_status);
            }
        }
    }

    if (status != NULL) {
        *status = local_status;
    }
}

void ADCS_RW_setPwmDutyCycle(ADCS_RW_Handle *handle,
                             ADCS_RW_DutyCycle duty_cycle,
                             ADCS_RW_Status *status)
{
    ADCS_RW_Status local_status = ADCS_RW_STATUS_ERROR;

    if (handle != NULL) {
        if (duty_cycle >= ADCS_RW_DUTY_CYCLE_MIN
            && duty_cycle <= ADCS_RW_DUTY_CYCLE_MAX) {
            handle->duty_cycle = duty_cycle;

            DRV_TIM_PwmDutyCycle drv_duty_cycle;
            ADCS_RW_convertRwDutyCycleToPwmDutyCycle(&drv_duty_cycle, handle->duty_cycle);
            DRV_TIM_pwmSetDutyCycle(drv_duty_cycle);

            local_status = ADCS_RW_STATUS_OK;
        } else {
            local_status = ADCS_RW_STATUS_ERROR_DUTY_CYCLE;
        }
    }

    if (status != NULL) {
        *status = local_status;
    }
}

void ADCS_RW_setDirection(ADCS_RW_Handle *handle,
                          ADCS_RW_Direction dir,
                          ADCS_RW_Status *status)
{
    ADCS_RW_Status local_status = ADCS_RW_STATUS_ERROR;

    if (handle != NULL) {
        if (dir < ADCS_RW_DIRECTION_LEN) {
            if (dir == ADCS_RW_DIRECTION_CW) {
                // TODO: move this into drivers or somewhere
                HAL_GPIO_WritePin(BSP_CONFIG_REACTION_WHEEL_DIR_GPIO_Port,
                                  BSP_CONFIG_REACTION_WHEEL_DIR_Pin,
                                  GPIO_PIN_SET);
            } else if (dir == ADCS_RW_DIRECTION_CCW) {
                HAL_GPIO_WritePin(BSP_CONFIG_REACTION_WHEEL_DIR_GPIO_Port,
                                  BSP_CONFIG_REACTION_WHEEL_DIR_Pin,
                                  GPIO_PIN_RESET);
            }

            handle->direction = dir;
            local_status      = ADCS_RW_STATUS_OK;
        } else {
            local_status = ADCS_RW_STATUS_ERROR_DIRECTION;
        }
    }

    if (status != NULL) {
        *status = local_status;
    }
}

void ADCS_RW_getPwmDutyCycle(ADCS_RW_Handle *handle,
                             ADCS_RW_DutyCycle *duty_cycle,
                             ADCS_RW_Status *status)
{
    ADCS_RW_Status local_status = ADCS_RW_STATUS_ERROR;

    if (handle != NULL) {
        if (duty_cycle != NULL) {
            *duty_cycle  = handle->duty_cycle;
            local_status = ADCS_RW_STATUS_OK;
        } else {
            local_status = ADCS_RW_STATUS_ERROR;
        }
    }

    if (status != NULL) {
        *status = local_status;
    }
}

void ADCS_RW_getDirection(ADCS_RW_Handle *handle,
                          ADCS_RW_Direction *dir,
                          ADCS_RW_Status *status)
{
    ADCS_RW_Status local_status = ADCS_RW_STATUS_ERROR;

    if (handle != NULL) {
        if (dir != NULL) {
            *dir         = handle->direction;
            local_status = ADCS_RW_STATUS_OK;
        } else {
            local_status = ADCS_RW_STATUS_ERROR;
        }
    }

    if (status != NULL) {
        *status = local_status;
    }
}

void ADCS_RW_encoderIntCallback(const uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BSP_CONFIG_ENCODER_CHA_Pin) {
        // TODO: add logic for channel A
    } else if (GPIO_Pin == BSP_CONFIG_ENCODER_CHB_Pin) {
        // TODO: add logic for channel B
    } else {
        // ignore
    }
}

static void ADCS_RW_initPwm(ADCS_RW_Status *status)
{
    ADCS_RW_Status local_status = ADCS_RW_STATUS_ERROR;

    DRV_TIM_pwmStart();
    local_status = ADCS_RW_STATUS_OK;

    *status = local_status;
}

static void
    ADCS_RW_convertRwDutyCycleToPwmDutyCycle(DRV_TIM_PwmDutyCycle *drv_duty_cycle,
                                             const ADCS_RW_DutyCycle rw_duty_cycle)
{
    const float x0 = ADCS_RW_DUTY_CYCLE_MIN;
    const float x1 = ADCS_RW_DUTY_CYCLE_MAX;
    const float y0 = DRV_TIM_PWM_DUTY_CYCLE_ZERO;
    const float y1 = DRV_TIM_PWM_DUTY_CYCLE_MAX;
    const float x  = rw_duty_cycle;

    *drv_duty_cycle = lerp(x, x0, x1, y0, y1);
}
