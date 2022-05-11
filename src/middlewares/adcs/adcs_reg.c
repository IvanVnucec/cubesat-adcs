/**
 ******************************************************************************
 * @file           : adcs_reg.c
 * @brief          : Source file.
 *                   This file contains the common code for the ADCS Attitude
 *                   regulator.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "adcs_reg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void ADCS_REG_quaternionConjugate(ADCS_REG_Quaternion *q_res,
                                         const ADCS_REG_Quaternion *q);
static void ADCS_REG_quaternionMultiply(ADCS_REG_Quaternion *k,
                                        const ADCS_REG_Quaternion *p,
                                        const ADCS_REG_Quaternion *q);

/* Private user code ---------------------------------------------------------*/

/**
 * @brief Initialize the regulator with the given constants. 
 * 
 * @param handle Regulator handle.
 * @param Pq Regulator constant (20.0f typical).
 * @param Pw Regulator constant ( 4.0f typical).
 */
void ADCS_REG_init(ADCS_REG_Handle *handle, const float Pq, const float Pw)
{
    handle->Pq = Pq;
    handle->Pw = Pw;

    handle->torque.tx = 0.0f;
    handle->torque.ty = 0.0f;
    handle->torque.tz = 0.0f;
}

/**
 * @brief Given the reference and measured quaternion and angular velocity vector, function calculates one iteration of the regulator.
 *        To get the regulator output torque use ADCS_REG_getTorque function.
 * 
 * @param handle Regulator handle.
 * @param q_ref Reference quaternion.
 * @param q_meas Measured quaternion.
 * @param ang_vel Measured angular velocity vector in radians per second.
 */
void ADCS_REG_iterate(ADCS_REG_Handle *handle,
                      const ADCS_REG_Quaternion *q_ref,
                      const ADCS_REG_Quaternion *q_meas,
                      const ADCS_REG_AngVel *ang_vel)
{
    ADCS_REG_Quaternion q_err;

    // calculate Axis_err
    ADCS_REG_quaternionMultiply(&q_err, q_ref, q_meas);
    /* if q0 < 0 then the desired rotation is more than pi 
    radians and the quaternion error is conjugate of itself */
    if (q_err.q0 < 0.0f) {
        ADCS_REG_quaternionConjugate(&q_err, &q_err);
    }

    // calculate torque
    handle->torque.tx = -handle->Pq * q_err.q1 - handle->Pw * ang_vel->wx;
    handle->torque.ty = -handle->Pq * q_err.q2 - handle->Pw * ang_vel->wy;
    handle->torque.tz = -handle->Pq * q_err.q3 - handle->Pw * ang_vel->wz;
}

/**
 * @brief Function gets the output regulator torque.
 * @note  Call this function after ADCS_REG_iterate to get the most recent calculated torque.
 * 
 * @param handle Regulator handle.
 * @param torque Regulator output torque. 
 */
void ADCS_REG_getTorque(ADCS_REG_Handle *handle, ADCS_REG_Torque *torque)
{
    *torque = handle->torque;
}

/**
 * @brief Function calculates the quaternion conjugate q* = [q0 -q1 -q2 -q3].
 * 
 * @param q_res Returned quaternion conjugate.
 * @param q Input quaternion.
 */
static void ADCS_REG_quaternionConjugate(ADCS_REG_Quaternion *q_res,
                                         const ADCS_REG_Quaternion *q)
{
    q_res->q0 = +q->q0;
    q_res->q1 = -q->q1;
    q_res->q2 = -q->q2;
    q_res->q3 = -q->q3;
}

/**
 * @brief Function calculates quaternion multiplication.
 * 
 * @param k Results of quaternion multiplication.
 * @param p First quaternion.
 * @param q Second quaternion.
 */
static void ADCS_REG_quaternionMultiply(ADCS_REG_Quaternion *k,
                                        const ADCS_REG_Quaternion *p,
                                        const ADCS_REG_Quaternion *q)
{
    k->q0 = p->q0 * q->q0 - p->q1 * q->q1 - p->q2 * q->q2 - p->q3 * q->q3;
    k->q1 = p->q0 * q->q1 + p->q1 * q->q0 + p->q2 * q->q3 - p->q3 * q->q2;
    k->q2 = p->q0 * q->q2 - p->q1 * q->q3 + p->q2 * q->q0 + p->q3 * q->q1;
    k->q3 = p->q0 * q->q3 + p->q1 * q->q2 - p->q2 * q->q1 + p->q3 * q->q0;
}
