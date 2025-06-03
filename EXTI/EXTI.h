#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

#include "Std_Types.h"

/* ============ EXTI REGISTERS STRUCTURE ============ */

typedef struct {
    uint32 IMR;
    uint32 EMR;
    uint32 RTSR;
    uint32 FTSR;
    uint32 SWIER;
    uint32 PR;
}EXTI_Type;

#define EXTI        ((EXTI_Type*)0x40013C00)

/* ============ NVIC REGISTERS STRUCTURE ============ */

typedef struct {
    uint32 NVIC_ISER[8];
    uint32 NVIC_ICER[8];
}NVIC_Type;


#define NVIC        ((NVIC_Type*)0xE000E100)


/* ============ SYSCFG REGISTERS STRUCTURE ============ */
typedef struct {
    uint32 MEMRMP;
    uint32 PMC;
    uint32 EXTICR[4];
}SYSCFG_Type;

#define SYSCFG      ((SYSCFG_Type*)0x40013800)


/* ============ EDGE TRIGGER TYPES ============ */
#define RisingEdge  0
#define FallingEdge 1
#define BothEdge    2

#define RiseLabel "Rise"
#define FallLabel "Fall"
#define BothLabel "Both"

#define True       1
#define False      0

#define __disable_irq()  __asm volatile ("cpsid i")
#define __enable_irq()   __asm volatile ("cpsie i")


extern uint8_t LedFlag;

void EXTI_Init(char port, uint8_t pin, char* edge_type);
void EXTI_enable(uint8_t pin);
void EXTI_disable(uint8_t pin);
uint8_t EXTI_GetIRQ(uint8_t pin);
uint8_t EXTI_GetPending(uint8_t pin);
void EXTI_ClearPending(uint8_t pin);

#endif