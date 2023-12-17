/**************************************
 * Module: Timer0_PWM
 * File name: timer0_pwm.c
 * Description: source file for the AVR Timer0_PWM driver
 * Author: amr mohsen
 *************************************/
#include<avr/io.h>
#include"timer0_pwm.h"
#include"gpio.h"

/*******************************************************************************
 *                             Functions Definitions                           *
 *******************************************************************************/
/*
 * Description:
 * The function responsible for trigger the Timer0 with the PWM Mode.
 * Setup the PWM mode with Non-Inverting.
 * Setup the prescaler with F_CPU/8.
 * Setup the compare value based on the required input duty cycle
 * Setup the direction for OC0 as output pin through the GPIO driver.
 * The generated PWM signal frequency will be 500Hz to control the DC Motor speed.
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
	DDRB|=(1<<PB3);
	TCCR0=(1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS01);
	TCNT0=0;

	/* Convert duty cycle (speed ) percentage to duty-cycle value */
	if(duty_cycle==100)
	{
		OCR0 = (uint8) (((float32)(duty_cycle)/100) * 255);
	}
	else
	{
		OCR0 = (uint8) (((float32)(duty_cycle)/100) * 256);
	}
}


