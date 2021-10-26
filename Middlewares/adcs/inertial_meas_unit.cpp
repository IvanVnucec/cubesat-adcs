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
    int retval = MPU9250::calibrateGyro();
    assert(retval == 1);
}

Data InertialMeasUnit::getData()
{
    Data data;

    int retval = MPU9250::readSensor();
    assert(retval == 1);

    data.gyr[0] = MPU9250::getGyroX_rads();
    data.gyr[1] = MPU9250::getGyroY_rads();
    data.gyr[2] = MPU9250::getGyroZ_rads();

    data.acc[0] = MPU9250::getAccelX_mss();
    data.acc[1] = MPU9250::getAccelY_mss();
    data.acc[2] = MPU9250::getAccelZ_mss();

    data.mag[0] = MPU9250::getMagX_uT();
    data.mag[1] = MPU9250::getMagY_uT();
    data.mag[2] = MPU9250::getMagZ_uT();

    return data;
}

void inertialMeasUnitThread(void *argument)
{
    InertialMeasUnit imu;
    Data imu_data;

    for (;;) {
        imu_data = imu.getData();

        // TODO: handle data
        (void)imu_data;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

}    // namespace InertialMeasUnit
