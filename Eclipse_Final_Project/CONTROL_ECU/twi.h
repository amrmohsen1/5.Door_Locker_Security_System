/**************************************
 * Module: TWI
 * File name: twi.h
 * Description: header file for the AVR TWI driver
 * Author: amr mohsen
 *************************************/

#ifndef TWI_H_
#define TWI_H_

#include"std_types.h"

/*******************************************************************************
 *                              Preprocessor Macros                            *
 *******************************************************************************/
#define TWI_START              0x08  /* start has been sent */
#define TWI_REP_START          0x10  /* repeated start */
#define TWI_MT_SLA_W_ACK       0x18  /* Master transmit (slave address + write request) to slave+ ACK received */
#define TWI_MT_SLA_R_ACK       0x40  /* Master transmit (slave address + read request) to slave+ ACK received */
#define TWI_MT_DATA_ACK        0x28  /* Master transmit data and ACK received from slave  */
#define TWI_MR_DATA_ACK        0x50  /* Master receive data and send ACK to slave  */
#define TWI_MR_DATA_NACK       0x58  /* Master receive data but doesn't send ACK to slave  */

/*******************************************************************************
 *                              Type Declarations                              *
 *******************************************************************************/

typedef enum
{
	NORMAL_MODE=100000,FAST_MODE=400000,FAST_MODE_PLUS=1000000,HIGH_SPEED_MODE=3400000
}TWI_BaudRate;

typedef struct
{
	uint32 TWI_Address;
	TWI_BaudRate bit_rate;

}TWI_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description:
 * Function responsible to initialize TWI/I2C.
 */
void TWI_init(const TWI_ConfigType *Config_Ptr);

/*
 * Description:
 * Function responsible to send start bit in TWI/I2C frame.
 */
void TWI_start(void);

/*
 * Description:
 * Function responsible to send stop bit in TWI/I2C frame.
 */
void TWI_stop(void);

/*
 * Description:
 * Function responsible to write byte in TWI/I2C frame.
 */
void TWI_writeByte(uint8 data);

/*
 * Description:
 * Function responsible to read byte and send ACK to confirm another read TWI/I2C frame.
 */
uint8 TWI_readByteWithACK(void);

/*
 * Description:
 * Function responsible to read byte and send NACK to confirm that is no need to read again TWI/I2C frame.
 */
uint8 TWI_readByteWithNACK(void);


/*
 * Description:
 * Function responsible to check if the send bit ( start /repeated start/stop/....) TWI/I2C frame.
 */
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
