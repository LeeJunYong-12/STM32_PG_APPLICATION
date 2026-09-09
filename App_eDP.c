#include	"App_eDP.h"

eDP_Handle_t	eDP_Handle;

u8 eDP_I2CAdrsScan(u8 *pAdrs, u8 *pCnt)
{
	u8	result = 0;
	u8	readCnt;

	readCnt = ExtI2C_AdrsScan(0, pAdrs);
	if(readCnt != 0)
	{
		*pCnt = readCnt;
	}
	else	*pCnt = 0;

	result = 1;

	return result;
}

u8 eDP_I2CWrite(u8 slaveAdrs, u8 *pData, u16 size)
{
	return ExtI2C_TransmitData(0, slaveAdrs, pData, size);
}

u8 eDP_I2CRead(u8 slaveAdrs, u8 *pData, u16 size)
{
	return ExtI2C_ReceiveData(0, slaveAdrs, pData, size);
}

u8 eDP_I2CMemWrite(u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size)
{
	return ExtI2C_MemWrite(0, slaveAdrs, memAdrs, adrsMode, pData, size);
}

u8 eDP_I2CMemRead(u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size)
{
	return ExtI2C_MemRead(0, slaveAdrs, memAdrs, adrsMode, pData, size);
}

u8 eDP_RegWrite(u8 regAdrs, u8 *pData, u16 size)
{
	return eDP_I2CMemWrite(0x30, regAdrs, 0, pData, size);
}

u8 eDP_RegRead(u8 regAdrs, u8 *pData, u16 size)
{
	return eDP_I2CMemRead(0x30, regAdrs, 0, pData, size);
}

