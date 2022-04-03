/**
  ******************************************************************************
  * @file           : middlewares.h
  * @brief          : Header for middlewares.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MIDDLEWARES_H
#define __MIDDLEWARES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define MIDDLEWARES_ALIVE_THREAD_STACK_SIZE (128 * 4)
#define MIDDLEWARES_ADCS_THREAD_STACK_SIZE (128 * 8)
#define MIDDLEWARES_COMM_THREAD_STACK_SIZE (128 * 4)

#define MIDDLEWARES_THREAD_STACK_SIZES                                        \
    (MIDDLEWARES_ALIVE_THREAD_STACK_SIZE + MIDDLEWARES_ADCS_THREAD_STACK_SIZE \
     + MIDDLEWARES_COMM_THREAD_STACK_SIZE)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void MIDDLEWARES_initThreads(void);

#ifdef __cplusplus
}
#endif

#endif /* __MIDDLEWARES_H */
