//
// Created by Rawan Shoaib on 6/5/2025.
//

#ifndef TIM2_PRIVARTE_H
#define TIM2_PRIVARTE_H


#include <stdint.h>

// Macro to define a 32-bit volatile register pointer
#define REG32(addr) ((volatile uint32_t *)(addr))

// APB1 Bus Peripherals
#define TIM2_BASE_ADDR          0x40000000 // TIM2 Base Address


// --- TIM2 Register Definitions (Offsets from TIM2_BASE_ADDR) ---

#define TIM2_CR1_OFFSET         0x00 // Control Register 1
#define TIM2_PSC_OFFSET         0x28 // Prescaler
#define TIM2_ARR_OFFSET         0x2C // Auto-Reload Register
#define TIM2_CCMR1_OFFSET       0x18 // Capture/Compare Mode Register 1
#define TIM2_CCER_OFFSET        0x20 // Capture/Compare Enable Register
#define TIM2_CCR1_OFFSET        0x34 // Capture/Compare Register 1
#define TIM2_SR_OFFSET          0x10 // Status Register

// Dereferenced Register Pointers for TIM2
#define TIM2_CR1                REG32(TIM2_BASE_ADDR + TIM2_CR1_OFFSET)
#define TIM2_PSC                REG32(TIM2_BASE_ADDR + TIM2_PSC_OFFSET)
#define TIM2_ARR                REG32(TIM2_BASE_ADDR + TIM2_ARR_OFFSET)
#define TIM2_CCMR1              REG32(TIM2_BASE_ADDR + TIM2_CCMR1_OFFSET)
#define TIM2_CCER               REG32(TIM2_BASE_ADDR + TIM2_CCER_OFFSET)
#define TIM2_CCR1               REG32(TIM2_BASE_ADDR + TIM2_CCR1_OFFSET)
#define TIM2_SR                 REG32(TIM2_BASE_ADDR + TIM2_SR_OFFSET)


#endif //TIM2_PRIVARTE_H
