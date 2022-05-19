/**
 ******************************************************************************
 * @file           : lerp.h
 * @brief          : Header for lerp.c file.
 *                   This file contains the common defines for a linear 
 *                   interpolation (lerp).
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LERP_H
#define __LERP_H

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
float lerp(float x, float x0, float x1, float y0, float y1);

#ifdef __cplusplus
}
#endif

#endif /* __LERP_H */
