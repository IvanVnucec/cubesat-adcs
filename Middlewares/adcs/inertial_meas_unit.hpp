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

class InertialMeasUnit {
  private:
    static constexpr uint8_t m_imu_i2c_address = (uint8_t) 0x68;

  public:
    InertialMeasUnit();
    ~InertialMeasUnit();
    void calibrateGyro();
    void getDataFromSensorAsync();
    void getGyroData();
    void getAccData();
    void getMagData();
};

void inertialMeasUnitThread(void *argument);

}    // namespace InertialMeasUnit

#endif /* INC_INERTIAL_MEAS_UNIT_HPP_ */
