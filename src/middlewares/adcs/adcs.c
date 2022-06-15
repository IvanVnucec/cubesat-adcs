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
#include "adcs_pid.h"
#include "cmsis_os2.h"
#include "middlewares/communication/comm.h"
#include "printf/printf.h"
#include "stm32l4xx_hal.h"
#include "task.h"
#include "utils/error/error.h"
#include "utils/lerp/lerp.h"
#include <math.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ADCS_Handle_T ADCS_handle;
static ADCS_RegulationMode_E ADCS_regulation_mode = ADCS_REGULATION_MODE_ATTITUDE;

// angular velocity regulator params
static const ADCS_PID_RegulatorCoeffs ADCS_pid_coeffs_angvel = {
    .Kp = 42.5f,
    .Ki = 14.8f,
    .Kd = 0.0f,
    .Ts = ADCS_THREAD_PERIOD_IN_SECONDS,
    .V  = 15.0f};
static const float ADCS_pid_angvel_max_out = 100.0f;

// angle regulator params
static const ADCS_PID_RegulatorCoeffs ADCS_pid_coeffs_angle = {
    .Kp = 36.1f,
    .Ki = 3.6f,
    .Kd = 91.0f,
    .Ts = ADCS_THREAD_PERIOD_IN_SECONDS,
    .V  = 15.0f};
static const float ADCS_pid_angle_max_out = 100.0f;

/* Private function prototypes -----------------------------------------------*/
static void ADCS_init(void);
static void ADCS_determineAttitude(ADCS_Quaternion_T quat,
                                   const ADCS_ImuData_T *imu_data);
static void ADCS_controlAngVel(const float ang_vel_ref, const float ang_vel);
static void ADCS_controlAngle(const float angle_ref, const float angle);
static void ADCS_sendQuaternion(const ADCS_Quaternion_T quat);
static void ADCS_sendAngVel(const float w[3]);
static void ADCS_sendAngVelPidOutput(const float pid_out);
static void ADCS_sendAnglePidOutput(const float pid_out);
static void ADCS_calculateEulerAngles(float e[3], const ADCS_Quaternion_T q);
static void ADCS_sendEulerAngles(const float e[3]);
static void ADCS_sendMagneticField(const float m[3]);
// TODO: move regulator stuff in its own files
static void ADCS_getRwDutyCycleAndDirectionBasedOnPidAngVelRegOut(
    ADCS_RW_DutyCycle *rw_duty_cycle,
    ADCS_RW_Direction *rw_direction,
    float pid_reg_out);
static void ADCS_getRwDutyCycleAndDirectionBasedOnPidAngleRegOut(
    ADCS_RW_DutyCycle *rw_duty_cycle,
    ADCS_RW_Direction *rw_direction,
    float pid_reg_out);
/* Private user code ---------------------------------------------------------*/
void ADCS_thread(void *argument)
{
    static ADCS_ImuData_T imu_data;
    static ADCS_Quaternion_T q_meas;
    static uint32_t tick;
    static ADCS_RegulationMode_E reg_mode;
    static float euler_angles[3];
    static const float euler_angles_ref[3] = {0.0f, 0.0f, 0.0f};

    // give time for sattelite to stabilize
    ADCS_delayMs(3000);

    ADCS_init();

    tick = osKernelGetTickCount();
    for (;;) {
        ADCS_IMU_getData(&imu_data);

        reg_mode = ADCS_getRegulationMode();
        if (reg_mode == ADCS_REGULATION_MODE_ATTITUDE) {
            ADCS_determineAttitude(q_meas, &imu_data);

            ADCS_calculateEulerAngles(euler_angles, q_meas);
            // first one (0th term) is the angle around z axis
            ADCS_controlAngle(euler_angles_ref[0], euler_angles[0]);
            
            //ADCS_sendQuaternion(q_meas);
            ADCS_sendEulerAngles(euler_angles);

            //ADCS_sendMagneticField(imu_data.mag);

        } else if (reg_mode == ADCS_REGULATION_MODE_ANGULAR_VELOCITY) {
            ADCS_sendAngVel(imu_data.gyr);

        } else {
            // throw error
            ERROR_assert(0);
        }

        tick += ADCS_THREAD_PERIOD_IN_MILISECONDS;    // overflow is safe
        osDelayUntil(tick);
    }
}

static void ADCS_init(void)
{
    ADCS_RW_Status rw_status   = ADCS_RW_STATUS_ERROR;
    ADCS_PID_Status pid_status = ADCS_PID_STATUS_ERROR;

    ADCS_IMU_init(&ADCS_handle);
    ADCS_OR_init(&ADCS_handle.optReqHandle);
    ADCS_RW_init(&ADCS_handle.reactionWheelHandle, &rw_status);
    ERROR_assert(rw_status == ADCS_RW_STATUS_OK);

    ADCS_PID_init(&ADCS_handle.pidHandleAngVel, &ADCS_pid_coeffs_angvel, &pid_status);
    ERROR_assert(pid_status == ADCS_PID_STATUS_OK);

    ADCS_PID_init(&ADCS_handle.pidHandleAngle, &ADCS_pid_coeffs_angle, &pid_status);
    ERROR_assert(pid_status == ADCS_PID_STATUS_OK);
}

static void ADCS_determineAttitude(ADCS_Quaternion_T quat, const ADCS_ImuData_T *imu_data)
{
    ADCS_OR_processImuData(&ADCS_handle.optReqHandle, imu_data);
    ADCS_OR_getQuaternion(&ADCS_handle.optReqHandle, quat);
}

static void ADCS_controlAngVel(const float ang_vel_ref, const float ang_vel)
{
    float reg_out;
    ADCS_PID_Status pid_status = ADCS_PID_STATUS_ERROR;
    ADCS_RW_Status tw_status;
    ADCS_RW_DutyCycle rw_duty_cycle;
    ADCS_RW_Direction rw_direction;
    const float ang_vel_error = ang_vel_ref - ang_vel;

    ADCS_PID_regulate(&ADCS_handle.pidHandleAngVel, &reg_out, ang_vel_error, &pid_status);
    ERROR_assert(pid_status == ADCS_PID_STATUS_OK);

    ADCS_getRwDutyCycleAndDirectionBasedOnPidAngVelRegOut(&rw_duty_cycle,
                                                          &rw_direction,
                                                          reg_out);
    ADCS_RW_setDirection(&ADCS_handle.reactionWheelHandle, rw_direction, &tw_status);
    ADCS_RW_setPwmDutyCycle(&ADCS_handle.reactionWheelHandle, rw_duty_cycle, &tw_status);

    //ADCS_sendAngVelPidOutput(reg_out);
}

static void ADCS_controlAngle(const float angle_ref, const float angle)
{
    float reg_out;
    ADCS_PID_Status pid_status = ADCS_PID_STATUS_ERROR;
    ADCS_RW_Status tw_status;
    ADCS_RW_DutyCycle rw_duty_cycle;
    ADCS_RW_Direction rw_direction;
    const float angle_error = angle_ref - angle;

    ADCS_PID_regulate(&ADCS_handle.pidHandleAngle, &reg_out, angle_error, &pid_status);
    ERROR_assert(pid_status == ADCS_PID_STATUS_OK);

    ADCS_getRwDutyCycleAndDirectionBasedOnPidAngleRegOut(&rw_duty_cycle,
                                                          &rw_direction,
                                                          reg_out);
    ADCS_RW_setDirection(&ADCS_handle.reactionWheelHandle, rw_direction, &tw_status);
    ADCS_RW_setPwmDutyCycle(&ADCS_handle.reactionWheelHandle, rw_duty_cycle, &tw_status);

    //ADCS_sendAnglePidOutput(reg_out);
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

__attribute__((unused)) static void ADCS_calculateEulerAngles(float e[3], const ADCS_Quaternion_T q)
{
    e[0] = atan2f(+2.0f * (q[1]*q[2] + q[0]*q[3]), (q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3]));
    e[1] =  asinf(-2.0f * (q[1]*q[3] - q[0]*q[2]));
    e[2] = atan2f(+2.0f * (q[2]*q[3] + q[0]*q[1]), (q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3]));
}

__attribute__((unused)) static void ADCS_sendEulerAngles(const float e[3])
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "e = [%.4f %.4f %.4f] deg\n",
                       e[0] / 3.14159f * 180.0f,
                       e[1] / 3.14159f * 180.0f,
                       e[2] / 3.14159f * 180.0f);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

__attribute__((unused)) static void ADCS_sendMagneticField(const float m[3])
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "%.4f, %.4f, %.4f\n",
                       m[0],
                       m[1],
                       m[2]);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

__attribute__((unused)) static void ADCS_sendAngVel(const float w[3])
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "w = [%.4f %.4f %.4f]\n",
                       w[0],
                       w[1],
                       w[2]);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

__attribute__((unused)) static void ADCS_sendAngVelPidOutput(const float pid_out)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "pid_angvel_out = [%.4f]\n",
                       pid_out);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

__attribute__((unused)) static void ADCS_sendAnglePidOutput(const float pid_out)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "pid_angle_out = [%.4f]\n",
                       pid_out);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

static void ADCS_getRwDutyCycleAndDirectionBasedOnPidAngVelRegOut(
    ADCS_RW_DutyCycle *rw_duty_cycle,
    ADCS_RW_Direction *rw_direction,
    float pid_reg_out)
{
    // set direction and make torque positive if negative
    if (pid_reg_out > 0.0f) {
        *rw_direction = ADCS_RW_DIRECTION_CCW;
    } else {
        *rw_direction = ADCS_RW_DIRECTION_CW;
        pid_reg_out *= -1.0f;
    }

    // clamp torque
    if (pid_reg_out > ADCS_pid_angvel_max_out)
        pid_reg_out = ADCS_pid_angvel_max_out;

    const float x0 = 0.0f;
    const float x1 = ADCS_pid_angvel_max_out;
    const float y0 = ADCS_RW_DUTY_CYCLE_MIN;
    const float y1 = ADCS_RW_DUTY_CYCLE_MAX;
    const float x  = pid_reg_out;

    *rw_duty_cycle = lerp(x, x0, x1, y0, y1);
}

static void ADCS_getRwDutyCycleAndDirectionBasedOnPidAngleRegOut(
    ADCS_RW_DutyCycle *rw_duty_cycle,
    ADCS_RW_Direction *rw_direction,
    float pid_reg_out)
{
    // set direction and make torque positive if negative
    if (pid_reg_out > 0.0f) {
        *rw_direction = ADCS_RW_DIRECTION_CCW;
    } else {
        *rw_direction = ADCS_RW_DIRECTION_CW;
        pid_reg_out *= -1.0f;
    }

    // clamp torque
    if (pid_reg_out > ADCS_pid_angle_max_out)
        pid_reg_out = ADCS_pid_angle_max_out;

    const float x0 = 0.0f;
    const float x1 = ADCS_pid_angle_max_out;
    const float y0 = ADCS_RW_DUTY_CYCLE_MIN;
    const float y1 = ADCS_RW_DUTY_CYCLE_MAX;
    const float x  = pid_reg_out;

    *rw_duty_cycle = lerp(x, x0, x1, y0, y1);
}

ADCS_RegulationMode_E ADCS_getRegulationMode(void)
{
    ADCS_RegulationMode_E retval;

    __disable_irq();
    retval = ADCS_regulation_mode;
    __enable_irq();

    return retval;
}

void ADCS_setRegulationMode(ADCS_RegulationMode_E reg_mode)
{
    if (reg_mode < ADCS_REGULATION_MODE_NUM) {
        __disable_irq();
        ADCS_regulation_mode = reg_mode;
        __enable_irq();

        if (reg_mode == ADCS_REGULATION_MODE_ATTITUDE) {
            ADCS_PID_Status pid_status;
            ADCS_PID_resetIntegral(&ADCS_handle.pidHandleAngle, &pid_status);
            ERROR_assert(pid_status == ADCS_PID_STATUS_OK);
        
        } else if (reg_mode == ADCS_REGULATION_MODE_ANGULAR_VELOCITY) {
            ADCS_PID_Status pid_status;
            ADCS_PID_resetIntegral(&ADCS_handle.pidHandleAngVel, &pid_status);
            ERROR_assert(pid_status == ADCS_PID_STATUS_OK);
        }
    } else {
        // error
        ERROR_assert(0);
    }
}

    else if (reg_mode == ADCS_REGULATION_MODE_ANGULAR_VELOCITY)
