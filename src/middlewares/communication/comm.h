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
#include <stdint.h>
#include "cmsis_os2.h"


/* Private defines -----------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define COMM_MESSAGE_MAX_BUFF_LEN (32U)

/* Exported types ------------------------------------------------------------*/
typedef struct {                                // object data type
  uint8_t Buf[COMM_MESSAGE_MAX_BUFF_LEN];
  uint8_t Idx;
} COMM_Message;

typedef enum {
    COMM_STATUS_OK,
    COMM_STATUS_ERROR,
} COMM_Status;


/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void COMM_thread(void *argument);
void COMM_sendMessage(const COMM_Message *const msg, COMM_Status *status);

#ifdef __cplusplus
}
#endif

#endif /* __COMM_H */
