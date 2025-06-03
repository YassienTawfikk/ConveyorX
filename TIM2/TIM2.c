//
// Created by Compu House on 5/30/2025.
//

#include "TIM2.h"
#include "Rcc.h"
#include "stm32f401xe.h"

// Assuming TIM2_CH2 is connected to PA1 for this example

void TIM2_Capture_Init(void) {
    // 1. Enable clock for TIM2
    Rcc_Enable(RCC_TIM2);


    // Configure PA1 MODER to Alternate Function mode (0b10)
    GPIOA->MODER &= ~(0x3UL << (1 * 2));
    GPIOA->MODER |= (0x2UL << (1 * 2)); // 0x2 corresponds to 0b10

    // Configure PA1 Alternate Function Register (AFRL for pins 0-7) to AF1 for TIM2_CH2
    GPIOA->AFR[0] &= ~(0xFUL << (1 * 4));
    GPIOA->AFR[0] |= (0x1UL << (1 * 4)); // 0x1 corresponds to AF1


    // 3. Configure TIM2 in Input Capture Mode
    // Set Prescaler
    // (TIM2_CLOCK_FREQ_HZ / 1000000) - 1 for 1MHz timer clock (1 tick = 1us)
    // For 84MHz: (84000000 / 1000000) - 1 = 84 - 1 = 83
    TIM2->PSC = (TIM2_CLOCK_FREQ_HZ / 1000000) - 1; // Set prescaler to get 1MHz timer clock

    // Set Auto-Reload Register (Period)
    TIM2->ARR = 0xFFFF; // Max period for 16-bit timer

    // Counter Mode: Up-counting (default, but can be explicitly set by clearing bits)
    TIM2->CR1 &= ~TIM_CR1_DIR; // Ensure up-counting mode (DIR bit 0)
    TIM2->CR1 &= ~TIM_CR1_CMS_Msk; // Ensure Edge-aligned mode (CMS bits 00)

    // 4. Configure TIM2 Input Capture Channel (CH2)
    // Configure CCMR1 (Capture/Compare Mode Register 1) for Channel 2
    // Set CC2S to 01 (Input, IC2 is mapped on TI2)
    // Clear current CC2S bits (bits 11:10) and then set them
    TIM2->CCMR1 &= ~TIM_CCMR1_CC2S_Msk;
    TIM2->CCMR1 |= (0x1UL << TIM_CCMR1_CC2S_Pos); // Set CC2S to 01 (Input, IC2 mapped on TI2)

    // Input Capture Prescaler: No prescaler (00) - Capture every event
    // Clear current IC2PSC bits (bits 13:12)
    TIM2->CCMR1 &= ~TIM_CCMR1_IC2PSC_Msk; // Already 0x0UL for DIV1 (no prescaler)

    // Input Capture Filter: No filter (0000)
    // Clear current IC2F bits (bits 15:12)
    TIM2->CCMR1 &= ~TIM_CCMR1_IC2F_Msk; // Already 0x0UL for no filter

    // Configure CCER (Capture/Compare Enable Register)
    // Set CC2P (Capture/Compare 2 Polarity) to 0 (non-inverted/rising edge)
    // Clear CC2P (bit 5) for rising edge. Note: CCxP = 0 for rising, 1 for falling.
    TIM2->CCER &= ~TIM_CCER_CC2P;

    // Enable Capture/Compare 2 (CC2E bit 4)
    TIM2->CCER |= TIM_CCER_CC2E; // Set CC2E to 1

    // 5. Enable the Timer Counter
    TIM2->CR1 |= TIM_CR1_CEN; // Enable TIM2 counter
}

uint32_t TIM2_GetCaptureValue(void) {
    return TIM2->CCR2; // Read the value from Capture/Compare Register 2
}

void TIM2_ClearCaptureFlag(void) {
    // Clear the Capture/Compare 2 Interrupt Flag (CC2IF) in the Status Register (SR)
    TIM2->SR &= ~TIM_SR_CC2IF;
}