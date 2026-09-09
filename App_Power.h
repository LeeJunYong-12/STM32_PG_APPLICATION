#ifndef	_APP_POWER_H
#define	_APP_POWER_H

#include	"E7602_PG_Application.h"

enum{
	PWR_BOARD_TYPE_1	= 0,
	PWR_BOARD_TYPE_2,
	PWR_BOARD_TYPE_END,
};

enum{
	PWR_DATA_INIT_NONE	= 0,
	PWR_DATA_INIT_DEFAULT,
	PWR_DATA_INIT_END,
};

enum{
	PWR_SRC_VM_0		= 0,
	PWR_SRC_VM_1,
	PWR_SRC_VM_2,
	PWR_SRC_VM_3,
	PWR_SRC_VM_4,
	PWR_SRC_VM_5,
	PWR_SRC_VM_6,
	PWR_SRC_IOVOLTAGE,
	PWR_SRC_FV5,
	PWR_SRC_END,
};

enum{
	PWR_VM_TYPE_12V3A		= 0,
	PWR_VM_TYPE_12V5A,
	PWR_VM_TYPE_N10V3A,
	PWR_VM_TYPE_END,
};

enum{
	PWR_VM_STATUS_OFF		= 0,
	PWR_VM_STATUS_ON,
	PWR_VM_STATUS_END,
};

enum{
	PWR_VM_RANGE_0			= 0,
	PWR_VM_RANGE_1,
	PWR_VM_RANGE_2,
	PWR_VM_RANGE_3,
	PWR_VM_RANGE_END,
};

enum{
	PWR_INIT_TYPE_ALL		= 0,
	PWR_INIT_TYPE_VM,
	PWR_INIT_TYPE_IOVOLTAGE,
	PWR_INIT_TYPE_END,
};

enum{
	PWR_MEASURE_MODE_CURRENT		= 0,
	PWR_MEASURE_MODE_VOLTAGE,
	PWR_MEASURE_MODE_END,
};

enum{
	PWR_CALIBRATION_SRC_V_OUT		= 0,
	PWR_CALIBRATION_SRC_V_MEASURE,
	PWR_CALIBRATION_SRC_C_MEASURE_R0,
	PWR_CALIBRATION_SRC_C_MEASURE_R1,
	PWR_CALIBRATION_SRC_C_MEASURE_R2,
	PWR_CALIBRATION_SRC_C_MEASURE_R3,
	PWR_CALIBRATION_SRC_END,
};

enum{
	PWR_FACTOR_SRC_VFB			= 0,
	PWR_FACTOR_SRC_OVR_0,
	PWR_FACTOR_SRC_OVR_1,
	PWR_FACTOR_SRC_OVR_2,
	PWR_FACTOR_SRC_MVR_0,
	PWR_FACTOR_SRC_MVR_1,
	PWR_FACTOR_SRC_MIR_0,
	PWR_FACTOR_SRC_MIR_1,
	PWR_FACTOR_SRC_RDS,
	PWR_FACTOR_SRC_SHUNT_0,
	PWR_FACTOR_SRC_SHUNT_1,
	PWR_FACTOR_SRC_SHUNT_2,
	PWR_FACTOR_SRC_SHUNT_3,
	PWR_FACTOR_SRC_END,
};

u8 PWR_M_GetVersion(u8 position, u8 *pData);
u8 PWR_M_GetBuildDate(u8 position, u8 *pData);

u8 PWR_FirmwareType(u8 position, u8 *pData, u16 *pSize);
u8 PWR_EthernetStatus(u8 position, u8 status);
u8 PWR_EthernetConfig(u8 position, u16 mode, u8 *pMAC, u8 *pIP, u8 *pNM, u8 *pGW, u16 port);
u8 PWR_Reboot(u8 position);
u8 PWR_OutputInit(u8 position, u8 type);
u8 PWR_OutputVoltage(u8 position, u8 pwrSrc, double voltage);
u8 PWR_OutputCurrent(u8 position, u8 pwrSrc, double current);
u8 PWR_MeasureRange(u8 position, u8 pwrSrc, u8 range);
u8 PWR_OutputEnable(u8 position, u8 pwrSrc, u8 enable);
u8 PWR_VfbState(u8 position, u8 state);
u8 PWR_MesureVI(u8 position, u8 pwrSrc, double *pVoltage, double *pCurrent);
u8 PWR_MesureAutoVI(u8 position, u8 pwrSrc, double *pVoltage, double *pCurrent);

u8 PWR_ExtVoltageMeasure(u8 position, u8 mode, double *pVoltage);
u8 PWR_ReadAlarm(u8 position, u8 *valarm, u8 *ialarm);
u8 PWR_SetError(u8 position, u8 error);

u8 PWR_CalState(u8 position, u8 state);
u8 PWR_CalOutputVoltage(u8 position, u8 pwrSrc, double *pIdeal, double *pReal);
u8 PWR_CalMeasureVoltage(u8 position, u8 pwrSrc, double *pIdeal, double *pReal);
u8 PWR_CalMeasureCurrent(u8 position, u8 pwrSrc, u8 src, double *pIdeal, double *pReal);
u8 PWR_CalSetData(u8 position, u8 pwrSrc, u8 src, double gain, double offset);
u8 PWR_CalGetData(u8 position, u8 pwrSrc, u8 src, double *pGain, double *pOffset);
u8 PWR_CalPMICVoltage(u8 position, u8 pwrSrc, double *pIdeal, double *pReal);
u8 PWR_CalPMICCurrent(u8 position, u8 pwrSrc, double *pIdeal, double *pReal);
u8 PWR_FactorSetData(u8 position, u8 pwrSrc, u8	src, double factor);
u8 PWR_FactorGetData(u8 position, u8 pwrSrc, u8 src, double *pFactor);
u8 PWR_DataClear(u8 position);
u8 PWR_DataLoad(u8 position);
u8 PWR_DataSave(u8 position);
u8 PWR_ExtRange(u8 position, double range);
u8 PWR_Vssel(u8 position, double range, double *current);
u8 PWR_Vssel_Voltage(u8 position, double *voltage);
u8 PWR_Vgl(u8 position, double range, double *current);
u8 PWR_Vgl_Voltage(u8 position, double *voltage);
u8 PWR_avddh_current(u8 position, double range, double *current);
u8 PWR_avddh_voltage(u8 position, double *voltage);
u8 PWR_vddel_voltage(u8 position, double *voltage);

u8 PWR_BootEnter(u8 position);
u8 PWR_BootErase(u8 position);
u8 PWR_BOOTCHECK(u8 position, u32 *status);
u8 PWR_EEPROMSET(u8 position, u32 status);

u8 CheckAlarmFlag(u8 position, u8 *valarm, u8 *ialarm, u8 *error);

u8 PWR_ADCcount(u8 position, u8 maxcount, u8 delcount);
#endif	// _APP_POWER_H
