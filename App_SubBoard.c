#include	"App_SubBoard.h"

static	u8	sbRxBuffer[4096], sbTxBuffer[4096], sbTransmitBuffer[8192], sbTempBuffer[4096];
static	u8	sbParsingRxBuffer[4096], sbParsingTxBuffer[4096];

static	bspGpio_t	sbGpio[4][2];

sbParsingData_t	sbParsingData;

static u8 SB_IoInit()
{
	u8	result = 0, rtn, errCnt = 0;
/*
	sbGpio[0][0].Port		= GPIOJ;
	sbGpio[0][0].Pin		= GPIO_PIN_0;
	sbGpio[0][0].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbGpio[0][0].wData		= HIGH;

	sbGpio[0][1].Port		= GPIOJ;
	sbGpio[0][1].Pin		= GPIO_PIN_1;
	sbGpio[0][1].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbGpio[0][1].wData		= HIGH;*/

	sbGpio[1][0].Port		= GPIOJ;
	sbGpio[1][0].Pin		= GPIO_PIN_2;
	sbGpio[1][0].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbGpio[1][0].wData		= HIGH;

	sbGpio[1][1].Port		= GPIOJ;
	sbGpio[1][1].Pin		= GPIO_PIN_3;
	sbGpio[1][1].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbGpio[1][1].wData		= HIGH;

	sbGpio[2][0].Port		= GPIOJ;
	sbGpio[2][0].Pin		= GPIO_PIN_4;
	sbGpio[2][0].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbGpio[2][0].wData		= HIGH;

	sbGpio[2][1].Port		= GPIOJ;
	sbGpio[2][1].Pin		= GPIO_PIN_5;
	sbGpio[2][1].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbGpio[2][1].wData		= HIGH;

	sbGpio[3][0].Port		= GPIOJ;
	sbGpio[3][0].Pin		= GPIO_PIN_6;
	sbGpio[3][0].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbGpio[3][0].wData		= HIGH;

	sbGpio[3][1].Port		= GPIOJ;
	sbGpio[3][1].Pin		= GPIO_PIN_7;
	sbGpio[3][1].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbGpio[3][1].wData		= HIGH;

//	rtn = BSP_GPIO_Init(&sbGpio[0][0]);
//	if(rtn == 0)			errCnt += 1;

//	rtn = BSP_GPIO_Init(&sbGpio[0][1]);
//	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&sbGpio[1][0]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&sbGpio[1][1]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&sbGpio[2][0]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&sbGpio[2][1]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&sbGpio[3][0]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&sbGpio[3][1]);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 SB_DataInit()
{
	u8	result = 0;

 	memset(&sbParsingData, NULL, sizeof(sbParsingData));

	sbParsingData.pRxData = sbParsingRxBuffer;
	sbParsingData.pTxData = sbParsingTxBuffer;

	result = 1;

	return result;
}

u8 SB_Init()
{
	u8	result = 0, rtn, errCnt = 0;
	sbHandle_t	handle;

	memset(&handle, NULL, sizeof(handle));

	handle.myID					= 0;
	handle.mode					= 0;
	handle.type					= 0;
	handle.spare				= 0;

	handle.pRxBuffer			= sbRxBuffer;
	handle.rxBufferSize			= sizeof(sbRxBuffer);
	handle.pTxBuffer			= sbTxBuffer;
	handle.txBufferSize			= sizeof(sbTxBuffer);
	handle.pTempBuffer			= sbTempBuffer;
	handle.tempBufferSize		= sizeof(sbTempBuffer);
	handle.pTransmitBuffer		= sbTransmitBuffer;
	handle.transmitBufferSize	= sizeof(sbTransmitBuffer);

	handle.pCmdList				= 0;

	handle.ChannelSelect		= SBC_UartChannelSelect;

	handle.GetChar				= SBC_UartGetChar;
	handle.TransmitData			= SBC_UartTransmitData;

	handle.RunningState			= 0;
	handle.ErrorState			= 0;

	rtn = SB_IoInit();
	if(rtn == 0)			errCnt += 1;

	rtn = SB_RegisterInterface(&handle);
	if(rtn == 0)			errCnt += 1;

	SBC_UartReceiveStart();

	if(errCnt == 0)			result = 1;

	return result;
}

u8 SB_IoCtrl(u8 position, u8 channel, u8 state)
{
	u8	result = 0;

	if(position >= BOARD_CHANNEL_MAX_COUNT)		return result;
	if(channel >= 2)							return result;
	if(state > 1)								return result;

	result = BSP_GPIO_WritePin(&sbGpio[position][channel], state);

	return result;
}


