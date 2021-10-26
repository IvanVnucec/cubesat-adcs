/*
 * adcs_tasks.cpp
 *
 *  Created on: 18. lis 2021.
 *      Author: ivnucec
 */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "angle_regulation.hpp"
#include "cmsis_os.h"
#include "inertial_meas_unit.hpp"
#include "optimal_request_interface.hpp"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static osThreadId_t optimalRequestHandle              = NULL;
const static osThreadAttr_t optimalRequest_attributes = {
    .name       = "optimalRequestTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal2,
};

static osThreadId_t inertialMeasUnitHandle              = NULL;
const static osThreadAttr_t inertialMeasUnit_attributes = {
    .name       = "inertialMeasUnitTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityNormal3,
};

/* Private function prototypes -----------------------------------------------*/

/* Private application code --------------------------------------------------*/
void initAdcsThreads()
{
    // TODO [Ivan Vnucec]: Check handles for null ptrs
    // inertial measurement unit thread
    inertialMeasUnitHandle = osThreadNew(InertialMeasUnit::inertialMeasUnitThread,
                                         NULL,
                                         &inertialMeasUnit_attributes);

    assert(inertialMeasUnitHandle != NULL);

    // optimal request thread
    optimalRequestHandle = osThreadNew(OptimalRequestInterface::optimalRequestThread,
                                       NULL,
                                       &optimalRequest_attributes);

    assert(optimalRequestHandle != NULL);
}

#ifdef __cplusplus
}
#endif
