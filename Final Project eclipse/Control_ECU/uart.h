 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the AVR UART driver
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	FIVE_BIT , SIX_BIT , SEVEN_BIT , EIGHT_BIT  ,NINE_BIT=7

}UART_BitData;


typedef enum
{
	DISABLE_PARITY,REVERSED,EVEN_PARITY,ODD_PARITY

}UART_Parity;


typedef enum
{
	ONE_STOP_BIT , TWO_STOP_BIT

}UART_StopBit;


typedef enum
{

	BAUD9600=9600

}UART_BaudRate;


typedef struct{
UART_BitData bit_data;
UART_Parity parity;
UART_StopBit stop_bit;
UART_BaudRate baud_rate;
}UART_ConfigType;




/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void UART_init(const UART_ConfigType *Config_Ptr);

void UART_sendByte(uint8 data);
uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *str);
void UART_receiveString(uint8 *str);

#endif /* UART_H_ */
