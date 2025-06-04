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


//                          INTERRUPT CALLBACKS                              //

void Emergency_Stop_Handler(void) {
    Emergency_Flag = 1;
}

void Reset_Button_Handler(void) {
    NVIC_SystemReset();
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
        // Existing Emergency Stop logic
        if (Emergency_Flag) {
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_WriteString(" EMERGENCY STOP");
            Emergency_Flag = 0; // Clear the flag after displaying the message
        }
        __enable_irq();

        // Existing Object Detection logic
        if (ObjectDetection_Update()) {
            uint16_t currentCount = ObjectDetection_Get_Count();

            if (currentCount != lastObjectCount) {
                lastObjectCount = currentCount;
                if (!Emergency_Flag) { // Only update if not in emergency mode
                    LCD_SetCursor(1, 0); // Row 1 for object count
                    LCD_WriteString("Count: ");
                    LCD_WriteInteger(currentCount);
                    LCD_WriteString("   "); // Clear trailing digits if new count is shorter
                }
            }
        }

        // Speed Measurement Logic
        if ((TIM2->SR & TIM_SR_CC1IF) != 0) { // Check Channel 1 flag
            uint32_t current_capture_value = TIM2_GetCaptureValue(); // Reads CCR1 (clears CC1IF)

            if (capture_sequence == 0) {
                capture1 = current_capture_value;
                capture_sequence = 1;

                // --- DIAGNOSTIC: Display the raw first capture value ---
                if (!Emergency_Flag) {
                    LCD_SetCursor(0, 7); // Position after "Speed: " on line 0
                    LCD_WriteInteger((uint16_t)capture1); // Cast to uint16_t for LCD
                    LCD_WriteString("      "); // Clear remaining characters
                }
                // ----------------------------------------------------
            } else {
                capture2 = current_capture_value;
                capture_sequence = 0;

                uint32_t period_us;
                if (capture2 >= capture1) {
                    period_us = capture2 - capture1;
                } else {
                    // Handle timer overflow (for a 32-bit TIM2 counter)
                    period_us = (0xFFFFFFFFUL - capture1) + capture2 + 1;
                }

                float frequency_Hz = 0.0f;
                if (period_us > 0) {
                    frequency_Hz = (float)1000000.0f / period_us; // Frequency in Hz (since period is in us)
                }

                // Speed Measurement Logic (Diagnostic: Display raw TIM2 counter)
                if (!Emergency_Flag) { // Only update if not in emergency mode
                    uint32_t current_timer_count = TIM2->CNT; // Read the current counter value

                    LCD_SetCursor(0, 7); // Position after "Speed: " on line 0
                    LCD_WriteInteger((uint16_t)(current_timer_count % 10000)); // Display last 4 digits for readability
                    LCD_WriteString("      "); // Clear remaining characters
                }
            }

            // Explicitly clear the flag (redundant if GetCaptureValue reads CCR1, but safe)
            TIM2_ClearCaptureFlag();
        }
    }

    return 0;
}