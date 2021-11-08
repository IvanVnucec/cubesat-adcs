/*
 * adcs_tasks.hpp
 *
 *  Created on: 18. lis 2021.
 *      Author: ivnucec
 */

#ifndef ADCS_ADCS_TASKS_HPP_
#define ADCS_ADCS_TASKS_HPP_

#include "FreeRTOS.h"
#include "cmsis_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
extern osThreadId_t faultHandlingHandle;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void initAdcsThreads();

#ifdef __cplusplus
}
#endif

#endif /* ADCS_ADCS_TASKS_HPP_ */
