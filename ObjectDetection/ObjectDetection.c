/* ObjectDetection.c */
#include "ObjectDetection.h"
#include "Gpio.h"
#include "LCD.h"          // ONLY for delay_ms(); remove if you have a separate delay util

static uint8  irPort;
static uint8  irPin;
static uint16 objectCount = 0;

void ObjectDetection_Init(uint8 port, uint8 pin)
{
    irPort = port;
    irPin  = pin;
    Gpio_Init(irPort, irPin, GPIO_INPUT, GPIO_PULL_UP);
}

/* 
 * Very-simple algorithm:
 *   1.  Wait until line goes LOW  ➜ object present / button pressed
 *   2.  Debounce (20 ms)
 *   3.  Wait until line returns HIGH ➜ object left / button released
 *   4.  Count++ and return 1
 *
 * It’s blocking for the time the object is in front of the sensor,
 * but because it is called once per main-loop iteration it works fine
 * in small projects like this.
 */
uint8 ObjectDetection_Task(void)
{
    /* Step-1: object? (active-LOW) */
    if (Gpio_ReadPin(irPort, irPin) == LOW)
    {
        /* Step-2: debounce */
        delay_ms(20);
        if (Gpio_ReadPin(irPort, irPin) == LOW)
        {
            /* Step-3: wait until line released */
            while (Gpio_ReadPin(irPort, irPin) == LOW);

            /* Step-4: count it */
            ++objectCount;
            return 1;          // tell caller “new object detected”
        }
    }
    return 0;                  // nothing new
}

uint16_t ObjectDetection_GetCount(void)
{
    return objectCount;
}
