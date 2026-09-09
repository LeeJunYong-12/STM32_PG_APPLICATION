#include	"Cmd_Base_R.h"

static u8 Cmd_BaseRedriverPwrDown(void *pVoid)
{
	u8	result = 0;
	u8	position, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	mode		= CMD_StrToUL(pData->pArgv[1]);

	result = BASE_R_eDPRedriverSetPwrDown(position, mode);

	return result;
}

static u8 Cmd_BaseRedriverSetSync(void *pVoid)
{
	u8	result = 0;
	u8	position, type, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	type		= CMD_StrToUL(pData->pArgv[1]);
	mode		= CMD_StrToUL(pData->pArgv[2]);

	result = BASE_R_eDPRedriverSetSync(position, type, mode);

	return result;
}

static u8 Cmd_BaseRedriverEqMode(void *pVoid)
{
	u8	result = 0;
	u8	position, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	mode		= CMD_StrToUL(pData->pArgv[1]);

	result = BASE_R_eDPRedriverSetEqMode(position, mode);

	return result;
}

static u8 Cmd_BaseRedriverChannelEnable(void *pVoid)
{
	u8	result = 0;
	u8	position, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	mode		= CMD_StrToUL(pData->pArgv[1]);

	result = BASE_R_eDPRedriverSetChannelEnable(position, mode);

	return result;
}

static u8 Cmd_BaseRedriverSetRxGain(void *pVoid)
{
	u8	result = 0;
	u8	position, type, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	type		= CMD_StrToUL(pData->pArgv[1]);
	mode		= CMD_StrToUL(pData->pArgv[2]);

	result = BASE_R_eDPRedriverSetRxGain(position, type, mode);

	return result;
}

static u8 Cmd_BaseRedriverSetEqGain(void *pVoid)
{
	u8	result = 0;
	u8	position, type, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	type		= CMD_StrToUL(pData->pArgv[1]);
	mode		= CMD_StrToUL(pData->pArgv[2]);

	result = BASE_R_eDPRedriverSetEqGain(position, type, mode);

	return result;
}

static u8 Cmd_BaseRedriverSetTxGain(void *pVoid)
{
	u8	result = 0;
	u8	position, type, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	type		= CMD_StrToUL(pData->pArgv[1]);
	mode		= CMD_StrToUL(pData->pArgv[2]);

	result = BASE_R_eDPRedriverSetTxGain(position, type, mode);

	return result;
}

static u8 Cmd_BaseRedriverSetEqValue(void *pVoid)
{
	u8	result = 0;
	u8	position, type, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	type		= CMD_StrToUL(pData->pArgv[1]);
	mode		= CMD_StrToUL(pData->pArgv[2]);

	result = BASE_R_eDPRedriverSetEqValue(position, type, mode);

	return result;
}

static u8 Cmd_BaseRedriverSetDriveEnable(void *pVoid)
{
	u8	result = 0;
	u8	position, type, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	type		= CMD_StrToUL(pData->pArgv[1]);
	mode		= CMD_StrToUL(pData->pArgv[2]);

	result = BASE_R_eDPRedriverSetDriveEnable(position, type, mode);

	return result;
}

static u8 Cmd_BaseRedriverSetEqEnable(void *pVoid)
{
	u8	result = 0;
	u8	position, type, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	type		= CMD_StrToUL(pData->pArgv[1]);
	mode		= CMD_StrToUL(pData->pArgv[2]);

	result = BASE_R_eDPRedriverSetEqEnable(position, type, mode);

	return result;
}

static u8 Cmd_BaseRedriverSetDrivePeak(void *pVoid)
{
	u8	result = 0;
	u8	position, type, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	position	= CMD_StrToUL(pData->pArgv[0]);
	type		= CMD_StrToUL(pData->pArgv[1]);
	mode		= CMD_StrToUL(pData->pArgv[2]);

	result = BASE_R_eDPRedriverSetDrivePeak(position, type, mode);

	return result;
}

const	commandList_t	cmdList_BASE_R[] =	{
//												{	Command,					Function,							Option,		Mode,		RootMessage					Note},
												{	"help",						Cmd_Help,							NULL,		0x01,		";HELP",					NULL},
												{	"redriver.pwrdown",			Cmd_BaseRedriverPwrDown,			NULL,		NULL,		";REDRIVER.PWRDOWN",		NULL},
												{	"redriver.sync",			Cmd_BaseRedriverSetSync,			NULL,		NULL,		";REDRIVER.SYNC",			NULL},
												{	"redriver.eqmode",			Cmd_BaseRedriverEqMode,				NULL,		NULL,		";REDRIVER.EQMODE",			NULL},
												{	"redriver.chenable",		Cmd_BaseRedriverChannelEnable,		NULL,		NULL,		";REDRIVER.CHENABLE",		NULL},
												{	"redriver.rxgain",			Cmd_BaseRedriverSetRxGain,			NULL,		NULL,		";REDRIVER.RXGAIN",			NULL},
												{	"redriver.eqgain",			Cmd_BaseRedriverSetEqGain,			NULL,		NULL,		";REDRIVER.EQGAIN",			NULL},
												{	"redriver.txgain",			Cmd_BaseRedriverSetTxGain,			NULL,		NULL,		";REDRIVER.TXGAIN",			NULL},
												{	"redriver.eqvalue",			Cmd_BaseRedriverSetEqValue,			NULL,		NULL,		";REDRIVER.EQVALUE",		NULL},
												{	"redriver.driveen",			Cmd_BaseRedriverSetDriveEnable,		NULL,		NULL,		";REDRIVER.DRIVEEN",		NULL},
												{	"redriver.eqen",			Cmd_BaseRedriverSetEqEnable,		NULL,		NULL,		";REDRIVER.EQEN",			NULL},
												{	"redriver.drivepeak",		Cmd_BaseRedriverSetDrivePeak,		NULL,		NULL,		";REDRIVER.DRIVEPEAK",		NULL},
												{	NULL,						NULL,								NULL,		NULL,		NULL,						NULL},
											};

