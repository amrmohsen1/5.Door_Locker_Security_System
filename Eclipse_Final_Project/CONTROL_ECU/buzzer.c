/**************************************
 * Module: BUZZER
 * File name: buzzer.h
 * Description: header file for the BUZZER driver
 * Author: amr mohsen
 *************************************/

#include"buzzer.h"
#include"gpio.h"
#include"common_macros.h"



/**************************************************
 *             Functions Definitions              *
 *************************************************/
/*
 * Description :
 * Setup the direction for the buzzer pin as output pin through GPIO driver.
 * Turn off the buzzer through the GPIO.
 */
void Buzzer_init()
{
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);

}

/*
 * Description :
 * Function to enable the buzzer through the GPIO.
 */
void Buzzer_on(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);

}

/*
 * Description :
 * Function to disable the buzzer through the GPIO.
 */
void Buzzer_off(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);

}


