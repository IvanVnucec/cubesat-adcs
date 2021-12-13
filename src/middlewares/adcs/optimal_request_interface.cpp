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

namespace OptimalRequestInterface {

OptimalRequest::OptimalRequest()
{
    // these values are not changing
    m_or_handle.Mu_noise_var  = 0.0008117f;
    m_or_handle.Eta_noise_var = 0.0000010f;
    m_or_handle.dT            = m_iterationPeriodInMiliSec / 1000.0f;    // ms to s

    // TODO [Ivan Vnucec]: Uncomment code below
    // TODO [Ivan Vnucec]: Get first measuerements
    //m_or_handle.fill_r(ref_acc[0], ref_mag[0]);
    //m_or_handle.fill_b(bdy_acc[0], bdy_mag[0]);
    //m_or_handle.fill_w(bdy_gyr[0]);

    //optimal_request_init(&m_or_handle);
}

OptimalRequest::~OptimalRequest()
{
}

void OptimalRequest::iterate()
{
    // TODO [Ivan Vnucec]: Uncomment code below
    // TODO [Ivan Vnucec]: Get measuerements
    //m_or_handle.fill_r(ref_acc[i], ref_mag[i]);
    //m_or_handle.fill_b(bdy_acc[i], bdy_mag[i]);
    //m_or_handle.fill_w(bdy_gyr[i]);

    //optimal_request(&m_or_handle);
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
    OptimalRequest optimal_request;

    for (;;) {
        // TODO: Get data from IMU here
        optimal_request.iterate();

        vTaskDelay(pdMS_TO_TICKS(optimal_request.m_iterationPeriodInMiliSec));
    }
}

}    // namespace OptimalRequestInterface
