#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H

#include "Std_Types.h"

void ObjectDetection_Init(uint8 port, uint8 pin); // Needed to set pin

void check_objects_count(uint32 *counter); // Detects falling edge and increments *counter

#endif
