#include	"App_Command.h"

commandList_t	*pCommandList = NULL;

static u8 CMD_RunningState(u8 data)
{
	return HSP_LedCtrl(1, data);
}

static u8 CMD_ErrorState(u8 data)
{
	return HSP_LedCtrl(2, data);
}

u8 CMD_RcvCheck()
{
	u8	result = 0;

	UsbPkt_PacketProcess();

	NetworkPKT_PacketProcess();

	Serial_PacketProcess();

	result = 1;

	return result;
}

u8 CMD_ColorConverter(u8 *pData, u32 *pColor)
{
	u8 result = 1;

//	BASED ON "10BIT COLOR"

	if(CMD_Compare(pData, "black"))					*pColor = 0x00000000;
	else if(CMD_Compare(pData, "white"))			*pColor = 0x3fffffff;
	else if(CMD_Compare(pData, "red"))				*pColor = 0x3ff00000;
	else if(CMD_Compare(pData, "green"))			*pColor = 0x000ffc00;
	else if(CMD_Compare(pData, "blue"))				*pColor = 0x000003ff;
	else if(CMD_Compare(pData, "cyan"))				*pColor = 0x000fffff;
	else if(CMD_Compare(pData, "magenta"))			*pColor = 0x3ff003ff;
	else if(CMD_Compare(pData, "yellow"))			*pColor = 0x3ffffc00;
	else if(CMD_Compare(pData, "gray75"))			*pColor = 0x0ff3fcff;
	else if(CMD_Compare(pData, "gray50"))			*pColor = 0x1ff7fdff;
	else if(CMD_Compare(pData, "gray25"))			*pColor = 0x2ffbfeff;
	else											result = 0;

	return result;
}

u8 CMD_SubCommandExecute(void *pvFuncData, void *pvCmdList)
{
	u8	result = 0, chkr = 0;
	u32	cmdCnt = 0;
	cmdFunctionData_t	*pFunctionData, functionData;
	commandList_t		*pCmdList;

	if(pvFuncData == NULL)				return result;

	pFunctionData	= (cmdFunctionData_t*)pvFuncData;
	pCmdList		= (commandList_t*)pvCmdList;

	if(pFunctionData->argc == 0)		return result;

	do{
		if(pCmdList[cmdCnt].pCmd == NULL)
		{
			if(pFunctionData->retryCnt == 0)
			{
				CMD_Printf(";NO_COMMAND");
				pFunctionData->retryCnt += 1;
			}
			
			return result;
		}

		chkr = CMD_Compare(pFunctionData->pArgv[0], (char*)pCmdList[cmdCnt].pCmd);
		if(chkr)
		{
			break;
		}

		cmdCnt += 1;
	}while(1);

	functionData.srcType	= pFunctionData->srcType;
	functionData.pCmdList	= (commandList_t*)pvCmdList;
	functionData.pArgv		= &pFunctionData->pArgv[1];
	functionData.argc		= pFunctionData->argc - 1;
	functionData.option		= pCmdList[cmdCnt].option;
	functionData.channel	= pFunctionData->channel;
	functionData.position	= pFunctionData->position;

	if(pCmdList[cmdCnt].mode != NULL)
	{
		if(pFunctionData->deactivate != NULL)
		{
			return result;
		}

		if(pFunctionData->retryCnt != 0)
		{
			result = 1;

			return result;
		}
	}

	if(pCmdList[cmdCnt].pMessage != NULL)
	{
		if(pFunctionData->retryCnt == 0)
		{
			CMD_Printf((char*)pCmdList[cmdCnt].pMessage);
		}
	}

	pFunctionData->retryCnt += 1;

	result = pCmdList[cmdCnt].pFunction(&functionData);

	return result;
}

u8 CMD_Init()
{
	u8	result = 0, rtn;
	cmdSetHandle_t	handle;

	if(pCommandList == NULL)						return result;

	memset(&handle, NULL, sizeof(handle));

	handle.parseMaxCount							= 8192;

	handle.receiveFlag								= 0;

	handle.rxSrcType								= 0;
	handle.rxDataSize								= 128 * 1024;
	handle.pRxData									= (u8*)MEMORY_MAP_SDRAM_CMD_RX_BUFFER_ADRS;
	handle.rxDataCnt								= 0;

	handle.txSrcType								= 0;
	handle.txDataSize								= 256 * 1024;
	handle.pTxData									= (u8*)MEMORY_MAP_SDRAM_CMD_TX_BUFFER_ADRS;
	handle.rxDataCnt								= 0;

	handle.pCmdList									= pCommandList;

	handle.RunningState								= CMD_RunningState;
	handle.ErrorState								= CMD_ErrorState;

	handle.TransmitData[CMD_COMM_SRC_TYPE_USB]		= UsbPkt_TransmitPacket;
	handle.TransmitData[CMD_COMM_SRC_TYPE_ETHERNET]	= NetworkPKT_TransmitPacket;
	handle.TransmitData[CMD_COMM_SRC_TYPE_SERIAL]	= Serial_TransmitData;
	handle.TransmitData[CMD_COMM_SRC_TYPE_SCRIPT]	= NULL;

	rtn = CMD_RegisterInterface(&handle);
	if(rtn != 0)									result = 1;

	return result;
}

