/**
 ******************************************************************************
 * @file           : adcs_or.h
 * @brief          : Header for adcs_or.c file.
 *                   This file contains API functions for the Optimal Request 
 *                   algorithm.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCS_OR_H
#define __ADCS_OR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "adcs_imu.h"
#include "opt_req/optimal_request.h"

/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_OR_init(struct0_T *optReqHandle);
void ADCS_OR_processImuData(struct0_T *optReqHandle, const ADCS_ImuData_T *imu_data);
void ADCS_OR_getQuaternion(const struct0_T *optReqHandle, ADCS_Quaternion_T quat);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_OR_H */
