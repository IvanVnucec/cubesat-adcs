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

#include "drivers/tim/tim.h"

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
    MX_TIM1_Init();
}

/**
 * @brief Deinitialize all the peripherals.
 */
void DRIVERS_deinit(void)
{
}
