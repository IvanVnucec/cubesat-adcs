/*
 * parser.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef PARSER_PARSER_HPP_
#define PARSER_PARSER_HPP_

#include "callbacks.hpp"
#include "uart_user.hpp"

#include <map>
#include <string>

namespace Parser {

using namespace UART_User;
using callback_t = void (*)(void *arg);

struct commandAndArg {
    std::string callback;
    std::string arg;
};

class Parser : public UART_User {
  private:
    // clang-format off
    std::map<std::string, callback_t> m_callbacks{
    	{"RegulationStart", ParserCallback::RegulationStart},
		  {"RegulationStop",  ParserCallback::RegulationStop}
    };
    // clang-format on

    commandAndArg extractCommandAndArgument(const char *uart_data,
                                            const unsigned uart_data_len);
    void private_assert(bool condition);

  public:
    Parser();
    ~Parser();
    void parserErrorHandle();
    commandAndArg getCommandFromUart();
    void callCallback(commandAndArg &command);
};

void parserThread(void *argument);
void sendString(std::string str);

}    // namespace Parser

#endif /* PARSER_PARSER_HPP_ */
