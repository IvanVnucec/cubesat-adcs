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
#include "libs/optimal_request/src/opt_req/get_quat_from_K.h"

/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef float ADCS_Quaternion_T[4];

typedef struct {
    float acc[3];
    float mag[3];
    float gyr[3];
} ADCS_ImuData_T;

typedef struct {
    struct0_T optReqHandle;
} ADCS_Handle_T;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_thread(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_H */
