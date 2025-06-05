#include "stm32f4xx.h"

// ───── Core Drivers ─────────────────────────────────────
#include "Std_Types.h"
#include "GPIO.h"
#include "Rcc.h"
#include "EXTI.h"

// ───── Application Drivers ──────────────────────────────
#include "LCD.h"
#include "ObjectDetection.h"
#include "TIM2.h"

// ───── Pin Definitions ──────────────────────────────────
#define IR_PIN             2   // GPIO_A
#define EMERGENCY_PIN      2   // GPIO_C
#define RESET_PIN          6   // GPIO_C
#define DISTANCE_MM     50.0f  // Distance moved per edge

// ───── Global Variables ─────────────────────────────────
volatile uint8  Emergency_Flag     = 0;
volatile uint32_t capture1         = 0;
volatile uint32_t capture2         = 0;
volatile uint8_t capture_sequence  = 0;
uint32_t objectCount               = 0;
int lcd_cleared                    = 0;

// ───── Interrupt Handlers ───────────────────────────────
void Emergency_Stop_Handler(void) {
    Emergency_Flag = 1;
}

void Reset_Button_Handler(void) {
    Emergency_Flag = 0;
    lcd_cleared = 0;
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_WriteString(" Speed: ");
}

// ───── Initialization Functions ─────────────────────────
void Init_Clocks(void) {
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);   // For IR sensor + TIM2_CH1
    Rcc_Enable(RCC_GPIOB);   // For LCD
    Rcc_Enable(RCC_GPIOC);   // For buttons
    Rcc_Enable(RCC_SYSCFG);  // For EXTI
    Rcc_Enable(RCC_TIM2);    // For TIM2 capture
}

void Init_LCD(void) {
    LCD_Init();
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_WriteString("Speed: ");
}

void Init_ObjectDetection(void) {
    ObjectDetection_Init(GPIO_A, IR_PIN);
}

void Init_ButtonsAndInterrupts(void) {
    GPIO_Init(GPIO_C, EMERGENCY_PIN, GPIO_INPUT, GPIO_PULL_UP);
    GPIO_Init(GPIO_C, RESET_PIN, GPIO_INPUT, GPIO_PULL_UP);

    Exti_Init(PortC, EMERGENCY_PIN, Falling_edge);
    Exti_Init(PortC, RESET_PIN, Falling_edge);

    Exti_SetCallback(EMERGENCY_PIN, Emergency_Stop_Handler);
    Exti_SetCallback(RESET_PIN, Reset_Button_Handler);

    Exti_Enable(EMERGENCY_PIN);
    Exti_Enable(RESET_PIN);
}

void Init_SpeedMeasurement(void) {
    TIM2_Capture_Init();  // Configure PA0 as TIM2_CH1
}

// ───── Main Program ─────────────────────────────────────
int main(void) {
    Init_Clocks();
    Init_LCD();
    Init_ObjectDetection();
    Init_ButtonsAndInterrupts();
    Init_SpeedMeasurement();

    while (1) {
        __disable_irq();
        if (Emergency_Flag) {
            if (!lcd_cleared) {
                LCD_Clear();
                LCD_SetCursor(0, 0);
                LCD_WriteString(" EMERGENCY STOP");
                lcd_cleared = 1;
            }
            __enable_irq();
            continue;
        }
        __enable_irq();
        lcd_cleared = 0;

        // ─── Object Detection ──────────────────────
        poll_for_object(&objectCount);

        LCD_SetCursor(1, 0);
        LCD_WriteString("Count: ");
        LCD_WriteInteger(objectCount);

        // ─── Speed Measurement ─────────────────────
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

                if (period_us > 0 && period_us < 1000000) {
                    float speed = DISTANCE_MM * (1000000.0f / period_us);
                    LCD_SetCursor(0, 7);
                    LCD_WriteString("     ");
                    LCD_SetCursor(0, 7);
                    LCD_WriteInteger((uint16_t)speed);
                    LCD_WriteString(" mm/s");
                }
            }

            TIM2_ClearCaptureFlag();
        }

        delay_ms(100);
    }

    return 0;
}