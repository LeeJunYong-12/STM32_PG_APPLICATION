#include	"Cmd_GPIO.h"

static u8 Cmd_GpioInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_GpioInit(pData->position);

	return result;
}

static u8 Cmd_GpioDirection(void *pVoid)
{
	u8	result = 0;
	u8	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);

	result = LCM_GpioDirection(pData->position, state);

	return result;
}

static u8 Cmd_GpioWrite(void *pVoid)
{
	u8	result = 0;
	u8	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);

	result = LCM_GpioWrite(pData->position, state);

	return result;
}

static u8 Cmd_GpioRead(void *pVoid)
{
	u8	result = 0;
	u8	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_GpioRead(pData->position, &state);

	if(result)		CMD_Printf(";CH%d 0x%02X", pData->channel, state);

	return result;
}

static u8 Cmd_GpioSet(void *pVoid)
{
	u8	result = 0;
	u8	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);

	result = LCM_GpioSet(pData->position, state);

	return result;
}

static u8 Cmd_GpioClear(void *pVoid)
{
	u8	result = 0;
	u8	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);

	result = LCM_GpioClear(pData->position, state);

	return result;
}

static u8 Cmd_GpioPulseInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_GpioPulseInit(pData->position);

	return result;
}

static u8 Cmd_GpioPulseDeInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_GpioPulseDeInit(pData->position);

	return result;
}

static u8 Cmd_GpioPulseRead(void *pVoid)
{
	u8	result = 0;
	float	pulseAverage;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_GpioPulseRead(pData->position, &pulseAverage);
	if(result)					CMD_Printf(";%fPulse/sec", pulseAverage);

	return result;
}

static u8 Cmd_GpioPulseCountRead(void *pVoid)
{
	u8	result = 0;
	u32	currentCnt, loopCnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_GpioPulseCountRead(pData->position, &currentCnt, &loopCnt);
	if(result)					CMD_Printf(";CURRENT=%d,LOOP=%d", currentCnt, loopCnt);

	return result;
}

const	commandList_t	cmdList_GPIO_M[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note},
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"init",					Cmd_GpioInit,				NULL,		NULL,		";INIT",				NULL},
												{	"dir",					Cmd_GpioDirection,			NULL,		NULL,		";DIR",					NULL},
												{	"write",				Cmd_GpioWrite,				NULL,		NULL,		";WRITE",				NULL},
												{	"read",					Cmd_GpioRead,				NULL,		NULL,		";READ",				NULL},
												{	"set",					Cmd_GpioSet,				NULL,		NULL,		";SET",					NULL},
												{	"clear",				Cmd_GpioClear,				NULL,		NULL,		";CLEAR",				NULL},
												{	"clr",					Cmd_GpioClear,				NULL,		NULL,		";CLR",					NULL},
												{	"pulse.init",			Cmd_GpioPulseInit,			NULL,		NULL,		";PULSE.INIT",			NULL},
												{	"pulse.deinit",			Cmd_GpioPulseDeInit,		NULL,		NULL,		";PULSE.DEINIT",		NULL},
												{	"pulse.read",			Cmd_GpioPulseRead,			NULL,		NULL,		";PULSE.READ",			NULL},
												{	"pulse.count",			Cmd_GpioPulseCountRead,		NULL,		NULL,		";PULSE.COUNT",			NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};

