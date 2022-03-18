/**
  ******************************************************************************
  * @file           : comm.h
  * @brief          : Header for comm.c file.
  *                   This file contains the common defines of the application
  *                   which gets and sends commands over bluetooth module.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMM_H
#define __COMM_H

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
void COMM_thread(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __COMM_H */
