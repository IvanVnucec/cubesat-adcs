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

static TwoWire Wire;    // TODO: Delete this when mpu9250 driver is implemented
static MPU9250 imu(Wire, 0x68);

InertialMeasUnit::InertialMeasUnit()
{
    imu.begin();
}

InertialMeasUnit::~InertialMeasUnit()
{
}

void InertialMeasUnit::calibrateGyro()
{
    imu.calibrateGyro();
}

void InertialMeasUnit::getDataFromSensorAsync()
{
    imu.readSensor();
}

void InertialMeasUnit::getGyroData(float g[3])
{
    g[0] = imu.getGyroX_rads();
    g[1] = imu.getGyroX_rads();
    g[2] = imu.getGyroX_rads();
}

void InertialMeasUnit::getAccData(float a[3])
{
    a[0] = imu.getAccelX_mss();
    a[1] = imu.getAccelY_mss();
    a[2] = imu.getAccelZ_mss();
}

void InertialMeasUnit::getMagData(float m[3])
{
    m[0] = imu.getMagX_uT();
    m[1] = imu.getMagY_uT();
    m[2] = imu.getMagZ_uT();
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
