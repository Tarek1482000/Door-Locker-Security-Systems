 /******************************************************************************
 *
 * Module: control
 *
 * File Name: control.h
 *
 * Description: Header file for responsible for all the processing and decisions in the system
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/
#ifndef CONTROL_H_
#define CONTROL_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define  MAX_TRAIL                  3
#define  ERROR_PASS                 10
#define  CREAT_PASS_AGAIN           1
#define  OPEN_DOOR                  1
#define  LOCK_DOOR                  2
#define  RESTART                    3
#define  CHANGE_PASS                2
#define  PASS_NOT_MATCH             3
#define  PASS_IS_MATCHED            1
#define  HMI_ECU_READY              0X01
#define  Control_ECU_READY          0X02



/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Function responsible for
 * count seconds
*/
void TIMER1_COUNTER(void);


/* Function responsible for
 * receive created and confirm password
 * from HMI_ECU and store it in EEPROM
 * if the two are matched
*/
void  CREAT_pass(void);



/* Function responsible for
 * receive option from HMI_ECU and do it
 * if password correct
*/
void  MAIN_option(void);


/*
 * Function responsible for
 *  open door if user want and
 *  entered right password
*/
void OPEN_door(void);

/*
 * Function responsible for
 * turn on alarm and display EEPROM on screen
 * if user entered wrong password for three times
*/
void WRONG_PASS_ERROR(void);


/*
 * Function responsible for
 * compare between two passwords
 * if not the same
 * return FALSE
*/
uint8 IS_passMatch(uint8 *str1 , uint8 *str2);



/*
 * Function responsible for
 * take user password
 */
void  RECEIVE_pass(uint8 *pass);

#endif /* CONTROL_H_ */
