#ifndef	_APP_SERIAL_H
#define	_APP_SERIAL_H

#include	"E7602_PG_Application.h"

u8 Serial_Init();
u8 Serial_GetChar(u8 *pData);
u8 Serial_TransmitData(u8 *pData, u32 size);
u8 Serial_PacketProcess();

#endif	// _APP_SERIAL_H