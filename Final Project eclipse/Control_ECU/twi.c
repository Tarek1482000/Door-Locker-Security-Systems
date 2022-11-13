/******************************************************************************
 *
 * Module: I2C
 *
 * File Name: twi.c
 *
 * Description: I2C communication protocol for AVR
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>


void TWI_init(const TWI_ConfigType *Config_Ptr)
{
	/* Bit Rate: depend on user input using zero pre-scaler TWPS=00  F_CPU=8MHZ*/
	TWBR = Config_Ptr->bit_rate;
	TWSR = 0x00;

	/* Two Wire Bus address my address if any master device want to call me: 0x1 (used in case this MC is a slave device)
       General Call Recognition: Off */
	TWAR = ((Config_Ptr->address)<<1); /* my address  + 0 (Call Recognition: Off)*/

	TWCR = (1<<TWEN); /* enable TWI */
}



void TWI_start(void)
{
	/* 
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR =(1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	/* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}



void TWI_stop(void)
{
	/* 
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR =(1<<TWINT) | (1<<TWSTO) | (1<<TWEN);

}




void TWI_writeByte(uint8 data)
{
	/* Put data On TWI data Register */
	TWDR=data;
	/* 
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
	TWCR =(1<<TWINT) | (1<<TWEN);
	/* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));

}



uint8 TWI_readBiteWithACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
	TWCR =(1<<TWINT) |(1<<TWEN) |(1<<TWEA);

	while(BIT_IS_CLEAR(TWCR,TWINT));

	return TWDR;
}



uint8 TWI_readByteWithNACK(void)
{
	TWCR =(1<<TWINT) |(1<<TWEN) ;

	while(BIT_IS_CLEAR(TWCR,TWINT));

	return TWDR;
}




uint8 TWI_getStatus(void)
{
	uint8 status=0;

	status = TWSR & 0xF8;

	return status;
}




