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

#include <stddef.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void ADCS_RW_initPwm(ADCS_RW_Status *status);

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
            // TODO: set pwm to match duty cycle (from 0 to 100)

            handle->duty_cycle = duty_cycle;
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
                // TODO: set gpio high or low
            } else if (dir == ADCS_RW_DIRECTION_CCW) {
                // TODO: set gpio low or high
            }

            handle->direction = dir;
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
            *duty_cycle = handle->duty_cycle;
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
            *dir = handle->direction;
        } else {
            local_status = ADCS_RW_STATUS_ERROR;
        }
    }

    if (status != NULL) {
        *status = local_status;
    }
}

static void ADCS_RW_initPwm(ADCS_RW_Status *status)
{
    ADCS_RW_Status local_status = ADCS_RW_STATUS_ERROR;

    // TODO

    *status = local_status;
}
