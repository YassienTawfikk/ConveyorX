
#include "Gpio.h"
#include "Rcc.h"
#include "sevenSegment.h"
#include "Std_Types.h"
#include "keypad.h"
#include "stm32f4xx.h"
#include "EXTI.h"
#include "LCD.h"

volatile uint8 Emergency_Flag = 0;

// Emergency stop handler (PC2)
void Emergency_Stop_Handler(void) {
    Emergency_Flag = 1;
}

// Reset button handler (PC6)
void Reset_Button_Handler(void) {
    NVIC_SystemReset();  // Software reset
}

int main(void) {
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_SYSCFG);

    LCD_Init();                  // Initialize LCD
    LCD_Clear();
    LCD_SetCursor(0, 0);         // Set cursor to row 0, column 0

    Gpio_Init(GPIO_C, 2, GPIO_INPUT, GPIO_PULL_UP);
    Gpio_Init(GPIO_C, 6, GPIO_INPUT, GPIO_PULL_UP);

    Exti_Init(PortC, 2, Falling_edge);
    Exti_Init(PortC, 6, Falling_edge);

    Exti_SetCallback(2, Emergency_Stop_Handler);
    Exti_SetCallback(6, Reset_Button_Handler);

    Exti_Enable(2);
    Exti_Enable(6);


    while (1) {
        __disable_irq();
        if (Emergency_Flag) {
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_WriteString(" EMERGENCY STOP");
            Emergency_Flag = 0;
        }
        __enable_irq();
    }
}
