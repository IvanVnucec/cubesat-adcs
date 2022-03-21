/**
  ******************************************************************************
  * @file           : alive.c
  * @brief          : Source file.
  *                   This file contains the common code of the application
  *                   which signals that everything is executing.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "alive.h"

#include "FreeRTOS.h"
#include "bsp/led/bsp_led.h"
#include "task.h"
#include "middlewares/communication/comm.h"
#include "utils/error/error.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I_AM_ALIVE_STR ("alive\n")

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const COMM_Message ALIVE_alive_msg = { I_AM_ALIVE_STR, 0 };

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void ALIVE_thread(void *argument)
{
    static COMM_Status status;
    for (;;) {
        BSP_LED_toggle(BSP_LED_INDEX_0);
        COMM_sendMessage(&ALIVE_alive_msg, &status);
        ERROR_assert(status == COMM_STATUS_OK);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
