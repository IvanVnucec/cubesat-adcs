/**
  ******************************************************************************
  * @file           : bsp_led_config.h
  * @brief          : Header for bsp_led_config.c file.
  *                   This file contains the common defines for LED on board.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LED_CONFIG_H
#define __BSP_LED_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "stm32l412xx.h"

/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define BSP_LED_CONFIG_LED_INDEX_0_GPIO_Port GPIOB
#define BSP_LED_CONFIG_LED_INDEX_0_Pin GPIO_PIN_3

/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LED_CONFIG_H */
