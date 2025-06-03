#include "EXTI.h"
#include "Gpio.h"
#include <string.h>

uint8_t LedFlag = 0;

void EXTI_Init(char port, uint8_t pin, char* edge_type) {
    if (pin > 15) return;

    uint8_t exti_register = pin / 4;
    uint8_t bit_position = (pin % 4) * 4;

    uint8_t port_code = 0;
    if (port == GPIO_A) port_code = 0;
    else if (port == GPIO_B) port_code = 1;
    else if (port == GPIO_C) port_code = 2;
    else if (port == GPIO_D) port_code = 3;
    else if (port == GPIO_E) port_code = 4;
    else return;  // Invalid port

    SYSCFG->EXTICR[exti_register] &= ~(0x0F << bit_position); // clear previous
    SYSCFG->EXTICR[exti_register] |= (port_code << bit_position); // set new port

    // Clear previous edge settings
    EXTI->RTSR &= ~(1 << pin);
    EXTI->FTSR &= ~(1 << pin);

    // Set new edge type
    if (strcmp(edge_type, RiseLabel) == 0) {
        EXTI->RTSR |= (1 << pin);
    } else if (strcmp(edge_type, FallLabel) == 0) {
        EXTI->FTSR |= (1 << pin);
    } else if (strcmp(edge_type, BothLabel) == 0) {
        EXTI->RTSR |= (1 << pin);
        EXTI->FTSR |= (1 << pin);
    }
}

uint8_t EXTI_GetIRQ(uint8_t pin) {
    if (pin <= 4){
        return 6 + pin;    // EXTI0–EXTI4 → IRQ6–10
        }
    else if (pin <= 9){
        return 23;         // EXTI5–9     → IRQ23
        }
    else if (pin <= 15){
      return 40;         // EXTI10–15   → IRQ40
      }
    else{
        return 255;        // Invalid EXTI pin
        }
}

void EXTI_enable(uint8_t pin) {
    if (pin > 15) return;

    EXTI->IMR |= (1 << pin);

    uint8_t irq = EXTI_GetIRQ(pin);
    NVIC->NVIC_ISER[irq / 32] |= (1 << (irq % 32));
}

void EXTI_disable(uint8_t pin) {
    if (pin > 15) return;

    EXTI->IMR &= ~(1 << pin);                            // Mask EXTI line

    uint8_t irq = EXTI_GetIRQ(pin);                    // Get IRQ number
    NVIC->NVIC_ICER[irq / 32] |= (1 << (irq % 32));      // Disable IRQ in NVIC
}

uint8_t EXTI_GetPending(uint8_t pin) {
    return (EXTI->PR & (1 << pin))? 1 : 0;
}

void EXTI_ClearPending(uint8_t pin) {
    EXTI->PR |= (1 << pin);
}

