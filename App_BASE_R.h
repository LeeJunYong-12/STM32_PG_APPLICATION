#ifndef	_APP_BASE_R_H
#define	_APP_BASE_R_H

#include	"E7602_PG_Application.h"

u8 BASE_R_GetVersion(u8 *pData);
u8 BASE_R_GetBuildDate(u8 *pData);
u8 BASE_R_eDPRedriverSetPwrDown(u8 position, u8 mode);
u8 BASE_R_eDPRedriverSetSync(u8 position, u8 type, u8 mode);
u8 BASE_R_eDPRedriverSetEqMode(u8 position, u8 mode);
u8 BASE_R_eDPRedriverSetChannelEnable(u8 position, u8 mode);
u8 BASE_R_eDPRedriverSetRxGain(u8 position, u8 type, u8 mode);
u8 BASE_R_eDPRedriverSetEqGain(u8 position, u8 type, u8 mode);
u8 BASE_R_eDPRedriverSetTxGain(u8 position, u8 type, u8 mode);
u8 BASE_R_eDPRedriverSetEqValue(u8 position, u8 type, u8 mode);
u8 BASE_R_eDPRedriverSetDriveEnable(u8 position, u8 type, u8 mode);
u8 BASE_R_eDPRedriverSetEqEnable(u8 position, u8 type, u8 mode);
u8 BASE_R_eDPRedriverSetDrivePeak(u8 position, u8 type, u8 mode);

#endif	// _APP_BASE_R_H