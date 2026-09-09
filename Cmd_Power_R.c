#include	"Cmd_Power_R.h"

static u8 Cmd_PowerVersion(void *pVoid)
{
	u8	result = 0;
	u8	buffer[32];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	memset(buffer, NULL, sizeof(buffer));

	result = PWR_R_Version(buffer);

	if(result)
	{
		CMD_Printf(";%s", buffer);
	}

	return result;
}

static u8 Cmd_PowerBuild(void *pVoid)
{
	u8	result = 0;
	u8	buffer[32];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	memset(buffer, NULL, sizeof(buffer));

	result = PWR_R_BuildDate(buffer);

	if(result)
	{
		CMD_Printf(";%s", buffer);
	}

	return result;
}

static u8 Cmd_PowerType(void *pVoid)
{
	u8	result = 0;
	u8	buffer[32];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	memset(buffer, NULL, sizeof(buffer));

	result = PWR_R_FirmwareType(buffer);

	if(result)
	{
		CMD_Printf(";%s", buffer);
	}

	return result;
}

static u8 Cmd_PowerReset(void * pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	result = PWR_R_Reboot();

	return result;
}

static u8 Cmd_PowerInit(void * pVoid)
{
	u8	result = 0;
	u8	type;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "all"))
	{
		type = PWR_INIT_TYPE_ALL;
	}
	else if(CMD_Compare(pData->pArgv[0], "vm"))
	{
		type = PWR_INIT_TYPE_VM;
	}
	else if(CMD_Compare(pData->pArgv[0], "gpio"))
	{
		type = PWR_INIT_TYPE_IOVOLTAGE;
	}
	else						return result;

	result = PWR_R_OutputInit(type);

	return result;
}

static u8 Cmd_PowerSetVoltage(void * pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)						return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else						return result;

	voltage = CMD_AToF(pData->pArgv[1]);

	result = PWR_R_OutputVoltage(pwrSrc, voltage);

	return result;
}

static u8 Cmd_PowerSetCurrent(void * pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	current;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)						return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else						return result;

	current = CMD_AToF(pData->pArgv[1]);

	result = PWR_R_OutputCurrent(pwrSrc, current);

	return result;
}

static u8 Cmd_PowerSetRange(void * pVoid)
{
	u8	result = 0;
	u8	pwrSrc, range;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else						return result;

	range = CMD_StrToUL(pData->pArgv[1]);

	result = PWR_R_MeasureRange(pwrSrc, range);

	return result;
}

static u8 Cmd_PowerEnable(void * pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	pwrChannel, pwrSrc[8], cnt, chCnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 2)		return result;
	if(pData->argc > 7)		return result;

	chCnt = pData->argc - 1;

	if(CMD_Compare(pData->pArgv[0], "ch1"))						pwrChannel = 0x01;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))				pwrChannel = 0x02;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))				pwrChannel = 0x04;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))				pwrChannel = 0x08;
	else if(CMD_Compare(pData->pArgv[0], "all"))				pwrChannel = 0x0F;
	else														return result;

	for(cnt = 0; cnt < chCnt; cnt++)
	{
		if(CMD_Compare(pData->pArgv[cnt + 1], "vm0"))			pwrSrc[cnt] = PWR_SRC_VM_0;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm1"))		pwrSrc[cnt] = PWR_SRC_VM_1;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm2"))		pwrSrc[cnt] = PWR_SRC_VM_2;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm3"))		pwrSrc[cnt] = PWR_SRC_VM_3;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm4"))		pwrSrc[cnt] = PWR_SRC_VM_4;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm5"))		pwrSrc[cnt] = PWR_SRC_VM_5;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "gpio"))		pwrSrc[cnt] = PWR_SRC_IOVOLTAGE;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vio"))		pwrSrc[cnt] = PWR_SRC_IOVOLTAGE;
		else													return result;
	}

	for(cnt = 0; cnt < chCnt; cnt++)
	{
		rtn = PWR_R_OutputEnable(pwrChannel, pwrSrc[cnt], 1);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)			result = 1;

	return result;
}

static u8 Cmd_PowerDisable(void * pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	pwrChannel, pwrSrc[8], cnt, chCnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 2)		return result;
	if(pData->argc > 7)		return result;

	chCnt = pData->argc - 1;

	if(CMD_Compare(pData->pArgv[0], "ch1"))						pwrChannel = 0x01;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))				pwrChannel = 0x02;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))				pwrChannel = 0x04;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))				pwrChannel = 0x08;
	else if(CMD_Compare(pData->pArgv[0], "all"))				pwrChannel = 0x0F;
	else														return result;

	for(cnt = 0; cnt < chCnt; cnt++)
	{
		if(CMD_Compare(pData->pArgv[cnt + 1], "vm0"))			pwrSrc[cnt] = PWR_SRC_VM_0;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm1"))		pwrSrc[cnt] = PWR_SRC_VM_1;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm2"))		pwrSrc[cnt] = PWR_SRC_VM_2;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm3"))		pwrSrc[cnt] = PWR_SRC_VM_3;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm4"))		pwrSrc[cnt] = PWR_SRC_VM_4;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vm5"))		pwrSrc[cnt] = PWR_SRC_VM_5;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "gpio"))		pwrSrc[cnt] = PWR_SRC_IOVOLTAGE;
		else if(CMD_Compare(pData->pArgv[cnt + 1], "vio"))		pwrSrc[cnt] = PWR_SRC_IOVOLTAGE;
		else													return result;
	}

	for(cnt = 0; cnt < chCnt; cnt++)
	{
		rtn = PWR_R_OutputEnable(pwrChannel, pwrSrc[cnt], 0);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)			result = 1;

	return result;
}

static u8 Cmd_PowerMesurement(void * pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	voltage[4], current[4];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else												return result;

	result = PWR_R_MesureVI(pwrSrc, voltage, current);

	if(result)
	{
		CMD_Printf(";CH1 VM%d %fV %fmA", pwrSrc, voltage[0], current[0]);
		CMD_Printf(";CH2 VM%d %fV %fmA", pwrSrc, voltage[1], current[1]);
		CMD_Printf(";CH3 VM%d %fV %fmA", pwrSrc, voltage[2], current[2]);
		CMD_Printf(";CH4 VM%d %fV %fmA", pwrSrc, voltage[3], current[3]);
	}

	return result;
}

const	commandList_t	cmdList_Power_R[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note},
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"version",				Cmd_PowerVersion,			NULL,		NULL,		";VERSION",				NULL},
												{	"build",				Cmd_PowerBuild,				NULL,		NULL,		";BUILD",				NULL},
												{	"type",					Cmd_PowerType,				NULL,		NULL,		";TYPE",				NULL},
												{	"reset",				Cmd_PowerReset,				NULL,		NULL,		";RESET",				NULL},
												{	"init",					Cmd_PowerInit,				NULL,		NULL,		";INIT",				NULL},
												{	"set.voltage",			Cmd_PowerSetVoltage,		NULL,		NULL,		";SET.VOLTAGE",			NULL},
												{	"set.current",			Cmd_PowerSetCurrent,		NULL,		NULL,		";SET.CURRENT",			NULL},
												{	"set.range",			Cmd_PowerSetRange,			NULL,		NULL,		";SET.RANGE",			NULL},
												{	"range",				Cmd_PowerSetRange,			NULL,		NULL,		";RANGE",				NULL},
												{	"enable",				Cmd_PowerEnable,			NULL,		NULL,		";ENABLE",				NULL},
												{	"disable",				Cmd_PowerDisable,			NULL,		NULL,		";DISABLE",				NULL},
												{	"read",					Cmd_PowerMesurement,		NULL,		NULL,		";READ",				NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};

