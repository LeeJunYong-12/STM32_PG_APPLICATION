#include	"App_CmdList_eDPr.h"

static u8 Cmd_GPIO(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_GPIO_R);

	return result;
}

static u8 Cmd_LCM(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_LCM_R);

	return result;
}

static u8 Cmd_Power(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_Power_R);

	return result;
}

static u8 Cmd_eDP(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_eDP_R);

	return result;
}

static u8 Cmd_Base(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_BASE_R);

	return result;
}

const	commandList_t	cmdList_eDP_Reliability[] =	{
//														{	Command,				Function,					Option,		Mode,	RootMessage				Note}
														{	"help",					Cmd_Help,					NULL,		NULL,	">>HELP",				NULL},
														{	"board",				Cmd_Board,					NULL,		NULL,	">>BOARD",				NULL},
														{	"file",					Cmd_File,					NULL,		NULL,	">>FILE",				NULL},
														{	"fpga",					Cmd_FPGA,					NULL,		NULL,	">>FPGA",				NULL},
														{	"image",				Cmd_IMAGE,					NULL,		NULL,	">>IMAGE",				NULL},
														{	"system",				Cmd_System,					NULL,		NULL,	">>SYSTEM",				NULL},
														{	"test",					Cmd_Test,					NULL,		NULL,	">>TEST",				NULL},
														{	"delay", 				Cmd_Delay,					NULL,		NULL,	">>DELAY",				NULL},
														{	"gpio",					Cmd_GPIO,					NULL,		NULL,	">>GPIO",				NULL},
														{	"lcm",					Cmd_LCM,					NULL,		NULL,	">>LCM",				NULL},
														{	"power",				Cmd_Power,					NULL,		NULL,	">>POWER",				NULL},
														{	"edp",					Cmd_eDP,					NULL,		NULL,	">>EDP",				NULL},
														{	"base",					Cmd_Base,					NULL,		NULL,	">>BASE",				NULL},
														{	NULL,					NULL,						NULL,		NULL,	NULL,					NULL},
													};

