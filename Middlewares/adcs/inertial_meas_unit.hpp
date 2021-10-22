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

class InertialMeasUnit : public MPU9250 {
  private:
    static constexpr uint8_t m_imu_i2c_address = (uint8_t)0x68;

  public:
    InertialMeasUnit();
    ~InertialMeasUnit();
    void calibrateGyro();
    void getDataFromSensorAsync();
    void getGyroData(float g[3]);
    void getAccData(float a[3]);
    void getMagData(float m[3]);
};

void inertialMeasUnitThread(void *argument);

}    // namespace InertialMeasUnit

#endif /* INC_INERTIAL_MEAS_UNIT_HPP_ */
