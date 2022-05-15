/**
 ******************************************************************************
 * @file           : drv_tim.h
 * @brief          : Header for drv_tim.c file.
 *                   This file contains the common defines for the timer 
 *                   operations.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_TIM_H
#define __DRV_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum DRV_TIM_PwmDutyCycle {
    DRV_TIM_PWM_DUTY_CYCLE_ZERO = 0,
    DRV_TIM_PWM_DUTY_CYCLE_MAX  = 99    // determined by the timer configuration
} DRV_TIM_PwmDutyCycle;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void DRV_TIM_init(void);
void DRV_TIM_deInit(void);
void DRV_TIM_pwmStart(void);
void DRV_TIM_pwmStop(void);
void DRV_TIM_pwmSetDutyCycle(DRV_TIM_PwmDutyCycle duty_cycle);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_TIM_H */
