/******************************************************************************
*
* Module: Keypad
*
* File Name: Keypad.h
*
* Description: Header file for the keypad driver
*
* Author: Malak Emad
*
*******************************************************************************/

#ifndef KEYPAD_H
#define KEYPAD_H

#include "Std_Types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Columns initialization */
#define KEYPAD_NUM_COLS             3
#define Keypad_C0           GPIO_A, 0
#define Keypad_C1           GPIO_A, 10
#define Keypad_C2           GPIO_B, 9

/* Rows initialization */
#define KEYPAD_NUM_ROWS             4
#define Keypad_R0           GPIO_A, 9
#define Keypad_R1           GPIO_B, 6
#define Keypad_R2           GPIO_B, 0
#define Keypad_R3           GPIO_C, 7

/* values used */
#define NOT_PRESSED_KEY            0
#define PRESSED_KEY                1

/* Struct to access the port and pin */
typedef struct {
    char keypadPort;
    uint8 keypadPin;
} PinConfig;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the Keypad:
 * Function shall not take any arguments. It will be used to initialize the
 * keypad driver variable(s).
 */
void Keypad_Init(void);

/*
 * Description:
 * Manage the calling of Keypad:
 * 1- Function shall not take any arguments.
 * 2- Function shall be called periodically from the infinite loop in main function.
 * 3- Function shall scan all keys to check which one is pressed.
 * 4- Once a valid key press is detected, function shall do the following:
          • Store the pressed key value.
          • Call a function “KeypadCallout_KeyPressNotification” and shall not pass any arguments to it.
 *  "Only a transition from no key pressed to one key press shall be considered as a valid press."
 * 5- After the key press, the value of the stored key shall not be changed until the key is released.
 * 6- The value of the stored key shall not be changed until key is released and Keypad_GetKey function is called.
 * 7- Function shall use lookup table implementation to define the value of the pressed key
 */
void Keypad_Manage(void);

/*
 * Description:
 * Function shall be called by application to get the last stored key.
 */
uint8 Keypad_GetKey(void);

/*
 * Description:
 * This is a callout function, and it is implemented by the user.
 */
void KeypadCallout_KeyPressNotification(void);

#endif //KEYPAD_H
