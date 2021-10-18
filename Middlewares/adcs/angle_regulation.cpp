// TODO: Port code in this file

#include "angle_regulation.hpp"

#include "FreeRTOS.h"
#include "task.h"

//static PID_Handle_t hpid;
static int regulation_running = 0;

void REG_ANGLE_init(void)
{
    /*
	PID_Init(&hpid,
	REG_ANGLE_PID_COEFF_P,
	REG_ANGLE_PID_COEFF_I,
	REG_ANGLE_PID_COEFF_D,
	REG_ANGLE_PID_COEFF_V,
	REG_ANGLE_PID_PERIOD_S);

	MOTOR_init();
	*/

    regulation_running = 0;
}

void REG_ANGLE_startRegulation(void)
{
    regulation_running = 1;
}

void REG_ANGLE_stopRegulation(void)
{
    regulation_running = 0;
    /*
	PID_ResetIntegral(&hpid);
	MOTOR_setPwm(0);
	*/
}

void REG_ANGLE_pauseRegulation(void)
{
    regulation_running = 0;
}

float REG_ANGLE_regulate(float desired_angle, float real_angle)
{
    float output = 0.0f;
    /*
	float error;

	if (regulation_running == 1) {
		error = desired_angle - real_angle;
		output = PID_Regulate(&hpid, error);
		MOTOR_setPwm(output);
	}
	*/

    return output;
}

/**
  * @brief  Function implementing the Angle regulation thread callback.
  * @param  argument: Not used
  * @retval None
  */
void REG_ANGLE_taskCallback(void *argument)
{
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        // TODO: Add Angle regulation code here
    }
}
