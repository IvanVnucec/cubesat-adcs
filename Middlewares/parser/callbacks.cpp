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
}

}    // namespace ParserCallback
