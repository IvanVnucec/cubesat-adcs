/*
 * inertial_meas_unit.hpp
 *
 *  Created on: 24. ruj 2021.
 *      Author: ivnucec
 */

#ifndef INC_INERTIAL_MEAS_UNIT_HPP_
#define INC_INERTIAL_MEAS_UNIT_HPP_


class InertialMeasUnit
{
private:

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
