/**************************************
 * Module: TWI
 * File name: twi.c
 * Description: source file for the AVR TWI driver
 * Author: amr mohsen
 *************************************/

#include"twi.h"
#include<avr/io.h>
#include"common_macros.h"


/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
/*
 * Description:
 * Function responsible to initialize TWI/I2C.
 */
void TWI_init(const TWI_ConfigType *Config_Ptr)
{
	/* Bit Rate 400 kbps using zero prescalar TWPS1=0,TWPS0=0 and F_CPU=8 Mhz */
	TWBR= ((F_CPU / (Config_Ptr->bit_rate)) -16) / 2;
	TWSR=0x00;

	/* set device address if any master device want to call me */
	TWAR=(Config_Ptr->TWI_Address);       /* my address=0x01 */

	TWCR=(1<<TWEN);           /* ENABLE TWI */
}


/*
 * Description:
 * Function responsible to send start bit in TWI/I2C frame.
 */
void TWI_start(void)
{
	/*
	 * 1. CLEAR TWINT FLAG before sending the start bit  TWINT =1
	 * 2. ENABLE TWI   TWEN=1
	 * 3. SEND START BIT  TWSTA=1
	 */
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA);

	while(BIT_IS_CLEAR(TWCR,TWINT));

}

/*
 * Description:
 * Function responsible to send stop bit in TWI/I2C frame.
 */
void TWI_stop(void)
{
	/*
	 * 1. CLEAR TWINT FLAG before sending the stop bit  TWINT =1
	 * 2. ENABLE TWI   TWEN=1
	 * 3. SEND STOP BIT  TWSTA=1
	 */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	/*
	 * the TWINT FLAG IS NOT RAISED AFTER STOP BIT IN CASE OF MASTER DEVICE
	 * so don't chack for TWINT flag in case of master as it will be stuck on pooling check
	 */


}

/*
 * Description:
 * Function responsible to write byte in TWI/I2C frame.
 */
void TWI_writeByte(uint8 data)
{
	/* put data in TWI data register */
	TWDR=data;
	/*
	 * CLEAR TWINT FLAG before sending data   TWINT=1
	 * enable the TWI MODULE  TWIN=1
	 */

	TWCR = (1<<TWINT) | (1<<TWEN);

	/* WAIT FOR TWINT FLAG TO BE raised ( DATA SENT SUCCESIVELY) */
	while(BIT_IS_CLEAR(TWCR,TWINT));

}

/*
 * Description:
 * Function responsible to read byte and send ACK to confirm another read TWI/I2C frame.
 */
uint8 TWI_readByteWithACK(void)
{
	/*
	 * CLEAR TWINT FLAG before sending data   TWINT=1
	 * enable the TWI MODULE  TWIN=1
	 * ENABLE ACK after reading/receiving data  TWEA=1
	 */

	TWCR =(1<<TWINT) | (1<<TWEN) | (1<<TWEA);

	/* WAIT FOR TWINT FLAG TO BE raised ( DATA RECEIVED SUCCESIVELY) */
	while(BIT_IS_CLEAR(TWCR,TWINT));

	/* Read data */
	return TWDR;

}

/*
 * Description:
 * Function responsible to read byte and send NACK to confirm that is no need to read again TWI/I2C frame.
 */
uint8 TWI_readByteWithNACK(void)
{
	/*
	 * CLEAR TWINT FLAG before sending data   TWINT=1
	 * enable the TWI MODULE  TWIN=1
	 */
	TWCR = (1<<TWINT) | (1<<TWEN);

	/* WAIT FOR TWINT FLAG TO BE raised ( DATA RECEIVED SUCCESIVELY) */
	while(BIT_IS_CLEAR(TWCR,TWINT));

	/* Read data */
	return TWDR;

}

/*
 * Description:
 * Function responsible to check if the send bit ( start /repeated start/stop/....) TWI/I2C frame.
 */
uint8 TWI_getStatus(void)
{
	uint8 status;

	/* masking to eliminate first 3 bits and get last 5 bits in status bits */
	status= TWSR & 0xF8;

	return status;

}

