// TODO: Implement code here

/*
 * inertial_meas_unit.cpp
 *
 *  Created on: 24. ruj 2021.
 *      Author: ivnucec
 */

#include "inertial_meas_unit.hpp"

#include "cmsis_os.h"
#include "fault_handling.hpp"
#include "mpu9250.hpp"

namespace InertialMeasUnit {

InertialMeasUnit::InertialMeasUnit() : MPU9250::MPU9250(m_imu_i2c_address)
{
}

InertialMeasUnit::~InertialMeasUnit()
{
}

void InertialMeasUnit::calibrateGyro()
{
    int retval = MPU9250::calibrateGyro();
    private_assert(retval == 1);
}

Data InertialMeasUnit::getData()
{
    using namespace MPU9250;

    Data data;

    int retval = readSensor();
    private_assert(retval == 1);

    data.gyr[0] = getGyroX_rads();
    data.gyr[1] = getGyroY_rads();
    data.gyr[2] = getGyroZ_rads();

    data.acc[0] = getAccelX_mss();
    data.acc[1] = getAccelY_mss();
    data.acc[2] = getAccelZ_mss();

    data.mag[0] = getMagX_uT();
    data.mag[1] = getMagY_uT();
    data.mag[2] = getMagZ_uT();

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

void InertialMeasUnit::private_assert(bool condition)
{
    if (not condition)
        imuErrorHandle();
}

void InertialMeasUnit::mpuDriverErrorHandle()
{
    imuErrorHandle();
}

void InertialMeasUnit::imuErrorHandle()
{
    Fault::setFaultState(Fault::IMU_FAULT);
    // TODO: yield task thread here because private asserts in drivers wont stop execution.
    // this also needs to be fixed across repo
}

}    // namespace InertialMeasUnit
