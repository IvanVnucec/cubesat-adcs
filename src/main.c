/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "bsp/bsp.h"
#include "cmsis_os.h"
#include "drivers/drivers.h"
#include "mcu/mcu.h"
#include "middlewares/middlewares.h"
#include "stm32l4xx_hal.h"
#include "utils/error/error.h"

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

    MCU_init();
    BSP_init();
    DRIVERS_init();

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
