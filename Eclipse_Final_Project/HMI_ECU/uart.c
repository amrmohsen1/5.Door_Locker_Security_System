/**************************************
 * Module: UART
 * File name: uart.c
 * Description: source file for the AVR UART driver
 * Author: amr mohsen
 *************************************/

#include"uart.h"
#include <avr\io.h>
#include "common_macros.h"


/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr)
{
	uint16 ubrr_value=0;

	/* U2X=1 FOR Double transmission speed */
	UCSRA=(1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * TXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB=(1<<RXEN)|(1<<TXEN);


	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  =   parity bit MODE
	 * USBS    =   number stop bits
	 * UCSZ1:0 =   bits data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/

	SET_BIT(UCSRC,URSEL);

	/* insert parity in the bit 4 and bit 5 */
	UCSRC=(UCSRC &0xCF) | ((Config_Ptr->parity) << 4);

	/* insert number of STOP BITs in the  bit 3 */
	UCSRC=(UCSRC &0xF8) | ((Config_Ptr->stop_bit) << 3);

	/* insert number of data BITs in the bit1 , bit2 */
	UCSRC=(UCSRC &0xF6) | ((Config_Ptr->bit_data) << 1);


	/* Calculate the UBRR REGISTER VALUE */

	ubrr_value= ((F_CPU / (8 * (Config_Ptr->baud_rate)))-1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH */
	UBRRL= ubrr_value;
	UBRRH= ubrr_value >> 8;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(uint8 data)
{
	while(BIT_IS_CLEAR(UCSRA,UDRE));          /*  WAIT until Uart Data Register Empty flag is raised to send data */
	UDR=data;                                 /* send data  */

}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC));           /*  wait until receive flag is raised ( wait for unread data) */

	return UDR;                               /* this step made the folLowing
	                                             * 1. clear RXCflag
	                                             * 2. read data in UDR   */
}


/*
 * Description :
 * Functional responsible for send string to another UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i=0;

	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}

	/* ******************Another Method ****************************
	while( *Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	****************************************************************/
}

/*
 * Description :
 * Functional responsible for receive string to another UART device.
 */
void UART_recieveString(uint8 *Str)     /*  receive until #  */
{

	/* receive first byte */
	/* then receive the whole string until the '#' */

	uint8 i=0;
	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i]='\0';
}
