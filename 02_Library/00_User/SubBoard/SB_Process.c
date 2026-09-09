#include	"SB_Process.h"

extern	sbHandle_t		sbHandle;

extern	uint32_t HAL_GetTick(void);

u8 SB_SubBoardXfer(void *pVoid)
{
	u8	result = 0, rtn = 0, chkr = 0, errCnt = 0;
	u8	srcID, rxTemp;
	u32	cmpChkr, sTick, eTick;
	u32	waitTime;
	sbCmd_t	cmd;
	sbParsingData_t	*pData;

	pData = (sbParsingData_t*)pVoid;

	if(sbHandle.ChannelSelect != NULL)		sbHandle.ChannelSelect(pData->channel);

	SBP_DataInit(SBP_TX_DATA);

	rtn = SBP_DataMaker(&sbHandle.txPkt, (u8)pData->position, pData->cmd, pData->pTxData, pData->txSize);
	if(rtn == 0)
	{
		return result;
	}

	if(pData->waitTime == 0)								waitTime = SUBBOARD_XFER_TYP_TIMEOUT;
	else if(pData->waitTime > SUBBOARD_XFER_MAX_TIMEOUT)	waitTime = SUBBOARD_XFER_MAX_TIMEOUT;
	else													waitTime = pData->waitTime;

	SBP_DataInit(SBP_RX_DATA);

	rtn = SBP_DataSender(&sbHandle.txPkt);
	if(rtn == 0)
	{
		return result;
	}

	sTick = HAL_GetTick();

	do{
		rtn = sbHandle.GetChar(&rxTemp);
		if(rtn)
		{
			chkr = SBP_DataReceiver(rxTemp, &sbHandle.rxPkt);
			if(chkr)
			{
				break;
			}
		}

		eTick = HAL_GetTick();
		if((eTick - sTick) > waitTime)
		{
			errCnt += 1;
			break;
		}
	}while(1);

	if(chkr)
	{
		rtn = SBP_DataChecker(&sbHandle.rxPkt, &srcID, &cmd.u16Data, sbHandle.pRxBuffer);
		if(rtn == 0)										errCnt += 1;

		if(sbHandle.rxPkt.cmd != (pData->cmd | 0x8000))		errCnt += 1;

		cmpChkr = memcmp(pData->pTxData, sbHandle.pRxBuffer, pData->txSize);
		if(cmpChkr != 0)									errCnt += 1;
	}

	if(errCnt == 0)
	{
		pData->rxSize = sbHandle.rxPkt.length - pData->txSize;

		memcpy(pData->pRxData, &sbHandle.pRxBuffer[pData->txSize], pData->rxSize);

		result = 1;
	}

	return result;
}

u8 SB_CommandExecute(void *pVoid)
{
	u8	result = 0, execute = 0;
	u32	cmdCount = 0;
	sbParsingData_t	*pData;

	pData = (sbParsingData_t*)pVoid;

	memcpy(pData->pTxData, pData->pRxData, pData->rxSize);
	pData->txSize = pData->rxSize;

	do{
		if(sbHandle.pCmdList[cmdCount].cmd == NULL)
		{
			execute = 0;
			break;
		}

		if(sbHandle.pCmdList[cmdCount].cmd == pData->cmd)
		{
			execute = 1;
			break;
		}

		cmdCount += 1;
	}while(1);

	if(execute)
	{
		result = sbHandle.pCmdList[cmdCount].pFunction(pVoid);
	}

	return result;
}

u8 SB_CommandProcess()
{
	u8	result = 0;
	u8	rtn, chkr, pktEnd;
	u8	srcID, rxTemp;
	sbCmd_t	cmd;
	sbParsingData_t	parsingData;

	if(sbHandle.GetChar == NULL)			return result;

	rtn = sbHandle.GetChar(&rxTemp);
	if(rtn == 0)							return result;
	else
	{
		pktEnd = SBP_DataReceiver(rxTemp, &sbHandle.rxPkt);
		if(pktEnd == 0)						return result;
	}

	if(sbHandle.RunningState	!= NULL)	sbHandle.RunningState(HIGH);
	if(sbHandle.ErrorState		!= NULL)	sbHandle.ErrorState(LOW);

	rtn = SBP_DataChecker(&sbHandle.rxPkt, &srcID, &cmd.u16Data, sbHandle.pRxBuffer);
	if(rtn == 0)
	{		
		SBP_DataInit(SBP_TX_DATA);

		rtn = SBP_ErrorSender(sbHandle.rxPkt.srcID, sbHandle.rxPkt.cmd);
		if(rtn == 0)
		{
			//	Error
		}

		if(sbHandle.RunningState	!= NULL)	sbHandle.RunningState(LOW);
		if(sbHandle.ErrorState		!= NULL)	sbHandle.ErrorState(HIGH);

		SBP_DataInit(SBP_RX_DATA);

		return result;
	}

	parsingData.cmd		= (u32)cmd.u16Data;
	parsingData.pRxData	= sbHandle.pRxBuffer;
	parsingData.pTxData	= sbHandle.pTxBuffer;
	parsingData.rxSize	= sbHandle.rxPkt.length;
	parsingData.txSize	= 0;

	rtn = SB_CommandExecute(&parsingData);
	
	SBP_DataInit(SBP_TX_DATA);

	if(rtn)
	{
		cmd.response	= 1;

		chkr = SBP_DataMaker(&sbHandle.txPkt, srcID, cmd.u16Data, parsingData.pTxData, parsingData.txSize);
		if(chkr == 0)
		{
			//	Error
		}

		chkr = SBP_DataSender(&sbHandle.txPkt);
		if(chkr == 0)
		{
			//	Error
		}
	}
	else
	{
		rtn = SBP_ErrorSender(sbHandle.rxPkt.srcID, sbHandle.rxPkt.cmd);
		if(rtn == 0)
		{
			//	Error
		}

		if(sbHandle.ErrorState != NULL)		sbHandle.ErrorState(HIGH);
	}

	if(sbHandle.RunningState != NULL)		sbHandle.RunningState(LOW);

	SBP_DataInit(SBP_RX_DATA);

	result = 1;

	return result;
}

u8 SB_SubBoardXfer_boot(void *pVoid)
{
	u8	result = 0, rtn = 0;
	sbParsingData_t	*pData;

	pData = (sbParsingData_t*)pVoid;

	if(sbHandle.ChannelSelect != NULL)		sbHandle.ChannelSelect(pData->channel);

	SBP_DataInit(SBP_TX_DATA);

	rtn = SBP_DataMaker(&sbHandle.txPkt, (u8)pData->position, pData->cmd, pData->pTxData, pData->txSize);
	if(rtn == 0)
	{
		return result;
	}

	rtn = SBP_DataSender(&sbHandle.txPkt);
	if(rtn == 1)
	{
		return result=1;
	}

	return result;
}

u8 SB_SubBoardXfer_boot_check(void *pVoid)
{
	u8	result = 0, rtn = 0, chkr = 0, errCnt = 0;
	u8	srcID, rxTemp;
	u32	cmpChkr, sTick, eTick;
	u32	waitTime;
	sbCmd_t	cmd;
	sbParsingData_t	*pData;

	pData = (sbParsingData_t*)pVoid;

	SBP_DataInit(SBP_RX_DATA);

	waitTime = pData->waitTime;

	sTick = HAL_GetTick();

	do{ 
		rtn = sbHandle.GetChar(&rxTemp);
		if(rtn)
		{
			chkr = SBP_DataReceiver(rxTemp, &sbHandle.rxPkt);
			if(chkr)
			{
				break;
			}
		}

		eTick = HAL_GetTick();
		if((eTick - sTick) > waitTime)
		{
			errCnt += 1;
			break;
		}
	}while(1);

	if(chkr)
	{
		rtn = SBP_DataChecker(&sbHandle.rxPkt, &srcID, &cmd.u16Data, sbHandle.pRxBuffer);
		//if(rtn == 1)										result =1;
		if(rtn == 0)										errCnt += 1;

		if(sbHandle.rxPkt.cmd != (pData->cmd | 0x8000))		errCnt += 1;

		cmpChkr = memcmp(pData->pTxData, sbHandle.pRxBuffer, pData->txSize); 
		if(cmpChkr != 0)									errCnt += 1;
	}

	if(errCnt == 0)
	{
		pData->rxSize = sbHandle.rxPkt.length - 4; //tx+rx value is in here

		memcpy(pData->pRxData, &sbHandle.pRxBuffer[4], pData->rxSize);

		result = 1;
	}

	return result;
}