//
// Created by Malak on 6/5/2025.
//

#ifndef PWM_H
#define PWM_H

#include "GPIO.h"
#include "Std_Types.h"


/*******************************************************************
 *                      Function prototype                         *
 *******************************************************************/

void PWM_Init(void);
void PWM_SetDutyCycle(uint16 duty);
void PWM_Stop(void);

#endif //PWM_H
