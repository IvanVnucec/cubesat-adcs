/*
 * callbacks.hpp
 *
 *  Created on: Nov 1, 2021
 *      Author: ivan
 */

#ifndef PARSER_CALLBACKS_HPP_
#define PARSER_CALLBACKS_HPP_

#include <string>

namespace ParserCallback {

void regulationStart(std::string arg);
void regulationStop(std::string arg);
void echo(std::string arg);
void is_pi(std::string arg);

}    // namespace ParserCallback

#endif /* PARSER_CALLBACKS_HPP_ */
