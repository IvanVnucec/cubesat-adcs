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
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_thread(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_H */
