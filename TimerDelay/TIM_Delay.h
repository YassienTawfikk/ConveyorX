//
// Created by Yassien Tawfik on 05/06/2025.
//
#ifndef TIM_DELAY_H
#define TIM_DELAY_H

#include "Std_Types.h"

void TIM_Delay_Init(void);

uint32 millis(void);

void delay_ms(uint32 ms);

void delay_us(uint32 us);

#endif
