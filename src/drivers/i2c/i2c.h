/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

extern I2C_HandleTypeDef hi2c3;

void MX_I2C3_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
