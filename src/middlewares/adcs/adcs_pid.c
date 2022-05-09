/**
 ******************************************************************************
 * @file           : adcs_pid.c
 * @brief          : Source file.
 *                   This file contains the common defines of the application
 *                   for PID regulator.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "adcs_pid.h"

#include <stddef.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/**
 * Initialize PID regulator
 * 
 * ADCS_PID_Handle *hpid - PID regulator handle. Regulator has Parrarel structure. 
 *    See documentation folder for more info.
 * ADCS_PID_RegulatorCoeffs - regulator coefficients
 *     float Kp - Proportional coefficient
 *     float Ki - Integral coefficient
 *     float Kd - Derivative coefficient
 *     float V  - Low pass coefficint in derivative term (typ. 5 to 20).
 *        Higher the value, lower the cutt-off frequency.
 *     float Ts - Sample time in seconds
 * ADCS_PID_Status *status - Return status
 * 
 * This function can be called when the User wants to change some coefficients.
 * But the function will also reset the Integral part of the controller to 
 * zero.
 **/
void ADCS_PID_init(ADCS_PID_Handle *hpid,
                   const ADCS_PID_RegulatorCoeffs *coeffs,
                   ADCS_PID_Status *status)
{
    ADCS_PID_Status local_status = ADCS_PID_STATUS_ERROR;

    if (hpid != NULL) {
        if (coeffs != NULL) {
            if (coeffs->Ts >= 0.0f) {
                hpid->coeffs.Kp = coeffs->Kp;
                hpid->coeffs.Ki = coeffs->Ki;
                hpid->coeffs.Kd = coeffs->Kd;
                hpid->coeffs.Ts = coeffs->Ts;
                hpid->coeffs.V  = coeffs->V;

                hpid->up0 = 0.0f;
                hpid->ui0 = 0.0f;
                hpid->ui1 = 0.0f;
                hpid->ud0 = 0.0f;
                hpid->ud1 = 0.0f;
                hpid->u0  = 0.0f;
                hpid->e0  = 0.0f;
                hpid->e1  = 0.0f;

                local_status = ADCS_PID_STATUS_OK;
            }
        }
    }

    if (status != NULL) {
        *status = local_status;
    }
}

/**
 * Function for the PID regulator output.
 * 
 * ADCS_PID_Handle *hpid - PID regulator handle
 * float *out - Output of the pid regulator
 * float error - defined as Set_point_value minus Measured_value
 * ADCS_PID_Status *status - output status
 * */
void ADCS_PID_regulate(ADCS_PID_Handle *hpid,
                       float *out,
                       float error,
                       ADCS_PID_Status *status)
{
    ADCS_PID_Status local_status = ADCS_PID_STATUS_ERROR;

    if (hpid != NULL) {
        /* transform PID coeff from Parralel to Standard form. See documentation */
        const float Kr = hpid->coeffs.Kp;
        const float Ti = Kr / hpid->coeffs.Ki;
        const float Td = hpid->coeffs.Kd / Kr;
        const float T  = hpid->coeffs.Ts;
        const float v  = hpid->coeffs.V;

        /* e(k) = error */
        hpid->e0 = error;

        /* up(k)  = Kr *   e(k) */
        hpid->up0 = Kr * hpid->e0;

        /* ui(k)  = ui(k-1)   + Kr * T / Ti * (  e(k)   + e(k-1)  ) / 2.0f */
        hpid->ui0 = hpid->ui1 + Kr * T / Ti * (hpid->e0 + hpid->e1) / 2.0f;

        /* ud(k)  = Td / (Td + v * T) * ud(k-1)   + Kr * v * Td / (Td + v * T) * (  e(k)   - e(k-1)  ) */
        hpid->ud0 = Td / (Td + v * T) * hpid->ud1
                    + Kr * v * Td / (Td + v * T) * (hpid->e0 - hpid->e1);

        /* u(k)  =    up(k)  +    ui(k)  +    ud(k) */
        hpid->u0 = hpid->up0 + hpid->ui0 + hpid->ud0;

        /* u(k-1) = u(k) */
        /* e(k-1) = e(k) */
        hpid->ui1 = hpid->ui0;
        hpid->ud1 = hpid->ud0;
        hpid->e1  = hpid->e0;

        /* Return regulator output value */
        if (out != NULL) {
            *out = hpid->u0;

            local_status = ADCS_PID_STATUS_OK;
        }
    }

    if (status != NULL) {
        *status = local_status;
    }
}

/**
 * @brief Reset the Integral part of the PID regulator
 * 
 * @param hpid PID regulator handle
 * @param status Return status:
 */
void ADCS_PID_resetIntegral(ADCS_PID_Handle *hpid, ADCS_PID_Status *status)
{
    ADCS_PID_Status local_status = ADCS_PID_STATUS_ERROR;

    if (hpid != NULL) {
        hpid->ui1 = 0.0f;

        local_status = ADCS_PID_STATUS_OK;
    }

    if (status != NULL) {
        *status = local_status;
    }
}
