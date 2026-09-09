#include	"App_CoreSPI.h"

static	SPI_HandleTypeDef	*phCoreSPI;
static	bspGpio_t			coreSpiCS[4];
static	u8					coreSpiDataMode = 0xff;

static void CoreSPI_MspInit()
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_SPI1_CLK_ENABLE();

	gpio.Pin			= GPIO_PIN_5 | GPIO_PIN_6;
	gpio.Mode			= GPIO_MODE_AF_PP;
	gpio.Pull			= GPIO_PULLUP;
	gpio.Speed			= GPIO_SPEED_HIGH;
	gpio.Alternate		= GPIO_AF5_SPI1;

	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Pin			= GPIO_PIN_5;

	HAL_GPIO_Init(GPIOB, &gpio);
}

u8 CoreSPI_Init()
{
	u8	result = 0, rtn;
	HAL_StatusTypeDef	chkr;
	bspSpiFops_t	fops;

	phCoreSPI = (SPI_HandleTypeDef*)BSP_SPI_GetHandle(BSP_SPI1);
	if(phCoreSPI == NULL)					return result;

	memset(&fops, NULL, sizeof(fops));

	fops.MspInit						= CoreSPI_MspInit;

	rtn = BSP_SPI_RegisterInterface(phCoreSPI, &fops);
	if(rtn == 0)						return result;

	phCoreSPI->Init.Mode					= SPI_MODE_MASTER;
	phCoreSPI->Init.Direction				= SPI_DIRECTION_2LINES;
	phCoreSPI->Init.DataSize				= SPI_DATASIZE_8BIT;
	phCoreSPI->Init.CLKPolarity				= SPI_POLARITY_LOW;
	phCoreSPI->Init.CLKPhase				= SPI_PHASE_1EDGE;
	phCoreSPI->Init.NSS						= SPI_NSS_SOFT;
	phCoreSPI->Init.BaudRatePrescaler		= SPI_BAUDRATEPRESCALER_16;
	phCoreSPI->Init.FirstBit				= SPI_FIRSTBIT_MSB;
	phCoreSPI->Init.TIMode					= SPI_TIMODE_DISABLE;
	phCoreSPI->Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLE;
	phCoreSPI->Init.CRCPolynomial			= 0;
	phCoreSPI->Init.CRCLength				= SPI_CRC_LENGTH_DATASIZE;
	phCoreSPI->Init.NSSPMode				= SPI_NSS_PULSE_DISABLE;

	chkr = HAL_SPI_Init(phCoreSPI);
	if(chkr == HAL_OK)					result = 1;

	return result;
}

u8 CoreSPI_IoInit()
{
	u8	result = 0, rtn, errCnt = 0;

	coreSpiCS[0].Port			= GPIOA;
	coreSpiCS[0].Pin			= GPIO_PIN_4;
	coreSpiCS[0].Mode			= BSP_GPIO_MODE_OUTPUT_PU;
	coreSpiCS[0].wData			= HIGH;

	coreSpiCS[1].Port			= GPIOG;
	coreSpiCS[1].Pin			= GPIO_PIN_6;
	coreSpiCS[1].Mode			= BSP_GPIO_MODE_OUTPUT_PU;
	coreSpiCS[1].wData			= HIGH;

	coreSpiCS[2].Port			= GPIOG;
	coreSpiCS[2].Pin			= GPIO_PIN_7;
	coreSpiCS[2].Mode			= BSP_GPIO_MODE_OUTPUT_PU;
	coreSpiCS[2].wData			= HIGH;

	coreSpiCS[3].Port			= GPIOG;
	coreSpiCS[3].Pin			= GPIO_PIN_12;
	coreSpiCS[3].Mode			= BSP_GPIO_MODE_OUTPUT_PU;
	coreSpiCS[3].wData			= HIGH;

	rtn = BSP_GPIO_Init(&coreSpiCS[0]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&coreSpiCS[1]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&coreSpiCS[2]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&coreSpiCS[3]);
	if(rtn == 0)		errCnt += 1;

	if(errCnt == 0)		result = 1;

	return result;
}

u8 CoreSPI_CsCtrl(u8 position, u8 data)
{
	u8	result = 0, rtn = 0;

	if(position >= 4)		return result;
	if(data > 1)			return result;

	rtn = BSP_GPIO_WritePin(&coreSpiCS[position], data);

	if(rtn != 0)			result = 1;

	return result;
}

u8 CoreSPI_DataModeCtrl(u8 mode)
{
	u8	result = 1;
	u32	regData, mask = 0x00001F00;

	regData = phCoreSPI->Instance->CR2;

	switch(mode)
	{
		case 0:						//	8 Bit Mode
			coreSpiDataMode = mode;

			regData &= ~mask;

			regData |= 0x0700;			// Data Size 8 Bit, DS = 0x07;
			regData |= 0x1000;			// FIFO Reception Threshold, FRXTH = 1;

			phCoreSPI->Init.DataSize	= SPI_DATASIZE_8BIT;
			break;

		case 1:						//	9 Bit Mode
			coreSpiDataMode = mode;

			regData &= ~mask;

			regData |= 0x0800;			// Data Size 9 Bit, DS = 0x08;
			regData |= 0x0000;			// FIFO Reception Threshold, FRXTH = 0;

			phCoreSPI->Init.DataSize	= SPI_DATASIZE_9BIT;
			break;

		default:
			result = 0;
			break;
	}

	phCoreSPI->Instance->CR2 = regData;

	return result;
}

u8 CoreSPI_WriteByte(u8 type, u8 data)
{
	u8	result = 0, rtn;
	u16	wData = 0;
	HAL_StatusTypeDef	chkr;

	if(coreSpiDataMode != CORE_SPI_DATA_MODE_WRITE)
	{
		rtn = CoreSPI_DataModeCtrl(CORE_SPI_DATA_MODE_WRITE);
		if(rtn == 0)		return result;
	}

	switch(type)
	{
		case CORE_SPI_WRITE_MODE_CMD:			//	CMD
			wData	= data;
			wData	|= 0x0000;
			break;

		case CORE_SPI_WRITE_MODE_DATA:			//	DATA
			wData	= data;
			wData	|= 0x0100;
			break;

		default:
			return result;
			break;
	}

	chkr = HAL_SPI_Transmit(phCoreSPI, (u8*)&wData, 1, 2);
	if(chkr == HAL_OK)		result = 1;

	return result;
}

u8 CoreSPI_Write(u8 type, u8 *pData, u32 size)
{
	u8	result = 0, rtn;
	u32	cnt;

	for(cnt = 0; cnt < size; cnt++)
	{
		rtn = CoreSPI_WriteByte(type, pData[cnt]);
		if(rtn == 0)		return result;
	}

	result = 1;

	return result;
}

u8 CoreSpi_Read(u8 *pData, u32 size)
{
	u8	result = 0, rtn;
	u32	cnt = 0, remainSize = size;
	HAL_StatusTypeDef	chkr;

	if(coreSpiDataMode != CORE_SPI_DATA_MODE_READ)
	{
		rtn = CoreSPI_DataModeCtrl(CORE_SPI_DATA_MODE_READ);
		if(rtn == 0)		return result;
	}

	do{
		if(remainSize > 10240)
		{
			chkr = HAL_SPI_Receive(phCoreSPI, &pData[cnt], 10240, 1000);
			if(chkr != HAL_OK)		return result;

			remainSize	-= 10240;
			cnt			+= 10240;
		}
		else
		{
			chkr = HAL_SPI_Receive(phCoreSPI, &pData[cnt], (u16)remainSize, 1000);
			if(chkr != HAL_OK)		return result;

			break;
		}
	}while(1);

	result = 1;

	return result;
}

u8 CoreSPI_TransmitData(u8 *pData, u16 size)
{
	u8	result = 0;
	u32	timeOut = 1000;
	HAL_StatusTypeDef	rtn;

	rtn = HAL_SPI_Transmit(phCoreSPI, pData, size, timeOut);
	if(rtn == HAL_OK)			result = 1;

	return result;
}

u8 CoreSPI_ReceiveData(u8 *pData, u16 size)
{
	u8	result = 0;
	u32	timeOut = 1000;
	HAL_StatusTypeDef	rtn;

	rtn = HAL_SPI_Receive(phCoreSPI, pData, size, timeOut);
	if(rtn == HAL_OK)			result = 1;

	return result;
}

