// ───── Core Drivers ─────────────────────────────────────
#include "Std_Types.h"
#include "GPIO.h"
#include "Rcc.h"
#include "EXTI.h"

// ───── Application Drivers ──────────────────────────────
#include "LCD.h"
#include "ObjectDetection.h"
#include "TIM2.h"
#include "TIM2_private.h"
#include "TIM_Delay.h"
#include "pwm.h"
#include "ADC.h"

// ───── Pin Definitions ──────────────────────────────────
#define IR_PIN             2   // GPIO_A
#define EMERGENCY_PIN      2   // GPIO_C
#define RESET_PIN          6   // GPIO_C
#define DISTANCE_MM     50.0f  // mm per rising edge

// ───── Global Variables ─────────────────────────────────
volatile uint8 Emergency_Flag = 0;
volatile uint32_t capture1 = 0;
volatile uint32_t capture2 = 0;
volatile uint8_t capture_sequence = 0;
uint32_t objectCount = 0;
int lcd_emergency_displayed = 0;
uint16 adc_value = 0;
uint16 last_speed = 0;

// ───── LCD Display Helpers ──────────────────────────────
void LCD_DisplaySpeed(uint16_t speed_mm_s) {
    LCD_SetCursor(0, 0);
    LCD_WriteString("Speed:     ");
    LCD_SetCursor(0, 7);
    LCD_WriteInteger(speed_mm_s);
    LCD_WriteString(" mm/s");
}

void LCD_DisplayCount(uint32_t count) {
    LCD_SetCursor(1, 0);
    LCD_WriteString("Cnt:    ");
    LCD_SetCursor(1, 4);
    LCD_WriteInteger(count);
}

void LCD_DisplayMotorSpeed(uint16 value) {
    uint16 duty = (uint16) (((double) value / 1023) * 100);

    LCD_SetCursor(1, 8);
    LCD_WriteString("PWM:");
    LCD_WriteString("         ");
    LCD_SetCursor(1, 12);
    LCD_WriteInteger(duty);
    LCD_WriteString("%");
}

void LCD_DisplayEmergency(void) {
    LCD_Clear();
    delay_ms(5);
    LCD_SetCursor(0, 0);
    LCD_WriteString("   EMERGENCY ");
    LCD_SetCursor(1, 0);
    LCD_WriteString("     STOP    ");
}

// ───── Interrupt Handlers ───────────────────────────────
void Emergency_Stop_Handler(void) {
    Emergency_Flag = 1;
}

void Reset_Button_Handler(void) {
    Emergency_Flag = 0;
    lcd_emergency_displayed = 0;
    LCD_Clear();
    delay_ms(5);
    LCD_DisplaySpeed(0);
    LCD_DisplayCount(objectCount);
    LCD_DisplayMotorSpeed(adc_value);
    adc_value = ADC_Read(1);
    PWM_SetDutyCycle(adc_value);
    LCD_DisplayMotorSpeed(adc_value);

}

// ───── Initialization Functions ─────────────────────────
void Init_Clocks(void) {
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_SYSCFG);
    Rcc_Enable(RCC_TIM2);
}

void Init_LCD(void) {
    LCD_Init();
    LCD_Clear();
    delay_ms(5);
    LCD_DisplaySpeed(0);
    LCD_DisplayCount(0);
    LCD_DisplayMotorSpeed(0);
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
    TIM2_Capture_Init(); // PA0 (CH1) input capture
}

void Init_ADC(void) {
    ADC_Init();
    adc_value = ADC_Read(1);
}

void Init_PWM(void) {
    PWM_Init();
    PWM_SetDutyCycle(adc_value);
    LCD_DisplayMotorSpeed(adc_value);
}

// ───── Speed Calculation ────────────────────────────────
uint16_t Process_SpeedMeasurement(void) {
    // Reading the CC1IF (Capture/Compare 1 Interrupt Flag) bit in TIM2_SR Register which is bit 1 to see if capture is occurred or not
    if ((* TIM2_SR & (0x1 << 1)) != 0) {
        uint32_t current = TIM2_GetCaptureValue();

        if (capture_sequence == 0) {
            capture1 = current;
            capture_sequence = 1;
        } else {
            capture2 = current;
            capture_sequence = 0;

            uint32_t period_us = (capture2 >= capture1)
                                     ? (capture2 - capture1)
                                     : ((0xFFFFFFFFUL - capture1) + capture2 + 1);

            if (period_us > 0 && period_us < 1000000) {
                float speed = DISTANCE_MM * (1000000.0f / period_us);
                return (uint16_t) speed;
            }
        }
    }
    return last_speed; // return last value if not updated
}

// ───── Main Program ─────────────────────────────────────
int main(void) {
    Init_Clocks();
    Init_LCD();
    Init_ObjectDetection();
    Init_ButtonsAndInterrupts();
    Init_SpeedMeasurement();
    Init_ADC();
    Init_PWM();

    uint32_t last_count = 0;
    uint16_t last_pwm = 0;

    while (1) {
        __disable_irq();
        if (Emergency_Flag) {
            if (!lcd_emergency_displayed) {
                LCD_DisplayEmergency();
                lcd_emergency_displayed = 1;
                PWM_Stop();
            }
            __enable_irq();
            continue;
        }
        __enable_irq();
        lcd_emergency_displayed = 0;

        // ─── Object Detection ──────────────────────
        check_objects_count(&objectCount);
        if (objectCount != last_count) {
            LCD_DisplayCount(objectCount);
            last_count = objectCount;
        }

        // ─── Speed Measurement ─────────────────────
        uint16_t current_speed = Process_SpeedMeasurement();
        if (current_speed != last_speed) {
            LCD_DisplaySpeed(current_speed);
            last_speed = current_speed;
        }

        // ─── ADC + PWM Update ──────────────────────
        adc_value = ADC_Read(1);
        uint16_t current_pwm = (uint16_t)(((double) adc_value / 1023) * 100);
        if (current_pwm != last_pwm) {
            LCD_DisplayMotorSpeed(adc_value);
            PWM_SetDutyCycle(adc_value);
            last_pwm = current_pwm;
        }

        delay_ms(20); // short, smooth refresh
    }

    return 0;
}
