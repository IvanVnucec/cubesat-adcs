/**
 ******************************************************************************
 * @file           : adcs_cf.c
 * @brief          : Source file.
 *                   This file contains API functions for the Complementary 
 *                   Filter algorithm.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "adcs_cf.h"

#include "adcs.h"
#include "adcs_imu.h"
#include "comp_filt_init.h"
#include "comp_filt_step.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const float ADCS_CF_acc_i[3] = {0.0f, 0.0f, -9.81f}; // m/s/s
static const float ADCS_CF_mag_i[3] = {22.2f, 1.7f, 42.7f}; // uT
static const float ADCS_CF_fs       = ADCS_THREAD_FREQ_IN_HZ;
static const float ADCS_CF_K_cf     = 0.9f;
static const float ADCS_CF_dJ_max   = 0.005f;
static const float ADCS_CF_N_max    = 10.0f;
static const float ADCS_CF_alpha    = 0.1f;

/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
void ADCS_CF_init(ADCS_CD_Handle_S *comp_filter_handle)
{
    comp_filt_init(ADCS_CF_acc_i, 
        ADCS_CF_mag_i,
        ADCS_CF_fs,
        ADCS_CF_K_cf,
        ADCS_CF_dJ_max,
        ADCS_CF_N_max,
        ADCS_CF_alpha,
        comp_filter_handle);
}

void ADCS_CF_step(const ADCS_CD_Handle_S *comp_filter_handle, ADCS_ImuData_T *imu_data, float qib_est[4])
{
    comp_filt_step(comp_filter_handle, imu_data->acc, imu_data->mag, imu_data->gyr, qib_est);
}
