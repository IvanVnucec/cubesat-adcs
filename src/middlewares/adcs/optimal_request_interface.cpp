/*
 * optimal_request_interface.cpp
 *
 *  Created on: Oct 18, 2021
 *      Author: ivnucec
 */

#include "optimal_request_interface.hpp"

#include "cmsis_os.h"
#include "optimal_request/get_quat_from_K.h"
#include "optimal_request/get_quat_from_K_initialize.h"
#include "optimal_request/get_quat_from_K_terminate.h"
#include "optimal_request/optimal_request.h"
#include "optimal_request/optimal_request_init.h"
#include "adcs/inertial_meas_unit.hpp"
#include "parser/parser.hpp"
#include "printf-5.1.0/src/printf.h"

namespace OptimalRequestInterface {
float ACC_REF[3] = { -0.0518488f, -0.0495620f, -0.9974243f }; // m/s^2
float MAG_REF[3] = { 0.8514420f, 0.5186020f, 0.0780922f }; // uT

InertialMeasUnit::Data imu_data = { 0 };

OptimalRequest::OptimalRequest()
{
    // these values are not changing
    m_or_handle.Mu_noise_var  = 0.0008117f;
    m_or_handle.Eta_noise_var = 0.0000010f;
    m_or_handle.dT            = m_iterationPeriodInMiliSec / 1000.0f;    // ms to s

    this->fill_r(ACC_REF, MAG_REF);

    InertialMeasUnit::getPublicImuData(&imu_data);
    this->fill_b(imu_data.acc, imu_data.mag);
    this->fill_w(imu_data.gyr);

    optimal_request_init(&m_or_handle);
}

OptimalRequest::~OptimalRequest()
{
}

void OptimalRequest::iterate()
{
    this->fill_r(ACC_REF, MAG_REF);

    InertialMeasUnit::getPublicImuData(&imu_data);
    this->fill_b(imu_data.acc, imu_data.mag);
    this->fill_w(imu_data.gyr);

    optimal_request(&m_or_handle);
}

void OptimalRequest::getQuaternion(float q[4])
{
    get_quat_from_K(m_or_handle.K, q);
}

void OptimalRequest::fill_r(float ref_acc[3], float ref_mag[3])
{
    m_or_handle.r[0] = ref_acc[0];
    m_or_handle.r[1] = ref_acc[1];
    m_or_handle.r[2] = ref_acc[2];
    m_or_handle.r[3] = ref_mag[0];
    m_or_handle.r[4] = ref_mag[1];
    m_or_handle.r[5] = ref_mag[2];
}

void OptimalRequest::fill_b(float bdy_acc[3], float bdy_mag[3])
{
    m_or_handle.b[0] = bdy_acc[0];
    m_or_handle.b[1] = bdy_acc[1];
    m_or_handle.b[2] = bdy_acc[2];
    m_or_handle.b[3] = bdy_mag[0];
    m_or_handle.b[4] = bdy_mag[1];
    m_or_handle.b[5] = bdy_mag[2];
}

void OptimalRequest::fill_w(float bdy_gyr[3])
{
    m_or_handle.w[0] = bdy_gyr[0];
    m_or_handle.w[1] = bdy_gyr[1];
    m_or_handle.w[2] = bdy_gyr[2];
}

void optimalRequestThread(void *argument)
{
    constexpr int Q_DATA_STR_LEN = 50;
    char q_data_str[Q_DATA_STR_LEN];

    OptimalRequest optimal_request;
    float quat[4];

    for (;;) {
        optimal_request.iterate();
        optimal_request.getQuaternion(quat);

        int cx = snprintf_(q_data_str,
                           Q_DATA_STR_LEN,
                           "q: %f %f %f %f\n",
                           quat[0],
                           quat[1],
                           quat[2],
                           quat[3]);

        if (cx >= 0 && cx < (int)Q_DATA_STR_LEN)
            Parser::sendString(q_data_str, cx);


        vTaskDelay(pdMS_TO_TICKS(optimal_request.m_iterationPeriodInMiliSec));
    }
}

}    // namespace OptimalRequestInterface
