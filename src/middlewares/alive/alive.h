/**
  ******************************************************************************
  * @file           : alive.h
  * @brief          : Header for alive.c file.
  *                   This file contains the common defines of the application
  *                   which signals that everything is executing.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ALIVE_H
#define __ALIVE_H

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
void ALIVE_thread(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __ALIVE_H */
