/**
 ******************************************************************************
 * @file           : zs040.h
 * @brief          : Header for zs040.c file.
 *                   This file contains the common code of the ZS-040 bluetooth
 *                   dongle.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ZS040_H
#define __ZS040_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include <stdint.h>

/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void ZS040_init(void);
void ZS040_receiveBlocking(uint8_t *data, unsigned *len);
void ZS040_sendBlocking(const uint8_t *data, const unsigned len);

#ifdef __cplusplus
}
#endif

#endif /* __ZS040_H */
