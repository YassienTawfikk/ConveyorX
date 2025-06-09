#include "ObjectDetection.h"
#include "GPIO.h"
#include "TIM_Delay.h"

static uint8 ir_port;
static uint8 ir_pin;

#define OBJ_PIN od_pin  // for identical naming as your example

void ObjectDetection_Init(uint8 port, uint8 pin) {
    ir_port = port;
    ir_pin = pin;

    GPIO_Init(ir_port, ir_pin, GPIO_INPUT, GPIO_PULL_UP);
}

void check_objects_count(uint32 *counter) {
    static uint8 last_pin_state = HIGH;
    uint8 current_pin_state = GPIO_ReadPin(ir_port, ir_pin);

    if (last_pin_state == HIGH && current_pin_state == LOW) {
        (*counter)++;
        delay_ms(100);
    }
    last_pin_state = current_pin_state;
}
