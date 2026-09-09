#ifndef	_APP_EXTCTRL_H
#define	_APP_EXTCTRL_H

#include	"E7602_PG_Application.h"

enum{
	EXTCTRL_MEASURE_NONE	= 0,
	EXTCTRL_MEASURE_VOLTAGE_1,
	EXTCTRL_MEASURE_VOLTAGE_2,
	EXTCTRL_MEASURE_VOLTAGE_3,
	EXTCTRL_MEASURE_CURRENT_1,
	EXTCTRL_MEASURE_CURRENT_2,
	EXTCTRL_MEASURE_CURRENT_3,
	EXTCTRL_MEASURE_CURRENT_4,
	EXTCTRL_MEASURE_END
};

enum{
	EXTCTRL_FACTOR_VALUE_START					= 0,
	EXTCTRL_FACTOR_VALUE_MAGNIFICATION_VOLTAGE,
	EXTCTRL_FACTOR_VALUE_MAGNIFICATION_CURRENT,
	EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_1,
	EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_2,
	EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_3,
	EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_4,
	EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_1,
	EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_2,
	EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_3,
	EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_4,
	EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_1,
	EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_2,
	EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_3,
	EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_4,
	EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_1,
	EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_2,
	EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_3,
	EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_4,
	EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_1,
	EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_2,
	EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_3,
	EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_4,
	EXTCTRL_FACTOR_END
};

typedef	union{
	u8	rawData[128];

	struct{
		u32		type;			//	4
		u32		version;		//	4

		float	vMag;			//	4
		float	iMag;			//	4

		float	vGain[4];		//	16
		float	vOffset[4];		//	16

		float	iShunt[4];		//	16
		float	iGain[4];		//	16
		float	iOffset[4];		//	16

		u8		dummy[32];
	};
}extctrlFactor_t;

u8 ExtCtrl_FactorSave(u8 channel);
u8 ExtCtrl_FactorLoad(u8 channel);
u8 ExtCtrl_FactorInit(u8 channel);
u8 ExtCtrl_FactorValueChange(u8 channel, u8 position, float value);
u8 ExtCtrl_FactorValueRead(u8 channel, u8 position, float *pData);
u8 ExtCtrl_SetAdcConfig(u8 bChannel, u8 channel);
u8 ExtCtrl_StartADC(u8 bChannel);
u8 ExtCtrl_GetReady(u8 bChannel, u8 *pState);
u8 ExtCtrl_GetData(u8 bChannel, s16 *pBuffer);
u8 ExtCtrl_GetAdcVoltage(u8 bChannel, u8 channel, float *pVoltage);
u8 ExtCtrl_MuxInit(u8 bChannel);
u8 ExtCtrl_MuxCtrl(u8 bChannel, u8 mode);
u8 ExtCtrl_CalibrationApply(u8 bChannel, u8 position, float *pIdeal, float *pReal);
u8 ExtCtrl_MeasureVoltage(u8 bChannel, u8 position, float *pVoltage);
u8 ExtCtrl_MeasureCurrent(u8 bChannel, u8 position, float *pCurrent);

#endif	// _APP_EXTCTRL_H
