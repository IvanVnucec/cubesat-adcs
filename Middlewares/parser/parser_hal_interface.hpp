/*
 * parser_hal_interface.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef PARSER_HAL_INTERFACE_HPP_
#define PARSER_HAL_INTERFACE_HPP_

void startReceivingFromUart(void);
void getDataFromUart(std::uint8_t *data, unsigned len);
void sendDataWithUart(std::uint8_t *data, unsigned len);

#endif /* PARSER_HAL_INTERFACE_HPP_ */
