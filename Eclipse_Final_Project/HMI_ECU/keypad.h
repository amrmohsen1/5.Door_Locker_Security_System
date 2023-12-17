/**************************************
 * Module: KEYPAD
 * File name: keypad.h
 * Description: header file for the AVR keypad driver
 * Author: amr mohsen
 *************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"


/**************************************************
 *                   Definitions                  *
 *************************************************/
/* keypad configurations for number of rows and columns */
#define KEYPAD_NUM_COLS             4
#define KEYPAD_NUM_ROWS             4

/* keypad port configuration */
#define KEYPAD_ROW_PORT_ID             PORTA_ID
#define KEYPAD_FIRST_ROW_PIN_ID        PIN0_ID

#define KEYPAD_COL_PORT_ID             PORTA_ID
#define KEYPAD_FIRST_COL_PIN_ID        PIN4_ID

/* keypad button logic configurations */
#define KEYPAD_BUTTON_PRESSED       LOGIC_LOW       /* (PULL UP RESISTOR)*/
#define KEYPAD_BUTTON_RELEASED      LOGIC_HIGH

/**************************************************
 *             Functions Prototypes               *
 *************************************************/
uint8 KEYPAD_getPressedKey(void);

#endif /* KEYPAD_H_ */
