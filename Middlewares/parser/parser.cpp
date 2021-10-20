/*
 * parser.cpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

// TODO [Ivan Vnucec]: Implement this

#include "parser.hpp"

#include <string>

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
