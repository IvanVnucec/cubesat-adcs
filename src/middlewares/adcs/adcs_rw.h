/**
 ******************************************************************************
 * @file           : adcs_rw.h
 * @brief          : Header for adcs_rw.c file.
 *                   This file contains the common defines of the application
 *                   defining Reaction Wheel API.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCS_RW_H
#define __ADCS_RW_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
    ADCS_RW_DUTY_CYCLE_MIN = 0u,
    ADCS_RW_DUTY_CYCLE_MAX = 100u,
} ADCS_RW_DutyCycle;

typedef enum {
    ADCS_RW_DIRECTION_CW,
    ADCS_RW_DIRECTION_CCW,
    ADCS_RW_DIRECTION_LEN,
} ADCS_RW_Direction;

typedef enum {
    ADCS_RW_STATUS_OK,
    ADCS_RW_STATUS_ERROR_DUTY_CYCLE,
    ADCS_RW_STATUS_ERROR_DIRECTION,
    ADCS_RW_STATUS_ERROR
} ADCS_RW_Status;

typedef struct {
    unsigned duty_cycle;
    ADCS_RW_Direction direction;
} ADCS_RW_Handle;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void ADCS_RW_init(ADCS_RW_Handle *handle, ADCS_RW_Status *status);
void ADCS_RW_setPwmDutyCycle(ADCS_RW_Handle *handle,
                             ADCS_RW_DutyCycle duty_cycle,
                             ADCS_RW_Status *status);
void ADCS_RW_setDirection(ADCS_RW_Handle *handle,
                          ADCS_RW_Direction dir,
                          ADCS_RW_Status *status);
void ADCS_RW_getPwmDutyCycle(ADCS_RW_Handle *handle,
                             ADCS_RW_DutyCycle *duty_cycle,
                             ADCS_RW_Status *status);
void ADCS_RW_getDirection(ADCS_RW_Handle *handle,
                          ADCS_RW_Direction *dir,
                          ADCS_RW_Status *status);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_RW_H */
