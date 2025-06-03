#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H

#include <stdint.h>
#include "Std_Types.h"
#include "Gpio.h"

// #define SEGMENT_A  GPIO_B, 0
// #define SEGMENT_B  GPIO_B, 1
// #define SEGMENT_C  GPIO_B, 2
// #define SEGMENT_D  GPIO_B, 3
// #define SEGMENT_E  GPIO_B, 4
// #define SEGMENT_F  GPIO_B, 5
// #define SEGMENT_G  GPIO_B, 6
//
// uint8 sevenSegmentMap[7][2] = {
//     {SEGMENT_A},
//     {SEGMENT_B},
//     {SEGMENT_C},
//     {SEGMENT_D},
//     {SEGMENT_E},
//     {SEGMENT_F},
//     {SEGMENT_G}
// };

void SevenSegment_Init(void);
void SevenSegment_Display(uint8_t number);

#endif