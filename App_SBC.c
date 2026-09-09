#include	"App_SBC.h"

static	UART_HandleTypeDef	*phSbcUart;
static	u8	sbcUartRxBuffer[SBC_UART_BUFFER_SIZE];
static	u8	sbcUartRxTemp;
static	u16	sbcUartRxCnt, sbcUartGetCnt;

static	u16			sbcUartChannel = 0xFFFF;
static	bspGpio_t	sbcUartMuxIo[3];

static void SBC_UartMspInit()
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_USART1_CLK_ENABLE();

	gpio.Pin		= GPIO_PIN_9 | GPIO_PIN_10;
	gpio.Mode		= GPIO_MODE_AF_PP;
	gpio.Pull		= GPIO_PULLUP;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Alternate	= GPIO_AF7_USART1;

	HAL_GPIO_Init(GPIOA, &gpio);
}

static void SBC_UartMspDeInit()
{
}

static void SBC_UartTxCpltCallback()
{
}

static void SBC_UartTxHalfCpltCallback()
{
}

static void SBC_UartRxCpltCallback()
{
	sbcUartRxBuffer[sbcUartRxCnt] = sbcUartRxTemp;

	sbcUartRxCnt += 1;

	if(sbcUartRxCnt > SBC_UART_BUFFER_SIZE)		sbcUartRxCnt = 0;

	SBC_UartReceiveStart();
}

static void SBC_UartRxHalfCpltCallback()
{
}

static void SBC_UartErrorCallback()
{
}

static void SBC_UartAbortCpltCallback()
{
}

static void SBC_UartAbortTransmitCpltCallback()
{
}

static void SBC_UartAbortReceiveCpltCallback()
{
}

u8 SBC_UartInit()
{
	u8	result = 0, rtn;
	HAL_StatusTypeDef	chkr;
	bspUartFops_t		fops;

	memset(&fops, NULL, sizeof(fops));

	phSbcUart = (UART_HandleTypeDef*)BSP_UART_GetHandle(BSP_UART1);
	if(phSbcUart == NULL)			return result;

	fops.MspInit					= SBC_UartMspInit;
	fops.MspDeInit					= SBC_UartMspDeInit;
	fops.TxCpltCallback				= SBC_UartTxCpltCallback;
	fops.TxHalfCpltCallback			= SBC_UartTxHalfCpltCallback;
	fops.RxCpltCallback				= SBC_UartRxCpltCallback;
	fops.RxHalfCpltCallback			= SBC_UartRxHalfCpltCallback;
	fops.ErrorCallback				= SBC_UartErrorCallback;
	fops.AbortCpltCallback			= SBC_UartAbortCpltCallback;
	fops.AbortTransmitCpltCallback	= SBC_UartAbortTransmitCpltCallback;
	fops.AbortReceiveCpltCallback	= SBC_UartAbortReceiveCpltCallback;

	rtn = BSP_UART_RegisterInterface(phSbcUart, &fops);
	if(rtn == 0)					return result;

	phSbcUart->Init.BaudRate		= 115200 * 16;
	phSbcUart->Init.WordLength		= UART_WORDLENGTH_8B;
	phSbcUart->Init.StopBits		= UART_STOPBITS_1;
	phSbcUart->Init.Parity			= UART_PARITY_NONE;
	phSbcUart->Init.Mode			= UART_MODE_TX_RX;
	phSbcUart->Init.HwFlowCtl		= UART_HWCONTROL_NONE;
	phSbcUart->Init.OverSampling	= UART_OVERSAMPLING_16;
	phSbcUart->Init.OneBitSampling	= UART_ONE_BIT_SAMPLE_ENABLE;

	chkr = HAL_UART_Init(phSbcUart);
	if(chkr == HAL_OK)				result = 1;

	HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	return result;
}

u8 SBC_IoInit()
{
	u8	result = 0, rtn, errCnt = 0;

	sbcUartMuxIo[0].Port		= GPIOA;
	sbcUartMuxIo[0].Pin			= GPIO_PIN_8;
	sbcUartMuxIo[0].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbcUartMuxIo[0].wData		= LOW;

	sbcUartMuxIo[1].Port		= GPIOG;
	sbcUartMuxIo[1].Pin			= GPIO_PIN_3;
	sbcUartMuxIo[1].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbcUartMuxIo[1].wData		= LOW;

	sbcUartMuxIo[2].Port		= GPIOK;
	sbcUartMuxIo[2].Pin			= GPIO_PIN_6;
	sbcUartMuxIo[2].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	sbcUartMuxIo[2].wData		= LOW;

	rtn = BSP_GPIO_Init(&sbcUartMuxIo[0]);
	if(rtn == 0)				errCnt += 1;

	rtn = BSP_GPIO_Init(&sbcUartMuxIo[1]);
	if(rtn == 0)				errCnt += 1;

	rtn = BSP_GPIO_Init(&sbcUartMuxIo[2]);
	if(rtn == 0)				errCnt += 1;

	if(errCnt == 0)				result = 1;

	return result;
}

u8 SBC_UartChannelSelect(u16 channel)
{
	u8	result = 0;
	bitCtrl16_t	bitData;

	if(channel > 0x0007)			return result;
	if(sbcUartChannel == channel)	return result;

	bitData.u16Data = channel;

	if(bitData.b0)			BSP_GPIO_WritePin(&sbcUartMuxIo[0], HIGH);
	else					BSP_GPIO_WritePin(&sbcUartMuxIo[0], LOW);

	if(bitData.b1)			BSP_GPIO_WritePin(&sbcUartMuxIo[1], HIGH);
	else					BSP_GPIO_WritePin(&sbcUartMuxIo[1], LOW);

	if(bitData.b2)			BSP_GPIO_WritePin(&sbcUartMuxIo[2], HIGH);
	else					BSP_GPIO_WritePin(&sbcUartMuxIo[2], LOW);

	sbcUartChannel = channel;

	DelayCycles(2000);

	result = 1;

	return result;
}

u8 SBC_UartReceiveStart()
{
	u8	result = 0;
	HAL_StatusTypeDef	rtn;

	rtn = HAL_UART_Receive_IT(phSbcUart, &sbcUartRxTemp, 1);
	if(rtn == HAL_OK)	result = 1;

	return result;
}

u8 SBC_UartGetChar(u8 *pData)
{
	u8	result = 0;

	if(sbcUartGetCnt == sbcUartRxCnt)			return result;

	*pData = sbcUartRxBuffer[sbcUartGetCnt];
	sbcUartGetCnt += 1;

	if(sbcUartGetCnt > SBC_UART_BUFFER_SIZE)	sbcUartGetCnt = 0;

	result = 1;

	return result;
}

u8 SBC_UartTransmitData(u8 *pData, u16 size)
{
	u8	result = 0;
	u32	timeOut = 1000;
	HAL_StatusTypeDef	rtn;

	rtn = HAL_UART_Transmit(phSbcUart, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}
