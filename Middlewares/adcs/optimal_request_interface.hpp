/*
 * optimal_request_interface.hpp
 *
 *  Created on: Oct 18, 2021
 *      Author: ivnucec
 */

#ifndef ADCS_OPTIMAL_REQUEST_INTERFACE_HPP_
#define ADCS_OPTIMAL_REQUEST_INTERFACE_HPP_

namespace OptimalRequestInterface {

class OptimalRequest {
  private:
  public:
    OptimalRequest();
    ~OptimalRequest();
};

void optimalRequestTaskCallback(void *argument);

}    // namespace OptimalRequestInterface

#endif /* ADCS_OPTIMAL_REQUEST_INTERFACE_HPP_ */
