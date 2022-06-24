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
#include "adcs_cf.h"
#include "adcs_imu.h"
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
static ADCS_RegulationMode_E ADCS_regulation_mode = ADCS_REGULATION_MODE_NO_REGULATION;

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
    .Kp = 50.0f,
    .Ki = 3.0f,
    .Kd = 100.0f,
    .Ts = ADCS_THREAD_PERIOD_IN_SECONDS,
    .V  = 15.0f};
static const float ADCS_pid_angle_max_out       = 100.0f;
static float ADCS_euler_angles_ref[3]           = {0.0f, 0.0f, 0.0f};
static float ADCS_ang_vel_ref[3]                = {0.0f, 0.0f, 0.0f};
static ADCS_RW_DutyCycle ADCS_rw_duty_cycle_ref = ADCS_RW_DUTY_CYCLE_MIN;

/* Private function prototypes -----------------------------------------------*/
static void ADCS_init(void);
static void ADCS_determineAttitude(ADCS_Quaternion_T quat, ADCS_ImuData_T *imu_data);
static void ADCS_controlAngVel(const float ang_vel_ref, const float ang_vel);
static void ADCS_controlAngle(const float angle_ref, const float angle);
static void ADCS_calculateEulerAngles(float e[3], const ADCS_Quaternion_T q);
// TODO: move regulator stuff in its own files
static void ADCS_getRwDutyCycleAndDirectionBasedOnPidAngVelRegOut(
    ADCS_RW_DutyCycle *rw_duty_cycle,
    ADCS_RW_Direction *rw_direction,
    float pid_reg_out);
static void
    ADCS_getRwDutyCycleAndDirectionBasedOnPidAngleRegOut(ADCS_RW_DutyCycle *rw_duty_cycle,
                                                         ADCS_RW_Direction *rw_direction,
                                                         float pid_reg_out);
static ADCS_RW_DutyCycle ADCS_getReactionWheelRefPwmDutyCycle(void);
static void ADCS_sendPacketInRegModeAngVel(float w_ref, float w, float controller_effort);
static void
    ADCS_sendPacketInRegModeAngle(float angle_ref, float angle, float controller_effort);
static void ADCS_sendPacketInRegModeNoReg(const float euler[3],
                                          const float w[3],
                                          const ADCS_RW_DutyCycle rw);

/* Private user code ---------------------------------------------------------*/
void ADCS_thread(void *argument)
{
    static ADCS_ImuData_T imu_data;
    static ADCS_Quaternion_T q_meas;
    static uint32_t tick;
    static ADCS_RegulationMode_E reg_mode;
    static float euler_angles[3];

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
            ADCS_controlAngle(ADCS_euler_angles_ref[0], euler_angles[0]);

            float effort;
            ADCS_PID_getControllerEffort(&ADCS_handle.pidHandleAngle, &effort);
            ADCS_sendPacketInRegModeAngle(ADCS_euler_angles_ref[0],
                                          euler_angles[0],
                                          effort);

        } else if (reg_mode == ADCS_REGULATION_MODE_ANGULAR_VELOCITY) {
            ADCS_controlAngVel(ADCS_ang_vel_ref[2], imu_data.gyr[2]);

            float effort;
            ADCS_PID_getControllerEffort(&ADCS_handle.pidHandleAngVel, &effort);
            ADCS_sendPacketInRegModeAngVel(ADCS_ang_vel_ref[2], imu_data.gyr[2], effort);

        } else if (reg_mode == ADCS_REGULATION_MODE_NO_REGULATION) {
            ADCS_RW_Status rw_status;
            ADCS_RW_DutyCycle duty_cycle_ref;
            ADCS_RW_DutyCycle duty_cycle_now;

            ADCS_RW_getPwmDutyCycle(&ADCS_handle.reactionWheelHandle,
                                    &duty_cycle_now,
                                    &rw_status);
            duty_cycle_ref = ADCS_getReactionWheelRefPwmDutyCycle();

            if (duty_cycle_now != duty_cycle_ref) {
                ADCS_RW_setPwmDutyCycle(&ADCS_handle.reactionWheelHandle,
                                        duty_cycle_ref,
                                        &rw_status);
            }

            ADCS_determineAttitude(q_meas, &imu_data);
            ADCS_calculateEulerAngles(euler_angles, q_meas);

            ADCS_sendPacketInRegModeNoReg(euler_angles, imu_data.gyr, duty_cycle_now);
        }

        tick += ADCS_THREAD_PERIOD_IN_MILISECONDS;    // overflow is safe
        osDelayUntil(tick);
    }
}

static void ADCS_init(void)
{
    ADCS_RW_Status rw_status   = ADCS_RW_STATUS_ERROR;
    ADCS_PID_Status pid_status = ADCS_PID_STATUS_ERROR;

    ADCS_IMU_init(&ADCS_handle.mpu9250Handle);
    ADCS_CF_init(&ADCS_handle.compFiltHandle);
    ADCS_RW_init(&ADCS_handle.reactionWheelHandle, &rw_status);
    ERROR_assert(rw_status == ADCS_RW_STATUS_OK);

    ADCS_PID_init(&ADCS_handle.pidHandleAngVel, &ADCS_pid_coeffs_angvel, &pid_status);
    ERROR_assert(pid_status == ADCS_PID_STATUS_OK);

    ADCS_PID_init(&ADCS_handle.pidHandleAngle, &ADCS_pid_coeffs_angle, &pid_status);
    ERROR_assert(pid_status == ADCS_PID_STATUS_OK);
}

static void ADCS_determineAttitude(ADCS_Quaternion_T quat, ADCS_ImuData_T *imu_data)
{
    ADCS_CF_step(&ADCS_handle.compFiltHandle, imu_data, quat);
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
}

void ADCS_delayMs(unsigned ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

__attribute__((unused)) static void ADCS_calculateEulerAngles(float e[3],
                                                              const ADCS_Quaternion_T q)
{
    e[0] = atan2f(+2.0f * (q[1] * q[2] + q[0] * q[3]),
                  q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
    e[1] = asinf(-2.0f * (q[1] * q[3] - q[0] * q[2]));
    e[2] = atan2f(+2.0f * (q[2] * q[3] + q[0] * q[1]),
                  q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
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

static void
    ADCS_getRwDutyCycleAndDirectionBasedOnPidAngleRegOut(ADCS_RW_DutyCycle *rw_duty_cycle,
                                                         ADCS_RW_Direction *rw_direction,
                                                         float pid_reg_out)
{
    // set direction and make torque positive if negative
    if (pid_reg_out > 0.0f) {
        *rw_direction = ADCS_RW_DIRECTION_CW;
    } else {
        *rw_direction = ADCS_RW_DIRECTION_CCW;
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
        } else if (reg_mode == ADCS_REGULATION_MODE_NO_REGULATION) {
            ADCS_RW_Status rw_status;
            ADCS_RW_setPwmDutyCycle(&ADCS_handle.reactionWheelHandle,
                                    ADCS_RW_DUTY_CYCLE_MIN,
                                    &rw_status);
            ERROR_assert(rw_status == ADCS_RW_STATUS_OK);
        }
    } else {
        // error
        ERROR_assert(0);
    }
}

ADCS_PID_Handle *ADCS_getCurrentPidRegulatorHandle(void)
{
    ADCS_RegulationMode_E reg_mode;

    reg_mode = ADCS_getRegulationMode();

    if (reg_mode == ADCS_REGULATION_MODE_ATTITUDE)
        return &ADCS_handle.pidHandleAngle;
    else if (reg_mode == ADCS_REGULATION_MODE_ANGULAR_VELOCITY)
        return &ADCS_handle.pidHandleAngVel;

    return NULL;    // error
}

void ADCS_setRefAngleZInDeg(const float z_deg)
{
    float z_rad;

    z_rad = z_deg * 3.14159f / 180.0f;

    __disable_irq();
    ADCS_euler_angles_ref[0] = z_rad;
    __enable_irq();
}

void ADCS_setRefAngVelZInRad(const float z_ang_vel)
{
    __disable_irq();
    ADCS_ang_vel_ref[2] = z_ang_vel;
    __enable_irq();
}

static ADCS_RW_DutyCycle ADCS_getReactionWheelRefPwmDutyCycle(void)
{
    // no need to disable irq because this function is called from
    // ...adcs thread only
    return ADCS_rw_duty_cycle_ref;
}

void ADCS_setReactionWheelRefPwmDutyCycle(ADCS_RW_DutyCycle duty_cycle)
{
    __disable_irq();
    ADCS_rw_duty_cycle_ref = duty_cycle;
    __enable_irq();
}

static void ADCS_sendPacketInRegModeAngVel(float w_ref, float w, float controller_effort)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "%d %d %d\n",
                       (int)roundf(1000.0f * w_ref),
                       (int)roundf(1000.0f * w),
                       (int)roundf(controller_effort));

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

static void
    ADCS_sendPacketInRegModeAngle(float angle_ref, float angle, float controller_effort)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "%d %d %d\n",
                       (int)roundf(180.0f / 3.14159f * angle_ref),
                       (int)roundf(180.0f / 3.14159f * angle),
                       (int)roundf(controller_effort));

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}

static void ADCS_sendPacketInRegModeNoReg(const float euler[3],
                                          const float w[3],
                                          const ADCS_RW_DutyCycle rw)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "%d %d %d ",
                       (int)roundf(180.0f / 3.14159f * euler[0]),
                       (int)roundf(180.0f / 3.14159f * euler[1]),
                       (int)roundf(180.0f / 3.14159f * euler[2]));

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);

    cx = snprintf_((char *)message.buffer,
                   COMM_MESSAGE_MAX_BUFF_LEN,
                   "%d %d %d ",
                   (int)roundf(1000.0f * w[0]),
                   (int)roundf(1000.0f * w[1]),
                   (int)roundf(1000.0f * w[2]));

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);

    cx = snprintf_((char *)message.buffer, COMM_MESSAGE_MAX_BUFF_LEN, "%d\n", (int)rw);

    ERROR_assert(cx >= 0 && cx < COMM_MESSAGE_MAX_BUFF_LEN);

    message.msg_len = cx;

    COMM_sendMessage(&message, &status);
}
