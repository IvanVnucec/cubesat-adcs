/**
  ******************************************************************************
  * @file           : drivers.c
  * @brief          : Source file for initialization of all the drivers.
  *                   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drivers.h"

#include "drivers/i2c/i2c.h"
#include "drivers/spi/spi.h"
#include "drivers/tim/tim.h"
#include "drivers/uart/usart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/**
 * @brief Initialize all the peripherals.
 */
void DRIVERS_init(void)
{
    MX_SPI1_Init();
    MX_TIM1_Init();
    MX_USART1_UART_Init();
    MX_I2C3_Init();
}

/**
 * @brief Deinitialize all the peripherals.
 */
void DRIVERS_deinit(void)
{
}
