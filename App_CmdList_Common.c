#include	"App_CmdList_eDPm.h"

u8 Cmd_Board(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_Board);

	return result;
}

u8 Cmd_File(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_FILE);

	return result;
}

u8 Cmd_FPGA(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_FPGA);

	return result;
}

u8 Cmd_IMAGE(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_IMAGE);

	return result;
}

u8 Cmd_System(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_System);

	return result;
}

u8 Cmd_Test(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_Test);

	return result;
}

u8 Cmd_Delay(void *pVoid)
{
	u8	result = 0;
	u32	dlyCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	dlyCnt = CMD_StrToUL(pData->pArgv[0]);
	if(dlyCnt >= 10000)			dlyCnt = 10000;

	HAL_Delay(dlyCnt);

	result = 1;

	return result;
}

#if 1 // yeom_20210118 [start]
u8 Cmd_Log(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_Log);

	return result;
}
#endif // yeom_20210118 [end]


