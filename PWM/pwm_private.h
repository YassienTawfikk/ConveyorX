//
// Created by Malak on 6/5/2025.
//

#ifndef PWM_PRIVATE_H
#define PWM_PRIVATE_H
#include <Std_Types.h>

typedef struct {
    uint32 TIMx_CR1;
    uint32 TIMx_CR2;
    uint32 TIMx_SMCR;
    uint32 TIMx_DIER;
    uint32 TIMx_SR;
    uint32 TIMx_EGR;
    uint32 TIMx_CCMR1;
    uint32 TIMx_CCMR2;
    uint32 TIMx_CCER;
    uint32 TIMx_CNT;
    uint32 TIMx_PSC;
    uint32 TIMx_ARR;
    uint32 TIMx_RCR;
    uint32 TIMx_CCR1;
    uint32 TIMx_CCR2;
    uint32 TIMx_CCR3;
    uint32 TIMx_CCR4;
    uint32 TIMx_BDTR;
    uint32 TIMx_DCR;
    uint32 TIMx_DMAR;
} TIM_type;

#define TIM1   ((TIM_type*)0x40010000)

#endif //PWM_PRIVATE_H
