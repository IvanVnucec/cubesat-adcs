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

InertialMeasUnit::InertialMeasUnit() : MPU9250(m_imu_i2c_address)
{
}

InertialMeasUnit::~InertialMeasUnit()
{
}

void InertialMeasUnit::calibrateGyro()
{
    MPU9250::calibrateGyro();
}

InertialMeasUnit::Data InertialMeasUnit::getData()
{
    MPU9250::readSensor();

    m_data.gyr[0] = MPU9250::getGyroX_rads();
    m_data.gyr[1] = MPU9250::getGyroY_rads();
    m_data.gyr[2] = MPU9250::getGyroZ_rads();

    m_data.acc[0] = MPU9250::getAccelX_mss();
    m_data.acc[1] = MPU9250::getAccelY_mss();
    m_data.acc[2] = MPU9250::getAccelZ_mss();

    m_data.mag[0] = MPU9250::getMagX_uT();
    m_data.mag[1] = MPU9250::getMagY_uT();
    m_data.mag[2] = MPU9250::getMagZ_uT();

    return m_data;
}

void inertialMeasUnitThread(void *argument)
{
    InertialMeasUnit imu;
    InertialMeasUnit::Data imu_data;

    for (;;) {
        imu_data = imu.getData();

        // TODO: handle data
        (void)imu_data;

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

}    // namespace InertialMeasUnit
