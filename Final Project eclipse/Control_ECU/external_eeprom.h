 /******************************************************************************
 *
 * Module:EEBROM header file
 *
 * File Name: external_eeprom.h
 *
 * Description: External EEBROM for AVR
 *
 * Author: Tarek Ahmed
 *
 *******************************************************************************/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define SUCCESS    1
#define ERROR      0


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/


uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data);

uint8 EEPROM_readByte(uint16 address , uint8 *data);


#endif /* EXTERNAL_EEPROM_H_ */
