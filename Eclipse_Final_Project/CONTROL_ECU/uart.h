/**************************************
 * Module: UART
 * File name: uart.h
 * Description: header file for the AVR UART driver
 * Author: amr mohsen
 *************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                              Type Declarations                              *
 *******************************************************************************/
typedef enum
{
	PARITY_DISABLED,PARITY_RESERVED,EVEN_PARITY,ODD_PARITY

}UART_Parity;

typedef enum
{
	ONE_STOP,TWO_STOP

}UART_StopBit;

typedef enum
{
	FIVE_BITS,SIX_BITS,SEVEN_BITS,EIGHT_BITS

}UART_BitData;



typedef struct
{
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit  stop_bit;
	uint32 baud_rate;

}UART_ConfigType;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);


/*
 * Description :
 * Functional responsible for send string to another UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Functional responsible for receive string to another UART device.
 */
void UART_recieveString(uint8 *Str);   /*  receive until #  */


#endif /* UART_H_ */
