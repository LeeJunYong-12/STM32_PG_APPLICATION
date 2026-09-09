#ifndef	_CP_SOURCE_H
#define	_CP_SOURCE_H

#include	"ConfigPacket.h"

u8 CP_RegisterInterface(void *pVoid);
u16 CP_GetDataSize();
u8 CP_WritePacket(u8 *pData, u16 size);
u8 CP_ReadPacket(u8 *pData, u16 size);
u8 CP_VerifyPacket(u8 *pData, u16 size);

#endif	//	_CP_SOURCE_H