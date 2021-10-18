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
static osThreadId_t angleRegulationTaskHandle;
const static osThreadAttr_t angleRegulationTask_attributes = {
    .name       = "angleRegulationTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal1,    // TODO: Set priorities
};

static osThreadId_t optimalRequestHandle;
const static osThreadAttr_t optimalRequest_attributes = {
    .name       = "optimalRequestTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal2,
};

static osThreadId_t inertialMeasUnitHandle;
const static osThreadAttr_t inertialMeasUnit_attributes = {
    .name       = "inertialMeasUnitTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal3,
};

/* Private function prototypes -----------------------------------------------*/

/* Private application code --------------------------------------------------*/
void initAdcsThreads()
{
    // angle regulation thread
    angleRegulationTaskHandle =
        osThreadNew(REG_ANGLE_thread, NULL, &angleRegulationTask_attributes);

    // optimal request thread
    optimalRequestHandle = osThreadNew(OptimalRequestInterface::optimalRequestThread,
                                       NULL,
                                       &optimalRequest_attributes);

    // inertial measurement unit thread
    inertialMeasUnitHandle = osThreadNew(InertialMeasUnit::inertialMeasUnitThread,
                                         NULL,
                                         &inertialMeasUnit_attributes);
}

#ifdef __cplusplus
}
#endif
