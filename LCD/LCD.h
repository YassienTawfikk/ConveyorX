#ifndef LCD_H
#define LCD_H

#include "Std_Types.h"
#include "Gpio.h"

#define LCD_ROWS          2
#define LCD_COLS          16
#define LCD_DATA_MODE     4  // or 8

// GPIO Port and Pin definitions to match your GPIO driver
#define LCD_CONTROL_PORT        GPIO_B
#define LCD_RS_PIN              0
#define LCD_EN_PIN              1
#define LCD_DATA_PORT           GPIO_B
#define LCD_D4_PIN              4
#define LCD_D5_PIN              5
#define LCD_D6_PIN              6
#define LCD_D7_PIN              7

// GPIO pin states
#define GPIO_HIGH               1
#define GPIO_LOW                0

// LCD Commands
#define LCD_CLEAR_DISPLAY         0x01
#define LCD_RETURN_HOME           0x02

/* Character entry mode instructions */
#define LCD_INC_SHIFT_OFF         0x06    //default
#define LCD_DEC_SHIFT_OFF         0x04
#define LCD_INC_SHIFT_ON          0x07
#define LCD_DEC_SHIFT_ON          0x05   //default

/*Display ON/OFF Control instructions */
#define LCD_DISPLAY_ON                    0x0C
#define LCD_DISPLAY_ON_CURSOR_ON          0x0E
#define LCD_DISPLAY_ON_BLINK_ON           0x0D
#define LCD_DISPLAY_ON_CURSOR_ON_BLINK_ON 0x0F
#define LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF 0x08

// Cursor and Display shift instructions
#define LCD_DISPLAY_SHIFT_RIGHT    0x1C
#define LCD_DISPLAY_SHIFT_LEFT     0x18
#define LCD_CURSOR_SHIFT_RIGHT     0x14
#define LCD_CURSOR_SHIFT_LEFT      0x10

// Function Set Instructions

//FOR 8-Bit Data:
#define LCD_8BIT_5X8_2LINE        0x38
#define LCD_8BIT_5x8_1LINE        0x30
#define LCD_8BIT_5X10_2LINE       0x3C
#define LCD_8BIT_5x10_1LINE       0x34

//FOR 4-Bit Data:
#define LCD_4BIT_5X8_2LINE        0x28
#define LCD_4BIT_5x8_1LINE        0x20
#define LCD_4BIT_5X10_2LINE       0x2C
#define LCD_4BIT_5x10_1LINE       0x24

// Row and Col initial Address
#define LCD_ROW0COL0_ADDR         0x80
#define LCD_ROW1COL0_ADDR         0xC0

// LCD Macros Flags
#define DATA    1
#define CMD     0
#define RIGHT   1
#define LEFT    0

// Function declarations
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8 row, uint8 col);
void LCD_Display_Scroll(uint8 direction);
void LCD_Cursor_Shift(uint8 direction);  // Fixed function name (removed extra underscore)
void LCD_WriteChar(uint8 character);
void LCD_WriteString(const char* string);
void LCD_WriteInteger(uint16 intvalue);
void delay_ms(uint32 ms);
void delay_us(uint32 us);

#endif //LCD_H