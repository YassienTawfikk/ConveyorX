//
// Created by Malak on 6/5/2025.
//

#ifndef ADC_H
#define ADC_H

#include "Std_Types.h"
#include "GPIO.h"

/*********************************************************************************
 *                            Function Prototype                                 *
 *********************************************************************************/

void ADC_Init(void);
uint16 ADC_Read(uint8 channel);

#endif //ADC_H

