#include	"App_ExtI2C.h"

static	I2C_HandleTypeDef	*phExtI2C[2];
static	bspGpio_t			extI2C1Mux;

u8 ExtI2C_IoInit()
{
	u8	result = 0;

	extI2C1Mux.Port		= GPIOB;
	extI2C1Mux.Pin		= GPIO_PIN_12;
	extI2C1Mux.Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	extI2C1Mux.wData	= LOW;

	result = BSP_GPIO_Init(&extI2C1Mux);

	return result;
}

u8 ExtI2C_ConnectEEPROM(u8 state)
{
	u8	result = 0;

	if(state > 1)			return result;

	result = BSP_GPIO_WritePin(&extI2C1Mux, state);

	return result;
}

static void ExtI2C_I2C0MspInit()
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_I2C4_CLK_ENABLE();

	gpio.Pin		= GPIO_PIN_8 | GPIO_PIN_9;
	gpio.Mode		= GPIO_MODE_AF_OD;
	gpio.Pull		= GPIO_PULLUP;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Alternate	= GPIO_AF1_I2C4;

	HAL_GPIO_Init(GPIOB, &gpio);
}

static void ExtI2C_I2C0MspDeInit()
{
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);

	__HAL_RCC_I2C4_CLK_DISABLE();
}

u8 ExtI2C_I2C0Init()
{
	u8	result = 0, rtn;
	u32	timing;
	HAL_StatusTypeDef	chkr;
	bspI2CFops_t	fops;

	memset(&fops, NULL, sizeof(fops));

	phExtI2C[0] = (I2C_HandleTypeDef*)BSP_I2C_GetHandle(BSP_I2C4);
	if(phExtI2C[0] == NULL)			return result;

	fops.MspInit					= ExtI2C_I2C0MspInit;
	fops.MspDeInit					= ExtI2C_I2C0MspDeInit;

	rtn = BSP_I2C_RegisterInterface(phExtI2C[0], &fops);
	if(rtn == 0)					return result;

	rtn = BSP_I2C_GetTiming(100, &timing);
	if(rtn == 0)					return result;

	phExtI2C[0]->Init.Timing				= timing;
	phExtI2C[0]->Init.OwnAddress1			= 0;
	phExtI2C[0]->Init.AddressingMode		= I2C_ADDRESSINGMODE_7BIT;
	phExtI2C[0]->Init.DualAddressMode		= I2C_DUALADDRESS_DISABLE;
	phExtI2C[0]->Init.OwnAddress2			= 0;
	phExtI2C[0]->Init.OwnAddress2Masks		= I2C_OA2_NOMASK;
	phExtI2C[0]->Init.GeneralCallMode		= I2C_GENERALCALL_DISABLE;
	phExtI2C[0]->Init.NoStretchMode			= I2C_NOSTRETCH_ENABLE;

	chkr = HAL_I2C_Init(phExtI2C[0]);
	if(chkr != HAL_OK)				return result;

	chkr = HAL_I2CEx_ConfigDigitalFilter(phExtI2C[0], 0);
	if(chkr != HAL_OK)				return result;

	chkr = HAL_I2CEx_ConfigAnalogFilter(phExtI2C[0], I2C_ANALOGFILTER_ENABLE);
	if(chkr != HAL_OK)				return result;

	HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C4);

	result = 1;

	return result;
}

u8 ExtI2C_I2C0DeInit()
{
	u8	result = 0, rtn;
	HAL_StatusTypeDef	chkr;
	bspI2CFops_t	fops;

	memset(&fops, NULL, sizeof(fops));

	phExtI2C[0] = (I2C_HandleTypeDef*)BSP_I2C_GetHandle(BSP_I2C4);
	if(phExtI2C[0] == NULL)			return result;

	fops.MspInit					= ExtI2C_I2C0MspInit;
	fops.MspDeInit					= ExtI2C_I2C0MspDeInit;

	rtn = BSP_I2C_RegisterInterface(phExtI2C[0], &fops);
	if(rtn == 0)					return result;

	chkr = HAL_I2C_DeInit(phExtI2C[0]);
	if(chkr != HAL_OK)				return result;

	result = 1;

	return result;
}

static void ExtI2C_I2C1MspInit()
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_I2C2_CLK_ENABLE();

	gpio.Mode		= GPIO_MODE_AF_OD;
	gpio.Pull		= GPIO_NOPULL;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Alternate	= GPIO_AF4_I2C2;

	gpio.Pin		= GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &gpio);

	gpio.Pin		= GPIO_PIN_4;
	HAL_GPIO_Init(GPIOH, &gpio);
}

static void ExtI2C_I2C1MspDeInit()
{
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);
	HAL_GPIO_DeInit(GPIOH, GPIO_PIN_4);

	__HAL_RCC_I2C2_CLK_DISABLE();
}

u8 ExtI2C_I2C1Init()
{
	u8	result = 0, rtn;
	u32	timing;
	HAL_StatusTypeDef	chkr;
	bspI2CFops_t	fops;

	memset(&fops, NULL, sizeof(fops));

	phExtI2C[1] = (I2C_HandleTypeDef*)BSP_I2C_GetHandle(BSP_I2C2);
	if(phExtI2C[1] == NULL)			return result;

	fops.MspInit					= ExtI2C_I2C1MspInit;
	fops.MspDeInit					= ExtI2C_I2C1MspDeInit;

	rtn = BSP_I2C_RegisterInterface(phExtI2C[1], &fops);
	if(rtn == 0)					return result;

	rtn = BSP_I2C_GetTiming(100, &timing);
	if(rtn == 0)					return result;

	phExtI2C[1]->Init.Timing				= timing;
	phExtI2C[1]->Init.OwnAddress1			= 0;
	phExtI2C[1]->Init.AddressingMode		= I2C_ADDRESSINGMODE_7BIT;
	phExtI2C[1]->Init.DualAddressMode		= I2C_DUALADDRESS_DISABLE;
	phExtI2C[1]->Init.OwnAddress2			= 0;
	phExtI2C[1]->Init.OwnAddress2Masks		= I2C_OA2_NOMASK;
	phExtI2C[1]->Init.GeneralCallMode		= I2C_GENERALCALL_DISABLE;
	phExtI2C[1]->Init.NoStretchMode			= I2C_NOSTRETCH_ENABLE;

	chkr = HAL_I2C_Init(phExtI2C[1]);
	if(chkr != HAL_OK)				return result;

	chkr = HAL_I2CEx_ConfigDigitalFilter(phExtI2C[1], 0);
	if(chkr != HAL_OK)				return result;

	chkr = HAL_I2CEx_ConfigAnalogFilter(phExtI2C[1], I2C_ANALOGFILTER_ENABLE);
	if(chkr != HAL_OK)				return result;

	HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C2);

	result = 1;

	return result;
}

u8 ExtI2C_I2C1DeInit()
{
	u8	result = 0, rtn;
	HAL_StatusTypeDef	chkr;
	bspI2CFops_t	fops;

	memset(&fops, NULL, sizeof(fops));

	phExtI2C[1] = (I2C_HandleTypeDef*)BSP_I2C_GetHandle(BSP_I2C2);
	if(phExtI2C[1] == NULL)			return result;

	fops.MspInit					= ExtI2C_I2C1MspInit;
	fops.MspDeInit					= ExtI2C_I2C1MspDeInit;

	rtn = BSP_I2C_RegisterInterface(phExtI2C[1], &fops);
	if(rtn == 0)					return result;

	chkr = HAL_I2C_DeInit(phExtI2C[1]);
	if(chkr != HAL_OK)				return result;

	result = 1;

	return result;
}

u8 ExtI2C_AdrsScan(u8 position, u8 *pAdrs)
{
	u8	result = 0;
	u8	cnt = 0;
	u16	devAdrs;
	u32	timeOut = 10;
	HAL_StatusTypeDef	rtn;

	if(position >= 2)		return result;

	do{
		devAdrs = (cnt << 1) & ~0x01;

		rtn = HAL_I2C_IsDeviceReady(phExtI2C[position], devAdrs, 2, timeOut);
		if(rtn == HAL_OK)
		{
			pAdrs[result] = (u8)devAdrs;

			result += 1;
		}

		cnt += 1;

		if(cnt >= 0x80)		break;
	}while(1);

	return result;
}

u8 ExtI2C_TransmitData(u8 position, u8 devAdrs, u8 *pData, u16 size)
{
	u8	result = 0;
	u32	timeOut = 1000;
	HAL_StatusTypeDef	rtn;

	if(position >= 2)		return result;

	rtn = HAL_I2C_Master_Transmit(phExtI2C[position], devAdrs, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 ExtI2C_ReceiveData(u8 position, u8 devAdrs, u8 *pData, u16 size)
{
	u8	result = 0;
	u32	timeOut = 1000;
	HAL_StatusTypeDef	rtn;

	if(position >= 2)		return result;

	rtn = HAL_I2C_Master_Receive(phExtI2C[position], devAdrs, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 ExtI2C_MemWrite(u8 position, u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size)
{
	u8	result = 0;
	u16	memAdrsSize = I2C_MEMADD_SIZE_8BIT;
	u32	timeOut = 1000;
	HAL_StatusTypeDef	rtn;

	if(position >= 2)		return result;
	if(adrsMode > 1)		return result;

	if(adrsMode == 1)		memAdrsSize = I2C_MEMADD_SIZE_16BIT;

	rtn = HAL_I2C_Mem_Write(phExtI2C[position], devAdrs, memAdrs, memAdrsSize, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 ExtI2C_MemRead(u8 position, u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size)
{
	u8	result = 0;
	u16	memAdrsSize = I2C_MEMADD_SIZE_8BIT;
	u32	timeOut = 1000;
	HAL_StatusTypeDef	rtn;

	if(position >= 2)		return result;
	if(adrsMode > 1)		return result;

	if(adrsMode == 1)		memAdrsSize = I2C_MEMADD_SIZE_16BIT;

	rtn = HAL_I2C_Mem_Read(phExtI2C[position], devAdrs, memAdrs, memAdrsSize, pData, size, timeOut);
	if(rtn == HAL_OK)		result = 1;

	return result;
}

