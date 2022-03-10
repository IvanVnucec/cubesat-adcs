#if 0
#include "pid_regulator.hpp"

#include <cassert>

PidRegulator::PidRegulator(float Kp, float Ki, float Kd, float V, float Ts)
    : m_Ti(Kp / Ki), m_Td(Kd / Kp)
{
    assert(Ts > 0.0f);
    assert(V > 0.0f);

    /* Set coefficients */
    m_Kp = Kp;
    m_Ki = Ki;
    m_Kd = Kd;
    m_Ts = Ts;
    m_V  = V;
}

PidRegulator::~PidRegulator()
{
}

float PidRegulator::regulate(float error)
{
    /* e(k) = error */
    m_e0 = error;

    /* up(k)  = Kr *   e(k) */
    m_up0 = m_Kp * m_e0;

    /* ui(k)  = ui(k-1)   + Kr * T / Ti * (  e(k)   + e(k-1)  ) / 2.0f */
    m_ui0 = m_ui1 + m_Kp * m_Ts / m_Ti * (m_e0 + m_e1) / 2.0f;

    /* ud(k)  = Td / (Td + v * T) * ud(k-1)   + Kr * v * Td / (Td + v * T) * (  e(k)   - e(k-1)  ) */
    m_ud0 = m_Td / (m_Td + m_V * m_Ts) * m_ud1
            + m_Kp * m_V * m_Td / (m_Td + m_V * m_Ts) * (m_e0 - m_e1);

    /* u(k)  =    up(k)  +    ui(k)  +    ud(k) */
    m_u0 = m_up0 + m_ui0 + m_ud0;

    /* u(k-1) = u(k) */
    /* e(k-1) = e(k) */
    m_ui1 = m_ui0;
    m_ud1 = m_ud0;
    m_e1  = m_e0;

    /* Return regulator output value */
    return m_u0;
}

void PidRegulator::resetIntegral()
{
    m_ui1 = 0.0f;
}
#endif