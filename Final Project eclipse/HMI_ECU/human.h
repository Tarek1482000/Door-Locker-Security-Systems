 /******************************************************************************
 *
 * Module: Human interface
 *
 * File Name: human.h
 *
 * Description: header file for communication with user
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#ifndef HUMAN_H_
#define HUMAN_H_

#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define  MAX_TRAIL                   3
#define  ERROR_PASS                  10
#define  CREAT_PASS_AGAIN            1
#define  OPEN_DOOR                   1
#define  LOCK_DOOR                   2
#define  RESTART                     3
#define  CHANGE_PASS                 2
#define  NUM_OF_PASS                 5
#define  PASS_IS_MATCHED             1
#define  PASS_NOT_MATCH              3
#define  HMI_ECU_READY               0X01
#define  Control_ECU_READY           0X02



/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/


/* Function responsible for
 * sending created and confirm password
 * to Control_ECU
*/
void  CREAT_pass(void);


/* Function responsible for
 * sending user option
 * to Control_ECU
*/
void MAIN_option(void);


/*
 * Function responsible for
 * open door if user want and
 * entered right password
*/
void OPEN_door(void);


/*
 * Function responsible for
 * hold the system for 1 minute
 * if user entered wrong password for three times
*/
void WRONG_PASS_ERROR(void);


/*
 * Function responsible for
 * take user inputs
 * and display password entering in LCD
*/
void  ENTER_pass(uint8 *pass);


#endif /* HUMAN_H_ */
