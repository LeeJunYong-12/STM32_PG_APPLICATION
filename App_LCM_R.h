#ifndef	_APP_LCM_R_H
#define	_APP_LCM_R_H

#include	"E7602_PG_Application.h"

u8 LCM_R_SpiInit(void *pVoid);
u8 LCM_R_SpiDeInit();
u8 LCM_R_SpiCsCtrl(u8 position, u8 state);
u8 LCM_R_SpiTransmitData(u8 *pData, u16 size);
u8 LCM_R_SpiReceiveData(u8 *pData, u16 size);
u8 LCM_R_SpiTransmitReceiveData(u8 *pTxData, u8 *pRxData, u16 size);
u8 LCM_R_SpiTransmitDataCustom(u8 position, u8 *pData, u16 size);
u8 LCM_R_SpiReceiveDataCustom(u8 position, u8 *pTxData, u8 *pRxData, u16 txSize, u16 rxSize);
u8 LCM_R_SpiTransmitReceiveDataCustom(u8 position, u8 *pTxData, u8 *pRxData, u16 size);
u8 LCM_R_I2CInit();
u8 LCM_R_I2CDeInit();
u8 LCM_R_I2CGetReady(u8 position, u8 channel, u8 slaveAdrs, u8 *pReady);
u8 LCM_R_I2CGetSlaveAdrs(u8 position, u8 channel, u8 *pData, u8 *pCnt);
u8 LCM_R_I2CTransmitData(u8 position, u8 channel, u8 slaveAdrs, u8 *pData, u16 size);
u8 LCM_R_I2CReceiveData(u8 position, u8 channel, u8 slaveAdrs, u8 *pData, u16 size);
u8 LCM_R_I2CMemRead(u8 position, u8 channel, u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);

#endif	// _APP_LCM_R_H