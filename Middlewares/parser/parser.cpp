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
#include "uart_user.hpp"

#include <map>
#include <memory>
#include <string>

namespace Parser {

using namespace UART_User;

Parser::Parser() : UART_User()
{
}

Parser::~Parser()
{
}

commandAndArg Parser::extractCommandAndArgument(const char *uart_data,
                                                const unsigned uart_data_len)
{
    commandAndArg retval;

    // command example: commandName(3.14) or commandName2(True) etc.
    std::string data(uart_data, uart_data_len);
    auto open_bracket_pos  = data.find('(');
    auto close_bracket_pos = data.find(')');

    private_assert(open_bracket_pos != std::string::npos);
    private_assert(close_bracket_pos != std::string::npos);

    retval.callback = data.substr(0, open_bracket_pos);
    retval.arg      = data.substr(open_bracket_pos, close_bracket_pos);

    return retval;
}

commandAndArg Parser::getCommandFromUart()
{
    static constexpr unsigned uart_data_max_len = 100;
    char uart_data[uart_data_max_len];

    unsigned chars_read =
        readDataAsyncUntilChar((uint8_t *)uart_data, '\n', uart_data_max_len);
    commandAndArg ca = extractCommandAndArgument(uart_data, chars_read);

    return ca;
}

void Parser::callCallback(commandAndArg &ca)
{
    // TODO: convert ca.arg (std::string) to function argument (void *)
    m_callbacks[ca.callback](nullptr);
}

void Parser::private_assert(bool condition) 
{
    if (not condition)
        parserErrorHandle();
}

void Parser::uartDriverErrorHandle()
{
    parserErrorHandle();
}


void Parser::parserErrorHandle()
{
#ifdef DEBUG
    while(true);
#endif
}

void parserThread(void *argument)
{
    Parser parser;

    for (;;) {
        commandAndArg command = parser.getCommandFromUart();
        parser.callCallback(command);
    }
}


}    // namespace Parser
