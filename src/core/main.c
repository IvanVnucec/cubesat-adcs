/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp/bsp_config.h"
#include "drivers/drivers.h"
#include "drivers/clock/clock.h"
#include "middlewares/middlewares.h"
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

    DRIVERS_init();

    /* Init scheduler */
    osKernelInitialize(); /* Call init function for freertos objects (in freertos.c) */

    MIDDLEWARES_initThreads();

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    while (1) {
    }
}


void StartDefaultTask(void *argument)
{
    for (;;) {
        HAL_GPIO_TogglePin(BSP_CONFIG_LD3_GPIO_Port, BSP_CONFIG_LD3_Pin);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
