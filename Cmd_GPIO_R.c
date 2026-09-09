#include	"Cmd_GPIO_R.h"

static u8 Cmd_GpioInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = GPIO_R_Init(pData->option);

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

	result = GPIO_R_Direction(pData->option, state);

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

	result = GPIO_R_Write(pData->option, state);

	return result;
}

static u8 Cmd_GpioRead(void *pVoid)
{
	u8	result = 0;
	u8	state[4];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = GPIO_R_Read(state);

	if(result)		CMD_Printf(";CH1=0x%02X, CH2=0x%02X, CH3=0x%02X, CH4=0x%02X", state[0], state[1], state[2], state[3]);

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

	result = GPIO_R_Set(pData->option, state);

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

	result = GPIO_R_Clear(pData->option, state);

	return result;
}

const	commandList_t	cmdList_GPIO_R[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note},
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"init",					Cmd_GpioInit,				0x0F,		NULL,		";INIT",				NULL},
												{	"dir",					Cmd_GpioDirection,			0x0F,		NULL,		";DIR",					NULL},
												{	"write",				Cmd_GpioWrite,				0x0F,		NULL,		";WRITE",				NULL},
												{	"read",					Cmd_GpioRead,				0x0F,		NULL,		";READ",				NULL},
												{	"set",					Cmd_GpioSet,				0x0F,		NULL,		";SET",					NULL},
												{	"clear",				Cmd_GpioClear,				0x0F,		NULL,		";CLEAR",				NULL},
												{	"clr",					Cmd_GpioClear,				0x0F,		NULL,		";CLR",					NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};


