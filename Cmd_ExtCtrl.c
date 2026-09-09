#include	"Cmd_ExtCtrl.h"

static u32	extctrlCalDisable[BOARD_CHANNEL_MAX_COUNT];

static u8 Cmd_ExtCtrl_FactorInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = ExtCtrl_FactorInit(pData->position);

	return result;
}

static u8 Cmd_ExtCtrl_FactorSave(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = ExtCtrl_FactorSave(pData->position);

	return result;
}

static u8 Cmd_ExtCtrl_FactorLoad(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = ExtCtrl_FactorLoad(pData->position);

	return result;
}

static u8 Cmd_ExtCtrl_SetFactorData(void *pVoid)
{
	u8	result = 0;
	u8	position;
	float	value;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	if(CMD_Compare(pData->pArgv[0], "vmag"))			position = EXTCTRL_FACTOR_VALUE_MAGNIFICATION_VOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "imag"))		position = EXTCTRL_FACTOR_VALUE_MAGNIFICATION_CURRENT;
	else if(CMD_Compare(pData->pArgv[0], "shunt1"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_1;
	else if(CMD_Compare(pData->pArgv[0], "shunt2"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_2;
	else if(CMD_Compare(pData->pArgv[0], "shunt3"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_3;
	else if(CMD_Compare(pData->pArgv[0], "shunt4"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_4;
	else												return result;

	value = (float)CMD_AToF(pData->pArgv[1]);

	result = ExtCtrl_FactorValueChange(pData->position, position, value);

	return result;
}

static u8 Cmd_ExtCtrl_GetFactorData(void *pVoid)
{
	u8	result = 0;
	u8	position;
	float	value;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "vmag"))			position = EXTCTRL_FACTOR_VALUE_MAGNIFICATION_VOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "imag"))		position = EXTCTRL_FACTOR_VALUE_MAGNIFICATION_CURRENT;
	else if(CMD_Compare(pData->pArgv[0], "shunt1"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_1;
	else if(CMD_Compare(pData->pArgv[0], "shunt2"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_2;
	else if(CMD_Compare(pData->pArgv[0], "shunt3"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_3;
	else if(CMD_Compare(pData->pArgv[0], "shunt4"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_4;
	else if(CMD_Compare(pData->pArgv[0], "vgain1"))		position = EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_1;
	else if(CMD_Compare(pData->pArgv[0], "vgain2"))		position = EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_2;
	else if(CMD_Compare(pData->pArgv[0], "vgain3"))		position = EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_3;
	else if(CMD_Compare(pData->pArgv[0], "vgain4"))		position = EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_4;
	else if(CMD_Compare(pData->pArgv[0], "voffset1"))	position = EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_1;
	else if(CMD_Compare(pData->pArgv[0], "voffset2"))	position = EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_2;
	else if(CMD_Compare(pData->pArgv[0], "voffset3"))	position = EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_3;
	else if(CMD_Compare(pData->pArgv[0], "voffset4"))	position = EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_4;
	else if(CMD_Compare(pData->pArgv[0], "igain1"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_1;
	else if(CMD_Compare(pData->pArgv[0], "igain2"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_2;
	else if(CMD_Compare(pData->pArgv[0], "igain3"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_3;
	else if(CMD_Compare(pData->pArgv[0], "igain4"))		position = EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_4;
	else if(CMD_Compare(pData->pArgv[0], "ioffset1"))	position = EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_1;
	else if(CMD_Compare(pData->pArgv[0], "ioffset2"))	position = EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_2;
	else if(CMD_Compare(pData->pArgv[0], "ioffset3"))	position = EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_3;
	else if(CMD_Compare(pData->pArgv[0], "ioffset4"))	position = EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_4;
	else												return result;

	result = ExtCtrl_FactorValueRead(pData->position, position, &value);
	if(result)
	{
		CMD_Printf(";CH%d Value=%f", pData->channel, value);
	}

	return result;
}

static u8 Cmd_ExtCtrl_CalEnable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	extctrlCalDisable[pData->position] = 0;

	result = 1;

	return result;
}

static u8 Cmd_ExtCtrl_CalDisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	extctrlCalDisable[pData->position] = 1;

	result = 1;

	return result;
}

static u8 Cmd_ExtCtrl_CalVoltageInput(void *pVoid)
{
	u8	result = 0;
	u8	position;
	float	ideal[2], real[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)		return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))				position = EXTCTRL_MEASURE_VOLTAGE_1;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))		position = EXTCTRL_MEASURE_VOLTAGE_2;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))		position = EXTCTRL_MEASURE_VOLTAGE_3;
	else												return result;

	ideal[0]	= CMD_AToF(pData->pArgv[1]);
	real[0]		= CMD_AToF(pData->pArgv[2]);
	ideal[1]	= CMD_AToF(pData->pArgv[3]);
	real[1]		= CMD_AToF(pData->pArgv[4]);

	result = ExtCtrl_CalibrationApply(pData->position, position, ideal, real);

	return result;
}

static u8 Cmd_ExtCtrl_CalCurrentInput(void *pVoid)
{
	u8	result = 0;
	u8	position;
	float	ideal[2], real[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)		return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))				position = EXTCTRL_MEASURE_CURRENT_1;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))		position = EXTCTRL_MEASURE_CURRENT_2;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))		position = EXTCTRL_MEASURE_CURRENT_3;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))		position = EXTCTRL_MEASURE_CURRENT_4;
	else												return result;

	ideal[0]	= CMD_AToF(pData->pArgv[1]);
	real[0]		= CMD_AToF(pData->pArgv[2]);
	ideal[1]	= CMD_AToF(pData->pArgv[3]);
	real[1]		= CMD_AToF(pData->pArgv[4]);

	result = ExtCtrl_CalibrationApply(pData->position, position, ideal, real);

	return result;
}

static u8 Cmd_ExtCtrl_MeasureVoltage(void *pVoid)
{
	u8	result = 0;
	u8	position;
	float	voltageData[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))				position = EXTCTRL_MEASURE_VOLTAGE_1;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))		position = EXTCTRL_MEASURE_VOLTAGE_2;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))		position = EXTCTRL_MEASURE_VOLTAGE_3;
	else												return result;

	result = ExtCtrl_MeasureVoltage(pData->position, position, voltageData);
	if(result)
	{
		if(extctrlCalDisable[pData->position])			CMD_Printf(";CH%d %.03fV", pData->channel, voltageData[0]);
		else											CMD_Printf(";CH%d %.03fV", pData->channel, voltageData[1]);
	}

	return result;
}

static u8 Cmd_ExtCtrl_MeasureCurrent(void *pVoid)
{
	u8	result = 0;
	u8	position;
	float	currentData[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))				position = EXTCTRL_MEASURE_CURRENT_1;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))		position = EXTCTRL_MEASURE_CURRENT_2;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))		position = EXTCTRL_MEASURE_CURRENT_3;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))		position = EXTCTRL_MEASURE_CURRENT_4;
	else												return result;

	result = ExtCtrl_MeasureCurrent(pData->position, position, currentData);
	if(result)
	{
		if(extctrlCalDisable[pData->position])			CMD_Printf(";CH%d %.06fmA", pData->channel, currentData[0]);
		else											CMD_Printf(";CH%d %.06fmA", pData->channel, currentData[1]);
	}

	return result;
}

static u8 Cmd_ExtCtrl_AdcVoltage(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	float	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	channel = CMD_StrToUL(pData->pArgv[0]);
	if(channel >= 4)			return result;

	result = ExtCtrl_GetAdcVoltage(pData->position, channel, &voltage);
	if(result)
	{
		CMD_Printf(";CH%d %.06fV", voltage);
	}

	return result;
}

const	commandList_t	cmdList_ExtCtrl[] =	{
//												{	Command,				Function,							Option,		Mode,		RootMessage,			Note}
												{	"help",					Cmd_Help,							NULL,		0x01,		";HELP",				NULL},
												{	"factor.init",			Cmd_ExtCtrl_FactorInit,				NULL,		NULL,		";FACTOR.INIT",			NULL},
												{	"factor.save",			Cmd_ExtCtrl_FactorSave,				NULL,		NULL,		";FACTOR.SAVE",			NULL},
												{	"factor.load",			Cmd_ExtCtrl_FactorLoad,				NULL,		NULL,		";FACTOR.LOAD",			NULL},
												{	"factor.set",			Cmd_ExtCtrl_SetFactorData,			NULL,		NULL,		";FACTOR.SET",			NULL},
												{	"factor.get",			Cmd_ExtCtrl_GetFactorData,			NULL,		NULL,		";FACTOR.GET",			NULL},
												{	"cal.enable",			Cmd_ExtCtrl_CalEnable,				NULL,		NULL,		";CAL.ENABLE",			NULL},
												{	"cal.disable",			Cmd_ExtCtrl_CalDisable,				NULL,		NULL,		";CAL.DISABLE",			NULL},
												{	"cal.vin",				Cmd_ExtCtrl_CalVoltageInput,		NULL,		NULL,		";CAL.VIN",				NULL},
												{	"cal.cin",				Cmd_ExtCtrl_CalCurrentInput,		NULL,		NULL,		";CAL.CIN",				NULL},
												{	"measure.voltage",		Cmd_ExtCtrl_MeasureVoltage,			NULL,		NULL,		";MEASURE.VOLTAGE",		NULL},
												{	"measure.current",		Cmd_ExtCtrl_MeasureCurrent,			NULL,		NULL,		";MEASURE.CURRENT",		NULL},
												{	"adc.voltage",			Cmd_ExtCtrl_AdcVoltage,				NULL,		NULL,		";ADC.VOLTAGE",			NULL},
												{	NULL,					NULL,								NULL,		NULL,		NULL,					NULL},
											};

