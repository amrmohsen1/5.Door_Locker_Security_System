/**************************************
 * Module: BUZZER
 * File name: buzzer.h
 * Description: header file for the BUZZER driver
 * Author: amr mohsen
 *************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/************************************************************
 *                       Definitions                        *
 ************************************************************/

/* Buzzer Port Configurations */
#define BUZZER_PORT_ID         PORTD_ID
#define BUZZER_PIN_ID          PIN7_ID

/***********************************************************
 *                   Functions Prototypes                  *
 **********************************************************/
/*
 * Description :
 * Setup the direction for the buzzer pin as output pin through GPIO driver.
 * Turn off the buzzer through the GPIO.
 */
void Buzzer_init();

/*
 * Description :
 * Function to enable the buzzer through the GPIO.
 */
void Buzzer_on(void);

/*
 * Description :
 * Function to disable the buzzer through the GPIO.
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
