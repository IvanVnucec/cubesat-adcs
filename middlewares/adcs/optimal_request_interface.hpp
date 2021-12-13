/*
 * optimal_request_interface.hpp
 *
 *  Created on: Oct 18, 2021
 *      Author: ivnucec
 */

#ifndef ADCS_OPTIMAL_REQUEST_INTERFACE_HPP_
#define ADCS_OPTIMAL_REQUEST_INTERFACE_HPP_

#include "get_quat_from_K.h"

namespace OptimalRequestInterface {

class OptimalRequest {
  private:
    struct0_T m_or_handle;

    void fill_r(float ref_acc[3], float ref_mag[3]);
    void fill_b(float bdy_acc[3], float bdy_mag[3]);
    void fill_w(float bdy_gyr[3]);

  public:
    static constexpr unsigned m_iterationPeriodInMiliSec = 1000u;
    
    OptimalRequest();
    ~OptimalRequest();
    void iterate();
    void getQuaternion(float q[4]);
};

void optimalRequestThread(void *argument);

}    // namespace OptimalRequestInterface

#endif /* ADCS_OPTIMAL_REQUEST_INTERFACE_HPP_ */
