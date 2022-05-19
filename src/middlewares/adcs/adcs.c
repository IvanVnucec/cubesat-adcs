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
#include "adcs_reg.h"
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
static void ADCS_controlAttitude(const ADCS_Quaternion_T q_ref,
                                 const ADCS_Quaternion_T q_meas,
                                 const float gyr[3]);
static void ADCS_sendQuaternion(const ADCS_Quaternion_T quat);
static void ADCS_sendTorque(const ADCS_REG_Torque t);
static void ADCS_getRwDutyCycleAndDirectionBasedOnTorque(ADCS_RW_DutyCycle *rw_duty_cycle,
                                                         ADCS_RW_Direction *rw_direction,
                                                         float torque);

/* Private user code ---------------------------------------------------------*/
void ADCS_thread(void *argument)
{
    static ADCS_ImuData_T imu_data;
    static ADCS_Quaternion_T q_ref;
    static ADCS_Quaternion_T q_meas;

    ADCS_init();

    for (;;) {
        ADCS_IMU_getData(&imu_data);
        ADCS_determineAttitude(q_meas, &imu_data);

        // TODO: remove this, for test only
        q_ref[0] = 1.0f;
        q_ref[1] = 0.0f;
        q_ref[2] = 0.0f;
        q_ref[3] = 0.0f;
        ADCS_controlAttitude(q_ref, q_meas, imu_data.gyr);

        //ADCS_sendQuaternion(q_meas);
        ADCS_delayMs(ADCS_THREAD_PERIOD_IN_MILISECONDS);
    }
}

static void ADCS_init(void)
{
    ADCS_RW_Status rw_status = ADCS_RW_STATUS_ERROR;

    ADCS_IMU_init(&ADCS_handle);
    ADCS_OR_init(&ADCS_handle.optReqHandle);
    ADCS_RW_init(&ADCS_handle.reactionWheelHandle, &rw_status);

    // TODO: enable me once implemented
    //ERROR_assert(rw_status == ADCS_RW_STATUS_OK);

    ADCS_REG_init(&ADCS_handle.adcsRegHandle, ADCS_REG_PQ, ADCS_REG_PW);
}

static void ADCS_determineAttitude(ADCS_Quaternion_T quat, const ADCS_ImuData_T *imu_data)
{
    ADCS_OR_processImuData(&ADCS_handle.optReqHandle, imu_data);
    ADCS_OR_getQuaternion(&ADCS_handle.optReqHandle, quat);
}

static void ADCS_controlAttitude(const ADCS_Quaternion_T q_ref,
                                 const ADCS_Quaternion_T q_meas,
                                 const float gyr[3])
{
    ADCS_REG_Torque torque;
    ADCS_REG_Quaternion q_ref_reg;
    ADCS_REG_Quaternion q_meas_reg;
    ADCS_REG_AngVel ang_vel_reg;

    // populate ADCS_REG structures
    q_ref_reg.q0 = q_ref[0];
    q_ref_reg.q1 = q_ref[1];
    q_ref_reg.q2 = q_ref[2];
    q_ref_reg.q3 = q_ref[3];

    q_meas_reg.q0 = q_meas[0];
    q_meas_reg.q1 = q_meas[1];
    q_meas_reg.q2 = q_meas[2];
    q_meas_reg.q3 = q_meas[3];

    ang_vel_reg.wx = gyr[0];
    ang_vel_reg.wy = gyr[1];
    ang_vel_reg.wz = gyr[2];

    ADCS_REG_iterate(&ADCS_handle.adcsRegHandle, &q_ref_reg, &q_meas_reg, &ang_vel_reg);
    ADCS_REG_getTorque(&ADCS_handle.adcsRegHandle, &torque);
    ADCS_sendTorque(torque);

    // regulate only around z axis
    ADCS_RW_Status tw_status;
    ADCS_RW_DutyCycle rw_duty_cycle;
    ADCS_RW_Direction rw_direction;

    ADCS_getRwDutyCycleAndDirectionBasedOnTorque(&rw_duty_cycle,
                                                 &rw_direction,
                                                 torque.tz);
    ADCS_RW_setDirection(&ADCS_handle.reactionWheelHandle, rw_direction, &tw_status);
    ADCS_RW_setPwmDutyCycle(&ADCS_handle.reactionWheelHandle, rw_duty_cycle, &tw_status);
}

void ADCS_delayMs(unsigned ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

__attribute__((unused)) static void ADCS_sendQuaternion(const ADCS_Quaternion_T quat)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "q = [%.4f %.4f %.4f %.4f]\n",
                       quat[0],
                       quat[1],
                       quat[2],
                       quat[3]);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

static void ADCS_sendTorque(const ADCS_REG_Torque t)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "t = [%.4f %.4f %.4f]\n",
                       t.tx,
                       t.ty,
                       t.tz);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

static void ADCS_getRwDutyCycleAndDirectionBasedOnTorque(ADCS_RW_DutyCycle *rw_duty_cycle,
                                                         ADCS_RW_Direction *rw_direction,
                                                         float torque)
{
    // set direction and make torque positive if negative
    if (torque > 0.0f) {
        *rw_direction = ADCS_RW_DIRECTION_CCW;
    } else {
        *rw_direction = ADCS_RW_DIRECTION_CW;
        torque *= -1.0f;
    }

    // clamp torque
    const float max_torque = 20.0f;
    if (torque > max_torque)
        torque = max_torque;

    // TODO: create lerp function in utils because we are using it in multiple files
    // lerp y = y0 + (x - x0) * (y1 - y0) / (x1 - x0);
    const float x0 = 0.0f;
    const float x1 = max_torque;
    const float y0 = ADCS_RW_DUTY_CYCLE_MIN;
    const float y1 = ADCS_RW_DUTY_CYCLE_MAX;
    const float x  = torque;
    const float y  = y0 + (x - x0) * (y1 - y0) / (x1 - x0);

    *rw_duty_cycle = y;
}
