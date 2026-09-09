#include	"App_GPIO.h"

u8 LCM_GpioInit(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_INIT;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_GpioDirection(u8 position, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_DIRECTION;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_GpioWrite(u8 position, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_WRITE;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_GpioRead(u8 position, u8 *pState)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_READ;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 1)				return result;

	*pState = sbParsingData.pRxData[0];

	result = 1;

	return result;
}

u8 LCM_GpioSet(u8 position, u8 data)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_SET;
	sbParsingData.pTxData[sbParsingData.txSize]	= data;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_GpioClear(u8 position, u8 data)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_CLEAR;
	sbParsingData.pTxData[sbParsingData.txSize]	= data;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_GpioPulseInit(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_PULSE_INIT;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_GpioPulseDeInit(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_PULSE_DEINIT;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_GpioPulseRead(u8 position, float *pPulseAverage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_PULSE_READ;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 4)				return result;

	*pPulseAverage = SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 LCM_GpioPulseCountRead(u8 position, u32 *pCurrentCnt, u32 *pLoopCnt)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_GPIO_PULSE_COUNT_READ;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 8)				return result;

	*pCurrentCnt	= SYS_HexToWord(&sbParsingData.pRxData[0]);
	*pLoopCnt		= SYS_HexToWord(&sbParsingData.pRxData[4]);

	result = 1;

	return result;
}

