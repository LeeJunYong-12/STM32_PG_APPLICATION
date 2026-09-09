#include	"CmdSet_Process.h"

cmdSetHandle_t	cmdSetHandle;

extern	u8 CMD_Analysis();

u8 CMD_RegisterInterface(void *pVoid)
{
	u8	result = 0;
	cmdSetHandle_t	*pData;

	if(pVoid == NULL)								return result;

	pData = (cmdSetHandle_t*)pVoid;

	if(pData->parseMaxCount > CMDSET_MAX_COUNT)		return result;

	memcpy(&cmdSetHandle, pVoid, sizeof(cmdSetHandle));

	result = 1;

	return result;
}

u8 CMD_PutReceiveData(u32 src, u8 *pData, u32 length)
{
	u8	result = 0;

	if(src >= CMDSET_COMM_SRC_COUNT)			return result;
	if(length > cmdSetHandle.rxDataSize)		return result;

	memcpy(cmdSetHandle.pRxData, pData, length);
	cmdSetHandle.rxDataCnt	= length;

	memset(&cmdSetHandle.pRxData[length], NULL, 4);

	cmdSetHandle.rxSrcType = src;

	cmdSetHandle.receiveFlag = 1;

	result = 1;

	return result;
}

static u8 CMD_ReceiveCheck()
{
	u8	result = 0;

	result = (u8)cmdSetHandle.receiveFlag;

	return result;
}

static u8 CMD_ReceiveDataClear()
{
	u8	result = 0;

	cmdSetHandle.rxSrcType = 0;
	cmdSetHandle.rxDataCnt = 0;
//	memset(cmdSetHandle.pRxData, NULL, cmdSetHandle.rxDataSize);

	cmdSetHandle.receiveFlag = 0;

	result = 1;

	return result;
}

static u8 CMD_TransmitDataClear()
{
	u8	result = 0;

	cmdSetHandle.txSrcType = cmdSetHandle.rxSrcType;
	cmdSetHandle.txDataCnt = 0;
//	memset(cmdSetHandle.pTxData, NULL, cmdSetHandle.txDataSize);

	result = 1;

	return result;
}

u8 CMD_Process()
{
	u8	result = 0, rtn = 0;
	u8	chkr;

	chkr = CMD_ReceiveCheck();
	if(chkr == 0)				return result;

	CMD_RunningState(HIGH);
	CMD_ErrorState(LOW);

	chkr = CMD_Analysis();
	if(chkr == 0)
	{
		CMD_TransmitDataClear();
		CMD_Printf(">>NO_COMMAND;ERROR\r\n");
		CMD_TransmitData();
		goto CMD_PROCESS_ERROR;
	}				

	CMD_TransmitDataClear();
	
	rtn = CMD_Execute();
	if(rtn == 0)
	{
		CMD_ErrorState(HIGH);
	}

	chkr = CMD_TransmitData();
	
	if(chkr != 0)				result = 1;

	CMD_RunningState(LOW);

	CMD_PROCESS_ERROR:
	CMD_ReceiveDataClear();

	return result;
}
