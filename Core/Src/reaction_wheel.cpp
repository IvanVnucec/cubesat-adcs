// TODO: Port code in this file


#include "reaction_wheel.hpp"


ReactionWheel::ReactionWheel()
{
/*
 * // Initialize pins for motor control
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_DIRECTION_PIN, OUTPUT);

  // Turn off motor
  analogWrite(MOTOR_PIN, 0u);

  // Set motor direction to default
  digitalWrite(MOTOR_DIRECTION_PIN, 0u);
 */
}


ReactionWheel::~ReactionWheel()
{
}


void ReactionWheel::setAngularVelocity(int angular_vel)
{
	/*
	 * TODO: Change pwm variable to angular_vel


	   if (pwm < 0) {
    digitalWrite(MOTOR_DIRECTION_PIN, MOTOR_DIRECTION_CW);
  } else {
    digitalWrite(MOTOR_DIRECTION_PIN, MOTOR_DIRECTION_ACW);
  }

  if (pwm < 0) {
    pwm = -pwm;
  }

  if (pwm > MOTOR_MAX_PWM) {
    pwm = MOTOR_MAX_PWM;
  }

  analogWrite(MOTOR_PIN, pwm);
	 */
}
