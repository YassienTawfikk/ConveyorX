#include "TIM_Delay.h"
#include "stm32f4xx.h"


void delay_ms(uint32 ms) {
    uint32 delay = ms * 668;
    for (int i = 0; i < delay; i++) // 668 cycles per ms (rough estimation)
        __asm__("nop");
}

void delay_us(uint32 us) {
    uint32 delay = (us * 0.668) + 1;
    for (int i = 0; i < delay; i++)
        __asm__("nop");
}
