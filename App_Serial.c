#include	"App_Serial.h"

static	UART_HandleTypeDef	*phSerial;

static	u8	*pSerialRxBuffer = (u8*)MEMORY_MAP_SDRAM_SERIAL_RX_BUFFER_ADRS;
static	u8	serialRxTemp;
static	u16	serialRxCnt, serialGetCnt;

static	u8	*pSerialCmdBuffer = (u8*)MEMORY_MAP_SDRAM_SERIAL_CMD_BUFFER_ADRS;
static	u32	serialCmdRxSize;

static void Serial_MspInit()
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_USART6_CLK_ENABLE();

	gpio.Pin		= GPIO_PIN_6 | GPIO_PIN_7;
	gpio.Mode		= GPIO_MODE_AF_PP;
	gpio.Pull		= GPIO_PULLUP;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Alternate	= GPIO_AF8_USART6;

	HAL_GPIO_Init(GPIOC, &gpio);
}

static void Serial_MspDeInit()
{
}

static void Serial_TxCpltCallback()
{
}

static void Serial_TxHalfCpltCallback()
{
}

static void Serial_RxCpltCallback()
{
	pSerialRxBuffer[serialRxCnt] = serialRxTemp;

	serialRxCnt += 1;

	HAL_UART_Receive_IT(phSerial, &serialRxTemp, 1);
}

static void Serial_RxHalfCpltCallback()
{
}

static void Serial_ErrorCallback()
{
}

static void Serial_AbortCpltCallback()
{
}

static void Serial_AbortTransmitCpltCallback()
{
}

static void Serial_AbortReceiveCpltCallback()
{
}

u8 Serial_DataInit()
{
	u8	result = 0;

	serialRxCnt = 0;
	serialGetCnt = 0;
	serialCmdRxSize = 0;

	return result;
}

u8 Serial_Init()
{
	u8	result = 0, rtn;
	bspUartFops_t	fops;
	HAL_StatusTypeDef	chkr;

	phSerial = (UART_HandleTypeDef*)BSP_UART_GetHandle(BSP_UART6);
	if(phSerial == NULL)				return result;

	memset(&fops, NULL, sizeof(fops));

	fops.MspInit						= Serial_MspInit;
	fops.MspDeInit						= Serial_MspDeInit;
	fops.TxCpltCallback					= Serial_TxCpltCallback;
	fops.TxHalfCpltCallback				= Serial_TxHalfCpltCallback;
	fops.RxCpltCallback					= Serial_RxCpltCallback;
	fops.RxHalfCpltCallback				= Serial_RxHalfCpltCallback;
	fops.ErrorCallback					= Serial_ErrorCallback;
	fops.AbortCpltCallback				= Serial_AbortCpltCallback;
	fops.AbortTransmitCpltCallback		= Serial_AbortTransmitCpltCallback;
	fops.AbortReceiveCpltCallback		= Serial_AbortReceiveCpltCallback;

	rtn = BSP_UART_RegisterInterface(phSerial, &fops);
	if(rtn == 0)						return result;

	phSerial->Init.BaudRate				= 115200;
	phSerial->Init.WordLength			= UART_WORDLENGTH_8B;
	phSerial->Init.StopBits				= UART_STOPBITS_1;
	phSerial->Init.Parity				= UART_PARITY_NONE;
	phSerial->Init.Mode					= UART_MODE_TX_RX;
	phSerial->Init.HwFlowCtl			= UART_HWCONTROL_NONE;
	phSerial->Init.OverSampling			= UART_OVERSAMPLING_16;
	phSerial->Init.OneBitSampling		= UART_ONE_BIT_SAMPLE_ENABLE;

	chkr = HAL_UART_Init(phSerial);
	if(chkr != HAL_OK)					return result;

	HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USART6_IRQn);

	Serial_DataInit();

	chkr = HAL_UART_Receive_IT(phSerial, &serialRxTemp, 1);
	if(chkr != HAL_OK)					return result;

	result = 1;

	return result;
}

u8 Serial_GetChar(u8 *pData)
{
	u8	result = 0;

	if(serialGetCnt == serialRxCnt)			return result;

	*pData = pSerialRxBuffer[serialGetCnt];

	serialGetCnt += 1;

	result = 1;

	return result;
}

u8 Serial_TransmitData(u8 *pData, u32 size)
{
	u8	result = 0;
	u32	writeSize = 1024, timeOut = 1000;
	u32	cnt = 0, remainSize = size;
	HAL_StatusTypeDef	rtn;

	do{
		if(remainSize > writeSize)
		{
			rtn = HAL_UART_Transmit(phSerial, &pData[cnt], (u16)writeSize, timeOut);
			if(rtn != HAL_OK)			return result;

			cnt			+= writeSize;
			remainSize	-= writeSize;
		}
		else
		{
			rtn = HAL_UART_Transmit(phSerial, &pData[cnt], (u16)remainSize, timeOut);
			if(rtn != HAL_OK)			return result;

			break;
		}
	}while(1);

	result = 1;

	return result;
}

u8 Serial_PacketProcess()
{
	u8	result = 0, rtn;
	u8	temp;

	do{
		rtn = Serial_GetChar(&temp);
		if(rtn == 0)		return result;

		pSerialCmdBuffer[serialCmdRxSize] = temp;

		serialCmdRxSize += 1;

		if(temp == '\r')	break;
	}while(1);

	rtn = CMD_PutReceiveData(CMD_COMM_SRC_TYPE_SERIAL, pSerialCmdBuffer, serialCmdRxSize);
	if(rtn != 0)			result = 1;

	serialCmdRxSize = 0;

	return result;
}

