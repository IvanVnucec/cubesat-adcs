/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

#include <assert.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
osThreadId_t defaultTaskHandle              = NULL;
const osThreadAttr_t defaultTask_attributes = {
    .name       = "defaultTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
void StartDefaultTask(void *argument);

/* Private application code --------------------------------------------------*/
void initAdcsThreads()
{
    // default task
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
    assert(defaultTaskHandle != NULL);
}

void StartDefaultTask(void *argument)
{
    for (;;) {
    }
}