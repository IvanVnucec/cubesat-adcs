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
#include "adcs_rw.h"
#include "mpu9250/mpu9250.h"
#include "opt_req/get_quat_from_K.h"

/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef float ADCS_Quaternion_T[4];

typedef struct {
    struct0_T optReqHandle;
    MPU9250_Handle_s mpu9250Handle;
    ADCS_RW_Handle reactionWheelHandle;
} ADCS_Handle_T;

/* Exported constants --------------------------------------------------------*/
#define ADCS_THREAD_PERIOD_IN_MILISECONDS (100U)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_thread(void *argument);
void ADCS_delayMs(unsigned ms);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_H */
