/*
 * i2c_user.hpp
 *
 *  Created on: Oct 21, 2021
 *      Author: ivnucec
 */

#ifndef I2C_USER_I2C_USER_HPP_
#define I2C_USER_I2C_USER_HPP_

#include "stm32l4xx.h"

#include <cstdint>

using namespace std;

class I2C_User {
  private:
    I2C_HandleTypeDef *m_hi2c_ptr;

  protected:
    I2C_User();
    ~I2C_User();

  public:
    void WriteMemAsync(uint16_t dev_address,
                       uint16_t mem_address,
                       uint8_t *data,
                       uint16_t data_len);
    void ReadMemAsync();
};

#endif /* I2C_USER_I2C_USER_HPP_ */