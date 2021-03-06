/**
 ******************************************************************************
 * @file           : lerp.c
 * @brief          : Source file.
 *                   This file contains the common defines for a linear 
 *                   interpolation (lerp).
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "lerp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
float lerp(float x, float x0, float x1, float y0, float y1)
{
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}
