//
// Created by Rawan Shoaib on 5/30/2025.
//

#include "TIM2.h"
#include "Rcc.h"
#include "GPIO.h"
#include "GPIO_Private.h"
#include "TIM2_private.h"


/* These define the bit positions and masks specific to TIM2's internal registers.*/

// TIM2 CR1 Register (Control Register 1)
#define TIM2_CR1_CEN_Pos                (0)
#define TIM2_CR1_CEN_Msk                (0x1 << TIM2_CR1_CEN_Pos)
#define TIM2_CR1_DIR_Pos                (4)
#define TIM2_CR1_DIR_Msk                (0x1 << TIM2_CR1_DIR_Pos)
#define TIM2_CR1_CMS_Pos                (5)
#define TIM2_CR1_CMS_Msk                (0x3 << TIM2_CR1_CMS_Pos)

// TIM2 CCMR1 Register (Capture/Compare Mode Register 1) - Input Mode
#define TIM2_CCMR1_CC1S_Pos             (0)
#define TIM2_CCMR1_CC1S_Msk             (0x3 << TIM2_CCMR1_CC1S_Pos) // Bits 1:0 for CC1S



#define TIM2_CCMR1_IC1PSC_Pos           (2)
#define TIM2_CCMR1_IC1PSC_Msk           (0x3 << TIM2_CCMR1_IC1PSC_Pos) // Bits 3:2 for IC1PSC
#define TIM2_CCMR1_IC1F_Pos             (4)
#define TIM2_CCMR1_IC1F_Msk             (0xF << TIM2_CCMR1_IC1F_Pos) // Bits 7:4 for IC1F

// TIM2 CCER Register (Capture/Compare Enable Register)
#define TIM2_CCER_CC1E_Pos              (0)
#define TIM2_CCER_CC1E_Msk              (0x1 << TIM2_CCER_CC1E_Pos)
#define TIM2_CCER_CC1P_Pos              (1)
#define TIM2_CCER_CC1P_Msk              (0x1 << TIM2_CCER_CC1P_Pos)
#define TIM2_CCER_CC1NP_Pos             (3)
#define TIM2_CCER_CC1NP_Msk             (0x1 << TIM2_CCER_CC1NP_Pos)

// TIM2 SR Register (Status Register)
#define TIM2_SR_CC1IF_Pos               (1)
#define TIM2_SR_CC1IF_Msk               (0x1 << TIM2_SR_CC1IF_Pos)


void TIM2_Capture_Init(void) {
    // 1. Enable clock for TIM2
    Rcc_Enable(RCC_TIM2);

    // 2. Configure PA0 to Alternate Function mode (AF1 for TIM2_CH1)
    GPIO_Init(GPIO_A, 0, GPIO_AF, GPIO_NO_PULL_DOWN);

    // Configure PA0 Alternate Function Register (AFRL for pins 0-7) to AF1 for TIM2_CH1
    *GPIOA_AFRL &= ~(0xF << (0 * 4)); // Clear AF bits for PA0 (bits 3:0)
    *GPIOA_AFRL |= (0x1 << (0 * 4));  // Set to 0x1 (AF1 for TIM2)




    // 3. Configure TIM2 in Input Capture Mode

    // 5. Enable the Timer Counter
    *TIM2_CR1 |= TIM2_CR1_CEN_Msk; // Enable TIM2 counter (CEN bit)

    // Counter Mode: Up-counting (default)
    *TIM2_CR1 &= ~TIM2_CR1_DIR_Msk;      // Ensure up-counting mode (DIR bit 0)
    *TIM2_CR1 &= ~TIM2_CR1_CMS_Msk;      // Ensure Edge-aligned mode (CMS bits 00)


    // Set Prescaler to get 1MHz timer clock (1 tick = 1us)
    // For 84MHz: (84000000 / 1000000) - 1 = 83
    *TIM2_PSC = (TIM2_CLOCK_FREQ_HZ / 1000000UL) - 1;

    // Set Auto-Reload Register (Period)
    *TIM2_ARR = 0xFFFFFFFFUL; // Set to max for 32-bit counting

    // Configure TIM2 Input Capture Channel 1 (CH1)
    // Configure CCMR1 (Capture/Compare Mode Register 1) for Channel 1
    // Set CC1S to 01 (Input, IC1 is mapped on TI1)
    *TIM2_CCMR1 &= ~TIM2_CCMR1_CC1S_Msk;
    *TIM2_CCMR1 |= (0x1 << TIM2_CCMR1_CC1S_Pos); // Set CC1S to 01 (Input, IC1 mapped on TI1)

    // Input Capture Prescaler: No prescaler (00) - Capture every event
    *TIM2_CCMR1 &= ~TIM2_CCMR1_IC1PSC_Msk; // Clear current IC1PSC bits (bits 3:2)

    // Input Capture Filter: No filter (0000)
    *TIM2_CCMR1 &= ~TIM2_CCMR1_IC1F_Msk; // Clear current IC1F bits (bits 7:4)

    // Configure CCER (Capture/Compare Enable Register)
    // Set CC1P (Capture/Compare 1 Polarity) to 0 (non-inverted/rising edge)
    // Set CC1NP (Capture/Compare 1 Complementary Polarity) to 0
    // Clear both bits for rising edge
    *TIM2_CCER &= ~(TIM2_CCER_CC1P_Msk);
    *TIM2_CCER &= ~(TIM2_CCER_CC1NP_Msk);

    // Enable Capture/Compare 1 (CC1E bit 0)
    *TIM2_CCER |= TIM2_CCER_CC1E_Msk; // Set CC1E to 1
}

uint32 TIM2_GetCaptureValue(void) {
    // Read the value from Capture/Compare Register 1
    return *TIM2_CCR1;
}

void TIM2_ClearCaptureFlag(void) {
    // Clear the Capture/Compare 1 Interrupt Flag (CC1IF) in the Status Register (SR)
    *TIM2_SR &= ~TIM2_SR_CC1IF_Msk;
}