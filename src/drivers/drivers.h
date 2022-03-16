/**
  ******************************************************************************
  * @file           : drivers.h
  * @brief          : Header for drivers.c file.
  *                   This file contains the common defines of the drivers.c file.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVERS_H
#define __DRIVERS_H

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
void DRIVERS_init(void);
void DRIVERS_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRIVERS_H */
