#ifndef __MCU_TIM_H__
#define __MCU_TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void MCU_TIM_init(void);
void MCU_TIM_pwmStart(void);
void MCU_TIM_pwmStop(void);
void MCU_TIM_pwmSetCompare(uint32_t val);

#ifdef __cplusplus
}
#endif

#endif /* __MCU_TIM_H__ */
