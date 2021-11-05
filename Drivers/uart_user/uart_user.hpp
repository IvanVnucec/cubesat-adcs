/*
 * uart_user.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef UART_USER_HPP_
#define UART_USER_HPP_

#include "usart.h"

#include <cstdint>

namespace Uart_User {

using namespace std;

class UART_User {
  private:
    UART_HandleTypeDef *m_hal_uart_ptr;
    void private_assert(bool condition);

  protected:
    UART_User();
    ~UART_User();
    virtual void uartDriverErrorHandle();
    void stopReceiving();
    void startReceiving();
    void writeDataAsync(const uint8_t *data, unsigned len);
    uint8_t readByteAsync();
    void readDataAsync(uint8_t *data, unsigned len);
    unsigned readDataAsyncUntilChar(uint8_t *data, char c, unsigned max_len);

  public:
};

}    // namespace Uart_User

#endif /* UART_USER_HPP_ */
