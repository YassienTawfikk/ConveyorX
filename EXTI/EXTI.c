#include "stm32f4xx.h"
#include "EXTI.h"
#include "Gpio.h"

static void (*Exti_Callbacks[16])(void) = {0}; // Holds function pointers for EXTI0 - EXTI15

void Exti_SetCallback(uint8 Line_NO, void (*callback)(void)) {
    if (Line_NO < 16) {
        Exti_Callbacks[Line_NO] = callback;
    }
}


void Exti_Init(uint8 Port_Name, uint8 Line_NO, uint8 Line_status) {
    //tell which port and line no we are going to read
    if (Line_NO >= 0 && Line_NO <= 3) {
        SYSCFG->EXTICR[0] &= ~(0x0F << ((Line_NO % 4) * 4));
        SYSCFG->EXTICR[0] |= (Port_Name << ((Line_NO % 4) * 4));
    };
    if (Line_NO >= 4 && Line_NO <= 7) {
        SYSCFG->EXTICR[1] &= ~(0x0F << ((Line_NO % 4) * 4));
        SYSCFG->EXTICR[1] |= (Port_Name << ((Line_NO % 4) * 4));
    };
    if (Line_NO >= 8 && Line_NO <= 11) {
        SYSCFG->EXTICR[2] &= ~(0x0F << ((Line_NO % 4) * 4));
        SYSCFG->EXTICR[2] |= (Port_Name << ((Line_NO % 4) * 4));
    };
    if (Line_NO >= 12 && Line_NO <= 15) {
        SYSCFG->EXTICR[3] &= ~(0x0F << ((Line_NO % 4) * 4));
        SYSCFG->EXTICR[3] |= (Port_Name << ((Line_NO % 4) * 4));
    };

    //telling if the line is falling edge or rising edge or both
    if (Line_status == Falling_edge) {
        EXTI->FTSR |= (0x1 << (Line_NO));
    };
    if (Line_status == Rising_edge) {
        EXTI->RTSR |= (0x1 << (Line_NO));
    };
    if (Line_status == Both_edge) {
        EXTI->FTSR |= (0x1 << (Line_NO));
        EXTI->RTSR |= (0x1 << (Line_NO));
    };
};

void Exti_Enable(uint8 Line_NO) {
    // enable form the side of EXTI
    EXTI->IMR |= (0x1 << (Line_NO));

    //enable from the side of NVIC
    if (Line_NO >= 0 && Line_NO <= 4) {
        NVIC->NVIC_ISER[0] |= (0x1 << (Line_NO + 6));
    };
    if (Line_NO >= 5 && Line_NO <= 9) {
        NVIC->NVIC_ISER[0] |= (0x1 << 23);
    };
    if (Line_NO >= 10 && Line_NO <= 15) {
        NVIC->NVIC_ISER[1] |= (0x1 << 8);
    };
};

void Exti_Disable(uint8 Line_NO) {
    // enable form the side of EXTI
    EXTI->IMR &= ~(0x1 << (Line_NO));

    //enable from the side of the NVIC
    if (Line_NO >= 0 && Line_NO <= 4) {
        NVIC->NVIC_ICER[0] |= (0x1 << (Line_NO + 6));
    };
    if (Line_NO >= 5 && Line_NO <= 9) {
        NVIC->NVIC_ICER[0] |= (0x1 << 23);
    };
    if (Line_NO >= 10 && Line_NO <= 15) {
        NVIC->NVIC_ICER[1] |= (0x1 << 8);
    };
};


// Helper for shared IRQs
static void EXTI_GenericHandler(uint8 line_no) {
    if (EXTI->PR & (1 << line_no)) {
        EXTI->PR |= (1 << line_no); // Clear pending
        if (Exti_Callbacks[line_no]) {
            Exti_Callbacks[line_no](); // Call user function
        }
    }
};

// Individual ISRs
void EXTI0_IRQHandler(void) { EXTI_GenericHandler(0); }
void EXTI1_IRQHandler(void) { EXTI_GenericHandler(1); }
void EXTI2_IRQHandler(void) { EXTI_GenericHandler(2); }
void EXTI3_IRQHandler(void) { EXTI_GenericHandler(3); }
void EXTI4_IRQHandler(void) { EXTI_GenericHandler(4); }

void EXTI9_5_IRQHandler(void) {
    for (uint8 i = 5; i <= 9; i++) {
        EXTI_GenericHandler(i);
    }
};

void EXTI15_10_IRQHandler(void) {
    for (uint8 i = 10; i <= 15; i++) {
        EXTI_GenericHandler(i);
    }
}
