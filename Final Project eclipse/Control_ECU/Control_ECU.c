 /******************************************************************************
 *
 * Module: CONTROL_ECU
 *
 * File Name: CONTROL_ECU.c
 *
 * Description: Is responsible for all the processing and decisions in the system
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#include "dc_motor.h"
#include "control.h"
#include "timer1.h"
#include "buzzer.h"
#include "uart.h"
#include "twi.h"
#include <avr/io.h>

void main(void)
{
	/* I2C configuration pointer */
	TWI_ConfigType TWI_Config_Ptr={FOUR_HAUNDRED_KBITS_S,FIRST};

	/* UART configuration pointer */
	UART_ConfigType U_Config_Ptr={EIGHT_BIT , DISABLE_PARITY , ONE_STOP_BIT , BAUD9600};

	/* Timer1 configuration pointer */
	Timer1_ConfigType Timer1_Config_Ptr={0,7300,F_CPU_1024,CTC};/*  7000 -> SECOUND */

	/*Setup UART */
	UART_init(&U_Config_Ptr);

	/*Setup UART */
	Buzzer_init();

	/*Setup I2C */
	TWI_init(&TWI_Config_Ptr);

	/* Setup DC Motor */
	DcMotor_Init();

	/*Setup Timer1  */
	Timer1_init(&Timer1_Config_Ptr);

	/* Turn on Timer1 */
	Timer1_setCallBack(TIMER1_COUNTER);

	/* To use Timer1  ISR */
	SREG |= (1<<7);


	for(;;)
	{
		/* Create system password */
		CREAT_pass();
		/* Display main option */
		MAIN_option();
	}

}
