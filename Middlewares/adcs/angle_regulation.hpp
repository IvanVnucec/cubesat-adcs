// TODO: Port code in this file

#ifndef REG_ANGLE_H__
#define REG_ANGLE_H__

#include "pid_regulator.hpp"
#include "reaction_wheel.hpp"

#define REG_ANGLE_PID_COEFF_P 17.0f
#define REG_ANGLE_PID_COEFF_I 0.0f
#define REG_ANGLE_PID_COEFF_D 10.0f
#define REG_ANGLE_PID_COEFF_V 10.0f
#define REG_ANGLE_PID_PERIOD_S 0.1f
#define REG_ANGLE_PID_PERIOD_MS (REG_ANGLE_PID_PERIOD_S * 1000.0f)

#define REG_ANGLE_RAD_TO_DEG(x) ((x) / 3.14159f * 180.0f)

void REG_ANGLE_init(void);
void REG_ANGLE_startRegulation(void);
void REG_ANGLE_stopRegulation(void);
void REG_ANGLE_pauseRegulation(void);
float REG_ANGLE_regulate(float desired_angle, float real_angle);

#endif /* REG_ANGLE_H__ */
