/**
 ******************************************************************************
 * @file           : adcs_pid.h
 * @brief          : Header for adcs_pid.c file.
 *                   This file contains the common defines of the application
 *                   for PID regulator.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCS_PID_H
#define __ADCS_PID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum { ADCS_PID_STATUS_OK, ADCS_PID_STATUS_ERROR } ADCS_PID_Status;

/**
 * @brief PID Regulator Coefficients
 *        float Kp - Proportional coefficient
 *        float Ki - Integral coefficient
 *        float Kd - Derivative coefficient
 *        float V  - Low pass coefficint in derivative term (typ. 5 to 20).
 *           Higher the value, lower the cutt-off frequency.
 *        float Ts - Sample time in seconds
 */
typedef struct {
    float Kp; /* Proportional */
    float Ki; /* Integral 	*/
    float Kd; /* Derivative 	*/
    float Ts; /* Sample time 	*/
    float V;  /* Real derivative term */
} ADCS_PID_RegulatorCoeffs;

typedef struct {
    ADCS_PID_RegulatorCoeffs coeffs;

    float up0; /* output of P term up(k) */
    float ui0; /* output of I term ui(k) */
    float ui1; /* output of I term ui(k-1) */
    float ud0; /* output of D term ud(k) */
    float ud1; /* output of D term ud(k-1) */
    float u0;  /* output of PID regulator u(k) */

    float e0;
    float e1;
} ADCS_PID_Handle;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void ADCS_PID_init(ADCS_PID_Handle *hpid,
                   const ADCS_PID_RegulatorCoeffs *coeffs,
                   ADCS_PID_Status *status);
void ADCS_PID_regulate(ADCS_PID_Handle *hpid,
                       float *out,
                       float error,
                       ADCS_PID_Status *status);
void ADCS_PID_resetIntegral(ADCS_PID_Handle *hpid, ADCS_PID_Status *status);
void ADCS_PID_updateCoeffs(ADCS_PID_Handle *hpid,
                           const ADCS_PID_RegulatorCoeffs *coeffs,
                           ADCS_PID_Status *status);
void ADCS_PID_getCoeffs(const ADCS_PID_Handle *hpid, ADCS_PID_RegulatorCoeffs *coeffs);
void ADCS_PID_getControllerEffort(const ADCS_PID_Handle *hpid, float *effort);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_PID_H */
