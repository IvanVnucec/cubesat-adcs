/*
 * inertial_meas_unit.hpp
 *
 *  Created on: 24. ruj 2021.
 *      Author: ivnucec
 */

#ifndef INC_INERTIAL_MEAS_UNIT_HPP_
#define INC_INERTIAL_MEAS_UNIT_HPP_


#include <cstdint>
#include "mpu9250.hpp"


class InertialMeasUnit
{
private:
	const uint8_t m_imu_i2c_address = 0x68;

public:
	InertialMeasUnit();
	~InertialMeasUnit();
	void calibrateGyro();
	void getDataFromSensorAsync();
	void getGyroData();
	void getAccData();
	void getMagData();
};


#endif /* INC_INERTIAL_MEAS_UNIT_HPP_ */
