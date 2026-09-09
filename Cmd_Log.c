#include	"Cmd_Log.h"

static u8 Cmd_LogSave(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LOG_Save();

	return result;
}

static u8 Cmd_LogEnable(void *pVoid)
{
	//sysConfig_t cpy;
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	configE7602PG.logStatus = logUserEnable = 1;	

#if 0
	memcpy(cpy.u8Data, sysConfig.u8Data, sizeof(sysConfig_t));	

	System_SysConfigWrite(&cpy);
	System_SysConfigRead(&cpy);
	result = System_SysConfigCheck(&cpy);	

	if(result)
	{
		memcpy(sysConfig.u8Data, cpy.u8Data, sizeof(sysConfig_t));
	}
#endif

	result = 1;

	return result;
}

static u8 Cmd_LogDisable(void *pVoid)
{
	//sysConfig_t cpy;
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	configE7602PG.logStatus = logUserEnable = 0;	

#if 0
	memcpy(cpy.u8Data, sysConfig.u8Data, sizeof(sysConfig_t));	

	System_SysConfigWrite(&cpy);
	System_SysConfigRead(&cpy);
	result = System_SysConfigCheck(&cpy);	

	if(result)
	{
		memcpy(sysConfig.u8Data, cpy.u8Data, sizeof(sysConfig_t));
	}
#endif

	result = 1;
	
	return result;
}

static u8 Cmd_LogDebugSave(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = DBG_Save();	

	return result;
}

static u8 Cmd_LogDebugEnable(void *pVoid)
{
	//sysConfig_t cpy;
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	configE7602PG.debugStatus = logDebugEnable = 1;	

#if 0
	memcpy(cpy.u8Data, sysConfig.u8Data, sizeof(sysConfig_t));

	System_SysConfigWrite(&cpy);
	System_SysConfigRead(&cpy);
	result = System_SysConfigCheck(&cpy);	

	if(result)
	{
		memcpy(sysConfig.u8Data, cpy.u8Data, sizeof(sysConfig_t));
	}
#endif

	result = 1;
	
	return result;
}

static u8 Cmd_LogDebugDisable(void *pVoid)
{
	//sysConfig_t cpy;
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	configE7602PG.debugStatus = logDebugEnable = 0;

#if 0
	memcpy(cpy.u8Data, sysConfig.u8Data, sizeof(sysConfig_t));
	
	System_SysConfigWrite(&cpy);
	System_SysConfigRead(&cpy);
	result = System_SysConfigCheck(&cpy);	

	if(result)
	{
		memcpy(sysConfig.u8Data, cpy.u8Data, sizeof(sysConfig_t));
	}
#endif

	result = 1;
	
	return result;
}

const	commandList_t	cmdList_Log[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note},
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"save",					Cmd_LogSave,				NULL,		NULL,		";SAVE",				NULL},
												{	"enable",				Cmd_LogEnable,				NULL,		NULL,		";ENABLE",				NULL},
												{	"disable",				Cmd_LogDisable,				NULL,		NULL,		";DISABLE",				NULL},
												{	"debug.save",			Cmd_LogDebugSave,			NULL,		NULL,		";DEBUG.SAVE",			NULL},
												{	"debug.enable",			Cmd_LogDebugEnable,			NULL,		NULL,		";DEBUG.ENABLE",		NULL},
												{	"debug.enable",			Cmd_LogDebugDisable,		NULL,		NULL,		";DEBUG.DISABLE",		NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};