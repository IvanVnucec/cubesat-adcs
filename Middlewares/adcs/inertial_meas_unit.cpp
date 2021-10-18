// TODO: Implement code here

/*
 * inertial_meas_unit.cpp
 *
 *  Created on: 24. ruj 2021.
 *      Author: ivnucec
 */

#include "inertial_meas_unit.hpp"

#include "cmsis_os.h"
#include "mpu9250.hpp"


namespace InertialMeasUnit {

InertialMeasUnit::InertialMeasUnit()
{
}

InertialMeasUnit::~InertialMeasUnit()
{
}

void InertialMeasUnit::calibrateGyro()
{
}

void InertialMeasUnit::getDataFromSensorAsync()
{
}

void InertialMeasUnit::getGyroData()
{
}

void InertialMeasUnit::getAccData()
{
}

void InertialMeasUnit::getMagData()
{
}

void inertialMeasUnitThread(void *argument)
{
    static InertialMeasUnit imu;

    for (;;) {
        imu.getDataFromSensorAsync();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

}    // namespace InertialMeasUnit
