/**
  ******************************************************************************
  * @file           : mpu.h
  * @brief          : Header for mpu.c file.
  *                   This file contains the common defines of the application
  *                   which gets the data from the imu.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MPU_H
#define __MPU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void MPU_thread(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __MPU_H */
