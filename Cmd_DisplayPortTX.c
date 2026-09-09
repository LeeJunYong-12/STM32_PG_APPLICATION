#include	"Cmd_DisplayPortTX.h"

static uc8	cmdNote_dpTxRegWrite[] = "E7602 DP TX Internal Register Write | A-Cnt : 2 | A-Value : Address, Data\0";
static u8 Cmd_DPTX_RegWrite(void *pVoid)
{
	u8	result = 0;
	u32	adrs, regData;
	dptxSFunction_t	sFunction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	adrs	= CMD_StrToUL(pData->pArgv[0]);
	regData	= CMD_StrToUL(pData->pArgv[1]);

	memset(&sFunction, NULL, sizeof(sFunction));

	sFunction.channel	= pData->position;

	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_WRITE;
	sFunction.buffer[0]	= adrs;
	sFunction.buffer[1]	= regData;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

static uc8	cmdNote_dpTxRegRead[] = "E7602 DP TX Internal Register Read | A-Cnt : 1 | A-Value : Address\0";
static u8 Cmd_DPTX_RegRead(void *pVoid)
{
	u8	result = 0;
	u32	adrs, regData;
	dptxSFunction_t	sFunction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)				return result;

	adrs	= CMD_StrToUL(pData->pArgv[0]);

	memset(&sFunction, NULL, sizeof(sFunction));

	sFunction.channel	= pData->position;

	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= adrs;

	result = DPTX_SpecialFunction(&sFunction);
	if(result)
	{
		regData = sFunction.buffer[1];

		CMD_Printf(";DATA=0x%08X", regData);
	}

	return result;
}

static uc8	cmdNote_dpTxHpdEnable[] = "E7602 DP TX HPD Enable | A-Cnt : 0\0";
static u8 Cmd_DPTX_HpdIrqEnable(void *pVoid)
{
	u8	result = 0;
	dptxDefaultSetup_t	defaultSetup;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	memset(&defaultSetup, NULL, sizeof(defaultSetup));

	defaultSetup.channel	= pData->position;
	defaultSetup.mode		= RP_DP_TX_DEFAULT_SETUP_MODE_HPD_EVENT_CONFIG;
	defaultSetup.buffer[0]	= 1;

	result = DPTX_DefaultSetup(&defaultSetup);

	HAL_Delay(50);

	return result;
}

static uc8	cmdNote_dpTxHpdDisable[] = "E7602 DP TX HPD Disable | A-Cnt : 0\0";
static u8 Cmd_DPTX_HpdIrqDisable(void *pVoid)
{
	u8	result = 0;
	dptxDefaultSetup_t	defaultSetup;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	memset(&defaultSetup, NULL, sizeof(defaultSetup));

	defaultSetup.channel	= pData->position;
	defaultSetup.mode		= RP_DP_TX_DEFAULT_SETUP_MODE_HPD_EVENT_CONFIG;
	defaultSetup.buffer[0]	= 0;

	result = DPTX_DefaultSetup(&defaultSetup);

	HAL_Delay(50);

	return result;
}

static uc8	cmdNote_dpTxVideoEnable[] = "E7602 DP TX Video Output Enable | A-Cnt : 0\0";
static u8 Cmd_DPTX_VideoEnable(void *pVoid)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	memset(&sFunction, NULL, sizeof(sFunction));

	sFunction.channel	= pData->position;

	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_VIDEO_OUTPUT_CTRL;
	sFunction.ctrl[0]	= 1;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

static uc8	cmdNote_dpTxVideoDisable[] = "E7602 DP TX Video Output Disable | A-Cnt : 0\0";
static u8 Cmd_DPTX_VideoDisable(void *pVoid)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	memset(&sFunction, NULL, sizeof(sFunction));

	sFunction.channel	= pData->position;

	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_VIDEO_OUTPUT_CTRL;
	sFunction.ctrl[0]	= 0;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

static uc8	cmdNote_dpTxCSpaceReinit[] = "E7602 DP TX Color Space Re-Initialize | A-Cnt : 0\0";
static u8 Cmd_DPTX_ColorSpaceReinit(void *pVoid)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	memset(&sFunction, NULL, sizeof(sFunction));

	sFunction.channel	= pData->position;

	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_COLOR_SPACE_REINIT;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

static uc8	cmdNote_dpTxAuxWrite[] = "E7602 DP TX AUX Write | A-Cnt : 2~257 | A-Value : DPCD Address, Data0 ... Data255\0";
static u8 Cmd_DPTX_AuxWrite(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	dptxTransaction_t	dptxTransaction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)				return result;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= pData->position;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_WRITE;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxTransaction.adrs	= CMD_StrToUL(pData->pArgv[0]);
	dptxTransaction.size	= pData->argc - 1;
	if(dptxTransaction.size == 0)		return result;
	if(dptxTransaction.size > 256)		return result;

	for(u32 cnt = 0; cnt < dptxTransaction.size; cnt++)
	{
		dptxTransaction.pData[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
	}

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		CMD_Printf(";STATUS=%d", dptxTransaction.status);
	}

	return result;
}

static uc8	cmdNote_dpTxAuxRead[] = "E7602 DP TX AUX Read | A-Cnt : 2 | A-Value : DPCD Address, DataCnt\0";
static u8 Cmd_DPTX_AuxRead(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	dptxTransaction_t	dptxTransaction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= pData->position;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_READ;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxTransaction.adrs	= CMD_StrToUL(pData->pArgv[0]);
	dptxTransaction.size	= CMD_StrToUL(pData->pArgv[1]);
	if(dptxTransaction.size == 0)		return result;
	if(dptxTransaction.size > 256)		return result;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		CMD_Printf(";STATUS=%d,DATA=", dptxTransaction.status);

		for(u32 cnt = 0; cnt < dptxTransaction.size; cnt++)
		{
			if(cnt != (dptxTransaction.size - 1))		CMD_Printf("0x%02X ", dptxTransaction.pData[cnt]);
			else										CMD_Printf("0x%02X", dptxTransaction.pData[cnt]);
		}
	}

	return result;
}

static uc8	cmdNote_dpTxEdidRead[] = "E7602 DP TX EDID Read | A-Cnt : 0\0";
static u8 Cmd_DPTX_EdidRead(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	dptxTransaction_t	dptxTransaction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel = pData->position;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_EDID_READ;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		CMD_Printf(";STATUS=%d,DATA=", dptxTransaction.status);

		if(dptxTransaction.status > 256)	dptxTransaction.size = 256;
		else								dptxTransaction.size = dptxTransaction.status;

		for(u32 cnt = 0; cnt < dptxTransaction.size; cnt++)
		{
			if(cnt != (dptxTransaction.size - 1))		CMD_Printf("0x%02X ", dptxTransaction.pData[cnt]);
			else										CMD_Printf("0x%02X", dptxTransaction.pData[cnt]);
		}
	}

	return result;
}

static uc8	cmdNote_dpTxI2cWrite[] = "E7602 DP TX I2C Write | A-Cnt : 3~258 | A-Value : DPCD Address, Device Address, Data0 ... Data255\0";
static u8 Cmd_DPTX_I2CWrite(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	dptxTransaction_t	dptxTransaction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)				return result;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= pData->position;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_I2C_WRITE;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxTransaction.adrs	= CMD_StrToUL(pData->pArgv[0]);
	dptxTransaction.size	= pData->argc - 1;
	if(dptxTransaction.size == 0)		return result;
	if(dptxTransaction.size > 256)		return result;

	for(u32 cnt = 0; cnt < dptxTransaction.size; cnt++)
	{
		dptxTransaction.pData[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
	}

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		CMD_Printf(";STATUS=%d", dptxTransaction.status);
	}

	return result;
}

static uc8	cmdNote_dpTxI2cRead[] = "E7602 DP TX I2C Read | A-Cnt : 3 | A-Value : DPCD Address, Device Address, DataCnt\0";
static u8 Cmd_DPTX_I2CRead(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	dptxTransaction_t	dptxTransaction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= pData->position;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_I2C_READ;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxTransaction.adrs = CMD_StrToUL(pData->pArgv[0]);
	dptxTransaction.size = CMD_StrToUL(pData->pArgv[1]);
	if(dptxTransaction.size == 0)		return result;
	if(dptxTransaction.size > 256)		return result;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		CMD_Printf(";STATUS=%d,DATA=", dptxTransaction.status);

		for(u32 cnt = 0; cnt < dptxTransaction.size; cnt++)
		{
			if(cnt != (dptxTransaction.size - 1))		CMD_Printf("0x%02X ", dptxTransaction.pData[cnt]);
			else										CMD_Printf("0x%02X", dptxTransaction.pData[cnt]);
		}
	}

	return result;
}

static uc8	cmdNote_dpTxSdpWrite[] = "E7602 DP TX SDP Write | A-Cnt : 5~36 | A-Value : Header0 ... Header3 Data0 ... Data31\0";
static u8 Cmd_DPTX_SdpWrite(void *pVoid)
{
	u8	result = 0;
	u8	*pBuffer;
	u16	size;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 5)				return result;
	if(pData->argc > 36)				return result;

	pBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	size = pData->argc;

	memset(pBuffer, NULL, (36 + 8));

	for(u16 cnt = 0; cnt < size; cnt++)
	{
		pBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt]);
	}

	size = 36;

	result = DPTX_SecondaryDataPacketWrite(pData->position, pBuffer, size);

	return result;
}

static uc8	cmdNote_dpTxSdpContainClear[] = "\0";
static u8 Cmd_DPTX_SdpContainClear(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	result = DPTX_SecondaryDataPacketContainClear(pData->position);

	return result;
}


static uc8	cmdNote_dpTxSdpContain[] = "\0";
static u8 Cmd_DPTX_SdpContain(void *pVoid)
{
	u8	result = 0;
	u8	*pBuffer;
	u16	size;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 5)				return result;
	if(pData->argc > 36)				return result;

	pBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	size = pData->argc;

	memset(pBuffer, NULL, (36 + 8));

	for(u16 cnt = 0; cnt < size; cnt++)
	{
		pBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt]);
	}

	size = 36;

	result = DPTX_SecondaryDataPacketMultiContain(pData->position, pBuffer, size);

	return result;
}

static uc8	cmdNote_dpTxSdpMultiWrite[] = "\0";
static u8 Cmd_DPTX_SdpMultiWrite(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	result = DPTX_SecondaryDataPacketMultiWrite(pData->position);

	return result;
}

static uc8	cmdNote_dpTxModeDIC[] = "\0";
static u8 Cmd_DPTX_ModeDIC(void *pVoid)
{
	u8	result = 0;
	u32	mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)							return result;

	if(CMD_Compare(pData->pArgv[0], "Type_1"))		mode = DP_TX_REG_DATA_MODE_TYPE_1;
	else											return result;

	result = DPTX_SetDataModeDIC(mode);

	return result;
}

static uc8	cmdNote_dpTxDataWriteDIC[] = "\0";
static u8 Cmd_DPTX_DataWriteDIC(void *pVoid)
{
	u8	result = 0;
	u32	cnt;
	dptxRegData_t	dptxRegData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 3)				return result;

	memset(&dptxRegData, NULL, sizeof(dptxRegData));
	dptxRegData.pBuffer	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxRegData.channel		= pData->position;

	dptxRegData.regAdrs		= CMD_StrToUL(pData->pArgv[0]);
	dptxRegData.dataAdrs	= CMD_StrToUL(pData->pArgv[1]);
	dptxRegData.dataSize	= pData->argc - 2;
	if(dptxRegData.dataSize > 1024)	return result;

	for(cnt = 0; cnt < dptxRegData.dataSize; cnt++)
	{
		dptxRegData.pBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 2]);
	}

	result = DPTX_RegDataWriteDIC(&dptxRegData);

	return result;
}

static uc8	cmdNote_dpTxDataWriteDIC_md[] = "\0";
static u8 Cmd_DPTX_DataWriteDIC_md(void *pVoid)
{
	u8	result = 0;
	u32	cnt;
	dptxRegData_t	dptxRegData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 3)				return result;

	memset(&dptxRegData, NULL, sizeof(dptxRegData));
	dptxRegData.pBuffer	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxRegData.channel		= pData->position;

	dptxRegData.regAdrs		= CMD_StrToUL(pData->pArgv[0]);
	dptxRegData.dataAdrs	= CMD_StrToUL(pData->pArgv[1]);
	dptxRegData.dataSize	= pData->argc - 2;
	if(dptxRegData.dataSize > 1024)	return result;

	for(cnt = 0; cnt < dptxRegData.dataSize; cnt++)
	{
		dptxRegData.pBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 2]);
	}

	result = DPTX_RegDataWriteDIC_md(&dptxRegData);

	return result;
}

static uc8	cmdNote_dpTxDataWriteDIC_mx[] = "\0";
static u8 Cmd_DPTX_DataWriteDIC_mx(void *pVoid)
{
	u8	result = 0;
	u32	cnt;
	dptxRegData_t	dptxRegData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 3)				return result;

	memset(&dptxRegData, NULL, sizeof(dptxRegData));
	dptxRegData.pBuffer	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxRegData.channel		= pData->position;

	dptxRegData.regAdrs		= CMD_StrToUL(pData->pArgv[0]);
	dptxRegData.dataAdrs	= CMD_StrToUL(pData->pArgv[1]);
	dptxRegData.dataSize	= pData->argc - 2;
	
	for(cnt = 0; cnt < dptxRegData.dataSize; cnt++)
	{
		dptxRegData.pBuffer[cnt] = CMD_StrToUL(pData->pArgv[cnt + 2]);
	}

	if(dptxRegData.dataSize <= 16)
	{
		result = DPTX_RegDataWriteDIC_mx(&dptxRegData);
	}
	else
	{
		result = DPTX_RegDataWriteDIC_mx_long(&dptxRegData);
	}	

	return result;
}

static uc8	cmdNote_dpTxDataReadDIC[] = "\0";
static u8 Cmd_DPTX_DataReadDIC(void *pVoid)
{
	u8	result = 0;
	u32	cnt;
	dptxRegData_t	dptxRegData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)				return result;

	memset(&dptxRegData, NULL, sizeof(dptxRegData));
	dptxRegData.pBuffer	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxRegData.channel		= pData->position;

	dptxRegData.regAdrs		= CMD_StrToUL(pData->pArgv[0]);
	dptxRegData.dataAdrs	= CMD_StrToUL(pData->pArgv[1]);
	dptxRegData.dataSize	= CMD_StrToUL(pData->pArgv[2]);
	if(dptxRegData.dataSize > 1024)	return result;

	result = DPTX_RegDataReadDIC(&dptxRegData);
	if(result)
	{
		CMD_Printf(";DATA=");
		for(cnt = 0; cnt < dptxRegData.dataSize; cnt++)
		{
			if((dptxRegData.dataSize - 1) != cnt)		CMD_Printf("0x%02X ", dptxRegData.pBuffer[cnt]);
			else										CMD_Printf("0x%02X", dptxRegData.pBuffer[cnt]);
		}
	}

	return result;
}

static uc8	cmdNote_dpTxDataReadDIC_md[] = "\0";
static u8 Cmd_DPTX_DataReadDIC_md(void *pVoid)
{
	u8	result = 0;
	u32	cnt;
	dptxRegData_t	dptxRegData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)				return result;

	memset(&dptxRegData, NULL, sizeof(dptxRegData));
	dptxRegData.pBuffer	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxRegData.channel		= pData->position;

	dptxRegData.regAdrs		= CMD_StrToUL(pData->pArgv[0]);
	dptxRegData.dataAdrs	= CMD_StrToUL(pData->pArgv[1]);
	dptxRegData.dataSize	= CMD_StrToUL(pData->pArgv[2]);
	if(dptxRegData.dataSize > 1024)	return result;

	result = DPTX_RegDataReadDIC_md(&dptxRegData);
	if(result)
	{
		CMD_Printf(";DATA=");
		for(cnt = 0; cnt < dptxRegData.dataSize; cnt++)
		{
			if((dptxRegData.dataSize - 1) != cnt)		CMD_Printf("0x%02X ", dptxRegData.pBuffer[cnt]);
			else										CMD_Printf("0x%02X", dptxRegData.pBuffer[cnt]);
		}
	}

	return result;
}

static uc8	cmdNote_dpTxDataReadDIC_mx[] = "\0";
static u8 Cmd_DPTX_DataReadDIC_mx(void *pVoid)
{
	u8	result = 0;
	u32	cnt;
	dptxRegData_t	dptxRegData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)				return result;

	memset(&dptxRegData, NULL, sizeof(dptxRegData));
	dptxRegData.pBuffer	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxRegData.channel		= pData->position;

	dptxRegData.regAdrs		= CMD_StrToUL(pData->pArgv[0]);
	dptxRegData.dataAdrs	= CMD_StrToUL(pData->pArgv[1]);
	dptxRegData.dataSize	= CMD_StrToUL(pData->pArgv[2]);
	if(dptxRegData.dataSize > 1024)	return result;

	result = DPTX_RegDataReadDIC_mx(&dptxRegData);
	if(result)
	{
		CMD_Printf(";DATA=");
		for(cnt = 0; cnt < dptxRegData.dataSize; cnt++)
		{
			if((dptxRegData.dataSize - 1) != cnt)		CMD_Printf("0x%02X ", dptxRegData.pBuffer[cnt]);
			else										CMD_Printf("0x%02X", dptxRegData.pBuffer[cnt]);
		}
	}

	return result;
}


static uc8	cmdNote_dpTxLaneConfig[] = "\0";
static u8 Cmd_DPTX_LaneConfig(void *pVoid)
{
	u8	result = 0;
	u32	lane;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)							return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))		lane = 0;
	else if(CMD_Compare(pData->pArgv[0], "1"))		lane = 1;
	else if(CMD_Compare(pData->pArgv[0], "2"))		lane = 2;
	else if(CMD_Compare(pData->pArgv[0], "3"))		lane = 3;
	else if(CMD_Compare(pData->pArgv[0], "4"))		lane = 4;
	else											return result;

	result = DPTX_LaneConfig(pData->position, lane);

	return result;
}

static uc8	cmdNote_dpTxDummyVideo[] = "\0";
static u8 Cmd_DPTX_DummyVideo(void *pVoid)
{
	u8	result = 0;
	u32	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))			state = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))		state = 1;
	else												return result;

	result = DPTX_DummyVideoCtrl(pData->position, state);

	return result;
}

static uc8	cmdNote_dpTxFixedMVID[] = "\0";
static u8 Cmd_DPTX_FixedMVID(void *pVoid)
{
	u8	result = 0;
	u32	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))			state = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))		state = 1;
	else												return result;

	result = DPTX_FixedMVID(pData->position, state);

	return result;
}

static uc8	cmdNote_dpTxLtFull[] = "\0";
static u8 Cmd_DPTX_LtFull(void *pVoid)
{
	u8	result = 0;
	dptxLinkTraining_t	ltParameter;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)									return result;

	memset(&ltParameter, NULL, sizeof(ltParameter));

	ltParameter.channel			= pData->position;
	ltParameter.mode			= RP_DP_TX_LINK_TRAINING_MODE_NORMAL;
	ltParameter.linkRate		= CMD_StrToUL(pData->pArgv[0]);
	ltParameter.laneCount		= CMD_StrToUL(pData->pArgv[1]);
	
	result = DPTX_LinkTraining(&ltParameter);

	return result;
}

static uc8	cmdNote_dpTxLtFast[] = "\0";
static u8 Cmd_DPTX_LtFast(void *pVoid)
{
	u8	result = 0;
	dptxLinkTraining_t	ltParameter;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4 && pData->argc != 8 && pData->argc != 11)		return result;

	memset(&ltParameter, NULL, sizeof(ltParameter));

	ltParameter.channel			= pData->position;
	ltParameter.mode			= RP_DP_TX_LINK_TRAINING_MODE_FAST;

	ltParameter.fTPattern		= 0;
	ltParameter.fTDelay			= 5;
	ltParameter.sTPattern		= 2;
	ltParameter.sTDelay 		= 5;
	ltParameter.dpcdCheck		= 1;
	ltParameter.capabilityCheck	= 0;
	ltParameter.statusCheck		= 1;

	ltParameter.linkRate		= CMD_StrToUL(pData->pArgv[0]);
	ltParameter.laneCount		= CMD_StrToUL(pData->pArgv[1]);
	ltParameter.voltSwing		= CMD_StrToUL(pData->pArgv[2]);
	ltParameter.preEmphasis		= CMD_StrToUL(pData->pArgv[3]);

	if(pData->argc == 8)
	{
		ltParameter.fTPattern		= CMD_StrToUL(pData->pArgv[4]);
		ltParameter.fTDelay			= CMD_StrToUL(pData->pArgv[5]);
		ltParameter.sTPattern		= CMD_StrToUL(pData->pArgv[6]);
		ltParameter.sTDelay 		= CMD_StrToUL(pData->pArgv[7]);
	}
	else if(pData->argc == 11)
	{
		ltParameter.dpcdCheck		= CMD_StrToUL(pData->pArgv[8]);
		ltParameter.capabilityCheck	= CMD_StrToUL(pData->pArgv[9]);
		ltParameter.statusCheck		= CMD_StrToUL(pData->pArgv[10]);
	}

	result = DPTX_LinkTraining(&ltParameter);

	return result;
}

static uc8	cmdNote_dpTxLtQuick[] = "\0";
static u8 Cmd_DPTX_LtQuick(void *pVoid)
{
	u8	result = 0;
	dptxLinkTraining_t	ltParameter;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4 && pData->argc != 8 && pData->argc != 11)		return result;

	memset(&ltParameter, NULL, sizeof(ltParameter));

	ltParameter.channel			= pData->position;
	ltParameter.mode			= RP_DP_TX_LINK_TRAINING_MODE_QUICK;

	ltParameter.fTPattern		= 0;
	ltParameter.fTDelay			= 5;
	ltParameter.sTPattern		= 2;
	ltParameter.sTDelay 		= 5;
	ltParameter.dpcdCheck		= 1;
	ltParameter.capabilityCheck	= 0;
	ltParameter.statusCheck		= 1;

	ltParameter.linkRate		= CMD_StrToUL(pData->pArgv[0]);
	ltParameter.laneCount		= CMD_StrToUL(pData->pArgv[1]);
	ltParameter.voltSwing		= CMD_StrToUL(pData->pArgv[2]);
	ltParameter.preEmphasis		= CMD_StrToUL(pData->pArgv[3]);

	if(pData->argc >= 8)
	{
		ltParameter.fTPattern		= CMD_StrToUL(pData->pArgv[4]);
		ltParameter.fTDelay			= CMD_StrToUL(pData->pArgv[5]);
		ltParameter.sTPattern		= CMD_StrToUL(pData->pArgv[6]);
		ltParameter.sTDelay 		= CMD_StrToUL(pData->pArgv[7]);
		if(pData->argc == 11)
		{
			ltParameter.dpcdCheck		= CMD_StrToUL(pData->pArgv[8]);
			ltParameter.capabilityCheck	= CMD_StrToUL(pData->pArgv[9]);
			ltParameter.statusCheck		= CMD_StrToUL(pData->pArgv[10]);
		}
	}

	result = DPTX_LinkTraining(&ltParameter);

	return result;
}

static uc8	cmdNote_dpTxLtCustom[] = "\0";
static u8 Cmd_DPTX_LtCustom(void *pVoid)
{
	u8	result = 0;
	u8	mode;
	dptxLinkTraining_t	ltParameter;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)									return result;

	memset(&ltParameter, NULL, sizeof(ltParameter));

	ltParameter.channel	= pData->position;
	ltParameter.mode	= RP_DP_TX_LINK_TRAINING_MODE_CUSTOM;

	if(CMD_Compare(pData->pArgv[0], "Auto"))				mode = 0;
	else if(CMD_Compare(pData->pArgv[0], "Type_1"))			mode = 1;
	else if(CMD_Compare(pData->pArgv[0], "Type_2"))			mode = 2;
	else if(CMD_Compare(pData->pArgv[0], "Type_3"))			mode = 3;
	else if(CMD_Compare(pData->pArgv[0], "Type_4"))			mode = 4;
	else													return result;

	if(mode == 0)											//	AUTO
	{
		if(pData->argc != 3)								return result;

		ltParameter.linkRate		= CMD_StrToUL(pData->pArgv[1]);
		ltParameter.laneCount		= CMD_StrToUL(pData->pArgv[2]);
	}
	else if(mode == 1)										//	TYPE_1
	{
		if(pData->argc != 5)								return result;

		ltParameter.linkRate		= CMD_StrToUL(pData->pArgv[1]);
		ltParameter.laneCount		= CMD_StrToUL(pData->pArgv[2]);
		ltParameter.voltSwing		= CMD_StrToUL(pData->pArgv[3]);
		ltParameter.preEmphasis		= CMD_StrToUL(pData->pArgv[4]);
	}
	else if(mode == 2)										//	TYPE_2
	{
		if(pData->argc != 5)								return result;

		ltParameter.linkRate		= CMD_StrToUL(pData->pArgv[1]);
		ltParameter.laneCount		= CMD_StrToUL(pData->pArgv[2]);
		if(CMD_Compare(pData->pArgv[3], "Auto"))			ltParameter.sTPattern = 0;
		else if(CMD_Compare(pData->pArgv[3], "TP2"))		ltParameter.sTPattern = 2;
		else if(CMD_Compare(pData->pArgv[3], "TP3"))		ltParameter.sTPattern = 3;
		else if(CMD_Compare(pData->pArgv[3], "TP4"))		ltParameter.sTPattern = 4;
		else												return result;

		ltParameter.sTDelay			= CMD_StrToUL(pData->pArgv[4]);
	}
	else if(mode == 3)
	{
		if(pData->argc != 5)								return result;

		ltParameter.linkRate		= CMD_StrToUL(pData->pArgv[1]);
		ltParameter.laneCount		= CMD_StrToUL(pData->pArgv[2]);
		ltParameter.fTPattern		= 1;
		ltParameter.fTDelay			= CMD_StrToUL(pData->pArgv[3]);
		ltParameter.sTPattern		= 2;
		ltParameter.sTDelay			= CMD_StrToUL(pData->pArgv[4]);
	}
	else if(mode == 4)
	{
		if(pData->argc != 5)								return result;

		ltParameter.linkRate		= CMD_StrToUL(pData->pArgv[1]);
		ltParameter.laneCount		= CMD_StrToUL(pData->pArgv[2]);
		ltParameter.voltSwing		= CMD_StrToUL(pData->pArgv[3]);
		ltParameter.preEmphasis		= CMD_StrToUL(pData->pArgv[4]);
		ltParameter.fTPattern		= 1;
		ltParameter.sTPattern		= 2;
	}

	result = DPTX_LinkTraining(&ltParameter);

	return result;
}

static uc8	cmdNote_dpTxLtDoneVideo[] = "\0";
static u8 Cmd_DPTX_LtDoneVideo(void *pVoid)
{
	u8	result = 0;
	u8	channel; 
	u8	autoVideoOutput = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)									return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))				autoVideoOutput = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))			autoVideoOutput = 1;
	else													return result;

	channel	= pData->position;

	result = DPTX_LtAutoVideoOutput(channel, autoVideoOutput);

	return result;
}

static	u8	cmdDpTxAuxBuffer[1024];
static	u32	cmdDpTxAuxSize;

static uc8	cmdNote_dpTxAuxWriteCustom[] = "\0";
static u8 Cmd_DPTX_LtAuxWriteCustom(void *pVoid)
{
	u8	result = 0;
	u8	type;
	u32	size;
	cmdFunctionData_t	*pData;
	dptxTransaction_t	dptxTransaction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)								return result;

	if(CMD_Compare(pData->pArgv[0], "Init"))			type = 0;
	else if(CMD_Compare(pData->pArgv[0], "Data"))		type = 1;
	else if(CMD_Compare(pData->pArgv[0], "Send"))		type = 2;

	if(type == 0)
	{
		if(pData->argc != 1)							return result;

		memset(cmdDpTxAuxBuffer, NULL, sizeof(cmdDpTxAuxBuffer));

		cmdDpTxAuxSize = 0;

		result = 1;
	}
	else if(type == 1)
	{
		if(pData->argc > (128 + 1))						return result;

		size = pData->argc - 1;
		if((cmdDpTxAuxSize + size) > 256)				return result;

		for(u32 cnt = 0; cnt < size; cnt++)
		{
			cmdDpTxAuxBuffer[cmdDpTxAuxSize + cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
		}
		cmdDpTxAuxSize += size;

		result = 1;
	}
	else if(type == 2)
	{
		if(pData->argc != 2)							return result;

		if(cmdDpTxAuxSize == 0)							return result;

		memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

		dptxTransaction.channel	= pData->position;

		dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_WRITE;
		dptxTransaction.pData	= (u8*)cmdDpTxAuxBuffer;

		dptxTransaction.adrs	= CMD_StrToUL(pData->pArgv[1]);
		dptxTransaction.size	= cmdDpTxAuxSize;

		result = DPTX_Transaction(&dptxTransaction);
		if(result)
		{
			CMD_Printf(";STATUS=%d", dptxTransaction.status);
		}
	}

	return result;
}

static uc8	cmdNote_dpTxAssrCtrl[] = "\0";
static u8 Cmd_DPTX_AssrCtrl(void *pVoid)
{
	u8	result = 0;
	u32	state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))			state = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))		state = 1;
	else												return result;

	result = DPTX_AssrCtrl(pData->position, state);

	return result;
}

static uc8	cmdNote_dpTxAlpmEnter[] = "\0";
static u8 Cmd_DPTX_AlpmEnter(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = DPTX_AlpmEnter(pData->position);

	return result;
}

static uc8	cmdNote_dpTxAlpmExit[] = "\0";
static u8 Cmd_DPTX_AlpmExit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = DPTX_AlpmExit(pData->position);

	return result;
}

static uc8	cmdNote_dpTxAlpmEnable[] = "\0";
static u8 Cmd_DPTX_AlpmEnable(void *pVoid)
{
	u8	result = 0;
	u32	mode = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 1)
	{
		if(CMD_Compare(pData->pArgv[0], "once"))		mode = 1;
		else											return result;
	}
	else if(pData->argc != 0)							return result;

	result = DPTX_AlpmEnable(pData->position, mode);

	return result;
}

static uc8	cmdNote_dpTxAlpmDisable[] = "\0";
static u8 Cmd_DPTX_AlpmDisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = DPTX_AlpmDisable(pData->position);

	return result;
}

static uc8	cmdNote_dpTxAlpmLinkRateChange[] = "\0";
static u8 Cmd_DPTX_AlpmLinkRateChange(void *pVoid)
{
	u8	result = 0;
	u32 lrc_enable = 0, lrc_mode = 0, lrc_frame_period = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)									return result;

	if(CMD_Compare(pData->pArgv[0], "Disable"))					lrc_enable = 0;
	else if(CMD_Compare(pData->pArgv[0], "Enable"))				lrc_enable = 1;

	lrc_mode			= CMD_StrToUL(pData->pArgv[1]);
	lrc_frame_period 	= CMD_StrToUL(pData->pArgv[2]);

	result = DPTX_AlpmLinkRateChange(pData->position, lrc_enable, lrc_mode, lrc_frame_period);

	return result;
}

static uc8	cmdNote_dpTxRawDataAUXWrite[] = "\0";
static u8 Cmd_ENY_PacketWrite(void *pVoid)
{
	u8	result = 0;
	eNY_PacketWrite_t	wPacket;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	wPacket.slotNo			= CMD_StrToUL(pData->pArgv[0]);
	wPacket.pktTxSize		= CMD_StrToUL(pData->pArgv[1]);
	wPacket.adr				= CMD_StrToUL(pData->pArgv[2]);
	wPacket.position		= pData->position;

	result = ENY_PacketWrite(&wPacket);

	return result;
}

static uc8	cmdNote_dpTxRawDataDICWrite[] = "\0";
static u8 Cmd_ENY_PacketDICWrite(void *pVoid)
{
	u8	result = 0;
	eNY_PacketWrite_t	wPacket;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)		return result;

	wPacket.slotNo			= CMD_StrToUL(pData->pArgv[0]);
	wPacket.pktTxSize		= CMD_StrToUL(pData->pArgv[1]);
	wPacket.adr				= CMD_StrToUL(pData->pArgv[2]);
	wPacket.reg_adr			= CMD_StrToUL(pData->pArgv[3]);
	wPacket.position		= pData->position;

	result = ENY_PacketWrite_DIC(&wPacket);

	return result;
}

static uc8	cmdNote_dpTxRawDataAUXRead[] = "\0";
static u8 Cmd_ENY_PacketRead(void *pVoid)
{
	u8	result = 0;
	eNY_PacketRead_t	rPacket;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)		return result;

	rPacket.slotNo			= CMD_StrToUL(pData->pArgv[0]);
	rPacket.pktRxSize		= CMD_StrToUL(pData->pArgv[1]);
	rPacket.adr				= CMD_StrToUL(pData->pArgv[2]);
	rPacket.cnt				= CMD_StrToUL(pData->pArgv[3]);
	rPacket.position		= pData->position;

	if(rPacket.cnt*rPacket.pktRxSize>65536)		return result;

	result = ENY_PacketRead(&rPacket);

	return result;
}

static uc8	cmdNote_dpTxRawDataDICRead[] = "\0";
static u8 Cmd_ENY_PacketDICRead(void *pVoid)
{
	u8	result = 0;
	eNY_PacketRead_t	rPacket;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)		return result;

	rPacket.slotNo			= CMD_StrToUL(pData->pArgv[0]);
	rPacket.pktRxSize		= CMD_StrToUL(pData->pArgv[1]);
	rPacket.adr				= CMD_StrToUL(pData->pArgv[2]);
	rPacket.reg_adr			= CMD_StrToUL(pData->pArgv[3]);
	rPacket.cnt				= CMD_StrToUL(pData->pArgv[4]);
	rPacket.position		= pData->position;

	if(rPacket.cnt*rPacket.pktRxSize>65536)		return result;

	result = ENY_PacketRead_DIC(&rPacket);

	return result;
}

static uc8	cmdNote_vvb[] = "\0";
static u8 Cmd_VVB(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	u32 vvb_hz=0;
	u32 common_chipselect =0;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	vvb_hz = CMD_StrToUL(pData->pArgv[0]);

	if(vvb_hz>120)
	{
		return result;
	}

	common_chipselect = pData->position-1;

	result = DP_VVB(pData->position, common_chipselect, vvb_hz);

	return result;
}

static uc8	cmdNote_dpTxLinkStatus[] = "\0";
static u8 Cmd_DPTX_LinkStatus(void *pVoid)
{
	u8	result = 0, chipSelect, rtn;
	u32	regData, linkrate_set, linkrate_actual;
	dptxTransaction_t	dptxTransaction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	rtn = DP_GetChipSelect(pData->position , &chipSelect);
	if(rtn == 0)										return result;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= pData->position;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_READ;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;
	dptxTransaction.adrs	= 0x00100;
	dptxTransaction.size	= 1;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		linkrate_actual = dptxTransaction.pData[0];
		if(dptxTransaction.status)						CMD_Printf(";AUX_ERROR");
	}
	else												return result;

	result = DPC_DirectRegReadSingle(chipSelect, CORE_REG_DP_TX_LANE_CONFIG, &regData);
	if(result)
	{
		linkrate_set = (regData & 0xFFFF0000) >> 16;
	}
	else												return result;

	CMD_Printf(";[Set]LinkRate=%d, [Actual]LinkRate=%d : ", linkrate_set, linkrate_actual);
	if(linkrate_set == linkrate_actual)					CMD_Printf("OK");
	else												CMD_Printf("NG");

	return result;
}

static uc8	cmdNote_dpTxLaneStatus[] = "\0";
static u8 Cmd_DPTX_LaneStatus(void *pVoid)
{
	u8	result = 0, sink, cr[4], eq[4], symbol[4], lanecnt, rtn;
	u32	tx_control;
	dptxTransaction_t	dptxTransaction;
	dptxSFunction_t	sFunction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	memset(&sFunction, NULL, sizeof(sFunction));
	sFunction.channel	= pData->position;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= 0x00;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)								tx_control = sFunction.buffer[1];
	else								return result;

	lanecnt = (tx_control & 0x000003E0) >> 5;	// Lane Count = bit [9:5]

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= pData->position;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_READ;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;
	dptxTransaction.adrs	= 0x00200;
	dptxTransaction.size	= 4;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		sink = dptxTransaction.pData[0];
		if(sink)								CMD_Printf(";SINK=%d", sink);
		else
		{
			CMD_Printf(";NO SINK DEVICE");
			return result;
		}

		for(int lane = 0, i = 0; lane < lanecnt; lane++)
		{
			if(lane > 2)				i = 1;
			
			CMD_Printf(";[Lane%d]CR=", lane);
			cr[lane] = dptxTransaction.pData[2 + i] & (0x01 << (i * 4));
			if(cr[lane])									CMD_Printf("OK, ");
			else											CMD_Printf("NG, ");
			CMD_Printf("EQ=");
			eq[lane] = dptxTransaction.pData[2 + i] & (0x02 << (i * 4));
			if(eq[lane])									CMD_Printf("OK, ");
			else											CMD_Printf("NG, ");
			CMD_Printf("SYMBOL=");
			symbol[lane] = dptxTransaction.pData[2 + i] & (0x04 << (i * 4));
			if(symbol[lane])								CMD_Printf("OK");
			else											CMD_Printf("NG");
		}
	}

	return result;
}

const	commandList_t	cmdList_DisplayPortTX[] = {
//		{	Command,				Function,						Option,		Mode,					RootMessage				Note},
		{	"help",					Cmd_Help,						NULL,		CMD_MODE_NONE,			";HELP",				NULL									},
		{	"Reg.Write",			Cmd_DPTX_RegWrite,				NULL,		CMD_MODE_NONE,			";REG.WRITE",			(u8*)cmdNote_dpTxRegWrite				},
		{	"Reg.Read",				Cmd_DPTX_RegRead,				NULL,		CMD_MODE_NONE,			";REG.READ",			(u8*)cmdNote_dpTxRegRead				},
		{	"HPD.Enable",			Cmd_DPTX_HpdIrqEnable,			NULL,		CMD_MODE_NONE,			";HPD.ENABLE",			(u8*)cmdNote_dpTxHpdEnable				},
		{	"HPD.Disable",			Cmd_DPTX_HpdIrqDisable,			NULL,		CMD_MODE_NONE,			";HPD.DISABLE",			(u8*)cmdNote_dpTxHpdDisable				},
		{	"Video.Enable",			Cmd_DPTX_VideoEnable,			NULL,		CMD_MODE_NONE,			";VIDEO.ENABLE",		(u8*)cmdNote_dpTxVideoEnable			},
		{	"Video.Disable",		Cmd_DPTX_VideoDisable,			NULL,		CMD_MODE_NONE,			";VIDEO.DISABLE",		(u8*)cmdNote_dpTxVideoDisable			},
		{	"CSpace.Reinit",		Cmd_DPTX_ColorSpaceReinit,		NULL,		CMD_MODE_NONE,			";CSPACE.REINIT",		(u8*)cmdNote_dpTxCSpaceReinit			},
		{	"AUX.Write",			Cmd_DPTX_AuxWrite,				NULL,		CMD_MODE_NONE,			";AUX.WRITE",			(u8*)cmdNote_dpTxAuxWrite				},
		{	"AUX.Read",				Cmd_DPTX_AuxRead,				NULL,		CMD_MODE_NONE,			";AUX.READ",			(u8*)cmdNote_dpTxAuxRead				},
		{	"EDID.Read",			Cmd_DPTX_EdidRead,				NULL,		CMD_MODE_NONE,			";EDID.READ",			(u8*)cmdNote_dpTxEdidRead				},
		{	"I2C.Write",			Cmd_DPTX_I2CWrite,				NULL,		CMD_MODE_NONE,			";I2C.WRITE",			(u8*)cmdNote_dpTxI2cWrite				},
		{	"I2C.Read",				Cmd_DPTX_I2CRead,				NULL,		CMD_MODE_NONE,			";I2C.READ",			(u8*)cmdNote_dpTxI2cRead				},
		{	"SDP.Write",			Cmd_DPTX_SdpWrite,				NULL,		CMD_MODE_NONE,			";SDP.WRITE",			(u8*)cmdNote_dpTxSdpWrite				},
		{	"SDP.Clear",			Cmd_DPTX_SdpContainClear,		NULL,		CMD_MODE_NONE,			";SDP.CLEAR",			(u8*)cmdNote_dpTxSdpContainClear		},
		{	"SDP.Contain",			Cmd_DPTX_SdpContain,			NULL,		CMD_MODE_NONE,			";SDP.CONTAIN",			(u8*)cmdNote_dpTxSdpContain				},
		{	"SDP.MultiWrite",		Cmd_DPTX_SdpMultiWrite,			NULL,		CMD_MODE_NONE,			";SDP.MULTIWRITE",		(u8*)cmdNote_dpTxSdpMultiWrite			},
		{	"DIC.Mode",				Cmd_DPTX_ModeDIC,				NULL,		CMD_MODE_NONE,			";DIC.MODE",			(u8*)cmdNote_dpTxModeDIC				},
		{	"DIC.Write",			Cmd_DPTX_DataWriteDIC,			NULL,		CMD_MODE_NONE,			";DIC.WRITE",			(u8*)cmdNote_dpTxDataWriteDIC			},
		{	"DIC.Write.md",			Cmd_DPTX_DataWriteDIC_md,		NULL,		CMD_MODE_NONE,			";DIC.WRITE.md",		(u8*)cmdNote_dpTxDataWriteDIC_md		},
		{	"DIC.Write.mx",			Cmd_DPTX_DataWriteDIC_mx,		NULL,		CMD_MODE_NONE,			";DIC.WRITE.mx",		(u8*)cmdNote_dpTxDataWriteDIC_mx		},
		{	"DIC.Read",				Cmd_DPTX_DataReadDIC,			NULL,		CMD_MODE_NONE,			";DIC.READ",			(u8*)cmdNote_dpTxDataReadDIC			},
		{	"DIC.Read.md",			Cmd_DPTX_DataReadDIC_md,		NULL,		CMD_MODE_NONE,			";DIC.READ.md", 		(u8*)cmdNote_dpTxDataReadDIC_md	    	},
		{	"DIC.Read.mx",			Cmd_DPTX_DataReadDIC_mx,		NULL,		CMD_MODE_NONE,			";DIC.READ.mx", 		(u8*)cmdNote_dpTxDataReadDIC_mx	    	},
		{	"Lane.Config",			Cmd_DPTX_LaneConfig,			NULL,		CMD_MODE_NONE,			";ML.CONFIG",			(u8*)cmdNote_dpTxLaneConfig				},
		{	"Video.Dummy",			Cmd_DPTX_DummyVideo,			NULL,		CMD_MODE_NONE,			";VIDEO.DUMMY",			(u8*)cmdNote_dpTxDummyVideo				},
		{	"ASSR.Ctrl",			Cmd_DPTX_AssrCtrl,				NULL,		CMD_MODE_NONE,			";ASSR.CTRL",			(u8*)cmdNote_dpTxAssrCtrl				},
		{	"MVID.Fixed",			Cmd_DPTX_FixedMVID,				NULL,		CMD_MODE_NONE,			";MVID.FIXED",			(u8*)cmdNote_dpTxFixedMVID				},
		{	"LT.Full",				Cmd_DPTX_LtFull,				NULL,		CMD_MODE_NONE,			";LT.FULL",				(u8*)cmdNote_dpTxLtFull					},
		{	"LT.Fast",				Cmd_DPTX_LtFast,				NULL,		CMD_MODE_NONE,			";LT.FAST",				(u8*)cmdNote_dpTxLtFast					},
		{	"LT.Quick",				Cmd_DPTX_LtQuick,				NULL,		CMD_MODE_NONE,			";LT.QUICK",			(u8*)cmdNote_dpTxLtQuick				},
		{	"LT.Custom",			Cmd_DPTX_LtCustom,				NULL,		CMD_MODE_NONE,			";LT.CUSTOM",			(u8*)cmdNote_dpTxLtCustom				},
		{	"LT.Done.video",		Cmd_DPTX_LtDoneVideo,			NULL,		CMD_MODE_NONE,			";LT.DONEVIDEO",		(u8*)cmdNote_dpTxLtDoneVideo			},
		{	"Aux.Write_C",			Cmd_DPTX_LtAuxWriteCustom,		NULL,		CMD_MODE_NONE,			";AUX.WRITE_C",			(u8*)cmdNote_dpTxAuxWriteCustom			},
		{	"ALPM.Enter",			Cmd_DPTX_AlpmEnter,				NULL,		CMD_MODE_NONE,			";ALPM.ENTER",			(u8*)cmdNote_dpTxAlpmEnter				},
		{	"ALPM.Exit",			Cmd_DPTX_AlpmExit,				NULL,		CMD_MODE_NONE,			";ALPM.EXIT",			(u8*)cmdNote_dpTxAlpmExit				},
		{	"ALPM.Enable",			Cmd_DPTX_AlpmEnable,			NULL,		CMD_MODE_NONE,			";ALPM.ENABLE",			(u8*)cmdNote_dpTxAlpmEnable				},
		{	"ALPM.Disable",			Cmd_DPTX_AlpmDisable,			NULL,		CMD_MODE_NONE,			";ALPM.DISABLE",		(u8*)cmdNote_dpTxAlpmDisable			},
		{	"ALPM.Lrc",				Cmd_DPTX_AlpmLinkRateChange,	NULL,		CMD_MODE_NONE,			";ALPM.LRCHANGE",		(u8*)cmdNote_dpTxAlpmLinkRateChange		},
		{	"RawData.AUX.Write",	Cmd_ENY_PacketWrite,			NULL,		CMD_MODE_NONE,			";RAWDATA.WRITEAUC",	(u8*)cmdNote_dpTxRawDataAUXWrite		},
		{	"RawData.DIC.Write",	Cmd_ENY_PacketDICWrite,			NULL,		CMD_MODE_NONE,			";RAWDATA.WRITEDIC",	(u8*)cmdNote_dpTxRawDataDICWrite		},
		{	"RawData.AUX.Read",		Cmd_ENY_PacketRead,				NULL,		CMD_MODE_NONE,			";RAWDATA.READAUC",		(u8*)cmdNote_dpTxRawDataAUXRead			},
		{	"RawData.DIC.Read",		Cmd_ENY_PacketDICRead,			NULL,		CMD_MODE_NONE,			";RAWDATA.READDIC",		(u8*)cmdNote_dpTxRawDataDICRead			},
		{	"vvb",					Cmd_VVB,						NULL,		CMD_MODE_NONE,			";VVB",					(u8*)cmdNote_vvb						},
		{	"link.status",			Cmd_DPTX_LinkStatus,			NULL,		CMD_MODE_NONE,			";LINK.STATUS",			(u8*)cmdNote_dpTxLinkStatus				},
		{	"lane.status",			Cmd_DPTX_LaneStatus,			NULL,		CMD_MODE_NONE,			";LANE.STATUS",			(u8*)cmdNote_dpTxLaneStatus				},

		{	NULL,					NULL,							NULL,		NULL,					NULL,					NULL									},
};

