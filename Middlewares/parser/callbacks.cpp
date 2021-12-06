#include "callbacks.hpp"

#include <string>

namespace ParserCallback {

void regulationStart(std::string arg)
{
    (void)arg;
    // turn regulation on
}

void regulationStop(std::string arg)
{
    (void)arg;
    // turn regulation off
}

}    // namespace ParserCallback
