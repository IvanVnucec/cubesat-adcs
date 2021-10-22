// TODO: Implement code here

/*
 * inertial_meas_unit.cpp
 *
 *  Created on: 24. ruj 2021.
 *      Author: ivnucec
 */

#include "inertial_meas_unit.hpp"

#include "cmsis_os.h"
#include "i2c.h"
#include "mpu9250.hpp"

namespace InertialMeasUnit {

InertialMeasUnit::InertialMeasUnit() : MPU9250(&hi2c1, 0x68)
{
    MPU9250::begin();
}

InertialMeasUnit::~InertialMeasUnit()
{
}

void InertialMeasUnit::calibrateGyro()
{
    MPU9250::calibrateGyro();
}

// TODO [Ivan Vnucec]: Add async reads
void InertialMeasUnit::getDataFromSensorAsync()
{
    MPU9250::readSensor();
}

void InertialMeasUnit::getGyroData(float g[3])
{
    g[0] = MPU9250::getGyroX_rads();
    g[1] = MPU9250::getGyroX_rads();
    g[2] = MPU9250::getGyroX_rads();
}

void InertialMeasUnit::getAccData(float a[3])
{
    a[0] = MPU9250::getAccelX_mss();
    a[1] = MPU9250::getAccelY_mss();
    a[2] = MPU9250::getAccelZ_mss();
}

void InertialMeasUnit::getMagData(float m[3])
{
    m[0] = MPU9250::getMagX_uT();
    m[1] = MPU9250::getMagY_uT();
    m[2] = MPU9250::getMagZ_uT();
}

void inertialMeasUnitThread(void *argument)
{
    InertialMeasUnit imu;

    for (;;) {
        imu.getDataFromSensorAsync();
        // TODO: Add data buffering and send notification
        //       that we have data to the other threads using IMU

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

}    // namespace InertialMeasUnit
