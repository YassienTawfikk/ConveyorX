#include "ObjectDetection.h"
#include "GPIO.h"
#include "TIM_Delay.h"

static uint8 od_port;
static uint8 od_pin;

#define OBJ_PIN od_pin  // for identical naming as your example

void ObjectDetection_Init(uint8 port, uint8 pin) {
    od_port = port;
    od_pin = pin;

    GPIO_Init(od_port, od_pin, GPIO_INPUT, GPIO_PULL_UP);
}


void poll_for_object(uint32 *counter) {
    static uint8 last_pin_state = HIGH;
    static uint32 last_detection_time = 0;

    uint8 current_pin_state = GPIO_ReadPin(od_port, od_pin);
    uint32 now = millis(); // Current time in ms

    if (last_pin_state == HIGH && current_pin_state == LOW) {
        if (now - last_detection_time >= 150) {
            // 150ms debounce
            (*counter)++;
            last_detection_time = now;
        }
    }

    last_pin_state = current_pin_state;
}
