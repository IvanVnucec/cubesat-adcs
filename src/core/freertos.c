#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void) {
}

/* Private application code --------------------------------------------------*/

/* Stack overflow hook
https://www.freertos.org/Stacks-and-stack-overflow-checking.html
*/
#ifdef DEBUG
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
  __disable_irq();
  while(1)
  {
  }
}
#endif /* DEBUG */
