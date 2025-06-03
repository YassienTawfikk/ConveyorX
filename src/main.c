#include "stm32f4xx.h"

// Core drivers
#include "Std_Types.h"
#include "Gpio.h"
#include "Rcc.h"
#include "EXTI.h"

// Application drivers
#include "LCD.h"
#include "sevenSegment.h"
#include "ObjectDetection.h"

// Pin definitions
#define IR_PIN            2  // GPIOA
#define EMERGENCY_PIN     2  // GPIOC
#define RESET_PIN         6  // GPIOC

// Global variables
volatile uint8 Emergency_Flag = 0;
uint16_t lastObjectCount = 0;



//                          INTERRUPT CALLBACKS                              //

void Emergency_Stop_Handler(void) {
    Emergency_Flag = 1;
}

void Reset_Button_Handler(void) {
    NVIC_SystemReset();  // Software reset
}



//                           INITIALIZATION FUNCTIONS                        //

void Init_Clocks(void) {
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_SYSCFG);
}

void Init_LCD(void) {
    LCD_Init();
    LCD_Clear();
    LCD_SetCursor(0, 0);
}

void Init_ObjectDetection(void) {
    ObjectDetection_Init(GPIO_A, IR_PIN);
}

void Init_ButtonsAndInterrupts(void) {
    // Configure emergency stop and reset buttons
    Gpio_Init(GPIO_C, EMERGENCY_PIN, GPIO_INPUT, GPIO_PULL_UP);
    Gpio_Init(GPIO_C, RESET_PIN, GPIO_INPUT, GPIO_PULL_UP);

    // Configure external interrupts
    Exti_Init(PortC, EMERGENCY_PIN, Falling_edge);
    Exti_Init(PortC, RESET_PIN, Falling_edge);

    Exti_SetCallback(EMERGENCY_PIN, Emergency_Stop_Handler);
    Exti_SetCallback(RESET_PIN, Reset_Button_Handler);

    Exti_Enable(EMERGENCY_PIN);
    Exti_Enable(RESET_PIN);
}



//                               MAIN PROGRAM                                //

int main(void) {
    Init_Clocks();
    Init_LCD();
    Init_ObjectDetection();
    Init_ButtonsAndInterrupts();

    while (1) {
        __disable_irq();

        if (Emergency_Flag) {
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_WriteString(" EMERGENCY STOP");
            Emergency_Flag = 0;
        }

        __enable_irq();

        // Update object counter display
        if (ObjectDetection_Update()) {
            uint16_t currentCount = ObjectDetection_Get_Count();

            if (currentCount != lastObjectCount) {
                lastObjectCount = currentCount;
                LCD_SetCursor(1, 0);
                LCD_WriteString("Count: ");
                LCD_PrintNumber(currentCount);
            }
        }
    }
}
