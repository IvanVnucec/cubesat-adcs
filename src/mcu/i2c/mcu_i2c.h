/**
  ******************************************************************************
  * @file    mcu_i2c.h
  * @brief   This file contains all the function prototypes for
  *          the mcu_i2c.c file
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_I2C_H__
#define __MCU_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

extern I2C_HandleTypeDef hi2c3;

void MCU_I2C_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MCU_I2C_H__ */
