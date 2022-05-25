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
    COMM_STATUS_NO_CALLBACK_FUNCTION,
    COMM_STATUS_PARSING_ERROR
} COMM_Status;

typedef void (*COMM_CallbackFunctionPtr)(const char *arg);

typedef struct {
    const char name[COMM_MESSAGE_MAX_BUFF_LEN];
    const COMM_CallbackFunctionPtr fun_ptr;
} COMM_CallbackFunction;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void COMM_thread(void *argument);
void COMM_sendMessage(const COMM_Message *const msg, COMM_Status *status);

#ifdef __cplusplus
}
#endif

#endif /* __COMM_H */
