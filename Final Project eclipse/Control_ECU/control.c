/******************************************************************************
 *
 * Module: control
 *
 * File Name: control.c
 *
 * Description: Source file for responsible for all the processing and decisions in the system
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/


#include "control.h"
#include "dc_motor.h"
#include "timer1.h"
#include "buzzer.h"
#include "uart.h"
#include "twi.h"
#include "external_eeprom.h"
#include <util/delay.h>
#include <avr/io.h>

volatile uint16 g_tick=0 ;/* Timer counter */

uint8 g_pass[5]       /* Store created password */
, g_re_pass[5]        /* Store created confirm password */
, g_pass_entered[5]   /* Store entered password */
, g_eeprom_pass[5];   /* Store  the password from EEPROM  */


/*
 * Function responsible for
 * count seconds
 */
void TIMER1_COUNTER(void)
{
	g_tick++;
}


/* Function responsible for
 * receive created and confirm password
 * from HMI_ECU and store it in EEPROM
 * if the two are matched
 */
void  CREAT_pass(void)
{
	/*******************************************************************************
	 *                               STEP 1                                        *
	 *******************************************************************************/
	/* Create a System Password */


	RECEIVE_pass(g_pass);      /* Receive created password */


	RECEIVE_pass(g_re_pass);   /* Receive confirmed password */


	UART_sendByte(IS_passMatch(g_pass,g_re_pass)); /* Send the compare result to HMI_ECU*/

	if(IS_passMatch(g_pass,g_re_pass)) /*If two passwords are the same store it in EEPROM*/
	{
		EEPROM_writeByte(0x1010, g_pass[0]);
		_delay_ms(10);
		EEPROM_writeByte(0x2020, g_pass[1]);
		_delay_ms(10);
		EEPROM_writeByte(0x3030, g_pass[2]);
		_delay_ms(10);
		EEPROM_writeByte(0x4040, g_pass[3]);
		_delay_ms(10);
		EEPROM_writeByte(0x5050, g_pass[4]);
		_delay_ms(10);
	}
	else  /*If two passwords are not the same creat password again */
	{
		CREAT_pass();
	}

}


/* Function responsible for
 * receive option from HMI_ECU and do it
 * if password correct
 */
void  MAIN_option(void)
{
	static uint8  trail=0;
	uint8 option=0;

	/*******************************************************************************
	 *                               STEP 2                                        *
	 *******************************************************************************/
	/*Main Options*/

	/*
	 * Reading password which saved in EEPROM
	 */
	EEPROM_readByte(0x1010, &g_eeprom_pass[0]);
	_delay_ms(10);
	EEPROM_readByte(0x2020, &g_eeprom_pass[1]);
	_delay_ms(10);
	EEPROM_readByte(0x3030, &g_eeprom_pass[2]);
	_delay_ms(10);
	EEPROM_readByte(0x4040, &g_eeprom_pass[3]);
	_delay_ms(10);
	EEPROM_readByte(0x5050, &g_eeprom_pass[4]);



	/* Receive entered password */
	RECEIVE_pass(g_pass_entered);

	/* If created password match with enterd password*/
	if(IS_passMatch(g_pass_entered,g_eeprom_pass))
	{
		/* Confirm to HMI_ECU that created password match with enterd password*/
		UART_sendByte(PASS_IS_MATCHED);

		/* Wait until interface HMI_ECU be ready */
		while(UART_recieveByte()!=HMI_ECU_READY);

		/* Receive users option if password is right*/
		option =UART_recieveByte();


		/*******************************************************************************
		 *                               STEP 3                                        *
		 *******************************************************************************/
		/*Open Door */
		if(option == OPEN_DOOR)
		{
			OPEN_door();
			MAIN_option();
		}

		/*******************************************************************************
		 *                               STEP 4                                        *
		 *******************************************************************************/
		/* Change Password*/
		else if(option ==CHANGE_PASS)
		{
			CREAT_pass();
			MAIN_option();
		}

	}

	/*******************************************************************************
	 *                               STEP 5                                        *
	 *******************************************************************************/
	/* If created password not match with enterd password*/
	else
	{

		/* Send to HMI_ECU that created password not match with enterd password*/
		UART_sendByte(PASS_NOT_MATCH);
		trail++;

		/* Two trail for entering wrong password*/
		if(trail<3)
		{
			MAIN_option();
		}

		/* If entering wrong password reach maximum turn on alarm and turn off HMI_ECU system inputs for 1 minute*/
		else if(trail==MAX_TRAIL)
		{
			trail=0;
			WRONG_PASS_ERROR();
			MAIN_option();
		}


	}

}


/*
 * Function responsible for
 *  open door if user want and
 *  entered right password
 */
void OPEN_door(void)
{
	g_tick=0;
	/* Wait until interface HMI_ECU be ready */
	while(UART_recieveByte()!=HMI_ECU_READY);
	UART_sendByte(OPEN_DOOR);
	DcMotor_Rotate(DC_MOTOR_CW ,100);
	/* Unlock the door for 15 seconds and turn on the motor clock wise */
	while(g_tick <= 15);


	g_tick=0;
	DcMotor_Rotate(DC_MOTOR_STOP , 0);
	/* Hold the motor  for 3 seconds */
	while(g_tick <= 3);



	g_tick=0;
	/* Wait until interface HMI_ECU be ready */
	while(UART_recieveByte()!=HMI_ECU_READY);
	UART_sendByte(LOCK_DOOR);
	DcMotor_Rotate(DC_MOTOR_ACW ,100);
	/* Lock the door for 15 seconds and turn on the motor anti clock wise */
	while(g_tick <= 15);


	/* Return HMI_ECU  to main option*/
	UART_sendByte(RESTART);

	/* Turn off the motor  for 3 seconds */
	DcMotor_Rotate(DC_MOTOR_STOP , 0);
}




/*
 * Function responsible for
 * turn on alarm and display EEPROM on screen
 * if user entered wrong password for three times
 */
void WRONG_PASS_ERROR(void)
{
	g_tick=0;

	while(g_tick<=60)
	{
		Buzzer_on();

		UART_sendByte(ERROR_PASS);
	}

	Buzzer_off();
	UART_sendByte(RESTART);
}


/*
 * Function responsible for
 * compare between two passwords
 * if not the same
 * return FALSE
 */
uint8 IS_passMatch(uint8 *str1 , uint8 *str2)
{
	uint8 i=0;

	for(i=0;i<5;i++)
	{
		if(str1[i]!=str2[i])
			return FALSE;
	}
	return TRUE;
}


/*
 * Function responsible for
 * take user password
 */
void  RECEIVE_pass(uint8 *pass)
{
	/* Wait until interface HMI_ECU be ready communicate*/
	while(UART_recieveByte()!=HMI_ECU_READY);
	pass[0]=UART_recieveByte();

	/* Wait until interface HMI_ECU be ready communicate*/
	while(UART_recieveByte()!=HMI_ECU_READY);
	pass[1]=UART_recieveByte();

	/* Wait until interface HMI_ECU be ready communicate*/
	while(UART_recieveByte()!=HMI_ECU_READY);
	pass[2]=UART_recieveByte();

	/* Wait until interface HMI_ECU be ready communicate*/
	while(UART_recieveByte()!=HMI_ECU_READY);
	pass[3]=UART_recieveByte();

	/* Wait until interface HMI_ECU be ready communicate*/
	while(UART_recieveByte()!=HMI_ECU_READY);
	pass[4]=UART_recieveByte();
}

