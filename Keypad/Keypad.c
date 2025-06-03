/******************************************************************************
*
* Module: Keypad
*
* File Name: Keypad.C
*
* Description: Source file for the keypad driver
*
* Author: Malak Emad
*
*******************************************************************************/

/* Included drivers */
#include "Keypad.h"
#include "Rcc.h"
#include "Gpio.h"
#include "Std_Types.h"

/* Used variables */
uint8 i, j;
uint8 keyIndex;
uint8 pressedKey;
uint8 lastKeyStatus = NOT_PRESSED_KEY;


/* This implementation of the lookup table is special for this keypad
 * due to the connections of the columns to the port */
uint8 keypadTable[12] = {
    1, 2, 3,
    4, 5, 6,
    7, 8, 9,
    0, 0, 0
};

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* array of type struct to access the rows */
PinConfig rows[] = {
    {Keypad_R0},
    {Keypad_R1},
    {Keypad_R2},
    {Keypad_R3}
};

/* array of type struct to access the columns */
PinConfig cols[] = {
    {Keypad_C0},
    {Keypad_C1},
    {Keypad_C2}
};

void Keypad_Init(void) {
    /* Enabling Ports Used */
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);

    /* Initializing Column pins port as Input */

    for (i = 0; i< KEYPAD_NUM_COLS; i++) {
        Gpio_Init(cols[i].keypadPort, cols[i].keypadPin, GPIO_INPUT, GPIO_PULL_UP);
    }

    /*
     * Initializing Rows pins port as Output
     * and setting the initial value as HIGH
     */

    for (i = 0; i< KEYPAD_NUM_ROWS; i++) {
        Gpio_Init(rows[i].keypadPort, rows[i].keypadPin, GPIO_OUTPUT, GPIO_PUSH_PULL);
        Gpio_WritePin(rows[i].keypadPort, rows[i].keypadPin, HIGH);

    }

}

void Keypad_Manage(void) {
    /*Flag to check if any key is pressed */
    uint8 keyFound = 0;
    /* Loops on rows one by one to make it low so we can detect the falling edge on the rows */
    for (i=0; i<KEYPAD_NUM_ROWS; i++) {
        Gpio_WritePin(rows[i].keypadPort, rows[i].keypadPin, LOW);

        /* Looping on each column to detect the pressed key */
        for (j=0; j<KEYPAD_NUM_COLS; j++) {
             if (Gpio_ReadPin(cols[j].keypadPort, cols[j].keypadPin) == LOW)   {
                /* Key is pressed */
                keyFound = 1;
                /* Checks if no key was pressed */
                if (lastKeyStatus == NOT_PRESSED_KEY) {
                    /* Valid key press */
                    lastKeyStatus = PRESSED_KEY;               // Assign that the key is pressed
                    keyIndex = i * KEYPAD_NUM_COLS + j;        // Equation to find the value pressed
                    pressedKey = keypadTable[keyIndex];        // Getting the value from the lookup table
                    KeypadCallout_KeyPressNotification();      // Calling the callout function inside main
                    break;
                }
            }
        }
        /* reset the last High pin */
        Gpio_WritePin(rows[i].keypadPort, rows[i].keypadPin, HIGH);

        if (keyFound == 1) {
            break;
        }
    }

    /* if no key is pressed then we set the status back to NOT_PRESSED_KEY */
    if (!keyFound && (lastKeyStatus == PRESSED_KEY)) {
        /* The key is released to allow the next key press to be detected */
        lastKeyStatus = NOT_PRESSED_KEY;
    }
}

uint8 Keypad_GetKey(void) {
    return pressedKey;
}

