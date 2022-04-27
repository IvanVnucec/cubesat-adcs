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
#include "cmsis_os2.h"

#include <stdint.h>

/* Private defines -----------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define COMM_MESSAGE_MAX_BUFF_LEN (40U)

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint8_t buffer[COMM_MESSAGE_MAX_BUFF_LEN];
    int msg_len;
} COMM_Message;

typedef enum {
    COMM_STATUS_OK,
    COMM_STATUS_ERROR,
    COMM_STATUS_NO_AVAILABLE_MESSAGE,
    COMM_STATUS_AVAILABLE_MESSAGE,
} COMM_Status;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void COMM_thread(void *argument);
void COMM_sendMessage(const COMM_Message *const msg, COMM_Status *status);

#ifdef __cplusplus
}
#endif

#endif /* __COMM_H */
