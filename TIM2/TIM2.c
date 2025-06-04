//
// Created by Compu House on 5/30/2025.
//

#include "TIM2.h"
#include "Rcc.h"
#include "GPIO.h" // Ensure GPIO.h is included for GPIO_Init if you use it in TIM2.c directly for testing
#include "stm32f401xe.h" // For TIM2, GPIOA, TIM_CCMR1_CC1S_Pos, etc.

// Now using PA0 for TIM2_CH1

void TIM2_Capture_Init(void) {
    // 1. Enable clock for TIM2
    Rcc_Enable(RCC_TIM2);

    // 2. Configure PA0 to Alternate Function mode (AF1 for TIM2)
    // Configure PA0 MODER to Alternate Function mode (0b10)
    GPIOA->MODER &= ~(0x3UL << (0 * 2)); // Clear bits for PA0
    GPIOA->MODER |= (0x2UL << (0 * 2)); // Set to 0b10 (Alternate Function)

    // Configure PA0 Alternate Function Register (AFRL for pins 0-7) to AF1 for TIM2_CH1
    GPIOA->AFR[0] &= ~(0xFUL << (0 * 4)); // Clear AF bits for PA0
    GPIOA->AFR[0] |= (0x1UL << (0 * 4)); // Set to 0x1 (AF1 for TIM2)

    // 3. Configure TIM2 in Input Capture Mode
    // Set Prescaler to get 1MHz timer clock (1 tick = 1us)
    // For 84MHz: (84000000 / 1000000) - 1 = 83
    TIM2->PSC = (TIM2_CLOCK_FREQ_HZ / 1000000) - 1;

    // Set Auto-Reload Register (Period)
    TIM2->ARR = 0xFFFFFFFFUL; // Set to max for 32-bit counting

    // Counter Mode: Up-counting (default)
    TIM2->CR1 &= ~TIM_CR1_DIR;      // Ensure up-counting mode (DIR bit 0)
    TIM2->CR1 &= ~TIM_CR1_CMS_Msk;  // Ensure Edge-aligned mode (CMS bits 00)

    // 4. Configure TIM2 Input Capture Channel 1 (CH1)
    // Configure CCMR1 (Capture/Compare Mode Register 1) for Channel 1
    // Set CC1S to 01 (Input, IC1 is mapped on TI1)
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S_Msk;
    TIM2->CCMR1 |= (0x1UL << TIM_CCMR1_CC1S_Pos); // Set CC1S to 01 (Input, IC1 mapped on TI1)

    // Input Capture Prescaler: No prescaler (00) - Capture every event
    TIM2->CCMR1 &= ~TIM_CCMR1_IC1PSC_Msk; // Clear current IC1PSC bits (bits 3:2)

    // Input Capture Filter: No filter (0000)
    TIM2->CCMR1 &= ~TIM_CCMR1_IC1F_Msk; // Clear current IC1F bits (bits 7:4)

    // Configure CCER (Capture/Compare Enable Register)
    // Set CC1P (Capture/Compare 1 Polarity) to 0 (non-inverted/rising edge)
    // Set CC1NP (Capture/Compare 1 Complementary Polarity) to 0
    TIM2->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP); // Clear both bits for rising edge

    // Enable Capture/Compare 1 (CC1E bit 0)
    TIM2->CCER |= TIM_CCER_CC1E; // Set CC1E to 1

    // 5. Enable the Timer Counter
    TIM2->CR1 |= TIM_CR1_CEN; // Enable TIM2 counter (CEN bit)
}

uint32_t TIM2_GetCaptureValue(void) {
    return TIM2->CCR1; // Read the value from Capture/Compare Register 1
}

void TIM2_ClearCaptureFlag(void) {
    // Clear the Capture/Compare 1 Interrupt Flag (CC1IF) in the Status Register (SR)
    TIM2->SR &= ~TIM_SR_CC1IF;
}