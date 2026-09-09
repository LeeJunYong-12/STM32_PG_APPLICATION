#ifndef	_APP_EDP_H
#define	_APP_EDP_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u32	coreType;
}eDP_Handle_t;

enum{
	EDP_HW_TYPE_NONE	= 0,
	EDP_HW_TYPE_CORE_MC4,
	EDP_HW_TYPE_END,
};

extern	eDP_Handle_t	eDP_Handle;

u8 eDP_I2CAdrsScan(u8 *pAdrs, u8 *pCnt);
u8 eDP_I2CWrite(u8 slaveAdrs, u8 *pData, u16 size);
u8 eDP_I2CRead(u8 slaveAdrs, u8 *pData, u16 size);
u8 eDP_I2CMemWrite(u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);
u8 eDP_I2CMemRead(u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size);
u8 eDP_RegWrite(u8 regAdrs, u8 *pData, u16 size);
u8 eDP_RegRead(u8 regAdrs, u8 *pData, u16 size);

#endif	// _APP_EDP_H