#if 0
// TODO: Implement code here

/*
 * inertial_meas_unit.cpp
 *
 *  Created on: 24. ruj 2021.
 *      Author: ivnucec
 */

#include "inertial_meas_unit.hpp"

#include "cmsis_os.h"
#include "drivers/user/mpu9250/mpu9250.hpp"
#include "fault_handling/fault_handling.hpp"
#include "parser/parser.hpp"
#include "printf-5.1.0/src/printf.h"

#include <cstring>
#include <string>

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
    static const unsigned IMU_DATA_STR_LEN = 200u;
    char imu_data_str[IMU_DATA_STR_LEN];    // TODO: Count exactly how many chars

    for (;;) {
        imu_data = imu.getData();

        int cx = snprintf_(imu_data_str,
                           IMU_DATA_STR_LEN,
                           "acc[x y z]: %f %f %f m/s^2\n"
                           "mag[x y z]: %f %f %f uT\n"
                           "gyr[x y z]: %f %f %f rad/s\n\n",
                           imu_data.acc[0],
                           imu_data.acc[1],
                           imu_data.acc[2],
                           imu_data.mag[0],
                           imu_data.mag[1],
                           imu_data.mag[2],
                           imu_data.gyr[0],
                           imu_data.gyr[1],
                           imu_data.gyr[2]);

        if (cx >= 0 && cx < (int)IMU_DATA_STR_LEN)    // check returned value
            Parser::sendString(imu_data_str, std::strlen(imu_data_str));

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void InertialMeasUnit::private_assert(bool condition)
{
    if (not condition)
        imuErrorHandle();
}

void InertialMeasUnit::imuErrorHandle()
{
    Fault::setFaultState(Fault::State::IMU_FAULT);
    // TODO: yield task thread here because private asserts in drivers wont stop execution.
    // this also needs to be fixed across repo
}

}    // namespace InertialMeasUnit
#endif