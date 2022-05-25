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

// thread stack sizes are being used by local variables
// thread heap sizes are being used by RTOS (mutexes, semaphores etc.)
// Warning: osPriorityHigh7 for example has higher priority than osPriorityHigh6
#define MIDDLEWARES_ALIVE_THREAD_STACK_SIZE (128 * 4)
#define MIDDLEWARES_ALIVE_THREAD_HEAP_SIZE (0)
#define MIDDLEWARES_ALIVE_THREAD_PRIORITY (osPriorityRealtime7)

#define MIDDLEWARES_ADCS_THREAD_STACK_SIZE (128 * 8)
#define MIDDLEWARES_ADCS_THREAD_HEAP_SIZE (0)
#define MIDDLEWARES_ADCS_THREAD_PRIORITY (osPriorityHigh7)

#define MIDDLEWARES_COMM_THREAD_STACK_SIZE (128 * 8)
#define MIDDLEWARES_COMM_THREAD_HEAP_SIZE (128 * 14)
#define MIDDLEWARES_COMM_THREAD_PRIORITY (osPriorityHigh6)

#define MIDDLEWARES_THREAD_STACK_SIZES                                        \
    (MIDDLEWARES_ALIVE_THREAD_STACK_SIZE + MIDDLEWARES_ADCS_THREAD_STACK_SIZE \
     + MIDDLEWARES_COMM_THREAD_STACK_SIZE)

#define MIDDLEWARES_THREAD_HEAP_SIZE                                        \
    (MIDDLEWARES_ALIVE_THREAD_HEAP_SIZE + MIDDLEWARES_ADCS_THREAD_HEAP_SIZE \
     + MIDDLEWARES_COMM_THREAD_HEAP_SIZE)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void MIDDLEWARES_initThreads(void);

#ifdef __cplusplus
}
#endif

#endif /* __MIDDLEWARES_H */
