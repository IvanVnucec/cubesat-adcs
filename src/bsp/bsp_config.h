/**
  ******************************************************************************
  * @file           : bsp_config.h
  * @brief          : Header for Board Support Package (BSP).
  *                   This file contains the common defines for BSP.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define BSP_CONFIG_SWDIO_Pin GPIO_PIN_13
#define BSP_CONFIG_SWDIO_GPIO_Port GPIOA
#define BSP_CONFIG_SWCLK_Pin GPIO_PIN_14
#define BSP_CONFIG_SWCLK_GPIO_Port GPIOA
#define BSP_CONFIG_LD3_Pin GPIO_PIN_3
#define BSP_CONFIG_LD3_GPIO_Port GPIOB
#define BSP_CONFIG_REACTION_WHEEL_DIR_Pin GPIO_PIN_3
#define BSP_CONFIG_REACTION_WHEEL_DIR_GPIO_Port GPIOH

/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __BSP_CONFIG_H */
