 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the AVR UART driver
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#include <avr/io.h>
#include "uart.h"
#include "common_macros.h"

void UART_init(const UART_ConfigType *Config_Ptr)
{
	uint16 ubrr_value;

	UCSRA = (1<<U2X);

	UCSRB= (1<<TXEN) | (1<<RXEN)

		 | ( GET_BIT((Config_Ptr->bit_data) ,2 ) <<UCSZ2);

	UCSRC= (1<<URSEL)

		   | ( GET_BIT((Config_Ptr->bit_data) ,0 ) <<UCSZ0) | ( GET_BIT((Config_Ptr->bit_data) ,1 ) <<UCSZ1)

		   | (GET_BIT((Config_Ptr->parity) , 0)<< UPM0)
		   | (GET_BIT((Config_Ptr->parity) , 1)<< UPM1)

	       | ((Config_Ptr->stop_bit) << USBS);


	ubrr_value=(uint16) ((F_CPU / ((Config_Ptr->baud_rate)* 8UL)) -1);

	UBRRH =ubrr_value>>8;
	UBRRL =ubrr_value;
}


void UART_sendByte(uint8 data)
{
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	UDR=data;
}


uint8 UART_recieveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	return UDR;
}


void UART_sendString(const uint8 *str)
{
	while(*str != '\0')
	{
		UART_sendByte(*str);
		str++;
	}
}


void UART_receiveString(uint8 *str)
{
	uint8 i=0;

	str[i]=UART_recieveByte();

	while(str[i] != '#')
	{
		i++;
		str[i]=UART_recieveByte();
	}
	str[i]='\0';
}
