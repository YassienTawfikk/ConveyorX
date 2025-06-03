#include "RCC.h"
#include "Gpio.h"
#include "EXTI.h"
#include "SevenSegment.h"

#define led_pin         3
#define button_pin      12
#define increment_pin   4
#define decrement_pin   10

uint8 toggle = 0;
static uint32 i;
uint8 firstDigits = 10;
uint8 secondDigit = 1;

int main() {
    /* Clock Initialization */
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_SYSCFG);

    /* GPIO Initialization */
    Gpio_Init(GPIO_A, led_pin, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_B, button_pin, GPIO_INPUT, GPIO_PULL_DOWN);
    Gpio_Init(GPIO_A, increment_pin, GPIO_INPUT, GPIO_PULL_UP);
    Gpio_Init(GPIO_B, decrement_pin, GPIO_INPUT, GPIO_PULL_UP);


    Gpio_Init(GPIO_B, 7, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_B, 8, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_B, 9, GPIO_OUTPUT, GPIO_PUSH_PULL);

    SevenSegment_Init();


    /* EXTI Initialization */
    EXTI_Init(GPIO_B, button_pin, BothLabel);
    EXTI_enable(button_pin);

    EXTI_Init(GPIO_A, increment_pin, FallLabel);
    EXTI_enable(increment_pin);

    EXTI_Init(GPIO_B, decrement_pin, RiseLabel);
    EXTI_enable(decrement_pin);


    /* Main Loop */
    while (1) {

        uint8 local_firstDigits;
        uint8 local_secondDigit;

        __disable_irq();  // Disable interrupts (critical section)
        local_firstDigits = firstDigits;
        local_secondDigit = secondDigit;
        __enable_irq();   // Re-enable interrupts

        Gpio_WritePin(GPIO_B, 7, LOW);
        SevenSegment_Display(local_secondDigit);
        for (i = 0; i < (1000000 / 1000) * 15; i++) {}
        Gpio_WritePin(GPIO_B, 7, HIGH);


        Gpio_WritePin(GPIO_B, 8, LOW);
        SevenSegment_Display(local_firstDigits / 10);
        for (i = 0; i < (1000000 / 1000) * 15; i++) {}
        Gpio_WritePin(GPIO_B, 8, HIGH);


        Gpio_WritePin(GPIO_B, 9, LOW);
        SevenSegment_Display(local_firstDigits % 10);
        for (i = 0; i < (1000000 / 1000) * 15; i++) {}
        Gpio_WritePin(GPIO_B, 9, HIGH);
    }

    return 0;
}


void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 12)) {
        toggle = !toggle;
        Gpio_WritePin(GPIO_A, led_pin, toggle);
    }
    else {
        if (firstDigits > 0) {
            firstDigits--;
        }
        else if (secondDigit > 0) {
            secondDigit--;
            firstDigits = 99;
        }
    }
}

void EXTI4_IRQHandler(){
    if (firstDigits < 99) {
        firstDigits++;
    }
    else if (secondDigit < 10) {
        firstDigits = 0;
        secondDigit++;
    }
}