/**************************************
 * Module: LCD
 * File name: lcd.h
 * Description: Source file for the AVR LCD driver
 * Author: amr mohsen
 *************************************/
#include "lcd.h"
#include "gpio.h"
#include "common_macros.h"
#include <util/delay.h>
#include <stdlib.h>


/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/

/*
 * Description :
 * Send the required command to the screen
*/
void LCD_sendCommand(uint8 command)
{
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID,LOGIC_LOW);    /* instruction mode RS=0 */
	_delay_ms(1);      /* delay for tas=50 ns */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);    /* enable LCD E=1 */
	_delay_ms(1);      /* delay for tpw-tdsw = 190 ns */

	#if(LCD_DATA_BITS_MODE==4)

	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command,7));

	_delay_ms(1);      /* delay for tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);    /* disable LCD E=0 */
	_delay_ms(1);      /* delay for th = 13 ns */

	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);    /* enable LCD E=1 */
	_delay_ms(1);      /* delay for tpw-tdsw = 190 ns */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command,3));

	_delay_ms(1);      /* delay for tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);    /* disable LCD E=0 */
	_delay_ms(1);      /* delay for th = 13 ns */

    #elif(LCD_DATA_BITS_MODE==8)
	GPIO_writePort(LCD_DATA_PORT_ID,command);               /* out the required command*/
	_delay_ms(1);      /* delay for tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);    /* disable LCD E=0 */
	_delay_ms(1);      /* delay for th = 13 ns */

    #endif

}

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 command)
{
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID,LOGIC_HIGH);    /* instruction mode RS=1 */
	_delay_ms(1);      /* delay for tas=50 ns */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);    /* enable LCD E=1 */
	_delay_ms(1);      /* delay for tpw-tdsw = 190 ns */

	#if(LCD_DATA_BITS_MODE==4)

	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command,7));

	_delay_ms(1);      /* delay for tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);    /* disable LCD E=0 */
	_delay_ms(1);      /* delay for th = 13 ns */

	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);    /* enable LCD E=1 */
	_delay_ms(1);      /* delay for tpw-tdsw = 190 ns */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command,3));

	_delay_ms(1);      /* delay for tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);    /* disable LCD E=0 */
	_delay_ms(1);      /* delay for th = 13 ns */

    #elif(LCD_DATA_BITS_MODE==8)
	GPIO_writePort(LCD_DATA_PORT_ID,command);               /* out the required command*/
	_delay_ms(1);      /* delay for tdsw = 100 ns */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);    /* disable LCD E=0 */
	_delay_ms(1);      /* delay for th = 13 ns */

    #endif
}

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init()
{
	/* SET  RS and E as OUTPUT PINS */
	GPIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_OUTPUT);

	_delay_ms(20);           /* LCD POWER ON DELAY ALWAYS > 15ms */

	#if(LCD_DATA_BITS_MODE==4)
	/* set 4 pins in data port as output pins*/
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,PIN_OUTPUT);

	/* SEND 4 BIT initialization OF LCD */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

	/* USE 2 LINES + 4 BITS DATA MODE + 5*7 DOT DISPLAY MODE */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);

    #elif(LCD_DATA_BITS_MODE==8)
	/* SET DATA port as output */
	GPIO_setupPortDirection(LCD_DATA_PORT_ID,PORT_OUTPUT);

	/* USE 2 LINES + 8 BITS DATA MODE + 5*7 DOT DISPLAY MODE */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);

    #endif

	LCD_sendCommand(LCD_CURSOR_OFF);       /* CURSOR IS OFF */
	LCD_sendCommand(LCD_CLEAR_COMMAND);    /* CLEAR LCD AT BEGINING */
}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *Str)
{
	uint8 i=0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}

	/************ ANOTHER METHOD ******************
	while((*Str) != '\0')
	{
		LCD_displayCharacter(* Str);
		Str++;
	}
	***********************************************/
}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 lcd_memory_address;

	switch(row)
	{
	case 0:
		lcd_memory_address=col;
		break;
	case 1:
		lcd_memory_address=col+ 0x40;
		break;
	case 2:
		lcd_memory_address=col+ 0x10;
		break;
	case 3:
		lcd_memory_address=col+ 0x50;
		break;
	}

	/* Move cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION) ;
}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *Str)
{
	LCD_moveCursor(row, col);          /* GO TO Required LCD cursor location */
	LCD_displayString(Str);            /* Display the string */
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_integerToString(int data)
{
	char buff[16];              /* array (string to hold the ascii result */
	itoa(data,buff,10);      /* use itoa function to convert data to the correspnding ascii,where 10 is the decimal base*/
	LCD_displayString(buff);  /* Display the string */
}

/*
 * Description :
 * Clear the screen command
 */
void clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}
