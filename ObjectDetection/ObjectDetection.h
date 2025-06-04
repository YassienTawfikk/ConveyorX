/*
 * ObjectDetection.h
 * Simple falling-edge object counter (active-LOW sensor)
 * Works with push-button or IR module that pulls the line LOW when triggered.
 */

#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H

#include "Std_Types.h"

/* Initialise the sensor pin.
 *  - port : GPIO_A / GPIO_B / …  (as defined in your Gpio driver)
 *  - pin  : pin number (0-15)
 */
void     ObjectDetection_Init(uint8 port, uint8 pin);

/* Call once every main-loop iteration.
 * Returns 1 exactly once per detected object, otherwise 0.
 */
uint8    ObjectDetection_Task(void);

/* Read total number of detected objects. */
uint16_t ObjectDetection_GetCount(void);

#endif /* OBJECTDETECTION_H */
