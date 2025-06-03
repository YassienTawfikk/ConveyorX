#include "Rcc.h"
#include "Gpio.h"
#include "LCD.h"

int main(void) {
    Rcc_Init();                   // Initialize RCC
    Rcc_Enable(RCC_GPIOB);       // Enable GPIOB for LCD

    LCD_Init();                  // Initialize LCD
    LCD_SetCursor(0, 0);         // Set cursor to row 0, column 0
    LCD_WriteString(" HELLO WORLD");

    while (1) {
        // Idle loop
    }
}
