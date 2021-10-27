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

#include <memory>
#include <string>

namespace Parser {

Parser::Parser(size_t num_of_commands)
    : m_commands(std::unique_ptr<std::string[]>(new std::string[num_of_commands])),
      m_callbacks(std::unique_ptr<callback_t[]>(new callback_t[num_of_commands]))
{
}

Parser::~Parser()
{
}

bool Parser::commandReceived()
{
    return false;
}

std::string Parser::getCommand()
{
    return std::string();
}

void Parser::sendCommand(std::string &command)
{
}

void Parser::registerCommandWithCallback(std::string &command, callback_t callback)
{
}

void parserThread(void *argument)
{
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

}    // namespace Parser
