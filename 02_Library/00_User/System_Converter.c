#include	"System_Converter.h"

u16	SYS_HexToHWord(u8 *pData)
{
	bitCtrl16_t data;

	data.u8Data[0] = pData[0];
	data.u8Data[1] = pData[1];

	return data.u16Data;
}

u32	SYS_HexToWord(u8 *pData)
{
	bitCtrl32_t data;

	data.u8Data[0] = pData[0];
	data.u8Data[1] = pData[1];
	data.u8Data[2] = pData[2];
	data.u8Data[3] = pData[3];

	return data.u32Data;
}

float SYS_HexToFloat(u8 *pData)
{
	floatCtrl_t	data;

	data.u8Data[0] = pData[0];
	data.u8Data[1] = pData[1];
	data.u8Data[2] = pData[2];
	data.u8Data[3] = pData[3];

	return data.fData[0];
}

double SYS_HexToDouble(u8 *pData)
{
	floatCtrl_t	data;

	data.u8Data[0] = pData[0];
	data.u8Data[1] = pData[1];
	data.u8Data[2] = pData[2];
	data.u8Data[3] = pData[3];
	data.u8Data[4] = pData[4];
	data.u8Data[5] = pData[5];
	data.u8Data[6] = pData[6];
	data.u8Data[7] = pData[7];

	return data.dData;
}

void SYS_HWordToHex(u16 hWord, u8 *pData)
{
	bitCtrl16_t data;

	data.u16Data = hWord;

	pData[0] = data.u8Data[0];
	pData[1] = data.u8Data[1];
}

void SYS_WordToHex(u32 word, u8 *pData)
{
	bitCtrl32_t data;

	data.u32Data = word;

	pData[0] = data.u8Data[0];
	pData[1] = data.u8Data[1];
	pData[2] = data.u8Data[2];
	pData[3] = data.u8Data[3];
}

void SYS_FloatToHex(float fData, u8 *pData)
{
	floatCtrl_t data;

	data.fData[0] = fData;

	pData[0] = data.u8Data[0];
	pData[1] = data.u8Data[1];
	pData[2] = data.u8Data[2];
	pData[3] = data.u8Data[3];
}

void SYS_DoubleToHex(double dData, u8 *pData)
{
	floatCtrl_t data;

	data.dData = dData;

	pData[0] = data.u8Data[0];
	pData[1] = data.u8Data[1];
	pData[2] = data.u8Data[2];
	pData[3] = data.u8Data[3];
	pData[4] = data.u8Data[4];
	pData[5] = data.u8Data[5];
	pData[6] = data.u8Data[6];
	pData[7] = data.u8Data[7];
}