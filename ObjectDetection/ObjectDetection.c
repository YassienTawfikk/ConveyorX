/*
 * ObjectDetection.c
 * Non-blocking edge detector with 10 ms debounce.
 */

#include "ObjectDetection.h"
#include "Gpio.h"
#include "LCD.h"          // for delay_ms();  use your own delay util if preferred

/* ---- static (module-local) data ---------------------------------------- */
static uint8   od_port;
static uint8   od_pin;
static uint8   prev_state = HIGH;      /* track previous pin state */
static uint16  obj_count  = 0;

/* ---- public API -------------------------------------------------------- */
void ObjectDetection_Init(uint8 port, uint8 pin)
{
    od_port = port;
    od_pin  = pin;

    Gpio_Init(od_port, od_pin, GPIO_INPUT, GPIO_PULL_UP);
    prev_state = Gpio_ReadPin(od_port, od_pin);   /* capture initial level */
}

uint8 ObjectDetection_Task(void)
{
    uint8 now = Gpio_ReadPin(od_port, od_pin);

    /* Detect falling edge: HIGH ➜ LOW */
    if (prev_state == HIGH && now == LOW)
    {
        delay_ms(10);                                 /* quick debounce      */
        if (Gpio_ReadPin(od_port, od_pin) == LOW)     /* still LOW? confirm  */
        {
            ++obj_count;
            prev_state = LOW;                         /* lock until released */
            return 1;                                 /* report one object   */
        }
    }
    /* Detect rising edge to re-arm detector */
    else if (prev_state == LOW && now == HIGH)
    {
        prev_state = HIGH;
    }

    return 0;                                         /* no new object       */
}

uint16_t ObjectDetection_GetCount(void)
{
    return obj_count;
}
