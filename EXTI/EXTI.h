

#ifndef EXTI_H
#define EXTI_H

#endif //EXTI_H

#include "Std_Types.h"
#include "stm32f4xx.h"
#include "Gpio.h"

#define PortA 0x0
#define PortB 0x1
#define PortC 0x2
#define PortD 0x3
#define PortE 0x4
#define PortH 0x7

typedef struct {
    uint32 IMR;
    uint32 EMR;
    uint32 RTSR;
    uint32 FTSR;
    uint32 SWIER;
    uint32 PR;
} ExtiType;

#define EXTI ((ExtiType*)0x40013C00)

typedef struct {
    uint32 NVIC_ISER[8];
    uint32 NVIC_ICER[8];
}NVICType;
#define NVIC ((NVICType*)0xE000E100)


#define Falling_edge 0
#define Rising_edge 1
#define Both_edge 2

// Callback registration
void Exti_SetCallback(uint8 Line_NO, void (*callback)(void));

void Exti_Init(uint8 Port_Name, uint8 Line_NO, uint8 Line_status);
void Exti_Enable(uint8 Line_NO);
void Exti_Disable(uint8 Line_NO);

