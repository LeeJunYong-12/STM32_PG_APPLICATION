#ifndef	_APP_EXTI2C_H
#define	_APP_EXTI2C_H

#include	"E7602_PG_Application.h"

u8 ExtI2C_IoInit();
u8 ExtI2C_ConnectEEPROM(u8 state);
u8 ExtI2C_I2C0Init();
u8 ExtI2C_I2C0DeInit();
u8 ExtI2C_I2C1Init();
u8 ExtI2C_I2C1DeInit();
u8 ExtI2C_AdrsScan(u8 position, u8 *pAdrs);
u8 ExtI2C_TransmitData(u8 position, u8 devAdrs, u8 *pData, u16 size);
u8 ExtI2C_ReceiveData(u8 position, u8 devAdrs, u8 *pData, u16 size);
u8 ExtI2C_MemWrite(u8 position, u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);
u8 ExtI2C_MemRead(u8 position, u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);

#endif	// _APP_EXTI2C_H