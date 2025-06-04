//
// Created by Yassien on 6/3/2025.
//

#include "ObjectDetection.h"
#include "Gpio.h"

static  uint8 sensorPort;
static  uint8 sensorPin;
static  uint8 previousStatus = HIGH;

static  uint16_t objectCount = 0;

void ObjectDetection_Init(uint8 port, uint8 pin) {
    sensorPort = port;
    sensorPin = pin;

    Gpio_Init(sensorPort, sensorPin, GPIO_INPUT, GPIO_PULL_UP);
    previousStatus = Gpio_ReadPin(sensorPort, sensorPin);
}

uint8 ObjectDetection_Update(void) {
    uint8 currentStatus = Gpio_ReadPin(sensorPort, sensorPin);

    if (previousStatus == HIGH && currentStatus == LOW) {
        delay_milli_second(20); // debounce to avoid multiple counts from bouncing
        if (Gpio_ReadPin(sensorPort, sensorPin) == LOW) { // confirm still LOW
            objectCount++;
            previousStatus = LOW;
            return 1;
        }
    }

    previousStatus = currentStatus;
    return 0;
}

uint16_t ObjectDetection_Get_Count(void){
    return objectCount;
}

void delay_milli_second(uint32 ms) {
    uint32 delay = ms * 1000;
    for (; delay > 0; delay--)
        __asm("nop");
}