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
#include "adcs_or.h"
#include "middlewares/communication/comm.h"
#include "printf/printf.h"
#include "task.h"
#include "utils/error/error.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ADCS_Handle_T ADCS_handle;

/* Private function prototypes -----------------------------------------------*/
static void ADCS_init(void);
static void ADCS_determineAttitude(ADCS_Quaternion_T quat,
                                   const ADCS_ImuData_T *imu_data);
static void ADCS_controlAttitude(const ADCS_Quaternion_T quat, const float time_delta);
static void ADCS_sendQuaternion(const ADCS_Quaternion_T quat);

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

        ADCS_sendQuaternion(quat);
        ADCS_delayMs(ADCS_THREAD_PERIOD_IN_MILISECONDS);
    }
}

static void ADCS_init(void)
{
    ADCS_IMU_init(&ADCS_handle);
    ADCS_OR_init(&ADCS_handle.optReqHandle);
}

static void ADCS_determineAttitude(ADCS_Quaternion_T quat, const ADCS_ImuData_T *imu_data)
{
    ADCS_OR_processImuData(&ADCS_handle.optReqHandle, imu_data);
    ADCS_OR_getQuaternion(&ADCS_handle.optReqHandle, quat);
}

static void ADCS_controlAttitude(const ADCS_Quaternion_T quat, const float time_delta)
{
}

void ADCS_delayMs(unsigned ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

static void ADCS_sendQuaternion(const ADCS_Quaternion_T quat)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "[%.4f %.4f %.4f %.4f]\n",
                       quat[0],
                       quat[1],
                       quat[2],
                       quat[3]);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}
