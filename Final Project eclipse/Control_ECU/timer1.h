 /******************************************************************************
 *
 * Module: Timer 1
 *
 * File Name: timer1.h
 *
 * Description: header file for the Timer1 driver
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"



/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024

}Timer1_Prescaler;


typedef enum
{
	NORMAL , CTC=4

}Timer1_Mode;

typedef struct
{
	uint16 initial_value; // Start count from this value
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;

}Timer1_ConfigType;



/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/



/*  Function responsible for
 *  Setup Timer1 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr);

/*  Function responsible for
 *  Turn off Timer1 */
void Timer1_deInit(void);

/*  Function responsible for
 *  call back Timer1 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
