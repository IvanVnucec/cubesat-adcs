/*
 * i2c_user.hpp
 *
 *  Created on: Oct 21, 2021
 *      Author: ivnucec
 */

#ifndef I2C_USER_I2C_USER_HPP_
#define I2C_USER_I2C_USER_HPP_

#include "FreeRTOS.h"
#include "semphr.h"
#include "stm32l4xx.h"

#include <cstdint>
using namespace std;

class I2C_User {
  private:
    I2C_HandleTypeDef *m_hi2c;

  public:
    I2C_User(I2C_HandleTypeDef *hi2c);
    ~I2C_User();
    void sendOverDMA(uint16_t dev_address,
                     uint16_t mem_address,
                     uint8_t *data,
                     unsigned data_len);
};

#endif /* I2C_USER_I2C_USER_HPP_ */
