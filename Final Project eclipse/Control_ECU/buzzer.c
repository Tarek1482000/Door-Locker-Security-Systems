 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for buzzer
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"

/*  Function responsible for
 *  Setup buzzer pins*/
void Buzzer_init(void)
{

	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);

	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

/*  Function responsible for
 * Turn on buzzer*/
void Buzzer_on(void)
{

	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}



/*  Function responsible for
 * Turn off buzzer*/
void Buzzer_off(void)
{

	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

