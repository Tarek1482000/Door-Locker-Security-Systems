 /******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm_timer0.c
 *
 * Description: Source file for the AVR PWM-Timer0 driver
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#include "pwm_timer0.h"
#include "gpio.h"
#include "avr/io.h" /* To use Timer0 Registers */

/*
 * Description :
 * Initialize the PWM module by:
 * 1. Trigger Timer0 with PWM Mode.
 * 2. Setup the PWM mode with Non-Inverting.
 * 3. Setup the prescaler with F_CPU/8
 * 4. Setup the compare value based on the required input duty cycle
 * 5. Setup the direction for OC0 as output pin
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = 0; /* Timer0 initial value */

	OCR0  = ((float)duty_cycle / 100) * 255; /* Set Compare value */

	/*
	 * Configure Timer0 control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);

	/* Configure PB3/ OC0 as output pin --> pin where the PWM signal is generated from Timer0 */
	GPIO_setupPinDirection(TIMER0_OCO_PORT_ID,TIMER0_OCO_PIN_ID,PIN_OUTPUT);
}



