//
// Created by Malak on 4/19/2025.
//
#include "Gpio.h"
#include "Gpio_Private.h"

uint32 addressMap[6] = {GPIOA_BASE_ADDR, GPIOB_BASE_ADDR, GPIOC_BASE_ADDR, GPIOD_BASE_ADDR, GPIOE_BASE_ADDR, GPIOH_BASE_ADDR};

void Gpio_Init(uint8 portName, uint8 pinNumber, uint8 pinMode, uint8 defaultState)
{
    uint8 addressIndex = portName - GPIO_A;

    volatile uint32* moderReg = GPIO_REG(addressMap[addressIndex], GPIOx_MODER);
    volatile uint32* pullUpDownReg = GPIO_REG(addressMap[addressIndex], GPIOx_PUPDR);
    volatile uint32* outputTypeReg = GPIO_REG(addressMap[addressIndex], GPIOx_PUPDR);

    *moderReg &= ~(0X03 << (pinNumber * 2));
    *moderReg |= (pinMode << (pinNumber * 2));
    if (pinMode == GPIO_INPUT)
    {
        *pullUpDownReg &= ~(0X03 << (pinNumber * 2));
        *pullUpDownReg |= (defaultState << (pinNumber * 2));
    }
    else
    {
        *outputTypeReg &= ~(0X01 << pinNumber);
        *outputTypeReg |= (defaultState << pinNumber);
    }
}

uint8 Gpio_WritePin(uint8 portName, uint8 pinNumber, uint8 data)
{
    uint8 addressIndex = portName - GPIO_A;

    volatile uint32* moderReg = GPIO_REG(addressMap[addressIndex], GPIOx_MODER);
    volatile uint32* outputDataReg = GPIO_REG(addressMap[addressIndex], GPIOx_ODR);
    if (((*moderReg >> (pinNumber * 2)) & 0x03) != GPIO_OUTPUT)
        return NOK;
    if (data) {
        *outputDataReg |= (0x01 << pinNumber);
    }
    else {
        *outputDataReg &= ~(0X01 << pinNumber);
    }
    return OK;
}

uint8 Gpio_ReadPin(uint8 portName, uint8 pinNumber)
{
    uint8 addressIndex = portName - GPIO_A;
    uint8 pinValue;

    volatile uint32* inputDataReg = GPIO_REG(addressMap[addressIndex], GPIOx_IDR);
    pinValue = *inputDataReg & (0x01 << pinNumber);

    return pinValue;
}