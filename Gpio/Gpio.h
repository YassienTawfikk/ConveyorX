//
// Created by Malak on 4/19/2025.
//

#ifndef GPIO_H
#define GPIO_H

#include "Std_Types.h"
#include "Bit_Operations.h"

#define NOK             0
#define OK              1

/* Port Name */
#define GPIO_A          'A'
#define GPIO_B          'B'
#define GPIO_C          'C'
#define GPIO_D          'D'
#define GPIO_E          'E'

/* Pin Mode */
#define GPIO_INPUT      0X00
#define GPIO_OUTPUT     0X01
#define GPIO_AF         0X02
#define GPIO_ANALOG     0X03

/* Default State */
#define GPIO_PUSH_PULL  0X00    // For output mode
#define GPIO_OPEN_DRAIN 0X01

#define GPIO_NO_PULL_UP 0X00    // For input mode
#define GPIO_PULL_UP    0X01
#define GPIO_PULL_DOWN  0X02

/* Data */
#define LOW             0
#define HIGH            1

void Gpio_Init(uint8 portName, uint8 pinNumber, uint8 pinMode, uint8 defaultState);
uint8 Gpio_WritePin(uint8 portName, uint8 pinNumber, uint8 data);
uint8 Gpio_ReadPin(uint8 portName, uint8 pinNumber);

#endif //GPIO_H
