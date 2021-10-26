/*
 * uart_user.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef UART_USER_HPP_
#define UART_USER_HPP_

#include <cstdint>
#include "usart.h"

using namespace std;

class UART_User {
    private:
        UART_HandleTypeDef *m_hal_uart_ptr;

    
    protected:
        UART_User();
        ~UART_User();

    public:
        void stopReceiving();
        void startReceiving();
        void writeDataAsync(uint8_t *data, unsigned len);
        void readDataAsync();
};

#endif /* UART_USER_HPP_ */
