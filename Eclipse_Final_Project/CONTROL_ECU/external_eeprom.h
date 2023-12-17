/**************************************
 * Module: External EEPROM
 * File name: external_eeprom.h
 * Description: header file for the External EEPROM Memory
 * Author: amr mohsen
 *************************************/

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include"std_types.h"

/*******************************************************************************
 *                              Preprocessor Macros                            *
 *******************************************************************************/
#define ERROR    0
#define SUCCESS  1


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * Function responsible to write byte in External EEPROM memory location.
 */
uint8 EEPROM_writeByte(uint16 u16address,uint8 u8data);

/*
 * Description:
 * Function responsible to read byte from EEPROM memory location.
 */

uint8 EEPROM_readByte(uint16 u16address,uint8 *u8data);





#endif /* EXTERNAL_EEPROM_H_ */
