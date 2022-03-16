/**
  ******************************************************************************
  * @file           : clock.h
  * @brief          : Header for clock.c file.
  *                   This file contains the common defines for the MCU clock.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLOCK_H
#define __CLOCK_H

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
void CLOCK_systemClockInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __CLOCK_H */
