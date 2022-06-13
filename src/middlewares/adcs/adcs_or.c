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
#include "opt_req/get_quat_from_K.h"
#include "opt_req/optimal_request.h"
#include "opt_req/optimal_request_init.h"

#include <math.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADCS_OPTREQ_MU_NOISE_VARIANCE (0.0008117f)             // no unit (normalized)
#define ADCS_OPTREQ_ETA_NOISE_VARIANCE (0.0000010f)            // no unit (normalized)
#define ADCS_OPTREQ_ACC_REF ((float[3]){0.0f, 0.0f, +1.0f})    // no unit (normalized)
#define ADCS_OPTREQ_MAG_REF \
    ((float[3]){0.459787381f, 0.037211461f, 0.887248859f})    // no unit (normalized)

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
    optReqHandle->dT            = ADCS_THREAD_PERIOD_IN_SECONDS;

    ADCS_IMU_getData(&imu_data);
    ADCS_OR_fillBMatrice(optReqHandle, imu_data.acc, imu_data.mag);
    ADCS_OR_fillWMatrice(optReqHandle, imu_data.gyr);

    optimal_request_init(optReqHandle);
}

void ADCS_OR_processImuData(struct0_T *optReqHandle, const ADCS_ImuData_T *imu_data)
{
    // normalize acc and mag measurements
    float acc[3]        = {imu_data->acc[0], imu_data->acc[1], imu_data->acc[2]};
    float mag[3]        = {imu_data->mag[0], imu_data->mag[1], imu_data->mag[2]};
    const float acc_len = sqrtf(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);
    const float mag_len = sqrtf(mag[0] * mag[0] + mag[1] * mag[1] + mag[2] * mag[2]);
    acc[0] /= acc_len;
    acc[1] /= acc_len;
    acc[2] /= acc_len;
    mag[0] /= mag_len;
    mag[1] /= mag_len;
    mag[2] /= mag_len;

    ADCS_OR_fillBMatrice(optReqHandle, acc, mag);
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
