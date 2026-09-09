#ifndef	_APP_I2C_H
#define	_APP_I2C_H

#include	"E7602_PG_Application.h"

u8 I2C_Init();
u8 I2C_AdrsScan(u8 *pData);
u8 I2C_Receive(u8 devAdrs, u8 *pData, u16 size);
u8 I2C_Transmit(u8 devAdrs, u8 *pData, u16 size);
u8 I2C_MemRead(u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);
u8 I2C_MemWrite(u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);

#endif	// _APP_I2C_H