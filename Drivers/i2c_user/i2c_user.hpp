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

namespace I2C_User {

using namespace std;

class I2C_User {
  private:
    I2C_HandleTypeDef *m_hi2c_ptr;

  protected:
    I2C_User();
    ~I2C_User();
    void WriteMemAsync(uint16_t dev_address,
                       uint16_t mem_address,
                       uint8_t *data,
                       uint16_t data_len);
    void ReadMemAsync(uint16_t dev_address,
                      uint16_t mem_address,
                      uint8_t *data,
                      uint16_t data_len);
};

} // namespace I2C_User 

#endif /* I2C_USER_I2C_USER_HPP_ */
