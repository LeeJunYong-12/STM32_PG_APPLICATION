#include	"Cmd_Extension.h"

static u8 Cmd_Extension_I2C_Init(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	channel = CMD_StrToUL(pData->pArgv[0]);

	result = Extension_Init(pData->position, channel);

	return result;
}

static u8 Cmd_Extension_I2C_Deinit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = Extension_Deinit(pData->position);

	return result;
}

static u8 Cmd_Extension_TCA6424A_Direction(void *pVoid)
{
	u8	result = 0;
	u8	port, pinStatus;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	port		= CMD_StrToUL(pData->pArgv[0]);
	pinStatus	= CMD_StrToUL(pData->pArgv[1]);

	result = Extension_TCA6424A_Direction(pData->position, port, pinStatus);

	return result;
}

static u8 Cmd_Extension_TCA6424A_Write(void *pVoid)
{
	u8	result = 0;
	u8	port, pinStatus;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	port		= CMD_StrToUL(pData->pArgv[0]);
	pinStatus	= CMD_StrToUL(pData->pArgv[1]);

	result = Extension_TCA6424A_OutputWrite(pData->position, port, pinStatus);

	return result;
}

static u8 Cmd_Extension_TCA6424A_Read(void *pVoid)
{
	u8	result = 0;
	u8	port, pinStatus;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	port		= CMD_StrToUL(pData->pArgv[0]);

	result = Extension_TCA6424A_InputRead(pData->position, port, &pinStatus);

	if(result)
	{
		CMD_Printf(";CH%d 0x%02X", pData->channel, pinStatus);
	}

	return result;
}

static u8 Cmd_Extension_LTC2633_Ref(void *pVoid)
{
	u8	result = 0;
	u8	refMode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "internal"))		refMode = 0;
	else if(CMD_Compare(pData->pArgv[0], "external"))	refMode = 1;
	else												return result;

	result = Extension_LTC2633_RefCtrl(pData->position, refMode);

	return result;
}

static u8 Cmd_Extension_LTC2633_PwrDown(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "port_a"))			channel = EXTENSION_LTC2633_ADRS_DAC_A;
	else if(CMD_Compare(pData->pArgv[0], "port_b"))		channel = EXTENSION_LTC2633_ADRS_DAC_B;
	else if(CMD_Compare(pData->pArgv[0], "port_all"))	channel = EXTENSION_LTC2633_ADRS_DAC_ALL;
	else												return result;

	result = Extension_LTC2633_PowerDown(pData->position, channel);

	return result;
}

static u8 Cmd_Extension_LTC2633_VoltCtrl(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)								return result;

	if(CMD_Compare(pData->pArgv[0], "port_a"))			channel = EXTENSION_LTC2633_ADRS_DAC_A;
	else if(CMD_Compare(pData->pArgv[0], "port_b"))		channel = EXTENSION_LTC2633_ADRS_DAC_B;
	else if(CMD_Compare(pData->pArgv[0], "port_all"))	channel = EXTENSION_LTC2633_ADRS_DAC_ALL;
	else												return result;

	voltage = CMD_AToF(pData->pArgv[1]);

	result = Extension_LTC2633_VoltCtrl(pData->position, channel, voltage);

	return result;
}

static u8 Cmd_Extension_E5ETBOARD_VoltOutput(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	double	voltage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)								return result;

	if(CMD_Compare(pData->pArgv[0], "ch0"))				channel = EXTENSION_LTC2633_ADRS_DAC_A;
	else if(CMD_Compare(pData->pArgv[0], "ch1"))		channel = EXTENSION_LTC2633_ADRS_DAC_B;
	else												return result;

	voltage = CMD_AToF(pData->pArgv[1]);

	result = Extension_E5ETBOARD_VoltOutput(pData->position, channel, voltage);

	return result;
}

const	commandList_t	cmdList_Extension[] =	{
//													{	Command,				Function,							Option,		Mode,		RootMessage				Note}
													{	"help",					Cmd_Help,							NULL,		0x01,		";HELP",				NULL},
													{	"i2c.init",				Cmd_Extension_I2C_Init,				NULL,		NULL,		";I2C.INIT",			NULL},
													{	"i2c.deinit",			Cmd_Extension_I2C_Deinit,			NULL,		NULL,		";I2C.DEINIT",			NULL},
													{	"tca6424a.dir",			Cmd_Extension_TCA6424A_Direction,	NULL,		NULL,		";TCA6424A.DIR",		NULL},
													{	"tca6424a.write",		Cmd_Extension_TCA6424A_Write,		NULL,		NULL,		";TCA6424A.WRITE",		NULL},
													{	"tca6424a.read",		Cmd_Extension_TCA6424A_Read,		NULL,		NULL,		";TCA6424A.READ",		NULL},
													{	"ltc2633.ref",			Cmd_Extension_LTC2633_Ref,			NULL,		NULL,		";LTC2633.REF",			NULL},
													{	"ltc2633.pwrdown",		Cmd_Extension_LTC2633_PwrDown,		NULL,		NULL,		";LTC2633.PWRDOWN",		NULL},
													{	"ltc2633.voltctrl",		Cmd_Extension_LTC2633_VoltCtrl,		NULL,		NULL,		";LTC2633.VOLTCTRL",	NULL},
													{	"e5etboard.voltout",	Cmd_Extension_E5ETBOARD_VoltOutput,	NULL,		NULL,		";E5ETBOARD.VOLTOUT",	NULL},
													{	NULL,					NULL,								NULL,		NULL,		NULL,					NULL},
												};

