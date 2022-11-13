/******************************************************************************
 *
 * Module: Timer 1
 *
 * File Name: timer1.h
 *
 * Description: Source file for the Timer1 driver
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#include "timer1.h"
#include "gpio.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/* Timer1 call back pointer */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER1_COMPA_vect)
{

	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}

}


ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}
}

/*  Function responsible for
 *  Setup Timer1 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr)
{
	TCNT1=Config_Ptr->initial_value;

	OCR1A=Config_Ptr->compare_value;

	if((Config_Ptr->mode)==NORMAL)
	{
		TIMSK |= (1<<TOIE1);

	}
	else if((Config_Ptr->mode)==CTC)
	{
		TIMSK |= (1<<OCIE1A);
	}


	TCCR1A |= (1<<FOC1A)

		     |(GET_BIT((Config_Ptr->mode),0 ) << WGM10)
			 |(GET_BIT((Config_Ptr->mode),1 ) << WGM11);


	TCCR1B |=  ((Config_Ptr->prescaler & 0x07)<< CS10)

    	      |(GET_BIT((Config_Ptr->mode),2 ) << WGM12)
		      |(GET_BIT((Config_Ptr->mode),3 ) << WGM13);
}

/*  Function responsible for
 *  Turn off Timer1 */
void Timer1_deInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;
	TIMSK &= ~(1<<TOIE1) &~(1<<OCIE1A) ;
}


/*  Function responsible for
 *  call back Timer1 */
void Timer1_setCallBack(void(*a_ptr)(void))
{

	g_callBackPtr=a_ptr;
}
