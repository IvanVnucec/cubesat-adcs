/*
 * uart_user.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef UART_USER_HPP_
#define UART_USER_HPP_

#include <cstdint>

using namespace std;

void startReceivingFromUart(void);
void getDataFromUart(uint8_t *data, unsigned len);
void sendDataWithUart(uint8_t *data, unsigned len);

#endif /* UART_USER_HPP_ */
