/*
 * parser.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef PARSER_PARSER_HPP_
#define PARSER_PARSER_HPP_

#include <memory>
#include <string>
#include <vector>

typedef void (*callback_t)(void *argument);

class Parser {
  private:
    std::unique_ptr<std::string[]> m_commands;
    std::unique_ptr<callback_t[]> m_callbacks;

  public:
    Parser(size_t num_of_commands);
    ~Parser();
    bool commandReceived();
    std::string getCommand();
    void sendCommand(std::string &command);
    void registerCommandWithCallback(std::string &command, callback_t callback);
};

#endif /* PARSER_PARSER_HPP_ */
