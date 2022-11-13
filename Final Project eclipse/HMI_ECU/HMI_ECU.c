/******************************************************************************
 *
 * Module: Responsible for interaction with the user just take inputs through keypad and display messages on the LCD.
 *
 * File Name: HMI_ECU.c
 *
 * Description: Source file for the AVR  Human Machine Interface
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#include "human.h"
#include "lcd.h"
#include "uart.h"
#include <avr/io.h>

void main(void)
{
	/* UART configuration pointer */
	UART_ConfigType U_Config_Ptr={EIGHT_BIT , DISABLE_PARITY , ONE_STOP_BIT , BAUD9600};

	/*Setup UART */
	UART_init(&U_Config_Ptr);

	/*Setup LCD */
	LCD_init();

	for(;;)
	{
		/* Create system password */
		CREAT_pass();
		/* Display main option */
		MAIN_option();
	}

}
