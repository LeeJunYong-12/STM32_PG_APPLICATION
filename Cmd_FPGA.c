#include	"Cmd_FPGA.h"

const	commandList_t	cmdList_FPGA[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"power.off",			Cmd_FpgaPowerOff,			NULL,		NULL,		";POWER.OFF",			NULL},
												{	"power.on",				Cmd_FpgaPowerOn,			NULL,		NULL,		";POWER.ON",			NULL},
												{	"reset",				Cmd_FpgaReset,				NULL,		NULL,		";RESET",				NULL},
												{	"read",					Cmd_FpgaRead,				NULL,		NULL,		";READ",				NULL},
												{	"write",				Cmd_FpgaWrite,				NULL,		NULL,		";WRITE",				NULL},
												{	"ptg",					Cmd_FpgaPTG,				NULL,		NULL,		";PTG",					NULL},
												{	"bus.path",				Cmd_FpgaBUSPath,			NULL,		NULL,		";BUSPATH",				NULL},
												{	"update.check",			Cmd_FpgaUpdateCheck,		NULL,		NULL,		";UPDATE.CHECK",		NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};

u8 Cmd_FpgaPowerOff(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = FPGA_PwrIoCtrl(0);

	return result;
}

u8 Cmd_FpgaPowerOn(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = FPGA_PwrIoCtrl(1);

	return result;
}

u8 Cmd_FpgaReset(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = FpgaPG_Reset();

	return result;
}

u8 Cmd_FpgaRead(void *pVoid)
{
	u8	result = 0;
	u32	cmd, data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	cmd = CMD_StrToUL(pData->pArgv[0]);

	data = FPGA_ReadSingle(cmd);

	CMD_Printf(";CMD=0x%08X,DATA=0x%08X", cmd, data);

	result = 1;

	return result;
}

u8 Cmd_FpgaWrite(void *pVoid)
{
	u8	result = 0;
	u32	cmd, data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	cmd = CMD_StrToUL(pData->pArgv[0]);

	data = CMD_StrToUL(pData->pArgv[1]);

	FPGA_WriteSingle(cmd, data);

	CMD_Printf(";CMD=0x%08X,DATA=0x%08X", cmd, data);

	result = 1;

	return result;
}

u8 Cmd_FpgaPTG(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;
#if 1 // yeom_20200803 [start]
	if(CMD_Compare(pData->pArgv[0], "enable"))
	{
		FPGA_WriteSingle(FPGA_CMD_PTG_ENABLE, 3);
		
		CMD_Printf(";ENABLE");
	}
#endif // yeom_20200803 [end]

	result = 1;

	return result;
}

u8 Cmd_FpgaBUSPath(void *pVoid)
{
	u8	result = 0;
	u32	path;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "pg"))					path = 0;
	else if(CMD_Compare(pData->pArgv[0], "core"))			path = 1;

	FPGA_WriteSingle(FPGA_CMD_CORE_BUS_BYPASS, path);

	CMD_Printf(";CMD=0x%08X,DATA=0x%08X", FPGA_CMD_CORE_BUS_BYPASS, path);

	result = 1;

	return result;
}

u8 Cmd_FpgaUpdateCheck(void *pVoid)
{
	u8	result = 0;
	u32	regData, mask = 0x10000000;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	regData = FPGA_ReadSingle(FPGA_CMD_ID);
	regData &= mask;
	regData = regData >> 28;

	CMD_Printf(";DATA=%d", regData);

	result = 1;

	return result;
}
