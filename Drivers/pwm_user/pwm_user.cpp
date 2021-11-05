#include "pwm_user.hpp"

#include <cassert>

static bool pwm_driver_initialized = false;

Pwm_User::Pwm_User()
{
    assert(pwm_driver_initialized == false);

    // add code here

    pwm_driver_initialized = true;
}

Pwm_User::~Pwm_User()
{
}
