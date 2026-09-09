#ifndef	_APP_LCM_H
#define	_APP_LCM_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u32	baudrate;
	u32	polarity;
	u32	phase;
}lcmSpiOption_t;

u8 LCM_SpiInit(u8 position, void *pVoid);
u8 LCM_SpiDeInit(u8 position);
u8 LCM_SpiCsCtrl(u8 position, u8 state);
u8 LCM_SpiTransmitData(u8 position, u8 *pData, u16 size);
u8 LCM_SpiReceiveData(u8 position, u8 *pData, u16 size);
u8 LCM_SpiTransmitReceiveData(u8 position, u8 *pTxData, u8 *pRxData, u16 size);
u8 LCM_SpiTransmitDataCustom(u8 position, u8 *pData, u16 size);
u8 LCM_SpiReceiveDataCustom(u8 position, u8 *pTxData, u8 *pRxData, u16 txSize, u16 rxSize);
u8 LCM_SpiTransmitReceiveDataCustom(u8 position, u8 *pTxData, u8 *pRxData, u16 size);
u8 LCM_SpiIoWrite(u8 position, u8 state);
u8 LCM_SpiIoRead(u8 position, u8 *pState);
u8 LCM_I2CInit(u8 position, u8 channel);
u8 LCM_I2CDeInit(u8 position, u8 channel);
u8 LCM_I2CGetReady(u8 position, u8 channel, u8 slaveAdrs, u8 *pReady);
u8 LCM_I2CGetSlaveAdrs(u8 position, u8 channel, u8 *pData, u8 *pCnt);
u8 LCM_I2CTransmitData(u8 position, u8 channel, u8 slaveAdrs, u8 *pData, u16 size);
u8 LCM_I2CReceiveData(u8 position, u8 channel, u8 slaveAdrs, u8 *pData, u16 size);
u8 LCM_I2CMemWrite(u8 position, u8 channel, u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);
u8 LCM_I2CMemRead(u8 position, u8 channel, u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);
u8 LCM_I2CIoDirection(u8 position, u8 channel, u8 state);
u8 LCM_I2CIoWrite(u8 position, u8 channel, u8 state);
u8 LCM_I2CIoRead(u8 position, u8 channel, u8 *pState);

#endif	// _APP_LCM_H
