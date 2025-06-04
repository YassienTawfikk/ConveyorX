//
// Created by Rawan Shoaib on 5/30/2025.
//
#ifndef TIM2_H
#define TIM2_H

#include "stm32f401xe.h"
#include "Std_Types.h" // Ensure this includes uint32_t

#define TIM2_CLOCK_FREQ_HZ 84000000UL // 84 MHz

void TIM2_Capture_Init(void);
uint32_t TIM2_GetCaptureValue(void); // Changed to uint32_t
void TIM2_ClearCaptureFlag(void);


#endif //TIM2_H