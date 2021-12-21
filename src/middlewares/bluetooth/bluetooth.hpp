#pragma once

#include <cstdint>

namespace Bluetooth
{

bool transmit(const std::uint8_t& data, unsigned len, unsigned timeout_in_ms);
bool receive(const std::uint8_t& data, unsigned len, unsigned timeout_in_ms);

} // namespace Bluetooth
