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

#include "drivers/user/drv_tim.h"

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
    DRV_TIM_init();
}

/**
 * @brief Deinitialize all the peripherals.
 */
void DRIVERS_deinit(void)
{
    DRV_TIM_deInit();
}
