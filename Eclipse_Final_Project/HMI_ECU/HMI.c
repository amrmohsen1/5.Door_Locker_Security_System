#include"lcd.h"
#include"keypad.h"
#include"uart.h"
#include"timer1.h"
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
 *                          Global Variables                          *
 **********************************************************************/
uint8 pass1[5];
uint8 pass2[5];
uint8 main_option_pass[5];
uint8 option;
uint8 pass_counter_fail=0;
uint8 UART_receive;
uint8 second_counter=0;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void send2PassToControl(void);
void take2PassFromUser(void);
void sendPass1(void);
void sendPass2(void);
void mainOptions(void);
void take1PassFromUser(void);
void send1PassToControl(void);
void secondsCounting(void);


int main(void)
{

	SREG|=(1<<7);

	LCD_init();

	UART_ConfigType Uart_Configurations={EIGHT_BITS,PARITY_DISABLED,ONE_STOP,9600};
	UART_init(&Uart_Configurations);

	Timer1_setCallBack(secondsCounting);

	UART_sendByte(MC1_READY);

	UART_receive = UART_recieveByte();

	if(UART_receive == PASSWORD_IS_NOT_EXIST)
	{
		do
		{
			take2PassFromUser();
			send2PassToControl();

			UART_receive = UART_recieveByte();
			if(UART_receive == PASSWORD_NOT_SAVED)
			{
				LCD_sendCommand(LCD_CLEAR_COMMAND);
				LCD_displayString("passwords not matched");
				_delay_ms(1000);
			}

		}while(UART_receive == PASSWORD_NOT_SAVED);

		LCD_sendCommand(LCD_CLEAR_COMMAND);
		LCD_displayString("Password is ");
		LCD_displayStringRowColumn(1,0,"saved");
		_delay_ms(1000);
	}



	while(1)
	{
		UART_receive = UART_recieveByte();
		while(UART_receive != GO_TO_MAIN_OPTION);
		mainOptions();
	}
	return 0;
}

/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
void take2PassFromUser()
{
	uint8 key;
	uint8 count=0;
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("plz enter pass:");
	LCD_moveCursor(1,0);
	while(count<5)
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(50);
		if(key <=9 && key>=0)
		{
			LCD_displayCharacter('*');
			pass1[count]=key;
			count++;
			_delay_ms(400);
		}
	}

	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("plz enter = to");
	LCD_displayStringRowColumn(1,0,"continue");


	while(key != '=')
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(400);
	}

	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("plz re-enter the");
	LCD_displayStringRowColumn(1,0,"same pass:");
	count=0;
	while(count<5)
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(50);
		if(key <=9 && key>=0)
		{
			LCD_displayCharacter('*');
			pass2[count]=key;
			count++;
			_delay_ms(400);
		}
	}

	while(key != '=')
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(400);
	}
}

void sendPass1()
{
	uint8 count;
	for(count=0;count<5;count++)
	{
		UART_sendByte(pass1[count]);
	}
}

void send2PassToControl()
{
	UART_sendByte(MC1_READY);
	sendPass1();

	UART_sendByte(MC1_READY);
	sendPass2();
}

void sendPass2()
{
	uint8 count;
	for(count=0;count<5;count++)
	{
		UART_sendByte(pass2[count]);
	}

}

void mainOptions()
{
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");

	option=KEYPAD_getPressedKey();
	_delay_ms(50);

	if( option == '+' )
	{
		UART_sendByte(OPEN_DOOR_OPTION);
		do{
			take1PassFromUser();
			_delay_ms(5);
			send1PassToControl();
			UART_receive = UART_recieveByte();
		}while(UART_receive == PASSWORD_NOT_MATCHED);

		if(UART_receive == PASSWORD_MATCHED)
		{
			UART_receive = UART_recieveByte();
			if(UART_receive == DOOR_IS_UNLOCKING)
			{
				LCD_sendCommand(LCD_CLEAR_COMMAND);
				LCD_displayStringRowColumn(0,5,"Door is");
				LCD_displayStringRowColumn(1,4,"Unlocking");
			}

			UART_receive = UART_recieveByte();
			if(UART_receive == DOOR_IS_OPENED)
			{
				LCD_sendCommand(LCD_CLEAR_COMMAND);
				LCD_displayStringRowColumn(0,0,"Door is Opened");
			}

			UART_receive = UART_recieveByte();
			if(UART_receive == DOOR_IS_LOCKING)
			{
				LCD_sendCommand(LCD_CLEAR_COMMAND);
				LCD_displayStringRowColumn(0,0,"Door is Locking");
			}
		}
		else if(UART_receive == PASS_WRONG_3_TIMES)
		{
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_displayString("ERROR,Alarm mode");
			LCD_displayStringRowColumn(1,0,"is activated");

			Timer1_ConfigType Timer1_Configurations={0,7812,F_CPU_1024,COMPARE_MODE};  /*setup timer1 for 60 sec blocking keypad */
			Timer1_init(&Timer1_Configurations);                                       /* during alarm mode */
			second_counter=0;
			while(second_counter != 60);
			Timer1_denit();
		}
	}

	else if( option == '-')
	{
		UART_sendByte(CHANGE_PASS_OPTION);
		do
		{
			take1PassFromUser();
			_delay_ms(5);
			send1PassToControl();
			UART_receive = UART_recieveByte();
		}while(UART_receive == PASSWORD_NOT_MATCHED);

		if(UART_receive == PASSWORD_MATCHED)
		{
			do
			{
				take2PassFromUser();
				send2PassToControl();

				UART_receive = UART_recieveByte();
				if(UART_receive == PASSWORD_NOT_SAVED){
					LCD_sendCommand(LCD_CLEAR_COMMAND);
					LCD_displayString("passwords not matched");
					_delay_ms(1000);
				}

			}while(UART_receive == PASSWORD_NOT_SAVED);

				LCD_sendCommand(LCD_CLEAR_COMMAND);
				LCD_displayString("Password is ");
				LCD_displayStringRowColumn(1,0,"saved");
				_delay_ms(1000);

		}
		else if(UART_receive == PASS_WRONG_3_TIMES)
		{
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_displayString("ERROR,Alarm mode");
			LCD_displayStringRowColumn(1,0,"is activated");

			Timer1_ConfigType Timer1_Configurations={0,7812,F_CPU_1024,COMPARE_MODE};  /*setup timer1 for 60 sec blocking keypad */
			Timer1_init(&Timer1_Configurations);                                       /* during alarm mode */
			second_counter=0;
			while(second_counter != 60);
			Timer1_denit();
		}
	}
}


void take1PassFromUser()
{
	uint8 key;
	uint8 count=0;
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("plz enter pass:");
	LCD_moveCursor(1,0);
	while(count<5)
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(50);
		if(key <=9 && key>=0)
		{
			LCD_displayCharacter('*');
			main_option_pass[count]=key;
			count++;
			_delay_ms(350);
		}
	}
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("plz enter = to");
	LCD_displayStringRowColumn(1,0,"continue");

	while(key != '=')
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(300);
	}

}

void send1PassToControl()
{
	uint8 count;

	UART_sendByte(MC1_READY);
	for(count=0;count<5;count++)
	{
		UART_sendByte(main_option_pass[count]);
	}
}

void secondsCounting(void)
{
	 second_counter++;
}
