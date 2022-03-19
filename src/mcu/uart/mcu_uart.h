/**
  ******************************************************************************
  * @file    mcu_uart.h
  * @brief   This file contains all the function prototypes for
  *          the mcu_uart.c file
  ******************************************************************************
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_UART_H__
#define __MCU_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

extern UART_HandleTypeDef huart1;

void MCU_UART_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MCU_UART_H__ */
