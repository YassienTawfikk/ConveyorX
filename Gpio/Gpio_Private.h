//
// Created by Malak on 4/19/2025.
//

#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include "Utils.h"

/*
 * Addresses definitions:
 * GPIOx_MODER : GPIO port mode register
 * GPIOx_OTYPER : GPIO port output type register
 * GPIOx_OSPEEDR : GPIO port output speed register
 * GPIOx_PUPDR : GPIO port pull-up/pull-down register
 * GPIOx_IDR : GPIO port input data register
 * GPIOx_ODR : GPIO port output data register
 * GPIOx_BSRR : GPIO port bit set/reset register
 * GPIOx_LCKR : GPIO port configuration lock register
 * GPIOx_AFRL : GPIO alternate function low register
 * GPIOx_AFRH : GPIO alternate function high register
 */

/* GPIO Base addresses */
#define GPIOA_BASE_ADDR    0x40020000
#define GPIOB_BASE_ADDR    0x40020400
#define GPIOC_BASE_ADDR    0x40020800
#define GPIOD_BASE_ADDR    0x40020C00
#define GPIOE_BASE_ADDR    0x40021000
#define GPIOH_BASE_ADDR    0x40021C00


#define GPIOx_MODER        0x00
#define GPIOx_OTYPER       0x04
#define GPIOx_OSPEEDR      0x08
#define GPIOx_PUPDR        0x0C
#define GPIOx_IDR          0x10
#define GPIOx_ODR          0x14
#define GPIOx_BSRR         0x18
#define GPIOx_LCKR         0x1C
#define GPIOx_AFRL         0x20
#define GPIOx_AFRH         0x24

#define GPIO_REG(PORT_ID, REG_ID)        ((volatile uint32 *)(PORT_ID + REG_ID))


#endif //GPIO_PRIVATE_H
