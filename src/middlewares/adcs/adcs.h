/**
  ******************************************************************************
  * @file           : adcs.h
  * @brief          : Header for adcs.c file.
  *                   This file contains the common defines of the application
  *                   which is determing and controling sattelite attitude.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCS_H
#define __ADCS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "adcs_cf.h"
#include "adcs_pid.h"
#include "adcs_rw.h"
#include "mpu9250/mpu9250.h"

/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum ADCS_RegulationMode_Enum {
    ADCS_REGULATION_MODE_ATTITUDE,
    ADCS_REGULATION_MODE_ANGULAR_VELOCITY,
    ADCS_REGULATION_MODE_NO_REGULATION,
    ADCS_REGULATION_MODE_NUM
} ADCS_RegulationMode_E;

typedef float ADCS_Quaternion_T[4];

typedef struct {
    ADCS_CD_Handle_S compFiltHandle;
    MPU9250_Handle_s mpu9250Handle;
    ADCS_RW_Handle reactionWheelHandle;
    ADCS_PID_Handle pidHandleAngVel;
    ADCS_PID_Handle pidHandleAngle;
} ADCS_Handle_T;

/* Exported constants --------------------------------------------------------*/
#define ADCS_THREAD_PERIOD_IN_MILISECONDS (100U)
#define ADCS_THREAD_PERIOD_IN_SECONDS (ADCS_THREAD_PERIOD_IN_MILISECONDS / 1000.0f)
#define ADCS_THREAD_FREQ_IN_HZ (1.0f / ADCS_THREAD_PERIOD_IN_SECONDS)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_thread(void *argument);
void ADCS_delayMs(unsigned ms);
ADCS_RegulationMode_E ADCS_getRegulationMode(void);
void ADCS_setRegulationMode(ADCS_RegulationMode_E reg_mode);
ADCS_PID_Handle *ADCS_getCurrentPidRegulatorHandle(void);
void ADCS_setRefAngleZInDeg(const float z_deg);
void ADCS_setRefAngVelZInRad(const float z_ang_vel);
void ADCS_setReactionWheelRefPwmDutyCycle(ADCS_RW_DutyCycle duty_cycle);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_H */
