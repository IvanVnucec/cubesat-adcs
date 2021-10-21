/*
 * uart_hal_interface.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef UART_HAL_INTERFACE_HPP_
#define UART_HAL_INTERFACE_HPP_

void startReceivingFromUart(void);
void getDataFromUart(std::uint8_t *data, unsigned len);
void sendDataWithUart(std::uint8_t *data, unsigned len);

#endif /* UART_HAL_INTERFACE_HPP_ */
