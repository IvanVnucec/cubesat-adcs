/**
 ******************************************************************************
 * @file           : adcs_imu.h
 * @brief          : Header for adcs_imu.c file.
 *                   This file contains files for the Inertial Measurement 
 *                   Unit.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCS_IMU_H
#define __ADCS_IMU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mpu9250/mpu9250.h"

/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    float acc[3];
    float mag[3];
    float gyr[3];
} ADCS_ImuData_T;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_IMU_init(MPU9250_Handle_s *mpu9250Handle);
void ADCS_IMU_getData(ADCS_ImuData_T *imu_data);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_IMU_H */
