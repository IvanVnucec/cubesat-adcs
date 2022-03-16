/**
  ******************************************************************************
  * @file           : bsp_led.h
  * @brief          : Header for bsp_led.c file.
  *                   This file contains the common defines for LED on board.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum { BSP_LED_INDEX_0, BSP_LED_INDEX_LENGTH } BSP_LED_Index;

typedef enum { BSP_LED_STATE_ON, BSP_LED_STATE_OFF, BSP_LED_STATE_LENGTH } BSP_LED_State;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void BSP_LED_init(void);
void BSP_LED_deinit(void);
void BSP_LED_setState(BSP_LED_Index index, BSP_LED_State state);
void BSP_LED_toggle(BSP_LED_Index index);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LED_H */
