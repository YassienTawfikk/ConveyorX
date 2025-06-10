//
// Created by Compu House on 4/21/2025.
//

#include "Std_Types.h"
#include "Bit_Operations.h"
#include "Utils.h"
#include "GPIO.h"
#include "GPIO_Private.h"

void GPIO_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState) {
    switch (PortName) {
        case GPIO_A:
            GPIOA_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOA_MODER |= (PinMode << (PinNumber * 2));

            if (PinMode == GPIO_INPUT) {
                GPIOA_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOA_PUPDR |= (DefaultState << (PinNumber * 2));
            } else {
                GPIOA_OTYPER &= ~(0x1 << PinNumber);
                GPIOA_OTYPER |= (DefaultState << (PinNumber));
            }
            break;
        case GPIO_B:
            GPIOB_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOB_MODER |= (PinMode << (PinNumber * 2));

            if (PinMode == GPIO_INPUT) {
                GPIOB_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOB_PUPDR |= (DefaultState << (PinNumber * 2));
            } else {
                GPIOB_OTYPER &= ~(0x1 << PinNumber);
                GPIOB_OTYPER |= (DefaultState << (PinNumber));
            }
            break;
        case GPIO_C:
            GPIOC_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOC_MODER |= (PinMode << (PinNumber * 2));

            if (PinMode == GPIO_INPUT) {
                GPIOC_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOC_PUPDR |= (DefaultState << (PinNumber * 2));
            } else {
                GPIOC_OTYPER &= ~(0x1 << PinNumber);
                GPIOC_OTYPER |= (DefaultState << (PinNumber));
            }
            break;
        case GPIO_D:
            GPIOD_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOD_MODER |= (PinMode << (PinNumber * 2));

            if (PinMode == GPIO_INPUT) {
                GPIOD_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOD_PUPDR |= (DefaultState << (PinNumber * 2));
            } else {
                GPIOD_OTYPER &= ~(0x1 << PinNumber);
                GPIOD_OTYPER |= (DefaultState << (PinNumber));
            }
            break;
        default:
            break;
    }
}

uint8 GPIO_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data) {
    /*if (PinMode == GPIO_INPUT){
      return NOK;
    }*/
    switch (PortName) {
        case GPIO_A:
            GPIOA_ODR &= ~(0x1 << PinNumber);
            GPIOA_ODR |= (Data << PinNumber);
            break;
        case GPIO_B:
            GPIOB_ODR &= ~(0x1 << PinNumber);
            GPIOB_ODR |= (Data << PinNumber);
            break;
        case GPIO_C:
            GPIOC_ODR &= ~(0x1 << PinNumber);
            GPIOC_ODR |= (Data << PinNumber);
            break;
        case GPIO_D:
            GPIOD_ODR &= ~(0x1 << PinNumber);
            GPIOD_ODR |= (Data << PinNumber);
            break;
        default:
            break;
    }
    return OK;
}

uint8 GPIO_ReadPin(uint8 PortName, uint8 PinNumber) {
    uint8 pin_value = 0;
    switch (PortName) {
        case GPIO_A:
            pin_value = (GPIOA_IDR >> PinNumber) & 0x1;
            break;
        case GPIO_B:
            pin_value = (GPIOB_IDR >> PinNumber) & 0x1;
            break;
        case GPIO_C:
            pin_value = (GPIOC_IDR >> PinNumber) & 0x1;
            break;
        case GPIO_D:
            pin_value = (GPIOD_IDR >> PinNumber) & 0x1;
            break;
        default:
            break;
    }
    return pin_value;
}


void GPIO_SetAlternateFunction(uint8 PortName, uint8 PinNumber, uint8 AF_Number) {
    switch (PortName) {
        case GPIO_A:
            if (PinNumber <= 7) {
                GPIOA_AFRL &= ~(0xF << (PinNumber * 4));
                GPIOA_AFRL |= (AF_Number << (PinNumber * 4));
            } else {
                uint8 localPin = PinNumber - 8;
                GPIOA_AFRH &= ~(0xF << (localPin * 4));
                GPIOA_AFRH |= (AF_Number << (localPin * 4));
            }
            break;

        case GPIO_B:
            if (PinNumber <= 7) {
                GPIOB_AFRL &= ~(0xF << (PinNumber * 4));
                GPIOB_AFRL |= (AF_Number << (PinNumber * 4));
            } else {
                uint8 localPin = PinNumber - 8;
                GPIOB_AFRH &= ~(0xF << (localPin * 4));
                GPIOB_AFRH |= (AF_Number << (localPin * 4));
            }
            break;

        case GPIO_C:
            if (PinNumber <= 7) {
                GPIOC_AFRL &= ~(0xF << (PinNumber * 4));
                GPIOC_AFRL |= (AF_Number << (PinNumber * 4));
            } else {
                uint8 localPin = PinNumber - 8;
                GPIOC_AFRH &= ~(0xF << (localPin * 4));
                GPIOC_AFRH |= (AF_Number << (localPin * 4));
            }
            break;

        case GPIO_D:
            if (PinNumber <= 7) {
                GPIOD_AFRL &= ~(0xF << (PinNumber * 4));
                GPIOD_AFRL |= (AF_Number << (PinNumber * 4));
            } else {
                uint8 localPin = PinNumber - 8;
                GPIOD_AFRH &= ~(0xF << (localPin * 4));
                GPIOD_AFRH |= (AF_Number << (localPin * 4));
            }
            break;

        default:
            break;
    }
}
