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
#include "zs040/zs040.h"
#include "task.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I_AM_ALIVE_STR ((const uint8_t *)"alive\n")
#define I_AM_ALIVE_STR_LEN (6u)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void ALIVE_thread(void *argument)
{
    int i = 0;

    ZS040_init();

    for (;;) {
        BSP_LED_toggle(BSP_LED_INDEX_0);

        i++;
        // send alive string every 10th time
        if (i >= 10) {
            ZS040_send(I_AM_ALIVE_STR, I_AM_ALIVE_STR_LEN);
            i = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
