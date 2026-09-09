#ifndef	_SYSTEM_CONVERTER_H
#define	_SYSTEM_CONVERTER_H

#include	"UserDefine.h"

u16	SYS_HexToHWord(u8 *pData);
u32	SYS_HexToWord(u8 *pData);
float SYS_HexToFloat(u8 *pData);
double SYS_HexToDouble(u8 *pData);
void SYS_HWordToHex(u16 hWord, u8 *pData);
void SYS_WordToHex(u32 word, u8 *pData);
void SYS_FloatToHex(float fData, u8 *pData);
void SYS_DoubleToHex(double dData, u8 *pData);

#endif	// _SYSTEM_CONVERTER_H