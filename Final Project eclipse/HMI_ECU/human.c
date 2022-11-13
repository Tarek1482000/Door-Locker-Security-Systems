/******************************************************************************
 *
 * Module: Human interface
 *
 * File Name: human.c
 *
 * Description: Source file for communication with user
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#include "human.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include <util/delay.h>
#include <avr/io.h>

uint8
g_key=0 /* Store user option */

, g_Password[5]      /* Store created password */
, g_re_Password[5]   /* Store created confirm password */
, g_Pass_Entered[5]; /* Store entered password */



/* Function responsible for
 * sending created and confirm password
 * to Control_ECU
 */
void  CREAT_pass(void)
{
	/*******************************************************************************
	 *                               STEP 1                                        *
	 *******************************************************************************/
	/* Create a System Password */

	/* Clear screen from last display */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Plz Enter Pass: ");
	LCD_moveCursor(1,0); /* Move the cursor to the second row */

	ENTER_pass(g_Password); /* Create password */



	/* Clear screen from last display */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Plz re-enter the");
	LCD_displayStringRowColumn(1, 0, "same Pass:");
	LCD_moveCursor(1,11);

	/* Confirm Password */
	ENTER_pass(g_re_Password);

	/*
	 * If compare result for two password
	 * which received from Control_ECU
	 * are matched display options for user
	 * if not creat password again
	 */
	if(UART_recieveByte() == PASS_IS_MATCHED)
	{
		/*******************************************************************************
		 *                               STEP 2                                        *
		 *******************************************************************************/
		/*Main Options*/

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Change pass");
		g_key=KEYPAD_getPressedKey() ;
		_delay_ms(300);
	}
	else
	{
		CREAT_pass();
	}

}


/* Function responsible for
 * sending user option
 * to Control_ECU
 */
void MAIN_option(void)
{
	static uint8  trail=0;

	if(g_key == '+')
	{
		/*******************************************************************************
		 *                               STEP 3                                        *
		 *******************************************************************************/
		/*Open Door */

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Plz enter pass:");
		LCD_moveCursor(1,0);

		/* Enter password must match to created password */
		ENTER_pass(g_Pass_Entered);

		/*
		 * If compare result for entered and created passwords
		 * which received from Control_ECU
		 * are matched open the door
		 * if not enter password again for two trail
		 * and hold the system for 1 minute for third trail
		 */
		if(UART_recieveByte() == PASS_IS_MATCHED)
		{
			OPEN_door();
			MAIN_option();
		}
		else
		{
			trail++;
			if(trail<3)
			{
				g_key = '+';
				MAIN_option();
			}
			else if(trail==MAX_TRAIL)
			{
				trail=0;
				WRONG_PASS_ERROR();
				MAIN_option();
			}


		}

	}

	/*******************************************************************************
	 *                               STEP 4                                        *
	 *******************************************************************************/
	/* Change Password*/

	else if(g_key == '-')
	{


		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Plz enter pass:");
		LCD_moveCursor(1,0);

		/* Enter password must match to created password */
		ENTER_pass(g_Pass_Entered);

		/*
		 * If compare result for entered and created passwords
		 * which received from Control_ECU
		 * are matched change password
		 * if not enter password again for two trail
		 * and hold the system for 1 minute for third trail
		 */
		if(UART_recieveByte() == PASS_IS_MATCHED)
		{
			UART_sendByte(HMI_ECU_READY);

			UART_sendByte(CHANGE_PASS);

			CREAT_pass();
			MAIN_option();
		}
		/*******************************************************************************
		 *                               STEP 5                                        *
		 *******************************************************************************/
		/* If created password not match with enterd password*/
		else
		{
			trail++;
			if(trail<3)
			{
				g_key = '-';
				MAIN_option();

			}
			else if(trail==MAX_TRAIL)
			{
				trail=0;
				WRONG_PASS_ERROR();
				MAIN_option();
			}

		}

	}

}


/*
 * Function responsible for
 * open door if user want and
 * entered right password
 */
void OPEN_door(void)
{
	/* Send to Control_ECU that HMI_ECU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);

	/* Ask Control_ECU to open door*/
	UART_sendByte(OPEN_DOOR);


	/* Send to Control_ECU that HMI_ECU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);
	/* Wait until Control_ECU open door*/
	while(UART_recieveByte()!=OPEN_DOOR);
	/* Display door state in LCD */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0,"DoorIs Unlocking");


	/* Send to Control_ECU that HMI_ECU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);
	/* Wait until Control_ECU lock door*/
	while(UART_recieveByte()!=LOCK_DOOR);
	/* Display door state in LCD */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0,"Door is locking");

	while(UART_recieveByte()!=RESTART);

	/* Return to main option */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+ : Open Door");
	LCD_displayStringRowColumn(1, 0, "- : Change pass");
	g_key=KEYPAD_getPressedKey() ;
}




/*
 * Function responsible for
 * hold the system for 1 minute
 * if user entered wrong password for three times
 */
void WRONG_PASS_ERROR(void)
{
	/* Wait until Control_ECU  to send error password for third trail */
	while(UART_recieveByte()!=ERROR_PASS);
	/* Display error in LCD */
	LCD_clearScreen();
	LCD_displayString("Error Password");

	/* Wait until Control_ECU restart the system*/
	while(UART_recieveByte()!=RESTART);

	/* Return to main option */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+ : Open Door");
	LCD_displayStringRowColumn(1, 0, "- : Change pass");
	g_key=KEYPAD_getPressedKey() ;

}


/*
 * Function responsible for
 * take user inputs
 * and display password entering in LCD
 */
void  ENTER_pass(uint8 *pass)
{
	uint8 counter=0 , key=0;

	for(counter=0;counter<NUM_OF_PASS;counter++)
	{
		/* Time for press the button*/
		_delay_ms(600);

		/* 	GET password value*/
		key = KEYPAD_getPressedKey();

		pass[counter]=key;
		/* 	Display * in LCD every 	Entering a number */
//		LCD_intgerToString(key);
		LCD_displayCharacter('*');
	}
	while(key!= '=')
		key = KEYPAD_getPressedKey();
	_delay_ms(300);

	/* Send entered password to Control_ECU */

	/* Send to Control_ECU that HMI_ECU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);
	UART_sendByte(pass[0]);

	/* Send to Control_ECU that HMI_ECU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);
	UART_sendByte(pass[1]);

	/* Send to Control_ECU that HMI_ECU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);
	UART_sendByte(pass[2]);

	/* Send to Control_ECU that HMI_ECU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);
	UART_sendByte(pass[3]);

	/* Send to Control_ECU that HMI_ECU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);
	UART_sendByte(pass[4]);
}



