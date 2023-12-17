/**************************************
 * Module: TIMER1
 * File name: timer1.h
 * Description: header file for the AVR TIMER1 driver
 * Author: amr mohsen
 *************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include"std_types.h"
#include "common_macros.h"
#include <avr/io.h>


/***********************************************************************
 *                          Types Declaration                          *
 **********************************************************************/
typedef enum
{
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024, CLOCK_FALLING, CLOCK_RAISING

}Timer1_Prescalar;

typedef enum
{
	OVERFLOW_MODE,COMPARE_MODE

}Timer1_Mode;

typedef struct
{
	uint16 initial_value;
	uint16 compare_value;
	Timer1_Prescalar prescalar;
	Timer1_Mode mode;

}Timer1_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description :
 * Function to initialize timer driver
 * 1. Set the required clock.
 * 2. Set the initial value.
 * 3. Set the compare value if needed ( in ctc mode )
 * 4. Set the timer mode (normal / compare)
 * 5. Enable interrupt
 */

void Timer1_init(const Timer1_ConfigType *Config_Ptr);


/*
 * Description :
 * Function to deinit timer driver
 */
void Timer1_denit(void);

/*
 * Description :
 * Function to set the Call Back function address
 */
void Timer1_setCallBack(void (*a_ptr)(void));



#endif /* TIMER1_H_ */
