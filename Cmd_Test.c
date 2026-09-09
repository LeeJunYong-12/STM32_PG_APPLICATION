#include	"Cmd_Test.h"

static u8 CRC_STATE = 0, CORE_CHANNEL = 3;
static u8 CRC_DATA_R0 = 0x00, CRC_DATA_R1 = 0x00;
static u8 CRC_DATA_G0 = 0x00, CRC_DATA_G1 = 0x00;
static u8 CRC_DATA_B0 = 0x00, CRC_DATA_B1 = 0x00;

static u8 Cmd_Core_Select(void *pVoid)
{
	u8	result = 0;

	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "core1"))				CORE_CHANNEL = 2;
	else if(CMD_Compare(pData->pArgv[0], "core2"))			CORE_CHANNEL = 3;
	else													return result;
	
	result = 1;
	
	return result;
}

static u8 Cmd_CRC(void *pVoid)
{
	u8	result = 0, state;

	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	dptxTransaction_t	dptxTransaction;
	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	if(CMD_Compare(pData->pArgv[0], "enable"))					state = 1;
	else if(CMD_Compare(pData->pArgv[0], "disable"))			state = 0;
	else													return result;

	if(state)
	{
		CRC_DATA_R0 = CMD_StrToUL(pData->pArgv[1]);
		CRC_DATA_R1 = CMD_StrToUL(pData->pArgv[2]);
		CRC_DATA_G0 = CMD_StrToUL(pData->pArgv[3]);
		CRC_DATA_G1 = CMD_StrToUL(pData->pArgv[4]);
		CRC_DATA_B0 = CMD_StrToUL(pData->pArgv[5]);
		CRC_DATA_B1 = CMD_StrToUL(pData->pArgv[6]);
		CRC_STATE = 1;

		dptxTransaction.channel	= CORE_CHANNEL + 1;
		dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_WRITE;
		dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;
		dptxTransaction.adrs	= 0x270;
		dptxTransaction.size	= 1;

		dptxTransaction.pData[0] = 1;

		DPTX_Transaction(&dptxTransaction);
	}		
	else
	{
		DPC_DirectRegWriteSingle(CORE_CHANNEL, 0x03, 0);
		CRC_STATE = 0;
	}			
	
	result = 1;

	return result;
}

static u8 Cmd_CRC_Read(void *pVoid)
{
	u8	result = 0;

	dptxTransaction_t	dptxTransaction;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= CORE_CHANNEL + 1;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_READ;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxTransaction.adrs	= 0x240;
	dptxTransaction.size	= 6;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		for(u32 cnt = 0; cnt < dptxTransaction.size; cnt++)
		{
			if(cnt != (dptxTransaction.size - 1))		CMD_Printf("0x%02X ", dptxTransaction.pData[cnt]);
			else										CMD_Printf("0x%02X", dptxTransaction.pData[cnt]);
		}
	}

	return result;
}

u8 CRC_Test_Function()
{
	u8	result = 0;

	if(!CRC_STATE)	return result;

	dptxTransaction_t	dptxTransaction;
	
	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= CORE_CHANNEL + 1;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_READ;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxTransaction.adrs	= 0x240;
	dptxTransaction.size	= 6;

	result = DPTX_Transaction(&dptxTransaction);

	if(result)
	{
		if(CRC_DATA_R0 !=dptxTransaction.pData[0])	DPC_DirectRegWriteSingle(CORE_CHANNEL, 0x03, 0x01);
		if(CRC_DATA_R1 !=dptxTransaction.pData[1])	DPC_DirectRegWriteSingle(CORE_CHANNEL, 0x03, 0x02);
		if(CRC_DATA_G0 !=dptxTransaction.pData[2])	DPC_DirectRegWriteSingle(CORE_CHANNEL, 0x03, 0x04);
		if(CRC_DATA_G1 !=dptxTransaction.pData[3])	DPC_DirectRegWriteSingle(CORE_CHANNEL, 0x03, 0x08);
		if(CRC_DATA_B0 !=dptxTransaction.pData[4])	DPC_DirectRegWriteSingle(CORE_CHANNEL, 0x03, 0x10);
		if(CRC_DATA_B1 !=dptxTransaction.pData[5])	DPC_DirectRegWriteSingle(CORE_CHANNEL, 0x03, 0x20);	
	}

	return result;
}

u8 Cmd_Uart1_Tx(void *pVoid)
{
	u8	result = 0, rtn, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= 2;
	sbParsingData.cmd							= SB_CMD_PWR_M_UART1_TX;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	
	sbParsingData.channel						= 3;
	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;
	
	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 Cmd_Uart1_Rx(void *pVoid)
{
	u8	result = 0, rtn, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	state = CMD_StrToUL(pData->pArgv[0]);

	rtn = SB_DataInit();
	if(rtn == 0)								return result;	

	sbParsingData.channel						= 2;
	sbParsingData.cmd							= SB_CMD_PWR_M_UART1_RX;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	
	sbParsingData.channel						= 3;
	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;
	
	if(sbParsingData.rxSize != 0)				return result;

	result = 1;
	
	return result;
}

const	commandList_t	cmdList_Test[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"crc.select",			Cmd_Core_Select,			NULL,		NULL,		";CRC.SELECT",			NULL},
												{	"crc",					Cmd_CRC,					NULL,		NULL,		";CRC",					NULL},
												{	"crc.read",				Cmd_CRC_Read,				NULL,		NULL,		";CRC.READ",			NULL},
												{	"uart1.tx",				Cmd_Uart1_Tx,				NULL,		NULL,		";UART1.TX",			NULL},
												{	"uart1.rx",				Cmd_Uart1_Rx,				NULL,		NULL,		";UART1.RX",			NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};