#include "ObjectDetection.h"
#include "Gpio.h"

static uint8 od_port;
static uint8 od_pin;

#define OBJ_PIN od_pin  // for identical naming as your example

void ObjectDetection_Init(uint8 port, uint8 pin)
{
    od_port = port;
    od_pin  = pin;

    Gpio_Init(od_port, od_pin, GPIO_INPUT, GPIO_PULL_UP);
}

void poll_for_object(uint32 *counter) {
    static uint8 last_pin_state = HIGH;
    uint8 current_pin_state = Gpio_ReadPin(od_port, od_pin);

    if (last_pin_state == HIGH && current_pin_state == LOW) {
        (*counter)++;
        for (int i = 0; i < 300000; i++);  // crude delay
    }

    last_pin_state = current_pin_state;
}
