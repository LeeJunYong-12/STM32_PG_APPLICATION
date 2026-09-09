#include	"App_I2C.h"

I2C_HandleTypeDef	*phI2C;

static void I2C_MspInit()
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_I2C1_CLK_ENABLE();

	gpio.Pin		= GPIO_PIN_6 | GPIO_PIN_7;
	gpio.Mode		= GPIO_MODE_AF_OD;
	gpio.Pull		= GPIO_NOPULL;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Alternate	= GPIO_AF4_I2C1;

	HAL_GPIO_Init(GPIOB, &gpio);
}

u8 I2C_Init()
{
	u8	result = 0, rtn;
	u32	timing;
	HAL_StatusTypeDef	chkr;
	bspI2CFops_t	fops;

	memset(&fops, NULL, sizeof(fops));

	phI2C = (I2C_HandleTypeDef*)BSP_I2C_GetHandle(BSP_I2C1);
	if(phI2C == NULL)				return result;

	fops.MspInit					= I2C_MspInit;

	rtn = BSP_I2C_RegisterInterface(phI2C, &fops);
	if(rtn == 0)					return result;

	rtn = BSP_I2C_GetTiming(100, &timing);
	if(rtn == 0)					return result;

	phI2C->Init.Timing				= timing;
	phI2C->Init.OwnAddress1			= 0;
	phI2C->Init.AddressingMode		= I2C_ADDRESSINGMODE_7BIT;
	phI2C->Init.DualAddressMode		= I2C_DUALADDRESS_DISABLE;
	phI2C->Init.OwnAddress2			= 0;
	phI2C->Init.OwnAddress2Masks	= I2C_OA2_NOMASK;
	phI2C->Init.GeneralCallMode		= I2C_GENERALCALL_DISABLE;
	phI2C->Init.NoStretchMode		= I2C_NOSTRETCH_ENABLE;

	chkr = HAL_I2C_Init(phI2C);
	if(chkr != HAL_OK)				return result;

	chkr = HAL_I2CEx_ConfigDigitalFilter(phI2C, 0);
	if(chkr != HAL_OK)				return result;

	chkr = HAL_I2CEx_ConfigAnalogFilter(phI2C, I2C_ANALOGFILTER_ENABLE);
	if(chkr != HAL_OK)				return result;

	HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);

	result = 1;

	return result;
}

u8 I2C_AdrsScan(u8 *pData)
{
	u8	cnt = 0, checkCnt = 0;
	u8 	devAdrs;
	HAL_StatusTypeDef	rtn;

	do{
		devAdrs = (cnt << 1) & 0xFE;

		rtn = HAL_I2C_IsDeviceReady(phI2C, devAdrs, 2, 2);
		if(rtn == HAL_OK)
		{
			pData[cnt]	= devAdrs;
			checkCnt	+= 1;
		}

		if(cnt >= 0x7F)		break;

		cnt += 1;
	}while(1);

	return checkCnt;
}

u8 I2C_Receive(u8 devAdrs, u8 *pData, u16 size)
{
	u8	result = 0;
	u32	timeOut;
	HAL_StatusTypeDef	rtn;

	timeOut = (size * 15) / 100;
	if(timeOut < 10)		timeOut = 10;

	rtn = HAL_I2C_Master_Receive(phI2C, devAdrs, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 I2C_Transmit(u8 devAdrs, u8 *pData, u16 size)
{
	u8	result = 0;
	u32	timeOut;
	HAL_StatusTypeDef	rtn;

	timeOut = (size * 15) / 100;
	if(timeOut < 10)		timeOut = 10;

	rtn = HAL_I2C_Master_Transmit(phI2C, devAdrs, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 I2C_MemRead(u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size)
{
	u8	result = 0;
	u16	mode;
	u32	timeOut;
	HAL_StatusTypeDef	rtn;

	if(adrsMode == 0)		mode = I2C_MEMADD_SIZE_8BIT;
	else if(adrsMode == 1)	mode = I2C_MEMADD_SIZE_16BIT;
	else					return result;

	timeOut = (size * 15) / 100;
	if(timeOut < 10)		timeOut = 10;

	rtn = HAL_I2C_Mem_Read(phI2C, devAdrs, memAdrs, mode, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 I2C_MemWrite(u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size)
{
	u8	result = 0;
	u16	mode;
	u32	timeOut;
	HAL_StatusTypeDef	rtn;

	if(adrsMode == 0)		mode = I2C_MEMADD_SIZE_8BIT;
	else if(adrsMode == 1)	mode = I2C_MEMADD_SIZE_16BIT;
	else					return result;

	timeOut = (size * 15) / 100;
	if(timeOut < 10)		timeOut = 10;

	rtn = HAL_I2C_Mem_Write(phI2C, devAdrs, memAdrs, mode, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}


