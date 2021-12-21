#include "bluetooth.hpp"
#include "FreeRTOS.h"
#include "semphr.h"

namespace Bluetooth
{

static bool initialized = false;
static SemaphoreHandle_t i2cTxSemaphore = NULL;
static SemaphoreHandle_t i2cRxSemaphore = NULL;

bool init()
{
    i2cTxSemaphore = xSemaphoreCreateBinary();
    if (i2cTxSemaphore == NULL)
        goto return_failure;

    i2cRxSemaphore = xSemaphoreCreateBinary();
    if (i2cRxSemaphore == NULL)
        goto return_failure;

    initialized = true;

    return true;

return_failure:
    return false;
}

bool transmit(const std::uint8_t& data, unsigned len, unsigned timeout_in_ms)
{
    if (not initialized)
        goto return_failure;

    // TODO: add code here
    return true;

return_failure:
    return false;
}


bool receive(const std::uint8_t& data, unsigned len, unsigned timeout_in_ms)
{
    if (not initialized)
        goto return_failure;

    // TODO: add code here
    return true;

return_failure:
    return false;
}

} // namespace Bluetooth
