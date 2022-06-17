/**
 ******************************************************************************
 * @file           : adcs_cf.h
 * @brief          : Header for adcs_cf.c file.
 *                   This file contains API functions for the Complementary 
 *                   Filter algorithm.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCS_CF_H
#define __ADCS_CF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "adcs_imu.h"
#include "comp_filt_init_types.h"

/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct0_T ADCS_CD_Handle_S;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void ADCS_CF_init(ADCS_CD_Handle_S *comp_filter_handle);
void ADCS_CF_step(ADCS_CD_Handle_S *comp_filter_handle,
                  ADCS_ImuData_T *imu_data,
                  float qib_est[4]);

#ifdef __cplusplus
}
#endif

#endif /* __ADCS_CF_H */
