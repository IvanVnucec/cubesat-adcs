/*
 * inertial_meas_unit.hpp
 *
 *  Created on: 24. ruj 2021.
 *      Author: ivnucec
 */

#ifndef INC_INERTIAL_MEAS_UNIT_HPP_
#define INC_INERTIAL_MEAS_UNIT_HPP_

#include "mpu9250.hpp"

#include <cstdint>

namespace InertialMeasUnit {

struct Data {
    double acc[3];    // m/s^2
    double mag[3];    // uT
    double gyr[3];    // rads
};

class InertialMeasUnit : public MPU9250::MPU9250 {
  private:
    static constexpr uint8_t m_imu_i2c_address = (uint8_t)0x68;

  public:
    Data m_data;

    InertialMeasUnit();
    ~InertialMeasUnit();
    void calibrateGyro();
    Data getData();
};

void inertialMeasUnitThread(void *argument);

}    // namespace InertialMeasUnit

#endif /* INC_INERTIAL_MEAS_UNIT_HPP_ */
