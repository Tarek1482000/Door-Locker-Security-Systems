 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for buzzer
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT       PORTB_ID
#define BUZZER_PIN        PIN0_ID

/*  Function responsible for
 *  Setup buzzer pins*/
void Buzzer_init(void);



/*  Function responsible for
 * Turn on buzzer*/
void Buzzer_on(void);


/*  Function responsible for
 * Turn off buzzer*/
void Buzzer_off(void);


#endif /* BUZZER_H_ */
