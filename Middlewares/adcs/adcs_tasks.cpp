/*
 * adcs_tasks.cpp
 *
 *  Created on: 18. lis 2021.
 *      Author: ivnucec
 */

#include "FreeRTOS.h"
#include "angle_regulation.hpp"
#include "cmsis_os.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

static osThreadId_t angleRegulationTaskHandle;
const static osThreadAttr_t angleRegulationTask_attributes = {
    .name       = "angleRegulationTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal1,
};

static void StartAngleRegulationTask(void *argument)
{
    REG_ANGLE_taskCallback(argument);
}

void initAdcsThreads()
{
    // angle regulation thread
    angleRegulationTaskHandle =
        osThreadNew(StartAngleRegulationTask, NULL, &angleRegulationTask_attributes);
}

#ifdef __cplusplus
}
#endif
