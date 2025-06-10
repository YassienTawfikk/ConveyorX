//
// Created by Rawan Shoaib on 5/30/2025.
//
#ifndef TIM2_H
#define TIM2_H

#include "Std_Types.h"

#define TIM2_CLOCK_FREQ_HZ 84000000UL

// Function Prototypes
void TIM2_Capture_Init(void);

uint32 TIM2_GetCaptureValue(void);

void TIM2_ClearCaptureFlag(void);

#endif //TIM2_H
