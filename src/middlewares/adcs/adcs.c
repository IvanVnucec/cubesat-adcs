/**
  ******************************************************************************
  * @file           : adcs.c
  * @brief          : Source file.
  *                   This file contains the common code of the application
  *                   which is determing and controling sattelite attitude.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adcs.h"

#include "FreeRTOS.h"
#include "adcs_imu.h"
#include "libs/optimal_request/src/opt_req/get_quat_from_K.h"
#include "libs/optimal_request/src/opt_req/optimal_request.h"
#include "libs/optimal_request/src/opt_req/optimal_request_init.h"
#include "task.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ADCS_OPTREQ_MU_NOISE_VARIANCE (0.0008117f)     // no unit
#define ADCS_OPTREQ_ETA_NOISE_VARIANCE (0.0000010f)    // no unit
#define ADCS_OPTREQ_TIME_PERIOD_IN_MILISEC (100u)      // miliseconds

/* Private variables ---------------------------------------------------------*/
static ADCS_Handle_T ADCS_handle;

/* Private function prototypes -----------------------------------------------*/
static void ADCS_init(void);
static void ADCS_determineAttitude(ADCS_Quaternion_T quat,
                                   const ADCS_ImuData_T *imu_data);
static void ADCS_controlAttitude(const ADCS_Quaternion_T quat, const float time_delta);
static void ADCS_initOptReq(struct0_T *optReqHandle);
static void ADCS_optReqFillRMatrice(struct0_T *optReqHandle,
                                    const float ref_acc[3],
                                    const float ref_mag[3]);
static void ADCS_optReqFillBMatrice(struct0_T *optReqHandle,
                                    const float bdy_acc[3],
                                    const float bdy_mag[3]);
static void ADCS_optReqFillWMatrice(struct0_T *optReqHandle, const float bdy_gyr[3]);

/* Private user code ---------------------------------------------------------*/
void ADCS_thread(void *argument)
{
    static ADCS_ImuData_T imu_data;
    static ADCS_Quaternion_T quat;
    static float time_delta;

    ADCS_init();

    for (;;) {
        ADCS_IMU_getData(&imu_data);
        ADCS_determineAttitude(quat, &imu_data);

        // TODO: calculate delta
        ADCS_controlAttitude(quat, time_delta);

        ADCS_delayMs(ADCS_THREAD_PERIOD_IN_MILISECONDS);
    }
}

static void ADCS_init(void)
{
    ADCS_IMU_init(&ADCS_handle);
    ADCS_initOptReq(&ADCS_handle.optReqHandle);
}

static void ADCS_determineAttitude(ADCS_Quaternion_T quat, const ADCS_ImuData_T *imu_data)
{
    ADCS_optReqFillBMatrice(&ADCS_handle.optReqHandle, imu_data->acc, imu_data->mag);
    ADCS_optReqFillWMatrice(&ADCS_handle.optReqHandle, imu_data->gyr);

    optimal_request(&ADCS_handle.optReqHandle);

    get_quat_from_K(ADCS_handle.optReqHandle.K, quat);
}

static void ADCS_controlAttitude(const ADCS_Quaternion_T quat, const float time_delta)
{
}

void ADCS_delayMs(unsigned ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

static void ADCS_initOptReq(struct0_T *optReqHandle)
{
    static const float ADCS_OPTREQ_ACC_REF[3] = {-0.0518488f,
                                                 -0.0495620f,
                                                 -0.9974243f};    // m/s^2
    static const float ADCS_OPTREQ_MAG_REF[3] = {0.8514420f,
                                                 0.5186020f,
                                                 0.0780922f};    // uT

    ADCS_ImuData_T imu_data;

    ADCS_optReqFillRMatrice(optReqHandle, ADCS_OPTREQ_ACC_REF, ADCS_OPTREQ_MAG_REF);

    optReqHandle->Mu_noise_var  = 0.0008117f;
    optReqHandle->Eta_noise_var = 0.0000010f;
    optReqHandle->dT            = 10.0f;

    ADCS_IMU_getData(&imu_data);
    ADCS_optReqFillBMatrice(optReqHandle, imu_data.acc, imu_data.mag);
    ADCS_optReqFillWMatrice(optReqHandle, imu_data.gyr);

    optimal_request_init(optReqHandle);
}

static void ADCS_optReqFillRMatrice(struct0_T *optReqHandle,
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

static void ADCS_optReqFillBMatrice(struct0_T *optReqHandle,
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

static void ADCS_optReqFillWMatrice(struct0_T *optReqHandle, const float bdy_gyr[3])
{
    optReqHandle->w[0] = bdy_gyr[0];
    optReqHandle->w[1] = bdy_gyr[1];
    optReqHandle->w[2] = bdy_gyr[2];
}