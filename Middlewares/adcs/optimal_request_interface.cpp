/*
 * optimal_request_interface.cpp
 *
 *  Created on: Oct 18, 2021
 *      Author: ivnucec
 */

#include "optimal_request_interface.hpp"

#include "cmsis_os.h"
#include "get_quat_from_K.h"
#include "get_quat_from_K_initialize.h"
#include "get_quat_from_K_terminate.h"
#include "optimal_request.h"
#include "optimal_request_init.h"

namespace OptimalRequestInterface {

OptimalRequest::OptimalRequest()
{
    // these values are not changing
    m_or_handle.Mu_noise_var  = 0.0008117f;
    m_or_handle.Eta_noise_var = 0.0000010f;
    m_or_handle.dT            = 10.0f;

    // TODO [Ivan Vnucec]: Uncomment code below
    // TODO [Ivan Vnucec]: Get first measuerements
    //m_or_handle.fill_r(ref_acc[0], ref_mag[0]);
    //m_or_handle.fill_b(bdy_acc[0], bdy_mag[0]);
    //m_or_handle.fill_w(bdy_gyr[0]);

    // init optimal_req on 1st meas
    optimal_request_init(&m_or_handle);
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

    optimal_request(&m_or_handle);
}

void OptimalRequest::getQuaternion(float q[4])
{
    get_quat_from_K(m_or_handle.K, q);
}

void optimalRequestTaskCallback(void *argument)
{
    static OptimalRequest optimal_request;

    for (;;) {
        optimal_request.iterate();

        vTaskDelay(pdMS_TO_TICKS(optimal_request.iterationPeriodInMiliSec));
    }
}

void OptimalRequest::fill_r(float vec1[3], float vec2[3])
{
    m_or_handle.r[0] = vec1[0];
    m_or_handle.r[1] = vec1[1];
    m_or_handle.r[2] = vec1[2];
    m_or_handle.r[3] = vec2[0];
    m_or_handle.r[4] = vec2[1];
    m_or_handle.r[5] = vec2[2];
}

void OptimalRequest::fill_b(float vec1[3], float vec2[3])
{
    m_or_handle.b[0] = vec1[0];
    m_or_handle.b[1] = vec1[1];
    m_or_handle.b[2] = vec1[2];
    m_or_handle.b[3] = vec2[0];
    m_or_handle.b[4] = vec2[1];
    m_or_handle.b[5] = vec2[2];
}

void OptimalRequest::fill_w(float vec_w[3])
{
    m_or_handle.w[0] = vec_w[0];
    m_or_handle.w[1] = vec_w[1];
    m_or_handle.w[2] = vec_w[2];
}

}    // namespace OptimalRequestInterface
