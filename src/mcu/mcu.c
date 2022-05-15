/**
 ******************************************************************************
 * @file           : mcu.c
 * @brief          : Source file.
 *                   This file contains the common code of the application.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "mcu.h"

#include "mcu/clock/mcu_clock.h"
#include "mcu/gpio/mcu_gpio.h"
#include "mcu/i2c/mcu_i2c.h"
#include "mcu/uart/mcu_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void MCU_init(void)
{
    MCU_CLOCK_init();
    MCU_GPIO_init();
    MCU_UART_init();
    MCU_I2C_init();
}
