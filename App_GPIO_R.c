#include	"App_GPIO_R.h"

u8 GPIO_R_Init(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= 0;
	sbParsingData.cmd							= SB_CMD_PWR_R_GPIO_INIT;
	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 GPIO_R_Direction(u8 position, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= 0;
	sbParsingData.cmd							= SB_CMD_PWR_R_GPIO_DIRECTION;
	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 GPIO_R_Write(u8 position, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= 0;
	sbParsingData.cmd							= SB_CMD_PWR_R_GPIO_WRITE;
	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 GPIO_R_Read(u8 *pState)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= 0;
	sbParsingData.cmd							= SB_CMD_PWR_R_GPIO_READ;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 4)				return result;

	memcpy(pState, sbParsingData.pRxData, sbParsingData.rxSize);

	result = 1;

	return result;
}

u8 GPIO_R_Set(u8 position, u8 data)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= 0;
	sbParsingData.cmd							= SB_CMD_PWR_R_GPIO_SET;
	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= data;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 GPIO_R_Clear(u8 position, u8 data)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= 0;
	sbParsingData.cmd							= SB_CMD_PWR_R_GPIO_CLEAR;
	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= data;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}


