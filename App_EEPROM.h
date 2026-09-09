#ifndef	_APP_EEPROM_H
#define	_APP_EEPROM_H

#include	"E7602_PG_Application.h"

u8 EEPROM_Read(u16 adrs, u8 *pData, u16 size);
u8 EEPROM_Write(u16 adrs, u8 *pData, u16 size);

#endif	// _APP_EEPROM_H