/**
  ******************************************************************************
  * @file           : error.h
  * @brief          : Header for error.c file.
  *                   This file contains the common defines of the error 
  *                   handling.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ERROR_H
#define __ERROR_H

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
void ERROR_assert(int condition);
void ERROR_signalFaultCondition(void);
void ERROR_signalFaultConditionAndWaitIndefinitely(void);

#ifdef __cplusplus
}
#endif

#endif /* __ERROR_H */
