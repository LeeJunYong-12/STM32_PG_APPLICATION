#include	"Cmd_eDP_R.h"

static u8 Cmd_eDPI2CAdrsScan(void *pVoid)
{
	u8	result = 0;
	u8	adrsBuffer[128], adrsCnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)	return result;

	memset(adrsBuffer, NULL, sizeof(adrsBuffer));

	result = eDP_I2CAdrsScan(adrsBuffer, &adrsCnt);
	if(adrsCnt == 0)
	{
		CMD_Printf(";NO_RESPONSE");
	}
	else
	{
		CMD_Printf(";");

		for(u8 cnt = 0; cnt < adrsCnt; cnt++)
		{
			if(cnt != (adrsCnt - 1))	CMD_Printf("0x%02X ", adrsBuffer[cnt]);
			else						CMD_Printf("0x%02X", adrsBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_eDPI2CWrite(void *pVoid)
{
	u8	result = 0;
	u8	slaveAdrs, *pBuffer;
	u16	cnt, dataCnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 2)		return result;

	slaveAdrs = CMD_StrToUL(pData->pArgv[0]);

	dataCnt = pData->argc - 1;

	if(dataCnt > 128)		return result;

	pBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	for(cnt = 0; cnt < dataCnt; cnt++)
	{
		pBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
	}

	result = eDP_I2CWrite(slaveAdrs, pBuffer, dataCnt);

	return result;
}

static u8 Cmd_eDPI2CRead(void *pVoid)
{
	u8	result = 0;
	u8	slaveAdrs, *pBuffer;
	u16	wData = 0, dataCnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	pBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	if(pData->argc == 2)		//	slaveAdrs, readCnt
	{
		slaveAdrs	= CMD_StrToUL(pData->pArgv[0]);
		dataCnt		= CMD_StrToUL(pData->pArgv[1]);
		if(dataCnt > 128)		return result;

		result		= eDP_I2CRead(slaveAdrs, pBuffer, dataCnt);
	}
	else if(pData->argc == 3)	//	slaveAdrs, wData0, readCnt
	{
		slaveAdrs	= CMD_StrToUL(pData->pArgv[0]);
		wData		|= CMD_StrToUL(pData->pArgv[1]) & 0x00FF;
		dataCnt		= CMD_StrToUL(pData->pArgv[2]);
		if(dataCnt > 128)		return result;

		result		= eDP_I2CMemRead(slaveAdrs, wData, 0, pBuffer, dataCnt);
	}
	else if(pData->argc == 4)	//	slaveAdrs, wData0, wData1, readCnt
	{
		slaveAdrs	= CMD_StrToUL(pData->pArgv[0]);
		wData		= (CMD_StrToUL(pData->pArgv[1]) << 8) | CMD_StrToUL(pData->pArgv[2]);
		dataCnt		= CMD_StrToUL(pData->pArgv[3]);
		if(dataCnt > 128)		return result;

		result		= eDP_I2CMemRead(slaveAdrs, wData, 1, pBuffer, dataCnt);
	}
	else						return result;

	if(result)
	{
		CMD_Printf(";");		

		for(u16 cnt = 0; cnt < dataCnt; cnt++)
		{ 
			if(cnt != (dataCnt - 1))	CMD_Printf("0x%02X ",	pBuffer[cnt]);
			else						CMD_Printf("0x%02X",	pBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_eDPRegWrite(void *pVoid)
{
	u8	result = 0;
	u8	adrs, data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	data = CMD_StrToUL(pData->pArgv[1]);

	result = eDP_RegWrite(adrs, &data, 1);
	if(result)		CMD_Printf(";ADRS=0x%02X,DATA=0x%02X", adrs, data);

	return result;
}

static u8 Cmd_eDPRegRead(void *pVoid)
{
	u8	result = 0;
	u8	adrs, data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);

	result = eDP_RegRead(adrs, &data, 1);
	if(result)		CMD_Printf(";ADRS=0x%02X,DATA=0x%02X", adrs, data);

	return result;
}


static u8 Cmd_eDPGetEDID(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	u8	*pBuffer;
	u16	cnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)	return result;

	channel = CMD_StrToUL(pData->pArgv[0]);
	if(channel > 4)			return result;

	pBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = eDPMC_GetEDID(channel, pBuffer);
	if(result)
	{
		CMD_Printf(";");

		for(cnt = 0; cnt < 128; cnt++)
		{
			if(cnt != 127)				CMD_Printf("0x%02X ",	pBuffer[cnt]);
			else						CMD_Printf("0x%02X",	pBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_eDPReadAUX(void *pVoid)
{
	u8	result = 0;
	u8	channel, *pBuffer;
	u32	adrs, size, cnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)	return result;

	channel = CMD_StrToUL(pData->pArgv[0]);
	if(channel == 0)		return result;
	else					channel -= 1;

	if(channel >= 4)		return result;

	adrs = CMD_StrToUL(pData->pArgv[1]);
	size = CMD_StrToUL(pData->pArgv[2]);
	if(size > 16)			return result;

	pBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = eDPMC_ReadAUX(channel, adrs, pBuffer, size);
	if(result)
	{
		CMD_Printf(";ADRS=0x%08X,DATA=", adrs);

		for(cnt = 0; cnt < size; cnt++)
		{
			if(cnt != (size - 1))	CMD_Printf("0x%02X ", pBuffer[cnt]);
			else					CMD_Printf("0x%02X", pBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_eDPWriteAUX(void *pVoid)
{
	u8	result = 0;
	u8	channel, *pBuffer;
	u32	adrs, size, cnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 3)	return result;

	channel = CMD_StrToUL(pData->pArgv[0]);
	if(channel == 0)		return result;
	else					channel -= 1;

	adrs = CMD_StrToUL(pData->pArgv[1]);
	size = pData->argc - 2;
	if(size > 16)			return result;

	pBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	for(cnt = 0; cnt < size; cnt++)
	{
		pBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 2]);
	}

	result = eDPMC_WriteAUX(channel, adrs, pBuffer, size);

	return result;
}


const	commandList_t	cmdList_eDP_R[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"i2c.scan",				Cmd_eDPI2CAdrsScan,			NULL,		NULL,		";I2C.SCAN",			NULL},
												{	"i2c.write",			Cmd_eDPI2CWrite,			NULL,		NULL,		";I2C.WRITE",			NULL},
												{	"i2c.read",				Cmd_eDPI2CRead,				NULL,		NULL,		";I2C.READ",			NULL},
												{	"reg.write",			Cmd_eDPRegWrite,			NULL,		NULL,		";REG.WRITE",			NULL},
												{	"reg.read",				Cmd_eDPRegRead,				NULL,		NULL,		";REG.READ",			NULL},
												{	"edid.get",				Cmd_eDPGetEDID,				NULL,		NULL,		";EDID.GET",			NULL},
												{	"aux.read",				Cmd_eDPReadAUX,				NULL,		NULL,		";AUX.READ",			NULL},
												{	"aux.write",			Cmd_eDPWriteAUX,			NULL,		NULL,		";AUX.WRITE",			NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};

