/**************************************
 * Module: External EEPROM
 * File name: external_eeprom.c
 * Description: source file for the External EEPROM
 * Author: amr mohsen
 *************************************/
#include "external_eeprom.h"
#include "twi.h"



/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
/*
 * Description:
 * Function responsible to write byte in External EEPROM memory location.
 */
uint8 EEPROM_writeByte(uint16 u16address,uint8 u8data)
{
	/* SEND Start bit */
	TWI_start();
	if(TWI_getStatus() != TWI_START)
		return ERROR;

	/*  Send device address (0x1010)
	 * + we need to get A8,A9,A10 address bits from memory location address
	 * + R/W =0 (write)
	 */
	TWI_writeByte((uint8)(((u16address & 0x0700)>>7) |0xA0));
	if(TWI_getStatus()!=TWI_MT_SLA_W_ACK)
		return ERROR;

	/* send the required memory location ( first 8 bits )*/
	TWI_writeByte((uint8)(u16address));
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* write byte in eeprom */
	TWI_writeByte(u8data);
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Stop bit */
	TWI_stop();

	return SUCCESS;

}


/*
 * Description:
 * Function responsible to read byte from EEPROM memory location.
 */

uint8 EEPROM_readByte(uint16 u16address,uint8 *u8data)
{
	/* SEND Start bit */
	TWI_start();
	if(TWI_getStatus()!=TWI_START)
		return ERROR;

	/*  Send device address (0x1010)
	 * + we need to get A8,A9,A10 address bits from memory location address
	 * + R/W =0 (write)
	 */
	TWI_writeByte((uint8)(((u16address & 0x0700)>>7) |0xA0));
	if(TWI_getStatus()!=TWI_MT_SLA_W_ACK)
		return ERROR;

	/* send the required memory location ( first 8 bits )*/
	TWI_writeByte((uint8)u16address);
	if(TWI_getStatus()!=TWI_MT_DATA_ACK)
		return ERROR;

	/* SEND Repeated Start bit */
	TWI_start();
	if(TWI_getStatus()!=TWI_REP_START)
		return ERROR;

	/*  Send device address (0x1010)
	 * + we need to get A8,A9,A10 address bits from memory location address
	 * + R/W =1 (read)
	 */
	TWI_writeByte((uint8)(((u16address & 0x0700)>>7)|0xA0 |0x01));
	if(TWI_getStatus()!=TWI_MT_SLA_R_ACK)
		return ERROR;

	/* Read byte from memory without send ACK */
	*u8data=TWI_readByteWithNACK();
	if(TWI_getStatus()!=TWI_MR_DATA_NACK)
		return ERROR;

	/* Send the Stop bit */
	TWI_stop();

	return SUCCESS;
}
