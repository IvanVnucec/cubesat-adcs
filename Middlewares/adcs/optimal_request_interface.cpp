/*
 * optimal_request_interface.cpp
 *
 *  Created on: Oct 18, 2021
 *      Author: ivnucec
 */

#include "optimal_request_interface.hpp"

#include "cmsis_os.h"

namespace OptimalRequestInterface {

OptimalRequest::OptimalRequest()
{
    // TODO: Add init code here
}

OptimalRequest::~OptimalRequest()
{
}

void optimalRequestTaskCallback(void *argument)
{
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        // TODO: Add code here
    }
}

}    // namespace OptimalRequestInterface
