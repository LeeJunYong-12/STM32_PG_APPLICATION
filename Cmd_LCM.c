#include	"Cmd_LCM.h"

char P_1 = 0x47, P_2 = 0x38, P_3 = 0x51; 
char Y;
char W1, W2;
char D;
char S1, S2, S3, S4;
char E1 = 0x50, E2 = 0x4e, E3 = 0x44, E4 = 0x38;
u8 E1_copy = 23, E2_copy = 22, E3_copy = 13, E4_copy = 8;
u8 P1_copy = 16, P2_copy = 8, P3_copy = 24;
char R= 0x30;
char X;
u8 ptxbuffer[17] = {0, };
u8 prxbuffer[17] = {0, };
u8 setpppflag = 0;
u8 seteeeeflag = 0;


const	commandList_t	cmdList_LCM[] =	{
//											{	Command,				Function,					Option,		Mode,		RootMessage						Note},
											{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",						NULL},
											{	"spi.init",				Cmd_LcmSpiInit,				NULL,		NULL,		";SPI.INIT",					NULL},
											{	"spi.deinit",			Cmd_LcmSpiDeInit,			NULL,		NULL,		";SPI.DEINIT",					NULL},
											{	"spi.spimode",			Cmd_LcmSpiInit,				NULL,		NULL,		";SPI.SPIMODE",					NULL},
											{	"spi.iomode",			Cmd_LcmSpiDeInit,			NULL,		NULL,		";SPI.IOMODE",					NULL},
											{	"spi.csctrl",			Cmd_LcmSpiCsCtrl,			NULL,		NULL,		";SPI.CSCTRL",					NULL},
											{	"spi.write",			Cmd_LcmSpiWrite,			NULL,		NULL,		";SPI.WRITE",					NULL},
											{	"spi.read",				Cmd_LcmSpiRead,				NULL,		NULL,		";SPI.READ",					NULL},
											{	"spi.write_read",		Cmd_LcmSpiWriteRead,		NULL,		NULL,		";SPI.WRITE_READ",				NULL},
											{	"spi.write_c",			Cmd_LcmSpiWriteCustom,		NULL,		NULL,		";SPI.WRITE_C",					NULL},
											{	"spi.read_c",			Cmd_LcmSpiReadCustom,		NULL,		NULL,		";SPI.READ_C",					NULL},
											{	"spi.write_read_c",		Cmd_LcmSpiWriteReadCustom,	NULL,		NULL,		";SPI.WRITE_READ_C",			NULL},
											{	"spi.iowrite",			Cmd_LcmSpiIoWrite,			NULL,		NULL,		";SPI.IOWRITE",					NULL},
											{	"spi.ioread",			Cmd_LcmSpiIoRead,			NULL,		NULL,		";SPI.IOREAD",					NULL},
											{	"i2c0.init",			Cmd_LcmI2CInit,				0x01,		NULL,		";I2C0.INIT",					NULL},
											{	"i2c0.deinit",			Cmd_LcmI2CDeInit,			0x01,		NULL,		";I2C0.DEINIT",					NULL},
											{	"i2c0.i2cmode",			Cmd_LcmI2CInit,				0x01,		NULL,		";I2C0.I2CMODE",				NULL},
											{	"i2c0.iomode",			Cmd_LcmI2CDeInit,			0x01,		NULL,		";I2C0.IOMODE",					NULL},
											{	"i2c0.ready",			Cmd_LcmI2CGetReady,			0x01,		NULL,		";I2C0.READY",					NULL},
											{	"i2c0.scan",			Cmd_LcmI2CScanAdrs,			0x01,		NULL,		";I2C0.SCAN",					NULL},
											{	"i2c0.write",			Cmd_LcmI2CWrite,			0x01,		NULL,		";I2C0.WRITE",					NULL},
											{	"i2c0.read",			Cmd_LcmI2CRead,				0x01,		NULL,		";I2C0.READ",					NULL},
											{	"i2c0.read_c",			Cmd_LcmI2CReadCustom,		0x01,		NULL,		";I2C0.REAC_C",					NULL},
											{	"i2c0.iodir",			Cmd_LcmI2CIoDirection,		0x01,		NULL,		";I2C0.IODIR",					NULL},
											{	"i2c0.iowrite",			Cmd_LcmI2CIoWrite,			0x01,		NULL,		";I2C0.IOWRITE",				NULL},
 											{	"i2c0.ioread",			Cmd_LcmI2CIoRead,			0x01,		NULL,		";I2C0.IOREAD",					NULL},
 											{	"i2c1.init",			Cmd_LcmI2CInit,				0x02,		NULL,		";I2C1.INIT",					NULL},
 											{	"i2c1.deinit",			Cmd_LcmI2CDeInit,			0x02,		NULL,		";I2C1.DEINIT",					NULL},
 											{	"i2c1.i2cmode",			Cmd_LcmI2CInit,				0x02,		NULL,		";I2C1.I2CMODE",				NULL},
 											{	"i2c1.iomode",			Cmd_LcmI2CDeInit,			0x02,		NULL,		";I2C1.IOMODE",					NULL},
 											{	"i2c1.ready",			Cmd_LcmI2CGetReady,			0x02,		NULL,		";I2C1.READY",					NULL},
 											{	"i2c1.scan",			Cmd_LcmI2CScanAdrs,			0x02,		NULL,		";I2C1.SCAN",					NULL},
 											{	"i2c1.write",			Cmd_LcmI2CWrite,			0x02,		NULL,		";I2C1.WRITE",					NULL},
 											{	"i2c1.read",			Cmd_LcmI2CRead,				0x02,		NULL,		";I2C1.READ",					NULL},
 											{	"i2c1.read_c",			Cmd_LcmI2CReadCustom,		0x02,		NULL,		";I2C1.READ_C",					NULL},
 											{	"i2c1.iodir",			Cmd_LcmI2CIoDirection,		0x02,		NULL,		";I2C1.IODIR",					NULL},
 											{	"i2c1.iowrite",			Cmd_LcmI2CIoWrite,			0x02,		NULL,		";I2C1.IOWRITE",				NULL},
 											{	"i2c1.ioread",			Cmd_LcmI2CIoRead,			0x02,		NULL,		";I2C1.IOREAD",					NULL},
 											{	"serial.create",		SerialCreate,				0x01,		NULL,		";I2C0.SERIAL",					NULL},
											{	"serial.set.ppp",		Serialppp,					0x01,		NULL,		";I2C0.SERIAL.set.ppp",			NULL},
											{	"serial.set.eeee",		Serialeeee,					0x01,		NULL,		";I2C0.SERIAL.set.eeee",		NULL},
											{	"serial.save",			SerialSave,					0x01,		NULL,		";I2C0.SERIAL.SAVE",			NULL},
											{	NULL,					NULL,						NULL,		NULL,		NULL,							NULL},
										};

u8 Cmd_LcmSpiInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_SpiInit(pData->position, NULL);

	return result;
}

u8 Cmd_LcmSpiDeInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_SpiDeInit(pData->position);

	return result;
}

u8 Cmd_LcmSpiCsCtrl(void *pVoid)
{
	u8	result = 0;
	u8	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "low"))
	{
		state = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "high"))
	{
		state = 1;
	}
	else						return result;

	result = LCM_SpiCsCtrl(pData->position, state);

	return result;
}

u8 Cmd_LcmSpiWrite(void *pVoid)
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

	result = LCM_SpiTransmitData(pData->position, pTxBuffer, txSize);

	return result;
}

u8 Cmd_LcmSpiRead(void *pVoid)
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

	result = LCM_SpiReceiveData(pData->position, pRxBuffer, rxSize);

	if(result)
	{
		CMD_Printf(";CH%d ", pData->channel);
		for(cnt = 0; cnt < rxSize; cnt++)
		{
			if(cnt == (rxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else					CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

u8 Cmd_LcmSpiWriteRead(void *pVoid)
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

	result = LCM_SpiTransmitReceiveData(pData->position, pTxBuffer, pRxBuffer, trxSize);

	if(result)
	{
		CMD_Printf(";CH%d ", pData->channel);
		for(cnt = 0; cnt < trxSize; cnt++)
		{
			if(cnt == (trxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else						CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

u8 Cmd_LcmSpiWriteCustom(void *pVoid)
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

	result = LCM_SpiTransmitDataCustom(pData->position, pTxBuffer, txSize);

	return result;
}

u8 Cmd_LcmSpiReadCustom(void *pVoid)
{
	u8	result = 0;
	u8	*pTxBuffer, *pRxBuffer;
	u32	cnt,  txSize, rxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	pTxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;
	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_2_ADRS;

	txSize = pData->argc - 1;
	if(txSize > 256)			return result;

	if(txSize != 0)
	{
		for(cnt = 0; cnt < txSize; cnt++)
		{
			pTxBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt]);
		}
	}

	rxSize = CMD_StrToUL(pData->pArgv[pData->argc - 1]);
	if(rxSize > 256)			return result;

	result = LCM_SpiReceiveDataCustom(pData->position, pTxBuffer, pRxBuffer, txSize, rxSize);

	if(result)
	{
		CMD_Printf(";CH%d ", pData->channel);
		for(cnt = 0; cnt < rxSize; cnt++)
		{
			if(cnt == (rxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else					CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

u8 Cmd_LcmSpiWriteReadCustom(void *pVoid)
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

	result = LCM_SpiTransmitReceiveDataCustom(pData->position, pTxBuffer, pRxBuffer, trxSize);

	if(result)
	{
		CMD_Printf(";CH%d ", pData->channel);
		for(cnt = 0; cnt < trxSize; cnt++)
		{
			if(cnt == (trxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else						CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

u8 Cmd_LcmSpiIoWrite(void *pVoid)
{
	u8	result = 0;
	u8	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);

	result = LCM_SpiIoWrite(pData->position, state);

	return result;
}

u8 Cmd_LcmSpiIoRead(void *pVoid)
{
	u8	result = 0;
	u8	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = LCM_SpiIoRead(pData->position, &state);

	if(result)
	{
		CMD_Printf(";CH%d 0x%02X", pData->channel, state);
	}

	return result;
}

u8 Cmd_LcmI2CInit(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	channel = pData->option - 1;

	result = LCM_I2CInit(pData->position, channel);

	return result;
}

u8 Cmd_LcmI2CDeInit(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	channel = pData->option - 1;

	result = LCM_I2CDeInit(pData->position, channel);

	return result;
}

u8 Cmd_LcmI2CGetReady(void *pVoid)
{
	u8	result = 0;
	u8	channel, slaveAdrs, ready;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	slaveAdrs = CMD_StrToUL(pData->pArgv[0]);

	channel = pData->option - 1;

	result = LCM_I2CGetReady(pData->position, channel, slaveAdrs, &ready);

	if(result)
	{
		if(ready)		CMD_Printf(";CH%d I2C%d 0x%02X Device is Ready", pData->channel, channel, slaveAdrs);
		else			CMD_Printf(";CH%d I2C%d 0x%02X Device is Empty(Busy)", pData->channel, channel, slaveAdrs);
	}

	return result;
}

u8 Cmd_LcmI2CScanAdrs(void *pVoid)
{
	u8	result = 0;
	u8	channel, buffer[256], rCnt;
	u8	cnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	channel = pData->option - 1;

	result = LCM_I2CGetSlaveAdrs(pData->position, channel, buffer, &rCnt);

	if(result)
	{
		if(rCnt == 0)		CMD_Printf(";NO_DATA");
		else
		{
			CMD_Printf(";CH%d ", pData->channel);
			for(cnt = 0; cnt < rCnt; cnt++)
			{
				if(cnt != (rCnt - 1))	CMD_Printf("0x%02X ", buffer[cnt]);
				else					CMD_Printf("0x%02X", buffer[cnt]);
			}
		}
	}

	return result;
}

u8 Cmd_LcmI2CWrite(void *pVoid)
{
	u8	result = 0;
	u8	channel, slaveAdrs;
	u8	*pTxBuffer;
	u32	cnt, txSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	channel = pData->option - 1;

	slaveAdrs = CMD_StrToUL(pData->pArgv[0]);

	txSize = pData->argc - 1;

	pTxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	for(cnt = 0; cnt < txSize; cnt++)
	{
		pTxBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
	}

	result = LCM_I2CTransmitData(pData->position, channel, slaveAdrs, pTxBuffer, txSize);

	return result;
}

u8 Cmd_LcmI2CRead(void *pVoid)
{
	u8	result = 0;
	u8	channel, slaveAdrs;
	u8	*pRxBuffer;
	u32	cnt, rxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	channel = pData->option - 1;

	slaveAdrs = CMD_StrToUL(pData->pArgv[0]);
	rxSize = CMD_StrToUL(pData->pArgv[1]);

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = LCM_I2CReceiveData(pData->position, channel, slaveAdrs, pRxBuffer, rxSize);

	if(result)
	{
		CMD_Printf(";CH%d ", pData->channel);
		for(cnt = 0; cnt < rxSize; cnt++)
		{
			if(cnt == (rxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else					CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

u8 Cmd_LcmI2CReadCustom(void *pVoid)
{
	u8	result = 0;
	u8	channel, slaveAdrs, mode;
	u8	*pRxBuffer;
	u16	memAdrs;
	u32	cnt, rxSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if((pData->argc != 3) && (pData->argc != 4))		return result;

	channel = pData->option - 1;

	slaveAdrs = CMD_StrToUL(pData->pArgv[0]);

	if(pData->argc == 3)
	{
		memAdrs = CMD_StrToUL(pData->pArgv[1]);

		rxSize = CMD_StrToUL(pData->pArgv[2]);

		mode = 0;
	}
	else if(pData->argc == 4)
	{
		memAdrs = CMD_StrToUL(pData->pArgv[1]);
		memAdrs <<= 8;
		memAdrs |= CMD_StrToUL(pData->pArgv[2]) & 0x00ff;

		rxSize = CMD_StrToUL(pData->pArgv[3]);

		mode = 1;
	}
	else												return result;

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = LCM_I2CMemRead(pData->position, channel, slaveAdrs, memAdrs, mode, pRxBuffer, rxSize);

	if(result)
	{
		CMD_Printf(";CH%d ", pData->channel);
		for(cnt = 0; cnt < rxSize; cnt++)
		{
			if(cnt == (rxSize - 1))	CMD_Printf("0x%02X", pRxBuffer[cnt]);
			else					CMD_Printf("0x%02X ", pRxBuffer[cnt]);
		}
	}

	return result;
}

u8 Cmd_LcmI2CIoDirection(void *pVoid)
{
	u8	result = 0;
	u8	channel, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);
	if(state > 0x07)			return result;

	channel = pData->option - 1;

	result = LCM_I2CIoDirection(pData->position, channel, state);

	return result;
}

u8 Cmd_LcmI2CIoWrite(void *pVoid)
{
	u8	result = 0;
	u8	channel, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);
	if(state > 0x07)			return result;

	channel = pData->option - 1;

	result = LCM_I2CIoWrite(pData->position, channel, state);

	return result;
}

u8 Cmd_LcmI2CIoRead(void *pVoid)
{
	u8	result = 0;
	u8	channel, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	channel = pData->option - 1;

	result = LCM_I2CIoRead(pData->position, channel, &state);

	if(result)		CMD_Printf(";CH%d 0x%02X", pData->channel, state);

	return result;
}


u8 SerialCreate(void *pVoid)
{
	u8	result = 0, rtn;
	u8	channel, slaveAdrs, mode;
	u8	*pRxBuffer;
	u16	memAdrs;
	u32	rxSize;
	cmdFunctionData_t	*pData;
	u8 Y_copy;
	u8 W_copy, W1_copy, W2_copy;
	u8 D_copy;
	u8 S1_copy, S2_copy, S3_copy, S4_copy;
	//u8 P1_copy =16, P2_copy = 8, P3_copy = 24;
	u8 R_copy = 0;
	u16 x1, x2, x;
	u8 x_result;

	pData = (cmdFunctionData_t*)pVoid;

	channel = pData->option - 1;

	slaveAdrs = 0xA2;

	memAdrs = 0xFA;

	rxSize = 6;

	mode = 0;

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	rtn = LCM_I2CMemRead(pData->position, channel, slaveAdrs, memAdrs, mode, pRxBuffer, rxSize);
	if(rtn == 0)	return	result;	

//for the test
	//pRxBuffer[0] = 0x29;
	//pRxBuffer[1] = 0x41;
	//pRxBuffer[2] = 0x00;
	//pRxBuffer[3] = 0xB8;
	//pRxBuffer[4] = 0x96;
	//pRxBuffer[5] = 0xBA;
	//pRxBuffer[2] = 0x00;
	//pRxBuffer[3] = 0xB5;
	//pRxBuffer[4] = 0x2A;
	//pRxBuffer[5] = 0xF1;
//Y calculate
	if(pRxBuffer[2] == 0x00)
	{
		Y_copy=0;
	}
	else if(pRxBuffer[2] == 0x01)
	{
		Y_copy= 1;

	}
	else if(pRxBuffer[2] == 0x12)
	{
		Y_copy=8;

	}
	sprintf(&Y, "%d", Y_copy);
//WW calculate
	W_copy = (pRxBuffer[3]/7)+1;
	W1_copy = W_copy/10;
	W2_copy = W_copy-W1_copy*10;
	sprintf(&W1, "%d", W1_copy);
	sprintf(&W2, "%d", W2_copy);
//D calculate
	D_copy = pRxBuffer[3]%7;
	D_copy = D_copy +1;
	sprintf(&D, "%d", D_copy);
//SSSS calculate
	S1_copy = (pRxBuffer[4]>>4);
	S2_copy = (pRxBuffer[4]-(S1_copy<<4));
	S3_copy = (pRxBuffer[5]>>4);
	S4_copy = (pRxBuffer[5]-(S3_copy<<4));
	sprintf(&S1, "%x", S1_copy);
	if(S1 >0x60) S1=S1-32;
	sprintf(&S2, "%x", S2_copy);
	if(S2 >0x60) S2=S2-32;
	sprintf(&S3, "%x", S3_copy);
	if(S3 >0x60) S3=S3-32;
	sprintf(&S4, "%x", S4_copy);
	if(S4 >0x60) S4=S4-32;

//PPP, EEEE Transfer
	x1 = (P2_copy + Y_copy + W2_copy + S1_copy + S3_copy + E1_copy + E3_copy + R_copy)*3;
	x2 =  P1_copy + P3_copy + W1_copy + D_copy + S2_copy + S4_copy + E2_copy + E4_copy;
	x= x1+x2;
	x = x%34;
	x_result = (u8)(34-x);
	//x_result = 30;
	if(x_result>15)
	{
	   if(x_result == 16)
	   {
	     X='G';
	   }
	   if(x_result == 17)
	   {
	     X='H';
	   }
	   if(x_result == 18)
	   {
	     X='J';
	   }
	   if(x_result == 19)
	   {
	     X='K';
	   }
	   if(x_result == 20)
	   {
	     X='L';
	   }
	   if(x_result == 21)
	   {
	     X='M';
	   }
	   if(x_result == 22)
	   {
	     X='N';
	   }
	   if(x_result == 23)
	   {
	     X='P';
	   }
	   if(x_result == 24)
	   {
	     X='Q';
	   }
	   if(x_result == 25)
	   {
	     X='R';
	   }
	   if(x_result == 26)
	   {
	     X='S';
	   }
	   if(x_result == 27)
	   {
	     X='T';
	   }
	   if(x_result == 28)
	   {
	     X='U';
	   }
	   if(x_result == 29)
	   {
	     X='V';
	   }
	   if(x_result == 30)
	   {
	     X='W';
	   }
	   if(x_result == 31)
	   {
	     X='X';
	   }
	   if(x_result == 32)
	   {
	     X='Y';
	   }
	   if(x_result == 33)
	   {
	     X='Z';
	   }
	   if(x_result == 34)
	   {
	     X='0';
	   }
	}

	if(x_result<=15)
	{
	  sprintf(&X, "%x", x_result);
	  if(X >0x60) X=X-32;
	}


	ptxbuffer[0] = P_1; ptxbuffer[1] = P_2; ptxbuffer[2] = P_3; ptxbuffer[3] = Y; ptxbuffer[4] = W1; ptxbuffer[5] = W2; ptxbuffer[6] = D; ptxbuffer[7] = S1; ptxbuffer[8] = S2;
	ptxbuffer[9] = S3; ptxbuffer[10] = S4; ptxbuffer[11] = E1; ptxbuffer[12] = E2; ptxbuffer[13] = E3; ptxbuffer[14] = E4; ptxbuffer[15] = R; ptxbuffer[16] = X;

	for(u8 cnt = 0; cnt < 17; cnt++)
	{
		CMD_Printf("0x%02X ", ptxbuffer[cnt]);
	}

	result = 1;
	//result = LCM_I2CMemWrite(pData->position, channel, slaveAdrs, memAdrs, mode, pRxBuffer, rxSize);

	return result;
}

u8 SerialSave(void *pVoid)
{
	cmdFunctionData_t	*pData;
	u8	result = 0, rtn;
	u8	channel, slaveAdrs;
	u8 	ptxbuffer2[16];

	pData = (cmdFunctionData_t*)pVoid;

	channel = pData->option - 1;

	slaveAdrs = 0xA2;

//	memAdrs = 0x61;

	//txSize = 8;

	//mode = 0;

	ptxbuffer2[0] = 0x61;
	for(u16 cnt=0; cnt<15;cnt++)
	{
	  ptxbuffer2[cnt+1] = ptxbuffer[cnt];
	}


//	for(u8 cnt = )
//	ptxbuffer2[1] = ;

	rtn = LCM_I2CTransmitData(pData->position, channel, slaveAdrs, ptxbuffer2, 16);
	if(rtn == 0)	return	result;	

//	result = LCM_I2CMemWrite(pData->position, channel, slaveAdrs, memAdrs, mode, ptxbuffer, txSize);

//	txSize = 9;

	HAL_Delay(5);

	ptxbuffer2[0] = 0x70;
	ptxbuffer2[1] = ptxbuffer[15];
	ptxbuffer2[2] = ptxbuffer[16];


	rtn = LCM_I2CTransmitData(pData->position, channel, slaveAdrs, ptxbuffer2, 3);
	if(rtn == 0)	return	result;	

	result = 1;

	return result;
}

u8 SerialReadCheck(void *pVoid)
{
	cmdFunctionData_t	*pData;
	u8	result = 0, rtn, mode;
	u8	channel, slaveAdrs;
	u8	ngSum = 0;
	u8	chkflag = 0;
	u8 	prxbuffer[16];
	u16	memAdrs;
	u32	rxSize;

	pData = (cmdFunctionData_t*)pVoid;

	channel = pData->option - 1;

	slaveAdrs = 0xA2;

	memAdrs = 0x61;

	rxSize = 17;

	mode = 0;
	
	rtn = LCM_I2CMemRead(pData->position, channel, slaveAdrs, memAdrs, mode, prxbuffer, rxSize);
	if(rtn == 0)	return	result;	

	HAL_Delay(20);

	if(prxbuffer[0] == 0x47 && prxbuffer[1] == 0x38 && prxbuffer[2] == 0x51)
	{
		chkflag = 1;
	}
	else	ngSum++;	

	if(chkflag)
	{
		for(u8 cnt = 0; cnt < 17; cnt++)
		{
			if(ptxbuffer[cnt] != prxbuffer[cnt])
			{
				ngSum++;
			}
		}
	}
	
	if(ngSum)	result = 0;
	else 	result = 1;

	return	result;
}

u8 Serialprotocol(u8 x_result)
{
  u8 result=0;

  if(x_result<=0x39)
  {
    result = x_result-0x30;
  }
  else if(x_result>0x39)
  {
    if(x_result == 'A')
    {
      result = 10;
    }
    if(x_result == 'B')
    {
      result = 11;
    }
    if(x_result == 'C')
    {
      result = 12;
    }
    if(x_result == 'D')
    {
      result = 13;
    }
    if(x_result == 'E')
    {
      result = 14;
    }
    if(x_result == 'F')
    {
      result = 15;
    }
    if(x_result == 'G')
    {
      result = 16;
    }
    if(x_result == 'H')
    {
      result = 17;
    }
    if(x_result == 'J')
    {
      result = 18;
    }
    if(x_result == 'K')
    {
      result = 19;
    }
    if(x_result == 'L')
    {
      result = 20;
    }
    if(x_result == 'M')
    {
      result = 21;
    }
    if(x_result == 'N')
    {
      result = 22;
    }
    if(x_result == 'P')
    {
      result = 23;
    }
    if(x_result == 'Q')
    {
      result = 24;
    }
    if(x_result == 'R')
    {
      result = 25;
    }
    if(x_result == 'S')
    {
      result = 26;
    }
    if(x_result == 'T')
    {
      result = 27;
    }
    if(x_result == 'U')
    {
      result = 28;
    }
    if(x_result == 'V')
    {
      result = 29;
    }
    if(x_result == 'W')
    {
      result = 30;
    }
    if(x_result == 'X')
    {
      result = 31;
    }
    if(x_result == 'Y')
    {
      result = 32;
    }
    if(x_result == 'Z')
    {
      result = 33;
    }
  }

  return result;

} 


u8 Serialppp(void *pVoid)
{
	cmdFunctionData_t	*pData;
	u8	result = 0, length = 0;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pData->pArgv[0], "d846"))
	{
	  	P_1 = 'G'; 
	 	P_2 = 'H'; 
	  	P_3 = '3'; 
	  	P1_copy = 16;
	  	P2_copy = 17;
	  	P3_copy = 3;
	}
	else if(CMD_Compare(pData->pArgv[0], "d847"))
	{
	  	P_1 = 'G'; 
	 	P_2 = 'H'; 
	  	P_3 = '3'; 
	  	P1_copy = 16;
	  	P2_copy = 17;
	  	P3_copy = 3;
	}
	else if(CMD_Compare(pData->pArgv[0], "default"))
	{
	 	P_1 = 'G'; 
	 	P_2 = '8'; 
	  	P_3 = 'Q'; 
	  	P1_copy = 16;
	  	P2_copy = 8;
	  	P3_copy = 24;
	}
	else
	{
		length = (u8)strlen((char*)pData->pArgv[0]);

		if(length != 3)	return result;

	    P_1 = pData->pArgv[0][0];
	    P_2 = pData->pArgv[0][1];
	    P_3 = pData->pArgv[0][2];

	    if(P_1 > 0x60) P_1 = P_1 - 32;
	    if(P_2 > 0x60) P_2 = P_2 - 32;
	    if(P_3 > 0x60) P_3 = P_3 - 32;

	    P1_copy = Serialprotocol(P_1);
	    P2_copy = Serialprotocol(P_2);
	    P3_copy = Serialprotocol(P_3);
	}

	setpppflag = 1;

	CMD_Printf(";P1=%x  P2=%x  P3=%x", P_1, P_2, P_3);
	CMD_Printf(";P1_TRANS=%d; P2_TRANS=%d; P3_TRANS=%d;", P1_copy, P2_copy, P3_copy);

	result = 1;

	return result;
}

u8 Serialeeee(void *pVoid)
{
	cmdFunctionData_t	*pData;
	u8	result = 0, length = 0;

	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pData->pArgv[0], "d847"))
	{
		E1 = 'L';
		E2 = '1';
		E3 = '7';
		E4 = 'H';
		E1_copy = 20; 
		E2_copy = 1;
		E3_copy = 7;
		E4_copy = 17;
	}
	else if(CMD_Compare(pData->pArgv[0], "d846"))
	{
		E1 = 'L';
		E2 = '1';
		E3 = '6';
		E4 = 'H';
		E1_copy = 20;
		E2_copy = 1;
		E3_copy = 6;
		E4_copy = 17;
	}
	else if(CMD_Compare(pData->pArgv[0], "default"))
	{
		E1 = 'P';
		E2 = 'N';
		E3 = 'D'; 
		E4 = '8';
		E1_copy = 23;
		E2_copy = 22;
		E3_copy = 13;
		E4_copy = 8;
	}
	else
    {
		length = (u8)strlen((char*)pData->pArgv[0]);

	    if(length != 4)	return result;

		E1 = pData->pArgv[0][0];
	    E2 = pData->pArgv[0][1];
	    E3 = pData->pArgv[0][2];
	    E4 = pData->pArgv[0][3];

	    if(E1 > 0x60) E1 = E1 - 32;
	    if(E2 > 0x60) E2 = E2 - 32;
	    if(E3 > 0x60) E3 = E3 - 32;
	    if(E4 > 0x60) E4 = E4 - 32;

		E1_copy = Serialprotocol(E1);
	    E2_copy = Serialprotocol(E2);
	    E3_copy = Serialprotocol(E3);
	    E4_copy = Serialprotocol(E4);
    }

	seteeeeflag = 1;

	CMD_Printf(";E1=0x%02X; E2=0x%02X; E3=0x%02X; E4=0x%02X", E1, E2, E3, E4);
	CMD_Printf(";E1_TRANS=%d; E2_TRANS=%d; E3_TRANS=%d; E4_TRANS=%d", E1_copy, E2_copy, E3_copy, E4_copy);
	 
	result = 1;

	return result;
}
