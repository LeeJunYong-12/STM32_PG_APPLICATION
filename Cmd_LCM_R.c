#include	"Cmd_LCM_R.h"

static u8 Cmd_LcmSpiInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_R_SpiInit(NULL);

	return result;
}

static u8 Cmd_LcmSpiDeInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_R_SpiDeInit();

	return result;
}

static u8 Cmd_LcmSpiCsCtrl(void *pVoid)
{
	u8	result = 0;
	u8	position, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)							return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))			position = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	position = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	position = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	position = 3;
	else											return result;

	if(CMD_Compare(pData->pArgv[1], "low"))			state = 0;
	else if(CMD_Compare(pData->pArgv[1], "high"))	state = 1;
	else											return result;

	result = LCM_R_SpiCsCtrl(position, state);

	return result;
}

static u8 Cmd_LcmSpiWrite(void *pVoid)
{
	u8	result = 0;
	u8	*pTxBuffer;
	u32	cnt, txSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	txSize = pData->argc;
	if(txSize > 256)			return result;

	pTxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	for(cnt = 0; cnt < txSize; cnt++)
	{
		pTxBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt]);
	}

	result = LCM_R_SpiTransmitData(pTxBuffer, txSize);

	return result;
}

static u8 Cmd_LcmSpiRead(void *pVoid)
{
	u8	result = 0;
	u8	*pRxBuffer;
	u32	cnt,  rxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	rxSize = CMD_StrToUL(pData->pArgv[0]);
	if(rxSize > 256)			return result;

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = LCM_R_SpiReceiveData(pRxBuffer, rxSize);

	if(result)
	{
		CMD_Printf(";");
		for(cnt = 0; cnt < rxSize; cnt++)
		{
			if(cnt == (rxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else					CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_LcmSpiWriteRead(void *pVoid)
{
	u8	result = 0;
	u8	*pTxBuffer, *pRxBuffer;
	u32	cnt, trxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	trxSize = pData->argc;
	if(trxSize > 256)			return result;

	pTxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;
	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_2_ADRS;

	for(cnt = 0; cnt < trxSize; cnt++)
	{
		pTxBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt]);
	}

	result = LCM_R_SpiTransmitReceiveData(pTxBuffer, pRxBuffer, trxSize);

	if(result)
	{
		CMD_Printf(";");
		for(cnt = 0; cnt < trxSize; cnt++)
		{
			if(cnt == (trxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else						CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_LcmSpiWriteCustom(void *pVoid)
{
	u8	result = 0;
	u8	*pTxBuffer, position;
	u32	cnt, txSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 2)								return result;

	txSize = pData->argc - 1;
	if(txSize > 256)								return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))			position = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	position = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	position = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	position = 3;
	else											return result;

	pTxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	for(cnt = 0; cnt < txSize; cnt++)
	{
		pTxBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
	}

	result = LCM_R_SpiTransmitDataCustom(position, pTxBuffer, txSize);

	return result;
}

static u8 Cmd_LcmSpiReadCustom(void *pVoid)
{
	u8	result = 0;
	u8	*pTxBuffer, *pRxBuffer, position;
	u32	cnt,  txSize, rxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 2)								return result;

	pTxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;
	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_2_ADRS;

	txSize = pData->argc - 2;
	if(txSize > 256)								return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))			position = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	position = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	position = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	position = 3;
	else											return result;
	
	if(txSize != 0)
	{
		for(cnt = 0; cnt < txSize; cnt++)
		{
			pTxBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
		}
	}
	
	rxSize = CMD_StrToUL(pData->pArgv[pData->argc - 1]);
	if(rxSize > 256)			return result;

	result = LCM_R_SpiReceiveDataCustom(position, pTxBuffer, pRxBuffer, txSize, rxSize);

	if(result)
	{
		CMD_Printf(";CH%d=", (position + 1));
		for(cnt = 0; cnt < rxSize; cnt++)
		{
			if(cnt == (rxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else					CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_LcmSpiWriteReadCustom(void *pVoid)
{
	u8	result = 0;
	u8	*pTxBuffer, *pRxBuffer, position;
	u32	cnt, trxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)							return result;

	trxSize = pData->argc - 1;
	if(trxSize > 256)								return result;

	pTxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;
	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_2_ADRS;

	if(CMD_Compare(pData->pArgv[0], "ch1"))			position = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	position = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	position = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	position = 3;
	else											return result;

	for(cnt = 0; cnt < trxSize; cnt++)
	{
		pTxBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
	}

	result = LCM_R_SpiTransmitReceiveDataCustom(position, pTxBuffer, pRxBuffer, trxSize);

	if(result)
	{
		CMD_Printf(";CH%d=", (position + 1));
		for(cnt = 0; cnt < trxSize; cnt++)
		{
			if(cnt == (trxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else						CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_LcmI2CInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_R_I2CInit();

	return result;
}

static u8 Cmd_LcmI2CDeInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_R_I2CDeInit();

	return result;
}

static u8 Cmd_LcmI2CGetReady(void *pVoid)
{
	u8	result = 0;
	u8	channel, slaveAdrs, ready, position;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)							return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))			channel = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	channel = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	channel = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	channel = 3;
	else											return result;

	position = pData->option - 1;

	slaveAdrs = CMD_StrToUL(pData->pArgv[1]);

	result = LCM_R_I2CGetReady(position, channel, slaveAdrs, &ready);

	if(result)
	{
		if(ready)	CMD_Printf(";CH%d=I2C%d 0x%02X Device is Ready", (position + 1), channel, slaveAdrs);
		else		CMD_Printf(";CH%d=I2C%d 0x%02X Device is Empty(Busy)", (position + 1), channel, slaveAdrs);
	}

	return result;
}

static u8 Cmd_LcmI2CScanAdrs(void *pVoid)
{
	u8	result = 0;
	u8	channel, buffer[256], rCnt, position;
	u8	cnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)							return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))			channel = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	channel = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	channel = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	channel = 3;
	else											return result;

	position = pData->option - 1;

	result = LCM_R_I2CGetSlaveAdrs(position, channel, buffer, &rCnt);

	if(result)
	{
		if(rCnt == 0)		CMD_Printf(";NO_DATA");
		else
		{
			CMD_Printf(";CH%d=", (position + 1));
			for(cnt = 0; cnt < rCnt; cnt++)
			{
				if(cnt != (rCnt - 1))	CMD_Printf("0x%02X ", buffer[cnt]);
				else					CMD_Printf("0x%02X", buffer[cnt]);
			}
		}
	}

	return result;
}

static u8 Cmd_LcmI2CWrite(void *pVoid)
{
	u8	result = 0;
	u8	channel, slaveAdrs, position;
	u8	*pTxBuffer;
	u32	cnt, txSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 3)								return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))			channel = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	channel = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	channel = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	channel = 3;
	else											return result;
	
	position = pData->option - 1;

	slaveAdrs = CMD_StrToUL(pData->pArgv[1]);

	txSize = pData->argc - 2;

	pTxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	for(cnt = 0; cnt < txSize; cnt++)
	{
		pTxBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 2]);
	}

	result = LCM_R_I2CTransmitData(position, channel, slaveAdrs, pTxBuffer, txSize);

	return result;
}

static u8 Cmd_LcmI2CRead(void *pVoid)
{
	u8	result = 0;
	u8	channel, slaveAdrs, position;
	u8	*pRxBuffer;
	u32	cnt, rxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)							return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))			channel = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	channel = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	channel = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	channel = 3;
	else											return result;

	position = pData->option - 1;

	slaveAdrs = CMD_StrToUL(pData->pArgv[1]);
	rxSize = CMD_StrToUL(pData->pArgv[2]);

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = LCM_R_I2CReceiveData(position, channel, slaveAdrs, pRxBuffer, rxSize);

	if(result)
	{
		CMD_Printf(";CH%d=", (position + 1));
		for(cnt = 0; cnt < rxSize; cnt++)
		{
			if(cnt == (rxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else					CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

static u8 Cmd_LcmI2CReadCustom(void *pVoid)
{
	u8	result = 0;
	u8	channel, slaveAdrs, mode, position;
	u8	*pRxBuffer;
	u16	memAdrs;
	u32	cnt, rxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if((pData->argc != 4) && (pData->argc != 5))		return result;

	if(CMD_Compare(pData->pArgv[0], "ch1"))				channel = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))		channel = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))		channel = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))		channel = 3;
	else												return result;

	position = pData->option - 1;

	slaveAdrs = CMD_StrToUL(pData->pArgv[1]);

	if(pData->argc == 3)
	{
		memAdrs = CMD_StrToUL(pData->pArgv[2]);

		rxSize = CMD_StrToUL(pData->pArgv[3]);

		mode = 0;
	}
	else if(pData->argc == 4)
	{
		memAdrs = CMD_StrToUL(pData->pArgv[2]);
		memAdrs <<= 8;
		memAdrs |= CMD_StrToUL(pData->pArgv[3]) & 0x00ff;

		rxSize = CMD_StrToUL(pData->pArgv[4]);

		mode = 1;
	}
	else												return result;

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = LCM_R_I2CMemRead(position, channel, slaveAdrs, memAdrs, mode, pRxBuffer, rxSize);

	if(result)
	{
		CMD_Printf(";CH%d=", (position + 1));
		for(cnt = 0; cnt < rxSize; cnt++)
		{
			if(cnt == (rxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else					CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

const	commandList_t	cmdList_LCM_R[] =	{
//											{	Command,				Function,					Option,		Mode,		RootMessage				Note},
											{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
											{	"spi.init",				Cmd_LcmSpiInit,				NULL,		NULL,		";SPI.INIT",			NULL},
											{	"spi.deinit",			Cmd_LcmSpiDeInit,			NULL,		NULL,		";SPI.DEINIT",			NULL},
											{	"spi.csctrl",			Cmd_LcmSpiCsCtrl,			NULL,		NULL,		";SPI.CSCTRL",			NULL},
											{	"spi.write",			Cmd_LcmSpiWrite,			NULL,		NULL,		";SPI.WRITE",			NULL},
											{	"spi.read",				Cmd_LcmSpiRead,				NULL,		NULL,		";SPI.READ",			NULL},
											{	"spi.write_read",		Cmd_LcmSpiWriteRead,		NULL,		NULL,		";SPI.WRITE_READ",		NULL},
											{	"spi.write_c",			Cmd_LcmSpiWriteCustom,		NULL,		NULL,		";SPI.WRITE_C",			NULL},
											{	"spi.read_c",			Cmd_LcmSpiReadCustom,		NULL,		NULL,		";SPI.READ_C",			NULL},
											{	"spi.write_read_c",		Cmd_LcmSpiWriteReadCustom,	NULL,		NULL,		";SPI.WRITE_READ_C",	NULL},
											{	"i2c.init",				Cmd_LcmI2CInit,				NULL,		NULL,		";I2C0.INIT",			NULL},
											{	"i2c.deinit",			Cmd_LcmI2CDeInit,			NULL,		NULL,		";I2C0.DEINIT",			NULL},
											{	"i2c0.ready",			Cmd_LcmI2CGetReady,			0x01,		NULL,		";I2C0.READY",			NULL},
											{	"i2c0.scan",			Cmd_LcmI2CScanAdrs,			0x01,		NULL,		";I2C0.SCAN",			NULL},
											{	"i2c0.write",			Cmd_LcmI2CWrite,			0x01,		NULL,		";I2C0.WRITE",			NULL},
											{	"i2c0.read",			Cmd_LcmI2CRead,				0x01,		NULL,		";I2C0.READ",			NULL},
											{	"i2c0.read_c",			Cmd_LcmI2CReadCustom,		0x01,		NULL,		";I2C0.REAC_C",			NULL},
											{	"i2c1.ready",			Cmd_LcmI2CGetReady,			0x02,		NULL,		";I2C1.READY",			NULL},
											{	"i2c1.scan",			Cmd_LcmI2CScanAdrs,			0x02,		NULL,		";I2C1.SCAN",			NULL},
											{	"i2c1.write",			Cmd_LcmI2CWrite,			0x02,		NULL,		";I2C1.WRITE",			NULL},
											{	"i2c1.read",			Cmd_LcmI2CRead,				0x02,		NULL,		";I2C1.READ",			NULL},
											{	"i2c1.read_c",			Cmd_LcmI2CReadCustom,		0x02,		NULL,		";I2C1.READ_C",			NULL},
											{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
										};

