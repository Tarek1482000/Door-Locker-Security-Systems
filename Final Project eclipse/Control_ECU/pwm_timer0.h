 /******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm_timer0.h
 *
 * Description: Header file for the AVR PWM-Timer0 driver
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#ifndef PWM_TIMER0_H_
#define PWM_TIMER0_H_

#include "std_types.h"

#define TIMER0_OCO_PORT_ID    1
#define TIMER0_OCO_PIN_ID     3

/*
 * Description :
 * Initialize the PWM module by:
 * 1. Trigger Timer0 with PWM Mode.
 * 2. Setup the PWM mode with Non-Inverting.
 * 3. Setup the precaler with F_CPU/8
 * 4. Setup the compare value based on the required input duty cycle
 * 5. Setup the direction for OC0 as output pin
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_TIMER0_H_ */
