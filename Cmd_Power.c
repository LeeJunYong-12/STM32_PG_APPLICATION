#include	"Cmd_Power.h"

static u8 Cmd_PowerVersion(void *pVoid)
{
	u8	result = 0;
	u8	buffer[32];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	memset(buffer, NULL, sizeof(buffer));

	result = PWR_M_GetVersion(pData->position, buffer);

	if(result)
	{
		LOG_Printf("AAAAA");
		CMD_Printf(";CH%d=%s", pData->channel, buffer);
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

	result = PWR_M_GetBuildDate(pData->position, buffer);

	if(result)
	{
		CMD_Printf(";CH%d=%s", pData->channel, buffer);
	}

	return result;
}

static u8 Cmd_PowerType(void *pVoid)
{
	u8	result = 0;
	u8	buffer[32];
	u16	rxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	memset(buffer, NULL, sizeof(buffer));

	result = PWR_FirmwareType(pData->position, buffer, &rxSize);

	if(result)
	{
		CMD_Printf(";CH%d=%s", pData->channel, buffer);
	}

	return result;
}
/*
static u8 Cmd_PowerEthernetStatus(void *pVoid)
{
	u8	result = 0;
	u8	status;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)						return result;

	if(CMD_Compare(pData->pArgv[0], "on"))			status = 1;
	else if(CMD_Compare(pData->pArgv[0], "off"))	status = 0;
	else										return result;

	result = PWR_EthernetStatus(pData->position, status);

	return result;
}

static u8 Cmd_PowerEthernetConfig(void *pVoid)
{
	u8	result = 0, rtn;
	u8	mac[6], ip[4], nm[4], gw[4];
	u8	*pIpString;
	u16	mode, port;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	if(CMD_Compare(pData->pArgv[0], "none"))			mode = NETWORK_MODE_NONE;
	else if(CMD_Compare(pData->pArgv[0], "tcp"))		mode = NETWORK_MODE_TCP;
	else if(CMD_Compare(pData->pArgv[0], "udp"))		mode = NETWORK_MODE_UDP;
	else						return result;

	pIpString	= pData->pArgv[1];

	port		= CMD_StrToUL(pData->pArgv[2]);

	rtn = CMD_IpDataParsing(pIpString, ip);
	if(rtn == 0)				return result;

//	Get Board Data
	rtn = Network_GetInfoData(NETWORK_INFO_MAC, mac);
	if(rtn == 0)				return result;

	mac[3]	= 'P';
	mac[4]	= 0;
	mac[5]	= pData->channel;

	rtn = Network_GetInfoData(NETWORK_INFO_NM, nm);
	if(rtn == 0)				return result;

	rtn = Network_GetInfoData(NETWORK_INFO_GW, gw);
	if(rtn == 0)				return result;

	rtn = PWR_EthernetConfig(pData->position, mode, mac, ip, nm, gw, port);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}
*/
static u8 Cmd_PowerReset(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	result = PWR_Reboot(pData->position);

	return result;
}

static u8 Cmd_PowerInit(void *pVoid)
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

	result = PWR_OutputInit(pData->position, type);

	return result;
}

static u8 Cmd_PowerSetVoltage(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else						return result;

	voltage = CMD_AToF(pData->pArgv[1]);

	result = PWR_OutputVoltage(pData->position, pwrSrc, voltage);

	return result;
}

static u8 Cmd_PowerSetCurrent(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	current;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else						return result;

	current = CMD_AToF(pData->pArgv[1]);

	result = PWR_OutputCurrent(pData->position, pwrSrc, current);

	return result;
}

static u8 Cmd_PowerSetRange(void *pVoid)
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
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else						return result;

	range = CMD_StrToUL(pData->pArgv[1]);

	result = PWR_MeasureRange(pData->position, pwrSrc, range);

	return result;
}

static u8 Cmd_PowerEnable(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	pwrSrc[8], cnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)	return result;
	if(pData->argc > 9)		return result;

	for(cnt = 0; cnt < pData->argc; cnt++)
	{
		if(CMD_Compare(pData->pArgv[cnt], "vm0"))			pwrSrc[cnt] = PWR_SRC_VM_0;
		else if(CMD_Compare(pData->pArgv[cnt], "vm1"))		pwrSrc[cnt] = PWR_SRC_VM_1;
		else if(CMD_Compare(pData->pArgv[cnt], "vm2"))		pwrSrc[cnt] = PWR_SRC_VM_2;
		else if(CMD_Compare(pData->pArgv[cnt], "vm3"))		pwrSrc[cnt] = PWR_SRC_VM_3;
		else if(CMD_Compare(pData->pArgv[cnt], "vm4"))		pwrSrc[cnt] = PWR_SRC_VM_4;
		else if(CMD_Compare(pData->pArgv[cnt], "vm5"))		pwrSrc[cnt] = PWR_SRC_VM_5;
		else if(CMD_Compare(pData->pArgv[cnt], "vm6"))		pwrSrc[cnt] = PWR_SRC_VM_6;
		else if(CMD_Compare(pData->pArgv[cnt], "gpio"))		pwrSrc[cnt] = PWR_SRC_IOVOLTAGE;
		else if(CMD_Compare(pData->pArgv[cnt], "vio"))		pwrSrc[cnt] = PWR_SRC_IOVOLTAGE;
		else												return result;
	}

	for(cnt = 0; cnt < pData->argc; cnt++)
	{
		rtn = PWR_OutputEnable(pData->position, pwrSrc[cnt], 1);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)			result = 1;

	return result;
}

static u8 Cmd_PowerDisable(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	pwrSrc[8], cnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)	return result;
	if(pData->argc > 8)		return result;

	for(cnt = 0; cnt < pData->argc; cnt++)
	{
		if(CMD_Compare(pData->pArgv[cnt], "vm0"))			pwrSrc[cnt] = PWR_SRC_VM_0;
		else if(CMD_Compare(pData->pArgv[cnt], "vm1"))		pwrSrc[cnt] = PWR_SRC_VM_1;
		else if(CMD_Compare(pData->pArgv[cnt], "vm2"))		pwrSrc[cnt] = PWR_SRC_VM_2;
		else if(CMD_Compare(pData->pArgv[cnt], "vm3"))		pwrSrc[cnt] = PWR_SRC_VM_3;
		else if(CMD_Compare(pData->pArgv[cnt], "vm4"))		pwrSrc[cnt] = PWR_SRC_VM_4;
		else if(CMD_Compare(pData->pArgv[cnt], "vm5"))		pwrSrc[cnt] = PWR_SRC_VM_5;
		else if(CMD_Compare(pData->pArgv[cnt], "vm6"))		pwrSrc[cnt] = PWR_SRC_VM_6;
		else if(CMD_Compare(pData->pArgv[cnt], "gpio"))		pwrSrc[cnt] = PWR_SRC_IOVOLTAGE;
		else if(CMD_Compare(pData->pArgv[cnt], "vio"))		pwrSrc[cnt] = PWR_SRC_IOVOLTAGE;
		else												return result;
	}

	for(cnt = 0; cnt < pData->argc; cnt++)
	{
		rtn = PWR_OutputEnable(pData->position, pwrSrc[cnt], 0);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)			result = 1;

	return result;
}

static u8 Cmd_PowerMesurement(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	voltage, current;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else						return result;

	result = PWR_MesureVI(pData->position, pwrSrc, &voltage, &current);

	if(result)
	{
		CMD_Printf(";CH%d VM%d %fV %fmA", pData->channel, pwrSrc, voltage, current);
	}

	return result;
}

static u8 Cmd_PowerMesurementAuto(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	voltage, current;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else						return result;

	result = PWR_MesureAutoVI(pData->position, pwrSrc, &voltage, &current);

	if(result)
	{
		CMD_Printf(";CH%d VM%d %fV %fmA", pData->channel, pwrSrc, voltage, current);
	}

	return result;
}

static u8 Cmd_PowerExtVoltMeasure(void *pVoid)
{
	u8	result = 0;
	u8	mode;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "voltage"))				mode = PWR_MEASURE_MODE_VOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "current"))		mode = PWR_MEASURE_MODE_CURRENT;
	else						return result;

	result = PWR_ExtVoltageMeasure(pData->position, mode, &voltage);

	if(result)
	{
		CMD_Printf(";CH%d EXT_VOLT %fV", pData->channel, voltage);
	}

	return result;
}

static u8 Cmd_PowerReadAlarm(void *pVoid)
{
	u8	result = 0, valarm, ialarm;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = PWR_ReadAlarm(pData->position, &valarm, &ialarm);

	if(result)
	{
		CMD_Printf(";CH%d V_Alarm: 0x%02X I_Alarm: 0x%02X", pData->channel, valarm, ialarm);
	}

	return result;
}

static u8 Cmd_CheckAlarm(void *pVoid)
{
	u8	result = 0, valarm, ialarm, error_cnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = CheckAlarmFlag(pData->position, &valarm, &ialarm, &error_cnt);

	if(result)
	{
	  CMD_Printf(";CH%d V_Alarm: 0x%02X I_Alarm: 0x%02X Alarm_count :%d", pData->channel, valarm, ialarm, error_cnt);
	}

	return result;
}

static u8 Cmd_SetAlarmCnt(void *pVoid)
{
	u8	result = 0, error_cnt=0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	error_cnt = CMD_StrToUL(pData->pArgv[0]);

	result = PWR_SetError(pData->position, error_cnt);

	return result;
}

static u8 Cmd_PowerVFBEnable(void *pVoid)
{
	u8	result = 0, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "enable"))			state = ON;
	else if(CMD_Compare(pData->pArgv[0], "disable"))	state = OFF;
	else												return result;

	result = PWR_VfbState(pData->position, state);

	return result;
}

static u8 Cmd_PowerFV5Enable(void *pVoid)
{
	u8	result = 0, state, pin;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "enable"))			state = ON;
	else if(CMD_Compare(pData->pArgv[0], "disable"))	state = OFF;
	else												return result;

	if(pData->position == 2)		pin = 6;
	else if(pData->position == 3)	pin = 7;
	else							return result;

	result = ExtGPIO_WritePin(pin, state); // CH1:extGpio[6], CH2:extGpio[7]

	return result;
}

static u8 Cmd_PowerCalEnable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = PWR_CalState(pData->position, 1);

	return result;
}

static u8 Cmd_PowerCalDisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = PWR_CalState(pData->position, 0);

	return result;
}

static u8 Cmd_PowerCalVoltageOutput(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	ideal[2], real[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)							return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))			pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))	pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))	pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))	pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))	pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))	pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))	pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "ext"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else											return result;

	ideal[0]	= CMD_AToF(pData->pArgv[1]);
	real[0]		= CMD_AToF(pData->pArgv[2]);
	ideal[1]	= CMD_AToF(pData->pArgv[3]);
	real[1]		= CMD_AToF(pData->pArgv[4]);

	result = PWR_CalOutputVoltage(pData->position, pwrSrc, ideal, real);

	return result;
}

static u8 Cmd_PowerCalVoltageInput(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	ideal[2], real[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)							return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))			pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))	pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))	pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))	pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))	pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))	pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))	pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "ext"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else											return result;

	ideal[0]	= CMD_AToF(pData->pArgv[1]);
	real[0]		= CMD_AToF(pData->pArgv[2]);
	ideal[1]	= CMD_AToF(pData->pArgv[3]);
	real[1]		= CMD_AToF(pData->pArgv[4]);

	result = PWR_CalMeasureVoltage(pData->position, pwrSrc, ideal, real);

	return result;
}

static u8 Cmd_PowerCalCurrentInput(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc, src;
	double	ideal[2], real[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 6)							return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))			pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))	pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))	pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))	pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))	pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))	pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))	pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "ext"))	pwrSrc = PWR_SRC_IOVOLTAGE;
	else											return result;

	src		= CMD_StrToUL(pData->pArgv[1]);

	ideal[0]	= CMD_AToF(pData->pArgv[2]);
	real[0]		= CMD_AToF(pData->pArgv[3]);
	ideal[1]	= CMD_AToF(pData->pArgv[4]);
	real[1]		= CMD_AToF(pData->pArgv[5]);

	result = PWR_CalMeasureCurrent(pData->position, pwrSrc, src, ideal, real);

	return result;
}

static u8 Cmd_PowerCalSetData(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc, src;
	double	gain, offset;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)								return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "ext"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else												return result;

	if(CMD_Compare(pData->pArgv[1], "vout"))			src = PWR_CALIBRATION_SRC_V_OUT;
	else if(CMD_Compare(pData->pArgv[1], "vin"))		src = PWR_CALIBRATION_SRC_V_MEASURE;
	else if(CMD_Compare(pData->pArgv[1], "cin_r0"))		src = PWR_CALIBRATION_SRC_C_MEASURE_R0;
	else if(CMD_Compare(pData->pArgv[1], "cin_r1"))		src = PWR_CALIBRATION_SRC_C_MEASURE_R1;
	else if(CMD_Compare(pData->pArgv[1], "cin_r2"))		src = PWR_CALIBRATION_SRC_C_MEASURE_R2;
	else if(CMD_Compare(pData->pArgv[1], "cin_r3"))		src = PWR_CALIBRATION_SRC_C_MEASURE_R3;
	else if(CMD_Compare(pData->pArgv[1], "vssel_current"))		src = 6;
	else if(CMD_Compare(pData->pArgv[1], "vgl_current"))		src = 7;
	else if(CMD_Compare(pData->pArgv[1], "avddh_current"))		src = 8;
	else if(CMD_Compare(pData->pArgv[1], "vssel200_current"))	src = 9;
	else if(CMD_Compare(pData->pArgv[1], "vssel_voltage"))		src = 10;
	else if(CMD_Compare(pData->pArgv[1], "vgl_voltage"))		src = 11;
	else if(CMD_Compare(pData->pArgv[1], "avddh_voltage"))		src = 12;
	else if(CMD_Compare(pData->pArgv[1], "vddel_voltage"))		src = 13;
	else												return result;

	gain = CMD_AToF(pData->pArgv[2]);
	offset = CMD_AToF(pData->pArgv[3]);

	result = PWR_CalSetData(pData->position, pwrSrc, src, gain, offset);

	return result;
}

static u8 Cmd_PowerCalGetData(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc, src;
	double	gain, offset;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)								return result;

	if(CMD_Compare(pData->pArgv[0], "vm0"))				pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "ext"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else												return result;

	if(CMD_Compare(pData->pArgv[1], "vout"))			src = PWR_CALIBRATION_SRC_V_OUT;
	else if(CMD_Compare(pData->pArgv[1], "vin"))		src = PWR_CALIBRATION_SRC_V_MEASURE;
	else if(CMD_Compare(pData->pArgv[1], "cin_r0"))		src = PWR_CALIBRATION_SRC_C_MEASURE_R0;
	else if(CMD_Compare(pData->pArgv[1], "cin_r1"))		src = PWR_CALIBRATION_SRC_C_MEASURE_R1;
	else if(CMD_Compare(pData->pArgv[1], "cin_r2"))		src = PWR_CALIBRATION_SRC_C_MEASURE_R2;
	else if(CMD_Compare(pData->pArgv[1], "cin_r3"))		src = PWR_CALIBRATION_SRC_C_MEASURE_R3;
	else if(CMD_Compare(pData->pArgv[1], "vssel_current"))		src = 6;
	else if(CMD_Compare(pData->pArgv[1], "vgl_current"))		src = 7;
	else if(CMD_Compare(pData->pArgv[1], "avddh_current"))		src = 8;
	else if(CMD_Compare(pData->pArgv[1], "vssel200_current"))	src = 9;
	else if(CMD_Compare(pData->pArgv[1], "vssel_voltage"))		src = 10;
	else if(CMD_Compare(pData->pArgv[1], "vgl_voltage"))		src = 11;
	else if(CMD_Compare(pData->pArgv[1], "avddh_voltage"))		src = 12;
	else if(CMD_Compare(pData->pArgv[1], "vddel_voltage"))		src = 13;
	else												return result;

	result = PWR_CalGetData(pData->position, pwrSrc, src, &gain, &offset);

	if(result)
	{
		CMD_Printf(";CH%d GAIN=%f OFFSET=%f", pData->channel, gain, offset);
	}

	return result;
}

static u8 Cmd_PowerRange(void *pVoid)
{
	u8	result = 0;
	double	range = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	range = CMD_AToF(pData->pArgv[0]);

	result = PWR_ExtRange(pData->position, range);

	return result;
}

static u8 Cmd_PowerCalPMIC_current(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	ideal[2], real[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)							return result;

	if(CMD_Compare(pData->pArgv[0], "vssel"))			pwrSrc = 6;
	else if(CMD_Compare(pData->pArgv[0], "vgl"))		pwrSrc = 7;
	else if(CMD_Compare(pData->pArgv[0], "avddh"))		pwrSrc = 8;
	else if(CMD_Compare(pData->pArgv[0], "vssel200"))	pwrSrc = 9;
	else												return result;

	ideal[0]	= CMD_AToF(pData->pArgv[1]);
	real[0]		= CMD_AToF(pData->pArgv[2]);
	ideal[1]	= CMD_AToF(pData->pArgv[3]);
	real[1]		= CMD_AToF(pData->pArgv[4]);

	result = PWR_CalPMICCurrent(pData->position, pwrSrc, ideal, real);

	return result;
}

static u8 Cmd_PowerCalPMIC_voltage(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc;
	double	ideal[2], real[2];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)							return result;

	if(CMD_Compare(pData->pArgv[0], "vssel"))			pwrSrc = 10;
	else if(CMD_Compare(pData->pArgv[0], "vgl"))		pwrSrc = 11;
	else if(CMD_Compare(pData->pArgv[0], "avddh"))		pwrSrc = 12;
	else if(CMD_Compare(pData->pArgv[0], "vddel"))		pwrSrc = 13;
	else												return result;

	ideal[0]	= CMD_AToF(pData->pArgv[1]);
	real[0]		= CMD_AToF(pData->pArgv[2]);
	ideal[1]	= CMD_AToF(pData->pArgv[3]);
	real[1]		= CMD_AToF(pData->pArgv[4]);

	result = PWR_CalPMICVoltage(pData->position, pwrSrc, ideal, real);

	return result;
}

static u8 Cmd_PowerVsselCurrent(void *pVoid)
{
	u8	result = 0;
	double	range = 0;
	double	current;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	range = CMD_AToF(pData->pArgv[0]);

	result = PWR_Vssel(pData->position, range, &current);

	if(result)
	{
		CMD_Printf(";CH%d %fmA", pData->channel, current);
	}

	return result;
}

static u8 Cmd_PowerVsselVoltage(void *pVoid)
{
	u8	result = 0;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = PWR_Vssel_Voltage(pData->position, &voltage);

	if(result)
	{
		//voltage = voltage * 3;
		CMD_Printf(";CH%d -%fV", pData->channel, voltage);
	}

	return result;
}

static u8 Cmd_PowerVglCurrent(void *pVoid)
{
	u8	result = 0;
	double	range = 0;
	double	current;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	range = CMD_AToF(pData->pArgv[0]);

	result = PWR_Vgl(pData->position, range, &current);

	if(result)
	{
		//current = current *1000;
		CMD_Printf(";CH%d %fmA", pData->channel, current);
	}

	return result;
}

static u8 Cmd_PowerVglVoltage(void *pVoid)
{
	u8	result = 0;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = PWR_Vgl_Voltage(pData->position, &voltage);

	if(result)
	{
		//voltage = voltage * 4;
		CMD_Printf(";CH%d -%fV", pData->channel, voltage);
	}

	return result;
}


static u8 Cmd_PoweravddhCurrent(void *pVoid)
{
	u8	result = 0;
	double	range = 0;
	double	current;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	range = CMD_AToF(pData->pArgv[0]);

	result = PWR_avddh_current(pData->position, range, &current);

	if(result)
	{
		//current = current *1000;
		CMD_Printf(";CH%d %fmA", pData->channel, current);
	}

	return result;
}

static u8 Cmd_PoweravddhVoltage(void *pVoid)
{
	u8	result = 0;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = PWR_avddh_voltage(pData->position, &voltage);

	if(result)
	{
		//voltage = voltage * 2;
		CMD_Printf(";CH%d %fV", pData->channel, voltage);
	}

	return result;
}

static u8 Cmd_PowervddelVoltage(void *pVoid)
{
	u8	result = 0;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = PWR_vddel_voltage(pData->position, &voltage);

	if(result)
	{
		//voltage = voltage * 2;
		CMD_Printf(";CH%d %fV", pData->channel, voltage);
	}

	return result;
}

static u8 Cmd_PowerFactorSetData(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc, src;
	double	factor;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm0")) 			pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else												return result;

	if(CMD_Compare(pData->pArgv[1], "vfb"))				src = PWR_FACTOR_SRC_VFB;
	else if(CMD_Compare(pData->pArgv[1], "ovr1"))		src = PWR_FACTOR_SRC_OVR_0;
	else if(CMD_Compare(pData->pArgv[1], "ovr2")) 		src = PWR_FACTOR_SRC_OVR_1;
	else if(CMD_Compare(pData->pArgv[1], "ovr3")) 		src = PWR_FACTOR_SRC_OVR_2;
	else if(CMD_Compare(pData->pArgv[1], "mvr0")) 		src = PWR_FACTOR_SRC_MVR_0;
	else if(CMD_Compare(pData->pArgv[1], "mvr1")) 		src = PWR_FACTOR_SRC_MVR_1;
	else if(CMD_Compare(pData->pArgv[1], "mir0")) 		src = PWR_FACTOR_SRC_MIR_0;
	else if(CMD_Compare(pData->pArgv[1], "mir1")) 		src = PWR_FACTOR_SRC_MIR_1;
	else if(CMD_Compare(pData->pArgv[1], "rds")) 		src = PWR_FACTOR_SRC_RDS;
	else if(CMD_Compare(pData->pArgv[1], "shunt0")) 	src = PWR_FACTOR_SRC_SHUNT_0;
	else if(CMD_Compare(pData->pArgv[1], "shunt1")) 	src = PWR_FACTOR_SRC_SHUNT_1;
	else if(CMD_Compare(pData->pArgv[1], "shunt2")) 	src = PWR_FACTOR_SRC_SHUNT_2;
	else if(CMD_Compare(pData->pArgv[1], "shunt3")) 	src = PWR_FACTOR_SRC_SHUNT_3;
	else												return result;

	factor = CMD_AToF(pData->pArgv[2]);

	result = PWR_FactorSetData(pData->position, pwrSrc, src, factor);

	return result;
}

static u8 Cmd_PowerFactorGetData(void *pVoid)
{
	u8	result = 0;
	u8	pwrSrc, src;
	double	factor;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm0")) 			pwrSrc = PWR_SRC_VM_0;
	else if(CMD_Compare(pData->pArgv[0], "vm1"))		pwrSrc = PWR_SRC_VM_1;
	else if(CMD_Compare(pData->pArgv[0], "vm2"))		pwrSrc = PWR_SRC_VM_2;
	else if(CMD_Compare(pData->pArgv[0], "vm3"))		pwrSrc = PWR_SRC_VM_3;
	else if(CMD_Compare(pData->pArgv[0], "vm4"))		pwrSrc = PWR_SRC_VM_4;
	else if(CMD_Compare(pData->pArgv[0], "vm5"))		pwrSrc = PWR_SRC_VM_5;
	else if(CMD_Compare(pData->pArgv[0], "vm6"))		pwrSrc = PWR_SRC_VM_6;
	else if(CMD_Compare(pData->pArgv[0], "gpio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else if(CMD_Compare(pData->pArgv[0], "vio"))		pwrSrc = PWR_SRC_IOVOLTAGE;
	else												return result;

	if(CMD_Compare(pData->pArgv[1], "vfb"))				src = PWR_FACTOR_SRC_VFB;
	else if(CMD_Compare(pData->pArgv[1], "ovr1"))		src = PWR_FACTOR_SRC_OVR_0;
	else if(CMD_Compare(pData->pArgv[1], "ovr2")) 		src = PWR_FACTOR_SRC_OVR_1;
	else if(CMD_Compare(pData->pArgv[1], "ovr3")) 		src = PWR_FACTOR_SRC_OVR_2;
	else if(CMD_Compare(pData->pArgv[1], "mvr0")) 		src = PWR_FACTOR_SRC_MVR_0;
	else if(CMD_Compare(pData->pArgv[1], "mvr1")) 		src = PWR_FACTOR_SRC_MVR_1;
	else if(CMD_Compare(pData->pArgv[1], "mir0")) 		src = PWR_FACTOR_SRC_MIR_0;
	else if(CMD_Compare(pData->pArgv[1], "mir1")) 		src = PWR_FACTOR_SRC_MIR_1;
	else if(CMD_Compare(pData->pArgv[1], "rds")) 		src = PWR_FACTOR_SRC_RDS;
	else if(CMD_Compare(pData->pArgv[1], "shunt0")) 	src = PWR_FACTOR_SRC_SHUNT_0;
	else if(CMD_Compare(pData->pArgv[1], "shunt1")) 	src = PWR_FACTOR_SRC_SHUNT_1;
	else if(CMD_Compare(pData->pArgv[1], "shunt2")) 	src = PWR_FACTOR_SRC_SHUNT_2;
	else if(CMD_Compare(pData->pArgv[1], "shunt3")) 	src = PWR_FACTOR_SRC_SHUNT_3;
	else												return result;

	result = PWR_FactorGetData(pData->position, pwrSrc, src, &factor);

	if(result)
	{
		CMD_Printf(";CH%d FACTOR=%f", pData->channel, factor);
	}

	return result;
}

static u8 Cmd_PowerDataClear(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = PWR_DataClear(pData->position);

	return result;
}

static u8 Cmd_PowerDataLoad(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = PWR_DataLoad(pData->position);

	return result;
}

static u8 Cmd_PowerDataSave(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = PWR_DataSave(pData->position);

	return result;
}

static u8 Cmd_EEPROMSet(void *pVoid)
{
	u8	result = 0;
	u32 status;

	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	status			= CMD_StrToUL(pData->pArgv[0]);

	result = PWR_EEPROMSET(pData->position, status);

	return result;
}

static u8 Cmd_PowerADCcount(void *pVoid)
{
	u8	result = 0;
	u8	maxcount = 0;
	u8	delcount = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	maxcount	= CMD_StrToUL(pData->pArgv[0]);
	delcount	= CMD_StrToUL(pData->pArgv[1]);

	result = PWR_ADCcount(pData->position, maxcount, delcount);

	return result;
}

const	commandList_t	cmdList_Power_M[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note},
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"version",				Cmd_PowerVersion,			NULL,		NULL,		";VERSION",			 	NULL},
												{	"build",				Cmd_PowerBuild,				NULL,		NULL,		";BUILD",			 	NULL},
												{	"type",					Cmd_PowerType,				NULL,		NULL,		";TYPE",			 	NULL},
												{	"reset",				Cmd_PowerReset,				NULL,		NULL,		";RESET",				NULL},
												{	"init",					Cmd_PowerInit,				NULL,		NULL,		";INIT",				NULL},
												{	"set.voltage",			Cmd_PowerSetVoltage,		NULL,		NULL,		";SET.VOLTAGE",			NULL},
												{	"set.current",			Cmd_PowerSetCurrent,		NULL,		NULL,		";SET.CURRENT",			NULL},
												{	"set.range",			Cmd_PowerSetRange,			NULL,		NULL,		";SET.RANGE",			NULL},
												{	"range",				Cmd_PowerSetRange,			NULL,		NULL,		";RANGE",				NULL},
												{	"enable",				Cmd_PowerEnable,			NULL,		NULL,		";ENABLE",				NULL},
												{	"disable",				Cmd_PowerDisable,			NULL,		NULL,		";DISABLE",				NULL},
												{	"read",					Cmd_PowerMesurement,		NULL,		NULL,		";READ",				NULL},
												{	"read.auto",			Cmd_PowerMesurementAuto,	NULL,		NULL,		";READ.AUTO",			NULL},
												{	"ext.read",				Cmd_PowerExtVoltMeasure,	NULL,		NULL,		";EXT.READ",			NULL},
												{	"read.alarmio",			Cmd_PowerReadAlarm,			NULL,		NULL,		";READ.ALARMIO",		NULL},
												{	"check.alarm",			Cmd_CheckAlarm,				NULL,		NULL,		";CHECK.ALARM",			NULL},
												{	"set.alarm.count",		Cmd_SetAlarmCnt,			NULL,		NULL,		";CHECK.ALARM.CNT",		NULL},
												{	"vfb",					Cmd_PowerVFBEnable,			NULL,		NULL,		";VFB",					NULL},
												{	"fv5",					Cmd_PowerFV5Enable,			NULL,		NULL,		";F5",					NULL},
												{	"cal.enable",			Cmd_PowerCalEnable,			NULL,		NULL,		";CAL.ENABLE",			NULL},
												{	"cal.disable",			Cmd_PowerCalDisable,		NULL,		NULL,		";CAL.DISABLE",			NULL},
												{	"cal.vout",				Cmd_PowerCalVoltageOutput,	NULL,		NULL,		";CAL.VOUT",			NULL},
												{	"cal.vin",				Cmd_PowerCalVoltageInput,	NULL,		NULL,		";CAL.VIN",				NULL},
												{	"cal.cin",				Cmd_PowerCalCurrentInput,	NULL,		NULL,		";CAL.CIN",				NULL},
												{	"cal.set",				Cmd_PowerCalSetData,		NULL,		NULL,		";CAL.SET",				NULL},
												{	"cal.get",				Cmd_PowerCalGetData,		NULL,		NULL,		";CAL.GET",				NULL},
												{	"cal.range",			Cmd_PowerRange,				NULL,		NULL,		";CAL.RANGE",			NULL},
												{	"cal.PMIC.current",		Cmd_PowerCalPMIC_current,	NULL,		NULL,		";CAL.PMIC.CURRENT",	NULL},
												{	"cal.PMIC.voltage",		Cmd_PowerCalPMIC_voltage,	NULL,		NULL,		";CAL.PMIC.VOLTAGE",	NULL},
												{	"vssel.current",		Cmd_PowerVsselCurrent,		NULL,		NULL,		";VSSEL.CURRENT",		NULL},
												{	"vssel.voltage",		Cmd_PowerVsselVoltage,		NULL,		NULL,		";VSSEL.VOLTAGE",		NULL},
												{	"vgl.current",			Cmd_PowerVglCurrent,		NULL,		NULL,		";VGL.CURRENT",			NULL},
												{	"vgl.voltage",			Cmd_PowerVglVoltage,		NULL,		NULL,		";VGL.VOLTAGE",			NULL},
												{	"avddh.current",		Cmd_PoweravddhCurrent,		NULL,		NULL,		";AVDDH.CURRENT",		NULL},
												{	"avddh.voltage",		Cmd_PoweravddhVoltage,		NULL,		NULL,		";AVDDH.VOLTAGE",		NULL},
												{	"vddel.voltage",		Cmd_PowervddelVoltage,		NULL,		NULL,		";VDDEL.VOLTAGE",		NULL},
												{	"factor.set",			Cmd_PowerFactorSetData,		NULL,		NULL,		";FACTOR.SET",			NULL},
												{	"factor.get",			Cmd_PowerFactorGetData,		NULL,		NULL,		";FACTOR.GET",			NULL},
												{	"data.clear",			Cmd_PowerDataClear,			NULL,		NULL,		";DATA.CLEAR",			NULL},
												{	"data.load",			Cmd_PowerDataLoad,			NULL,		NULL,		";DATA.LOAD",			NULL},
												{	"data.save",			Cmd_PowerDataSave,			NULL,		NULL,		";DATA.SAVE",			NULL},
												{	"Jump.App",				Cmd_EEPROMSet,				NULL,		NULL,		";SELF.EEPROMSET",		NULL},
												{	"adc.count",			Cmd_PowerADCcount,			NULL,		NULL,		";ADC.COUNT",			"power.adc.count [read maxcount] [delete count"},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};


