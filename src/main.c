/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"
#include "mcu/mcu.h"
#include "bsp/bsp.h"
#include "drivers/drivers.h"
#include "drivers/clock/clock.h"
#include "middlewares/middlewares.h"
#include "utils/error/error.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    CLOCK_systemClockInit();

    MCU_init();
    DRIVERS_init();
    BSP_init();

    /* Init scheduler */
    osKernelInitialize(); /* Call init function for freertos objects (in freertos.c) */

    MIDDLEWARES_initThreads();

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    
    ERROR_signalFaultConditionAndWaitIndefinitely();
    
    /* Infinite loop */
    while (1) {
    }
}