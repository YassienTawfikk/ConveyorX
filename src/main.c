#include "stm32f4xx.h"

// Core drivers
#include "Std_Types.h"
#include "Gpio.h"
#include "Rcc.h"
#include "EXTI.h"

// Application drivers
#include "LCD.h"
#include "sevenSegment.h"    // Assuming this is used elsewhere in your project
#include "ObjectDetection.h"
#include "TIM2.h"            // Include TIM2 header

// Pin definitions
#define IR_PIN            2  // GPIOA
#define EMERGENCY_PIN     2  // GPIOC
#define RESET_PIN         6  // GPIOC

// Global variables
volatile uint8 Emergency_Flag = 0;
uint16_t lastObjectCount = 0;

// Global variables for speed measurement
volatile uint32_t capture1 = 0;
volatile uint32_t capture2 = 0;
volatile uint8_t capture_sequence = 0; // 0: waiting for first capture, 1: waiting for second
#define DISTANCE_MM 50.0f  // Distance moved per rising edge
int lcd_cleared = 0;


//                          INTERRUPT CALLBACKS                              //

void Emergency_Stop_Handler(void) {
    Emergency_Flag = 1;
}

void Reset_Button_Handler(void) {
   // NVIC_SystemReset();
    Emergency_Flag = 0;
    lcd_cleared = 0;         // Allow the LCD to update again
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_WriteString(" Speed: ");
    // LCD_SetCursor(1, 0);
    // LCD_WriteString("TMR: ");

}


//                           INITIALIZATION FUNCTIONS                        //

void Init_Clocks(void) {
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);   // Needed for PA0 (TIM2_CH1) and IR_PIN
    Rcc_Enable(RCC_GPIOB);   // Needed for LCD and potentially other GPIOs
    Rcc_Enable(RCC_GPIOC);   // Needed for Emergency/Reset buttons
    Rcc_Enable(RCC_SYSCFG);  // Needed for EXTI
    Rcc_Enable(RCC_TIM2);
}

void Init_LCD(void) {
    LCD_Init();
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_WriteString("Speed: "); // Label for speed on LCD line 0
    // Object count will be displayed on line 1, so no initial label needed here.
    // LCD_SetCursor(1, 0);
    // LCD_WriteString("TMR: ");
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

// Initialization function for speed measurement
void Init_SpeedMeasurement(void) {
    // TIM2_Capture_Init() will configure PA0 for TIM2_CH1
    TIM2_Capture_Init();
}


//                               MAIN PROGRAM                                //

int main(void) {
    Init_Clocks();
    Init_LCD();
    Init_ObjectDetection();
    Init_ButtonsAndInterrupts();
    Init_SpeedMeasurement(); // Call speed measurement init

    while (1) {
        __disable_irq();
        // Emergency Stop
        if (Emergency_Flag) {
            if (!lcd_cleared) {
                LCD_Clear();
                LCD_SetCursor(0, 0);
                LCD_WriteString(" EMERGENCY STOP");
                lcd_cleared = 1;
                // Emergency_Flag = 0; // Clear the flag after displaying the message
            }
            __enable_irq();
            continue;
        }

        __enable_irq();
        lcd_cleared = 0;

        // … inside the main loop …
        
        if (ObjectDetection_Task())           // ← now using the new API
        {
            uint16_t cnt = ObjectDetection_GetCount();
        
            LCD_SetCursor(1, 0);
            LCD_WriteString("Count: ");
            LCD_WriteInteger(cnt);
            LCD_WriteString("   ");            // clear leftovers
        }



        // // timer counter
        // LCD_SetCursor(1, 4);
        // LCD_WriteInteger((uint16_t)(TIM2->CNT % 10000));
        // delay_ms(100);

        // rising-edge capture
        if ((TIM2->SR & TIM_SR_CC1IF) != 0) {
            uint32_t current_capture_value = TIM2_GetCaptureValue();

            if (capture_sequence == 0) {
                capture1 = current_capture_value;
                capture_sequence = 1;
            } else {
                capture2 = current_capture_value;
                capture_sequence = 0;

                uint32_t period_us;
                if (capture2 >= capture1)
                    period_us = capture2 - capture1;
                else
                    period_us = (0xFFFFFFFFUL - capture1) + capture2 + 1;

                // Filter out bad/zero readings
                if (period_us > 0 && period_us < 1000000) {
                    float speed_mm_per_s = DISTANCE_MM * (1000000.0f / period_us);
                    LCD_SetCursor(0, 7);
                    LCD_WriteString("     ");  // Clear old
                    LCD_SetCursor(0, 7);
                    LCD_WriteInteger((uint16_t)speed_mm_per_s);
                    LCD_WriteString(" mm/s");
                }
            }

            TIM2_ClearCaptureFlag();
        }

        delay_ms(100);
    }

    return 0;
}
