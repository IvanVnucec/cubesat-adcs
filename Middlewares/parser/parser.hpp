/*
 * parser.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef PARSER_PARSER_HPP_
#define PARSER_PARSER_HPP_

#include <string>

typedef void (*callback_t)(void *argument);

class Parser {
  private:
  public:
    Parser();
    ~Parser();
    bool commandReceived();
    std::string getCommand();
    void sendCommand(std::string &command);
    void registerCommandWithCallback(std::string &command, callback_t callback);
};

#endif /* PARSER_PARSER_HPP_ */
