//
// Created by Malak on 6/5/2025.
//

#include "ADC.h"

#include <LCD.h>
#include <Rcc.h>

#include "ADC_private.h"


/*********************************************************************************
 *                            Function Definition                                *
 *********************************************************************************/

void ADC_Init(void) {
    Rcc_Enable(RCC_ADC1);
    Rcc_Enable(RCC_GPIOA);

    GPIO_Init(GPIO_A, 1, GPIO_ANALOG, GPIO_NO_PULL_DOWN);

    /* Enable ADC */
    ADC1->ADC_CR2 &= ~(0x01 << 0);
    ADC1->ADC_CR2 |= (0x01 << 0);

    /* wait a short delay */
    for (volatile int i = 0; i < 1000; i++);

    /* using 10-bit resolution */
    ADC1->ADC_CR1 &= ~(0x03 << 24);
    ADC1->ADC_CR1 |= (0x01 << 24);

    /* select the sampling time = 480 cycles */
    ADC1->ADC_SMPR2 &= ~(0x07 << (3*1));
    ADC1->ADC_SMPR2 |= (0x07 << (3*1));
}

uint16 ADC_Read(uint8 channel) {
    /* set the channel to read from */
    ADC1->ADC_SQR3 = channel;

    /* start conversion */
    ADC1->ADC_CR2 &= ~(0x01 << 30);
    ADC1->ADC_CR2 |= (0x01 << 30);

    /* wait untill end of conversion */
    while (!(ADC1->ADC_SR & (1<<1)));
    uint16 value = ADC1->ADC_DR;

    /* clear EOC flag */
    ADC1->ADC_SR &= ~(1<<1);
    return value;
}


