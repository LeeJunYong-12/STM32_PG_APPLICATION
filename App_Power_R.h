#ifndef	_APP_POWER_R_H
#define	_APP_POWER_R_H

#include	"E7602_PG_Application.h"

u8 PWR_R_Version(u8 *pData);
u8 PWR_R_BuildDate(u8 *pData);
u8 PWR_R_FirmwareType(u8 *pData);
u8 PWR_R_Reboot();
u8 PWR_R_OutputInit(u8 type);
u8 PWR_R_OutputVoltage(u8 pwrSrc, double voltage);
u8 PWR_R_OutputCurrent(u8 pwrSrc, double current);
u8 PWR_R_MeasureRange(u8 pwrSrc, u8 range);
u8 PWR_R_OutputEnable(u8 position, u8 pwrSrc, u8 enable);
u8 PWR_R_MesureVI(u8 pwrSrc, double *pVoltage, double *pCurrent);
u8 PWR_R_ExtVoltageMeasure(u8 mode, double *pVoltage);

#endif	// _APP_POWER_R_H