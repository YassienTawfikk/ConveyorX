#include "SevenSegment.h"
#include "SevenSegment_Private.h"
#include "Gpio.h"
#include "Rcc.h"

void SevenSegment_Init() {
    // Enable clock for all GPIO banks used by the display
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_GPIOD);  // Added for new port usage

    // Configure all segment connections as outputs
    for (uint8 seg_idx = 0; seg_idx < 7; seg_idx++) {
        Gpio_Init(
            segment_connections[seg_idx].gpio_port,
            segment_connections[seg_idx].gpio_pin,
            GPIO_OUTPUT,
            GPIO_PUSH_PULL
        );
        // Initialize all segments to off state
        Gpio_WritePin(
            segment_connections[seg_idx].gpio_port,
            segment_connections[seg_idx].gpio_pin,
            LOW
        );
    }
}

void SevenSegment_Display(uint8 digit) {
    // Handle invalid digit input
    if (digit > 9) {
        // Clear all segments
        for (uint8 seg_idx = 0; seg_idx < 7; seg_idx++) {
            Gpio_WritePin(
                segment_connections[seg_idx].gpio_port,
                segment_connections[seg_idx].gpio_pin,
                LOW
            );
        }
        return;
    }

    // Activate segments according to digit pattern
    for (uint8 seg_idx = 0; seg_idx < 7; seg_idx++) {
        Gpio_WritePin(
            segment_connections[seg_idx].gpio_port,
            segment_connections[seg_idx].gpio_pin,
            digitToSegment[digit][seg_idx]
        );
    }
}