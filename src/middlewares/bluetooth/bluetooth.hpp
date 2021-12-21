#pragma once

#include <cstdint>

namespace Bluetooth
{

bool transmit(const std::uint8_t* data, unsigned len, unsigned timeout_in_ms);
bool receive(std::uint8_t* data, unsigned len, unsigned timeout_in_ms);
bool receive_byte(std::uint8_t* received, unsigned timeout_in_ms);
bool receive_until(char c, 
                   std::uint8_t* data, 
                   unsigned max_len, 
                   unsigned timeout_in_ms);

} // namespace Bluetooth
