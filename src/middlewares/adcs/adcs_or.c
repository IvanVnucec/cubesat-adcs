/**
 ******************************************************************************
 * @file           : adcs_or.c
 * @brief          : Source file.
 *                   This file contains API functions for the Optimal Request 
 *                   algorithm.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "adcs_or.h"

#include "adcs.h"
#include "adcs_imu.h"
#include "libs/optimal_request/src/opt_req/get_quat_from_K.h"
#include "libs/optimal_request/src/opt_req/optimal_request.h"
#include "libs/optimal_request/src/opt_req/optimal_request_init.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADCS_OPTREQ_MU_NOISE_VARIANCE (0.0008117f)     // no unit
#define ADCS_OPTREQ_ETA_NOISE_VARIANCE (0.0000010f)    // no unit
#define ADCS_OPTREQ_ACC_REF \
    ((float[3]){-0.0518488f, -0.0495620f, -0.9974243f})                         // m/s^2
#define ADCS_OPTREQ_MAG_REF ((float[3]){0.8514420f, 0.5186020f, 0.0780922f})    // uT

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void ADCS_OR_fillRMatrice(struct0_T *optReqHandle,
                                 const float ref_acc[3],
                                 const float ref_mag[3]);
static void ADCS_OR_fillBMatrice(struct0_T *optReqHandle,
                                 const float bdy_acc[3],
                                 const float bdy_mag[3]);
static void ADCS_OR_fillWMatrice(struct0_T *optReqHandle, const float bdy_gyr[3]);

/* Private user code ---------------------------------------------------------*/
void ADCS_OR_init(struct0_T *optReqHandle)
{
    ADCS_ImuData_T imu_data;

    ADCS_OR_fillRMatrice(optReqHandle, ADCS_OPTREQ_ACC_REF, ADCS_OPTREQ_MAG_REF);

    optReqHandle->Mu_noise_var  = ADCS_OPTREQ_MU_NOISE_VARIANCE;
    optReqHandle->Eta_noise_var = ADCS_OPTREQ_ETA_NOISE_VARIANCE;
    optReqHandle->dT = ADCS_THREAD_PERIOD_IN_MILISECONDS / 1000.0f;    // in seconds

    ADCS_IMU_getData(&imu_data);
    ADCS_OR_fillBMatrice(optReqHandle, imu_data.acc, imu_data.mag);
    ADCS_OR_fillWMatrice(optReqHandle, imu_data.gyr);

    optimal_request_init(optReqHandle);
}

void ADCS_OR_processImuData(struct0_T *optReqHandle, const ADCS_ImuData_T *imu_data)
{
    ADCS_OR_fillBMatrice(optReqHandle, imu_data->acc, imu_data->mag);
    ADCS_OR_fillWMatrice(optReqHandle, imu_data->gyr);

    optimal_request(optReqHandle);
}

void ADCS_OR_getQuaternion(const struct0_T *optReqHandle, ADCS_Quaternion_T quat)
{
    get_quat_from_K(optReqHandle->K, quat);
}

static void ADCS_OR_fillRMatrice(struct0_T *optReqHandle,
                                 const float ref_acc[3],
                                 const float ref_mag[3])
{
    optReqHandle->r[0] = ref_acc[0];
    optReqHandle->r[1] = ref_acc[1];
    optReqHandle->r[2] = ref_acc[2];
    optReqHandle->r[3] = ref_mag[0];
    optReqHandle->r[4] = ref_mag[1];
    optReqHandle->r[5] = ref_mag[2];
}

static void ADCS_OR_fillBMatrice(struct0_T *optReqHandle,
                                 const float bdy_acc[3],
                                 const float bdy_mag[3])
{
    optReqHandle->b[0] = bdy_acc[0];
    optReqHandle->b[1] = bdy_acc[1];
    optReqHandle->b[2] = bdy_acc[2];
    optReqHandle->b[3] = bdy_mag[0];
    optReqHandle->b[4] = bdy_mag[1];
    optReqHandle->b[5] = bdy_mag[2];
}

static void ADCS_OR_fillWMatrice(struct0_T *optReqHandle, const float bdy_gyr[3])
{
    optReqHandle->w[0] = bdy_gyr[0];
    optReqHandle->w[1] = bdy_gyr[1];
    optReqHandle->w[2] = bdy_gyr[2];
}
