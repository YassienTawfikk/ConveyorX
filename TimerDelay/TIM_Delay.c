#include "TIM_Delay.h"
#include "stm32f4xx.h"

static volatile uint32 system_ms = 0;

void TIM_Delay_Init(void) {
    // Configure SysTick for 1ms interrupt
    SysTick_Config(SystemCoreClock / 1000); // 1 ms tick

    // Configure TIM2 for microsecond delay
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 clock
    TIM2->PSC = 84 - 1; // Prescaler for 1 MHz (assuming 84 MHz system clock)
    TIM2->ARR = 0xFFFF; // Max value
    TIM2->CNT = 0; // Reset counter
    TIM2->CR1 |= TIM_CR1_CEN; // Enable TIM2
}

void SysTick_Handler(void) {
    system_ms++;
}

uint32 millis(void) {
    return system_ms;
}

void delay_ms(uint32 ms) {
    uint32 start = millis();
    while (millis() - start < ms);
}

void delay_us(uint32 us) {
    uint32 start = TIM2->CNT;

    while ((uint16) (TIM2->CNT - start) < us);
}
