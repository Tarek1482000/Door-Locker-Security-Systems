/******************************************************************************
 *
 * Module: I2C
 *
 * File Name: twi.h
 *
 * Description: I2C communication protocol for AVR
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define TWI_START             0X08
#define TWI_REP_START         0X10
#define TWI_MT_SLA_W_ACK      0X18
#define TWI_MT_S_W_NACK       0X20
#define TWI_MT_DATA_ACK       0X28
#define TWI_MT_DATA_NACK      0X30
#define TWI_MT_SLA_R_ACK      0X40
#define TWI_MR_S_W_NACK       0X48
#define TWI_MR_DATA_ACK       0X50
#define TWI_MR_DATA_NACK      0X58




/*******************************************************************************
 *                             Types Declaration                                  *
 *******************************************************************************/

typedef enum
{
	FIRST=0x01 , SECOND=0x02 , THIRD=0x03, FORTH=0x04
}TWI_Address;



typedef enum
{
	FOUR_HAUNDRED_KBITS_S=0X02 , ONE_HAUNDRED_KBITS_S=0X20 /* 32 */
}TWI_BaudRate;


typedef struct
{
	TWI_Address address;
	TWI_BaudRate bit_rate;
}TWI_ConfigType;



/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void TWI_init(const TWI_ConfigType *Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readBiteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);

#endif /* TWI_H_ */
