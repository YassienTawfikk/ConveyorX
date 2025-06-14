#include "Gpio.h"
#include "Std_Types.h"
#include "LCD.h"
#include "TIM_Delay.h"

static void LCD_Write(uint8 data, uint8 rs);

static void LCD_En_Pulse(void);

void LCD_Init(void) {
    // Initialize Control pins (RS and EN) as outputs
    GPIO_Init(LCD_CONTROL_PORT, LCD_RS_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    GPIO_Init(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);

    // Initialize Data pins (D4-D7) as outputs
    GPIO_Init(LCD_DATA_PORT, LCD_D4_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    GPIO_Init(LCD_DATA_PORT, LCD_D5_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    GPIO_Init(LCD_DATA_PORT, LCD_D6_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    GPIO_Init(LCD_DATA_PORT, LCD_D7_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);

    // Set initial state - all pins are initially low
    GPIO_WritePin(LCD_CONTROL_PORT, LCD_RS_PIN, GPIO_LOW);
    GPIO_WritePin(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_LOW);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, GPIO_LOW);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, GPIO_LOW);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, GPIO_LOW);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, GPIO_LOW);

    //LCD INITIALIZATION SEQUENCE:
    delay_ms(40); // Wait for 40 ms LCD initialization after LCD power ON
    LCD_Write(0x03, CMD); // Send 0x03
    delay_ms(5); // Wait for 5 ms
    LCD_Write(0x03, CMD); // again Send 0x03
    delay_us(150); // wait for 150 us
    LCD_Write(0x03, CMD); // again Send 0x03

    LCD_Write(0x02, CMD); // Send 0x02 (4-bit mode)

    LCD_Write(LCD_4BIT_5X8_2LINE, CMD); // Configure LCD in 4-Bit Mode (0x28)
    LCD_Write(LCD_DISPLAY_ON_CURSOR_ON, CMD); // Display ON, Cursor ON
    LCD_Write(LCD_CLEAR_DISPLAY, CMD); // Clear the display
    delay_ms(2); // After clear cmd wait for 2 ms

    LCD_Write(LCD_INC_SHIFT_OFF, CMD); // Increment the Cursor
    LCD_Write(LCD_ROW0COL0_ADDR, CMD); // Initial Cursor Position 0th position @ Line 1
}

void LCD_Display_Scroll(uint8 direction) {
    if (direction == 1) {
        LCD_Write(LCD_DISPLAY_SHIFT_RIGHT, CMD);
    } else {
        LCD_Write(LCD_DISPLAY_SHIFT_LEFT, CMD);
    }
}

void LCD_Cursor_Shift(uint8 direction) {
    if (direction == 1) {
        LCD_Write(LCD_CURSOR_SHIFT_RIGHT, CMD);
    } else {
        LCD_Write(LCD_CURSOR_SHIFT_LEFT, CMD);
    }
}

void LCD_Clear(void) {
    LCD_Write(LCD_CLEAR_DISPLAY, CMD);
    delay_ms(2);
    LCD_Write(LCD_ROW0COL0_ADDR, CMD); // Reset cursor to (0,0)
}

void LCD_SetCursor(uint8 row, uint8 col) {
    if (LCD_ROWS == 1) {
        LCD_Write(LCD_ROW0COL0_ADDR + col, CMD);
    } else if (LCD_ROWS == 2) {
        if (row == 0) {
            col = col + LCD_ROW0COL0_ADDR;
        } else {
            col = col + LCD_ROW1COL0_ADDR;
        }
        LCD_Write(col, CMD);
    }
}

// static void LCD_Write(uint8 data, uint8 rs)
// {
//     // Clear all Data lines of LCD
//     GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, GPIO_LOW);
//     GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, GPIO_LOW);
//     GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, GPIO_LOW);
//     GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, GPIO_LOW);
//
//     // Send Higher Nibble of Data First
//     if(data & 0x80)
//         GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, GPIO_HIGH);
//     if(data & 0x40)
//         GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, GPIO_HIGH);
//     if(data & 0x20)
//         GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, GPIO_HIGH);
//     if(data & 0x10)
//         GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, GPIO_HIGH);
//
//     // Write to LCD RS Pin for Data/Command
//     if(rs == DATA)
//     {
//         GPIO_WritePin(LCD_CONTROL_PORT, LCD_RS_PIN, GPIO_HIGH);
//     }
//     else
//     {
//         GPIO_WritePin(LCD_CONTROL_PORT, LCD_RS_PIN, GPIO_LOW);
//     }
//
//     LCD_En_Pulse();
//
//     // Clear all Data lines of LCD
//     GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, GPIO_LOW);
//     GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, GPIO_LOW);
//     GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, GPIO_LOW);
//     GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, GPIO_LOW);
//
//     // Send Lower Nibble of Data
//     if(data & 0x08)
//         GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, GPIO_HIGH);
//     if(data & 0x04)
//         GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, GPIO_HIGH);
//     if(data & 0x02)
//         GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, GPIO_HIGH);
//     if(data & 0x01)
//         GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, GPIO_HIGH);
//
//     LCD_En_Pulse();
// }

static void LCD_Write(uint8 data, uint8 rs) {
    // --- Send HIGH nibble ---
    GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, (data >> 4) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, (data >> 5) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, (data >> 6) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, (data >> 7) & 0x01);

    GPIO_WritePin(LCD_CONTROL_PORT, LCD_RS_PIN, rs ? GPIO_HIGH : GPIO_LOW);
    LCD_En_Pulse();

    // --- Send LOW nibble ---
    GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, data & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, (data >> 1) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, (data >> 2) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, (data >> 3) & 0x01);

    LCD_En_Pulse();
}

static void LCD_En_Pulse(void) {
    GPIO_WritePin(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_HIGH);
    delay_us(1);

    GPIO_WritePin(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_LOW);
    delay_us(2);
}

void LCD_WriteChar(uint8 character) {
    LCD_Write(character, DATA);
}

void LCD_WriteString(const char *string) {
    while (*string) {
        LCD_WriteChar(*string++);
        delay_ms(1);
    }
}

void LCD_WriteInteger(uint16 intvalue) {
    uint8 i = 0;

    if (intvalue > 999 && intvalue <= 9999) {
        i = intvalue / 1000; // divide 4-digit value by 1000 gives thousand's place integer
        LCD_WriteChar(i + 48);

        i = (intvalue / 100) % 10; // Hundred's place Integer
        LCD_WriteChar(i + 48);

        i = (intvalue % 100) / 10; // Ten's place Integer
        LCD_WriteChar(i + 48);

        i = (intvalue % 100) % 10; // One's place Integer
        LCD_WriteChar(i + 48);
    } else if (intvalue > 99 && intvalue <= 999) {
        i = intvalue / 100; // Hundred's place Integer
        LCD_WriteChar(i + 48);

        i = (intvalue % 100) / 10; // Ten's place Integer
        LCD_WriteChar(i + 48);

        i = (intvalue % 100) % 10; // One's place Integer
        LCD_WriteChar(i + 48);
    } else if (intvalue > 9 && intvalue <= 99) {
        i = intvalue / 10; // Ten's place Integer
        LCD_WriteChar(i + 48);

        i = intvalue % 10; // One's place Integer
        LCD_WriteChar(i + 48);
    } else {
        LCD_WriteChar(intvalue + 48);
    }
}
