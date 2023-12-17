/**************************************
 * Module: KEYPAD
 * File name:keypad.c
 * Description: source file for the AVR KEYPAD driver
 * Author: amr mohsen
 *************************************/

#include "keypad.h"
#include "gpio.h"
#include <util/delay.h>

/**************************************************
 *         Functions Prototypes(private)          *
 *************************************************/
#ifndef STANDARD_KEYPAD

#if(KEYPAD_NUM_COLS==3)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x3 keypad
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);
#elif (KEYPAD_NUM_COLS==4)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x3 keypad
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);
#endif
#endif /*STANDARD_KEYPAD */


/**************************************************
 *             Functions Definitions              *
 *************************************************/
uint8 KEYPAD_getPressedKey(void)
{
	uint8 row,col;

	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+1, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+2, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+3, PIN_INPUT);

	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID+1, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID+2, PIN_INPUT);
#if(KEYPAD_NUM_COLS==4)
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID+3, PIN_INPUT);
#endif

	while(1)
	{
		for(row=0; row<KEYPAD_NUM_ROWS; row++)
		{
			/*
			 * Each time setup the direction for all keypad port as input pins,
			 * except this row will be output pin
			*/
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+row, PIN_OUTPUT);

			/* Set/Clear the row output pin according to button connection*/
			GPIO_writePin(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID, KEYPAD_BUTTON_PRESSED);
			for(col=0; col<KEYPAD_NUM_COLS; col++)
			{
				if(GPIO_readPin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID+col)== KEYPAD_BUTTON_PRESSED)
				{
	                #if(KEYPAD_NUM_COLS==3)
	                    #ifdef STANDRAD_KEYPAD
							return (row*KEYPAD_NUM_COLS)+col+1;
						#else
							return KEYPAD_4x3_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1);
                        #endif
                    #elif(KEYPAD_NUM_COLS==4)
                        #ifdef STANDRAD_KEYPAD
		                    return (row*KEYPAD_NUM_COLS)+col+1;
                        #else
		                    return KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1);
	                    #endif
                    #endif
				}
			}

			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+row, PIN_INPUT);
			_delay_ms(5);
		}
	}
}

#ifndef STANDARD_KEYPAD

#if(KEYPAD_NUM_COLS==3)
/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x3 shape
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button=0;
	switch(button_number)
	{
	case 10:
		keypad_button='*';    //Ascii code of *
		break;
	case 11:
		keypad_button=0;
		break;
	case 12:
		keypad_button="#";     //ascii code of #
		break;
	}

	return keypad_button;
}

#elif(KEYPAD_NUM_COLS==4)
/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x4 shape
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button=0;
	switch(button_number)
	{
	case 1:
		keypad_button=7;
		break;
	case 2:
		keypad_button=8;
		break;
	case 3:
		keypad_button=9;
		break;
	case 4:
		keypad_button='/';
		break;
	case 5:
		keypad_button=4;
		break;
	case 6:
		keypad_button=5;
		break;
	case 7:
		keypad_button=6;
		break;
	case 8:
		keypad_button='*';
		break;
	case 9:
		keypad_button=1;
		break;
	case 10:
		keypad_button=2;
		break;
	case 11:
		keypad_button=3;
		break;
	case 12:
		keypad_button='-';
		break;
	case 13:
		keypad_button=13;
		break;
	case 14:
		keypad_button=0;
		break;
	case 15:
		keypad_button='=';
		break;
	case 16:
		keypad_button='+';
		break;
	default:
		keypad_button=button_number;
	}

	return keypad_button;
}

#endif

#endif /*STANDARD_KEYPAD*/



