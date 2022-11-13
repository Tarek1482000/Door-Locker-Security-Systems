/******************************************************************************
 *
 * Module: External EEBROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEBROM for AVR
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/


#include "external_eeprom.h"

#include "twi.h"

uint8 EEPROM_writeByte(uint16 u16address , uint8 data)
{
	/* Send the Start Bit */
	TWI_start();
	if(TWI_getStatus()!= TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8) ((u16address & 0x0700)>>7) | 0xA0);
	if(TWI_getStatus()!= TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8)u16address);
	if(TWI_getStatus()!= TWI_MT_DATA_ACK)
		return ERROR;

	   /* write byte to eeprom */
	TWI_writeByte(data);
	if(TWI_getStatus()!= TWI_MT_DATA_ACK)
		return ERROR;

	  /* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}




uint8 EEPROM_readByte(uint16 u16address , uint8 *data)
{
	TWI_start();
	if(TWI_getStatus()!= TWI_START)
		return ERROR;

	TWI_writeByte((uint8) ((u16address & 0x0700)>>7) | 0xA0);
	if(TWI_getStatus()!= TWI_MT_SLA_W_ACK)
		return ERROR;

	TWI_writeByte((uint8)u16address);
	if(TWI_getStatus()!= TWI_MT_DATA_ACK)
		return ERROR;

	  /* Send the Repeated Start Bit */
	TWI_start();
	if(TWI_getStatus()!= TWI_REP_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	     * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8) ((u16address & 0x0700)>>7) | 0xA0 | 1);
	if(TWI_getStatus()!= TWI_MT_SLA_R_ACK)
		return ERROR;

    /* Read Byte from Memory without send ACK */
	*data=TWI_readByteWithNACK();
	if(TWI_getStatus()!= TWI_MR_DATA_NACK)
		return ERROR;

	TWI_stop();

	return SUCCESS;
}
