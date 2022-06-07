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
#include "adcs_reg.h"
#include "cmsis_os2.h"
#include "middlewares/communication/comm.h"
#include "printf/printf.h"
#include "stm32l4xx_hal.h"
#include "task.h"
#include "utils/error/error.h"
#include "utils/lerp/lerp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ADCS_Handle_T ADCS_handle;
static ADCS_RegulationMode_E ADCS_regulation_mode = ADCS_REGULATION_MODE_ATTITUDE;
static const ADCS_PID_RegulatorCoeffs ADCS_pid_coeffs_angvel = {
    .Kp = 231.5f,
    .Ki = 117.0f,
    .Kd = 0.0f,
    .Ts = ADCS_THREAD_PERIOD_IN_SECONDS,
    .V  = 15.0f};
static const float ADCS_pid_angvel_max_out = 255.0f;

/* Private function prototypes -----------------------------------------------*/
static void ADCS_init(void);
static void ADCS_determineAttitude(ADCS_Quaternion_T quat,
                                   const ADCS_ImuData_T *imu_data);
static void ADCS_controlAttitude(const ADCS_Quaternion_T q_ref,
                                 const ADCS_Quaternion_T q_meas,
                                 const float gyr[3]);
static void ADCS_controlAngVel(const float ang_vel_ref, const float ang_vel);
static void ADCS_sendQuaternion(const ADCS_Quaternion_T quat);
static void ADCS_sendTorque(const ADCS_REG_Torque t);
static void ADCS_sendAngVelPidOutput(const float pid_out);
static void ADCS_getRwDutyCycleAndDirectionBasedOnTorque(ADCS_RW_DutyCycle *rw_duty_cycle,
                                                         ADCS_RW_Direction *rw_direction,
                                                         float torque);
static void ADCS_getRwDutyCycleAndDirectionBasedOnPidAngVelRegOut(
    ADCS_RW_DutyCycle *rw_duty_cycle,
    ADCS_RW_Direction *rw_direction,
    float pid_reg_out);

/* Private user code ---------------------------------------------------------*/
void ADCS_thread(void *argument)
{
    static ADCS_ImuData_T imu_data;
    static ADCS_Quaternion_T q_ref;
    static ADCS_Quaternion_T q_meas;
    static uint32_t tick;
    static ADCS_RegulationMode_E reg_mode;

    ADCS_init();

    tick = osKernelGetTickCount();
    for (;;) {
        ADCS_IMU_getData(&imu_data);

        reg_mode = ADCS_getRegulationMode();
        if (reg_mode == ADCS_REGULATION_MODE_ATTITUDE) {
            ADCS_determineAttitude(q_meas, &imu_data);

            // TODO: remove this, for test only
            q_ref[0] = 1.0f;
            q_ref[1] = 0.0f;
            q_ref[2] = 0.0f;
            q_ref[3] = 0.0f;
            ADCS_controlAttitude(q_ref, q_meas, imu_data.gyr);
            //ADCS_sendQuaternion(q_meas);

        } else if (reg_mode == ADCS_REGULATIOM_MODE_ANGULAR_VELOCITY) {
            const float ang_vel_ref = 0.0f;
            ADCS_controlAngVel(ang_vel_ref, imu_data.gyr[2]);

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

    ADCS_REG_init(&ADCS_handle.adcsRegHandle, ADCS_REG_PQ, ADCS_REG_PW);
    ADCS_PID_init(&ADCS_handle.pidHandleAngVel, &ADCS_pid_coeffs_angvel, &pid_status);
    ERROR_assert(pid_status == ADCS_PID_STATUS_OK);
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
    ADCS_RW_Status tw_status;
    ADCS_RW_DutyCycle rw_duty_cycle;
    ADCS_RW_Direction rw_direction;

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

    // regulate only around z axis
    ADCS_getRwDutyCycleAndDirectionBasedOnTorque(&rw_duty_cycle,
                                                 &rw_direction,
                                                 torque.tz);
    ADCS_RW_setDirection(&ADCS_handle.reactionWheelHandle, rw_direction, &tw_status);
    ADCS_RW_setPwmDutyCycle(&ADCS_handle.reactionWheelHandle, rw_duty_cycle, &tw_status);

    ADCS_sendTorque(torque);
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

    ADCS_sendAngVelPidOutput(reg_out);
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

static void ADCS_sendAngVelPidOutput(const float pid_out)
{
    COMM_Status status = COMM_STATUS_ERROR;
    COMM_Message message;

    int cx = snprintf_((char *)message.buffer,
                       COMM_MESSAGE_MAX_BUFF_LEN,
                       "pid_out = [%.4f]\n",
                       pid_out);

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

    const float x0 = 0.0f;
    const float x1 = max_torque;
    const float y0 = ADCS_RW_DUTY_CYCLE_MIN;
    const float y1 = ADCS_RW_DUTY_CYCLE_MAX;
    const float x  = torque;

    *rw_duty_cycle = lerp(x, x0, x1, y0, y1);
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

        if (reg_mode == ADCS_REGULATIOM_MODE_ANGULAR_VELOCITY) {
            ADCS_PID_Status pid_status;
            ADCS_PID_resetIntegral(&ADCS_handle.pidHandleAngVel, &pid_status);
            ERROR_assert(pid_status == ADCS_PID_STATUS_OK);
        }
    } else {
        // error
        ERROR_assert(0);
    }
}
