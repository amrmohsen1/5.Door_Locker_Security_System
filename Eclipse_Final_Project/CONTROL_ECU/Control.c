#include"uart.h"
#include"gpio.h"
#include"twi.h"
#include"external_eeprom.h"
#include"dc_motor.h"
#include"timer1.h"
#include"buzzer.h"
#include<avr/interrupt.h>
#include<util/delay.h>

/***********************************************************************
 *                            Definitions                              *
 ***********************************************************************/
#define MC1_READY               0x10
#define MC2_READY               0x20
#define PASSWORD_SAVED          0x30
#define PASSWORD_NOT_SAVED      0x40
#define PASSWORD_MATCHED        0x50
#define PASSWORD_NOT_MATCHED    0x60
#define RECEIVE_MAIN_OPTION     0x70
#define OPEN_DOOR_OPTION        0x80
#define CHANGE_PASS_OPTION      0x81
#define GO_TO_MAIN_OPTION       0x90
#define DOOR_IS_LOCKING         0x91
#define DOOR_IS_OPENED			0x92
#define DOOR_IS_UNLOCKING       0x93
#define PASSWORD_IS_EXIST       0X94
#define PASSWORD_IS_NOT_EXIST   0X95
#define PASS_WRONG_3_TIMES      0x96

/***********************************************************************
 *                          Types Declaration                          *
 **********************************************************************/
typedef enum{
	passToCompare_1,
	passToCompare_2,
	passToCheck,
}password_type;

/***********************************************************************
 *                          Global Variables                          *
 **********************************************************************/
uint8 Pass1[5];
uint8 Pass2[5];
uint8 main_option_pass[5];
uint8 pass_exist_value;
uint8 main_menu_option;
uint8 password_flag=0;           /* flag is set if two password match */
uint8 counter=0;                 /* counter to compare five number of the two password received */
uint8 pass_fail_counter=0;
uint8 second_counter=0;
uint8 uart_receive;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void receievePass(password_type password_t);
void saveNewPasstoMemory();
void secondsCounting();
void doorUnlocking();
void doorStop();
void doorLocking();
void compare2Pass(void);
void checkMainPass(void);
void alarmMode(void);

int main()
{
	UART_ConfigType Uart_Configurations={EIGHT_BITS,PARITY_DISABLED,ONE_STOP,9600};
	UART_init(&Uart_Configurations);

	TWI_ConfigType TWI_Configurations={0x01,FAST_MODE};
	TWI_init(&TWI_Configurations);

	Timer1_ConfigType Timer1_Configuraions={0,7812,F_CPU_1024,COMPARE_MODE};
	Timer1_setCallBack(secondsCounting);

	DcMotor_Init();

	Buzzer_init();

	SREG|=(1<<7);


	/* wait for HMI IS READY TO SEND */
	while(UART_recieveByte() != MC1_READY);

	EEPROM_readByte(0x0200,&pass_exist_value);
	if(pass_exist_value == PASSWORD_IS_EXIST)
	{
		UART_sendByte(PASSWORD_IS_EXIST);
		for(counter = 0; counter < 5; counter++)
		{
			EEPROM_readByte(0x0300 + counter,&Pass1[counter]);
			_delay_ms(10);
		}
	}

	else
	{
		UART_sendByte(PASSWORD_IS_NOT_EXIST);
		do
		{
			receievePass(passToCompare_1);            /* receive first password entered from user */

			receievePass(passToCompare_2);            /* receive second password entered from user */

			compare2Pass();                           /* compare two password */

			if(password_flag == 0)
			{
				UART_sendByte(PASSWORD_NOT_SAVED);
			}

			else
			{
				UART_sendByte(PASSWORD_SAVED);
				EEPROM_writeByte((0x0200),PASSWORD_IS_EXIST);
				_delay_ms(10);
				saveNewPasstoMemory();
			}
		}while(password_flag == 0);

	}


	while(1)
	{
		UART_sendByte(GO_TO_MAIN_OPTION);
		uart_receive = UART_recieveByte();

		switch(uart_receive)
		{
		case OPEN_DOOR_OPTION:
			pass_fail_counter = 0;
			do
			{
				receievePass(passToCheck);
				checkMainPass();
				if(password_flag == 0)
				{
					pass_fail_counter++;
					if(pass_fail_counter < 3)
					{
						UART_sendByte(PASSWORD_NOT_MATCHED);
					}
				}
			}while((password_flag == 0) && (pass_fail_counter < 3));

			if(pass_fail_counter == 3)
			{
				UART_sendByte(PASS_WRONG_3_TIMES);

				Timer1_init(&Timer1_Configuraions);        /* activate buzzer for 60 sec */
				alarmMode();
				Timer1_denit();
			}
			else
			{
				UART_sendByte(PASSWORD_MATCHED);

				UART_sendByte(DOOR_IS_UNLOCKING);           /* send this message to HMI TO show IY ON LCD */
				Timer1_init(&Timer1_Configuraions);
				doorUnlocking();
				Timer1_denit();

				UART_sendByte(DOOR_IS_OPENED);
				Timer1_init(&Timer1_Configuraions);
				doorStop();
				Timer1_denit();

				UART_sendByte(DOOR_IS_LOCKING);
				Timer1_init(&Timer1_Configuraions);
				doorLocking();
				Timer1_denit();

				DcMotor_Rotate(STOP,0);
			}
			break;

		case CHANGE_PASS_OPTION:
			pass_fail_counter = 0;
			do{
				receievePass(passToCheck);
				checkMainPass();
				if(password_flag == 0)
				{
					pass_fail_counter++;
					if(pass_fail_counter<3)
					{
						UART_sendByte(PASSWORD_NOT_MATCHED);
					}
				}
			}while((password_flag == 0) && pass_fail_counter < 3);

			if(pass_fail_counter == 3)
			{
				UART_sendByte(PASS_WRONG_3_TIMES);
				Timer1_init(&Timer1_Configuraions);        /* activate buzzer for 60 sec */
				alarmMode();
				Timer1_denit();
			}

			else
			{
				UART_sendByte(PASSWORD_MATCHED);
				do
				{
					receievePass(passToCompare_1);

					receievePass(passToCompare_2);

					compare2Pass();

					if(password_flag == 0)
					{
						UART_sendByte(PASSWORD_NOT_SAVED);
					}else
					{
						UART_sendByte(PASSWORD_SAVED);
						EEPROM_writeByte((0x0200),PASSWORD_IS_EXIST);
						_delay_ms(10);
						saveNewPasstoMemory();
					}
				}while(password_flag == 0);
			}
			break;
		}
	}
}
/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/

void receievePass(password_type password_t)
{
	uint8 count;

	uart_receive = UART_recieveByte();
	while(uart_receive != MC1_READY);

	switch (password_t) {
		case passToCompare_1:
			for(count=0; count<5;count++)
			{
				Pass1[count]=UART_recieveByte();
			}
			break;
		case passToCompare_2:
			for(count=0; count<5;count++)
			{
				Pass2[count]=UART_recieveByte();
			}
			break;
		case passToCheck:
			for(count=0; count<5;count++)
			{
				main_option_pass[count]=UART_recieveByte();
			}
			break;
	}
}

void saveNewPasstoMemory(void)
{
	uint8 count=0;

	for(count=0; count<5;count++)
	{
		EEPROM_writeByte((0x0300+count),Pass1[count]);
		_delay_ms(10);
	}
}

void doorUnlocking(void)
{
	second_counter=0;
	while(second_counter != 15)
	{
		DcMotor_Rotate(CW,100);
	}
}

void doorStop(void)
{
	second_counter=0;
	while(second_counter != 3)
	{
		DcMotor_Rotate(STOP,0);
	}
}

void doorLocking(void)
{
	second_counter=0;
	while(second_counter != 15)
	{
		DcMotor_Rotate(A_CW,100);
	}
}

void compare2Pass(void)
{
	uint8 counter;
	password_flag = 1;                       /* assume two password are matched until one number differs */
	for(counter=0; counter<5 ;counter++)
	{
		if(Pass1[counter] != Pass2[counter])
		{
			password_flag = 0;
		}
	}
}

void checkMainPass(void){
	uint8 counter;
	password_flag = 1;
	for(counter=0; counter<5 ;counter++)
	{
		if(Pass1[counter] != main_option_pass[counter])
		{
			password_flag = 0;
		}
	}
}

void secondsCounting(void)
{
	 second_counter++;
}

void alarmMode(void)
{
	Buzzer_on();
	second_counter=0;
	while(second_counter !=60);
	Buzzer_off();
}
