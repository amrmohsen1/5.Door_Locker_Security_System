/**************************************
 * Module: TIMER1
 * File name: timer1.c
 * Description: source file for the AVR TIMER1 driver
 * Author: amr mohsen
 *************************************/
#include"timer1.h"
#include"common_macros.h"
#include<avr\io.h>
#include<avr\interrupt.h>

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/

/* Global pointer to function to hold the address of the call back function in the application */
static volatile void(*g_callBackPtr)(void)=NULL_PTR;


/*******************************************************************************
 *                         Interrupt Service routines                          *
 *******************************************************************************/
ISR(TIMER1_OVF_vect)
{
	if ( g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if ( g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}
}


/*******************************************************************************
 *                              Functions Definitions                           *
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
void Timer1_init(const Timer1_ConfigType *Config_Ptr)
{
	/* set FOC1A for any timer mode except PWM */
	TCCR1A=(1<<FOC1A);

	/* set timer1 clock */
	TCCR1B=(TCCR1B & 0xF8) | (Config_Ptr->prescalar);

	/* set timer1 INIITAL VALUE */
	TCNT1= Config_Ptr->initial_value;

	if (Config_Ptr->mode == OVERFLOW_MODE)
	{
		/*
		 * SET CLOCK   (WGM 13:10 = 0)
		 * ENABLE INTERRUPT MODULE
		 */
		SET_BIT(TIMSK,TOIE1);
	}

	else if((Config_Ptr->mode) == COMPARE_MODE)
	{
		TCCR1B|=(1<<WGM12);

		/* set timer1 Compare VALUE */
		OCR1A= Config_Ptr->compare_value;

		/* ENABLE INTERRUPT MODULE */
		SET_BIT(TIMSK,OCIE1A);
	}

}

/*
 * Description :
 * Function to deinit timer driver
 */
void Timer1_denit(void)
{
	/*
	 * NO CLOCK SOURCE ( CS12:10 = 0)
	 */
	TCCR1B &= 0xF8;
	OCR1A=0;
	TCNT1=0;
}

/*
 * Description :
 * Function to set the Call Back function address
 */
void Timer1_setCallBack(void (*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}

