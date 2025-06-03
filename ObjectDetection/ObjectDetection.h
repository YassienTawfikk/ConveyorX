//
// Created by Yassien on 6/3/2025.
//

#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H

#include <stdint.h>

#include "Std_Types.h"
#include "Gpio.h"

void ObjectDetection_Init(uint8 port, uint8 pin);
uint8 ObjectDetection_Update(void); // Return 1 at detected falling edge
uint16_t ObjectDetection_Get_Count(void);


#endif //OBJECTDETECTION_H
