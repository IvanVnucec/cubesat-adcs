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
#include "adcs_pid.h"
#include "adcs_reg.h"
#include "adcs_rw.h"
#include "mpu9250/mpu9250.h"
#include "opt_req/get_quat_from_K.h"

/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum ADCS_RegulationMode_Enum {
    ADCS_REGULATION_MODE_ATTITUDE,
    ADCS_REGULATIOM_MODE_ANGULAR_VELOCITY,
    ADCS_REGULATION_MODE_NUM
} ADCS_RegulationMode_E;

typedef float ADCS_Quaternion_T[4];

typedef struct {
    struct0_T optReqHandle;
    MPU9250_Handle_s mpu9250Handle;
    ADCS_RW_Handle reactionWheelHandle;
    ADCS_REG_Handle adcsRegHandle;
    ADCS_PID_Handle pidHandleAngVel;
} ADCS_Handle_T;

/* Exported constants --------------------------------------------------------*/
#define ADCS_THREAD_PERIOD_IN_MILISECONDS (100U)
#define ADCS_THREAD_PERIOD_IN_SECONDS (ADCS_THREAD_PERIOD_IN_MILISECONDS / 1000.0f)
#define ADCS_REG_PQ (20.0f)
#define ADCS_REG_PW (4.0f)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_thread(void *argument);
void ADCS_delayMs(unsigned ms);
ADCS_RegulationMode_E ADCS_getRegulationMode(void);
void ADCS_setRegulationMode(ADCS_RegulationMode_E reg_mode);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_H */
