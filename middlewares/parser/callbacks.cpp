#include "callbacks.hpp"

#include "parser.hpp"

#include <string>

namespace ParserCallback {

void regulationStart(std::string arg)
{
    (void)arg;
    Parser::sendString("regulation started\n");
    // TODO: turn regulation on
}

void regulationStop(std::string arg)
{
    (void)arg;
    Parser::sendString("regulation stopped\n");
    // TODO: turn regulation off
}

void echo(std::string arg)
{
    Parser::sendString(arg + '\n');
}

void is_pi(std::string arg)
{
    float pi          = std::stof(arg);
    bool is_pi_indeed = (pi < 3.15f && pi > 3.13f) ? true : false;

    if (is_pi_indeed)
        Parser::sendString("it is PI\n");
    else
        Parser::sendString("it is not PI\n");
}

}    // namespace ParserCallback
