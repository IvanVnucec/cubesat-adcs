/*
 * parser.cpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

// TODO [Ivan Vnucec]: Implement this

#include "parser.hpp"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "uart_user.hpp"    // TODO: Delete me

#include <map>
#include <memory>
#include <string>

namespace Parser {

Parser::Parser()
{
}

Parser::~Parser()
{
}

bool Parser::commandReceived()
{
    return false;
}

commandAndArg Parser::extractCommandAndArgument(const char *uart_data)
{
    commandAndArg retval;

    // extract command and argument
    (void)uart_data;
    retval.callback = "asd";
    retval.arg      = "0.03";

    return retval;
}

command_t Parser::getCommandFromUart()
{
    static constexpr unsigned uart_data_max_len = 100;
    char uart_data[uart_data_max_len];

    UART_User::readDataAsyncUntilChar((uint8_t *)uart_data, '\n', uart_data_max_len);

    commandAndArg ca = extractCommandAndArgument(uart_data);

    return command_t((const char *)uart_data);
}

void Parser::callCallback(commandAndArg &ca)
{
    m_callbacks[ca.callback](ca.arg);
}

void Parser::registerCommandWithCallback(command_t &command, callback_t callback)
{
    // add command and callback to the list of registers
}

void parserThread(void *argument)
{
    Parser parser;

    for (;;) {
        command_t command = parser.getCommandFromUart();
        commandAndArg ca  = parser.callCallback(command);
    }
}

}    // namespace Parser
