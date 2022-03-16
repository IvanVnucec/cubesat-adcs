/**
  ******************************************************************************
  * @file           : bsp.h
  * @brief          : Header for bsp.c file.
  *                   This file contains the common defines for the BSP layer.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void BSP_init(void);
void BSP_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_H */
