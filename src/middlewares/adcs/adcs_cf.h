/**
 ******************************************************************************
 * @file           : adcs_cf.h
 * @brief          : Header for adcs_cf.c file.
 *                   This file contains API functions for the Complementary 
 *                   Filter algorithm.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCS_CF_H
#define __ADCS_CF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "adcs_imu.h"

/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_CF_init(struct0_T *comp_filter_handle);
void ADCS_CF_processImuData(struct0_T *comp_filter_handle, ADCS_ImuData_T *imu_data);
void ADCS_CF_getQuaternion(ADCS_Quaternion_T quat);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_CF_H */
