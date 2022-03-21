/**
 ******************************************************************************
 * @file           : zs040.c
 * @brief          : Source file.
 *                   This file contains the common code of the ZS-040 bluetooth
 *                   dongle.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "zs040/zs040.h"
#include "stm32l4xx_hal.h"
#include "mcu/uart/mcu_uart.h"
#include "utils/error/error.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ZS040_SEND_TIMEOUT_MS 1000U

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void ZS040_init(void)
{
    // TODO: check if MCU UART and DRIVERS are initialized
}

void ZS040_receive(uint8_t *data, unsigned *len)
{

}

void ZS040_send(const uint8_t *data, const unsigned len) {
    HAL_StatusTypeDef hal_status = HAL_UART_Transmit(&huart1, 
        (uint8_t *)data, 
        len, 
        ZS040_SEND_TIMEOUT_MS);
    ERROR_assert(hal_status == HAL_OK);
}
