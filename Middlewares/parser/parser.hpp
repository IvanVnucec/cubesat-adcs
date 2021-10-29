/*
 * parser.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef PARSER_PARSER_HPP_
#define PARSER_PARSER_HPP_

#include "uart_user.hpp"

#include <map>
#include <string>

namespace Parser {

using callback_t = void (*)(void *arg);
using command_t  = std::string;

struct commandAndArg {
    std::string callback;
    std::string arg;
};

class Parser : public UART_User {
  private:
    // clang-format off
    std::map<command_t, callback_t> m_callbacks{
    	{"start", nullptr},
		  {"stop", nullptr}
    };
    // clang-format on

    commandAndArg extractCommandAndArgument(const char *uart_data);

  public:
    Parser();
    ~Parser();
    bool commandReceived();
    command_t getCommandFromUart();
    void callCallback(commandAndArg &command);
    void registerCommandWithCallback(command_t &command, callback_t callback);
};

void parserThread(void *argument);

}    // namespace Parser

#endif /* PARSER_PARSER_HPP_ */
