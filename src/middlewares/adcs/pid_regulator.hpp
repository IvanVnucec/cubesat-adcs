/*
 * pid_regulator.hpp
 *
 *  Created on: 20. ruj 2021.
 *      Author: ivnucec
 */

#ifndef INC_PID_REGULATOR_HPP_
#define INC_PID_REGULATOR_HPP_

class PidRegulator {
  private:
    /* Regulator coefficients 	*/
    float m_Kp; /* Proportional */
    float m_Ki; /* Integral 	*/
    float m_Kd; /* Derivative 	*/
    float m_Ts; /* Sample time 	*/
    float m_V;  /* Real derivative term */

    float m_up0 = 0.0f; /* output of P term up(k) */
    float m_ui0 = 0.0f; /* output of I term ui(k) */
    float m_ui1 = 0.0f; /* output of I term ui(k-1) */
    float m_ud0 = 0.0f; /* output of D term ud(k) */
    float m_ud1 = 0.0f; /* output of D term ud(k-1) */
    float m_u0  = 0.0f; /* output of PID regulator u(k) */

    float m_e0 = 0.0f;
    float m_e1 = 0.0f;

    /* PID coeff from Parallel to Standard form. See documentation */
    const float m_Ti;
    const float m_Td;

  public:
    PidRegulator(float Kp, float Ki, float Kd, float V, float Ts);
    ~PidRegulator();
    float regulate(float error);
    void resetIntegral();
};

#endif /* INC_PID_REGULATOR_HPP_ */
