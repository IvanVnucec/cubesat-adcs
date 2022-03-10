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
#include "fault_handling/fault_handling.hpp"
#include "inertial_meas_unit.hpp"
#include "optimal_request_interface.hpp"
#include "parser/parser.hpp"
#include "task.h"

#include <cassert>

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
osThreadId_t defaultTaskHandle              = NULL;
const osThreadAttr_t defaultTask_attributes = {
    .name       = "defaultTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

osThreadId_t optimalRequestHandle                     = NULL;
const static osThreadAttr_t optimalRequest_attributes = {
    .name       = "optimalRequest",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal2,
};

osThreadId_t inertialMeasUnitHandle                     = NULL;
const static osThreadAttr_t inertialMeasUnit_attributes = {
    .name       = "inertialMeasUnit",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityNormal3,
};

osThreadId_t parserHandle                     = NULL;
const static osThreadAttr_t parser_attributes = {
    .name       = "parser",
    .stack_size = 256 * 4,
    .priority   = (osPriority_t)osPriorityNormal4,
};

osThreadId_t faultHandlingHandle                     = NULL;
const static osThreadAttr_t faultHandling_attributes = {
    .name       = "faultHandling",
    .stack_size = 256 * 4,
    .priority   = (osPriority_t)osPriorityNormal5,
};

/* Private function prototypes -----------------------------------------------*/
void StartDefaultTask(void *argument);

/* Private application code --------------------------------------------------*/
void initAdcsThreads()
{
    // default task
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
    assert(defaultTaskHandle != NULL);

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

    // parser thread
    parserHandle = osThreadNew(Parser::parserThread, NULL, &parser_attributes);
    assert(parserHandle != NULL);

    // fault handling thread
    faultHandlingHandle =
        osThreadNew(Fault::faultHandlingThread, NULL, &faultHandling_attributes);
    assert(faultHandlingHandle != NULL);
}

void StartDefaultTask(void *argument)
{
    for (;;) {
    }
}

#ifdef __cplusplus
}
#endif
