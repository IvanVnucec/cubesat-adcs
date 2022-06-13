/**
 ******************************************************************************
 * @file           : adcs_reg_quat.h
 * @brief          : Header for adcs_reg_quat.c file.
 *                   This file contains the common defines for the ADCS
 *                   Attitude regulator.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCS_REG_QUAT_H
#define __ADCS_REG_QUAT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Quaternion representation that Regulator uses.
 *        q = q0 + q1*i + q2*j + q3*k = [q0 q1 q2 q3]
 */
typedef struct ADCS_REG_Quaternion {
    float q0;
    float q1;
    float q2;
    float q3;
} ADCS_REG_QUAT_Quaternion;

/**
 * @brief Torque representation that Regulator uses.
 *        t = [tx ty tz]
 */
typedef struct ADCS_REG_QUAT_Torque {
    float tx;
    float ty;
    float tz;
} ADCS_REG_QUAT_Torque;

/**
 * @brief Regulator handle
 * 
 */
typedef struct ADCS_REG_QUAT_Handle {
    float Pq;
    float Pw;
    ADCS_REG_QUAT_Torque torque;
} ADCS_REG_QUAT_Handle;

typedef struct ADCS_REG_QUAT_AngVel {
    float wx;
    float wy;
    float wz;
} ADCS_REG_QUAT_AngVel;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_REG_QUAT_init(ADCS_REG_QUAT_Handle *handle, const float Pq, const float Pw);
void ADCS_REG_QUAT_iterate(ADCS_REG_QUAT_Handle *handle,
                      const ADCS_REG_QUAT_Quaternion *q_ref,
                      const ADCS_REG_QUAT_Quaternion *q_meas,
                      const ADCS_REG_QUAT_AngVel *ang_vel);
void ADCS_REG_QUAT_getTorque(ADCS_REG_QUAT_Handle *handle, ADCS_REG_QUAT_Torque *torque);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_REG_QUAT_H */
