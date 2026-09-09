#include	"BSP_GPIO.h"

static	bspGpioExtiFops_t	bspGpioExtiFops[16];

u8 BSP_GPIO_DataInit()
{
	u8	result = 0;

	memset(bspGpioExtiFops, NULL, sizeof(bspGpioExtiFops));

	result = 1;

	return result;
}

u8 BSP_GPIO_Init(void *pVoid)
{
	u8	result = 1;
	bspGpio_t	*pData;
	GPIO_InitTypeDef	gpio;

	pData = (bspGpio_t*)pVoid;

	switch(pData->Mode)
	{
		case BSP_GPIO_MODE_INPUT:
			gpio.Pin		= pData->Pin;
			gpio.Mode		= GPIO_MODE_INPUT;
			gpio.Pull		= GPIO_PULLUP;
			gpio.Speed		= GPIO_SPEED_HIGH;

			HAL_GPIO_Init(pData->Port, &gpio);

			pData->rData	= HAL_GPIO_ReadPin(pData->Port, pData->Pin);
			break;

		case BSP_GPIO_MODE_OUTPUT_OD:
			gpio.Pin		= pData->Pin;
			gpio.Mode		= GPIO_MODE_OUTPUT_OD;
			gpio.Pull		= GPIO_NOPULL;
			gpio.Speed		= GPIO_SPEED_HIGH;

			HAL_GPIO_Init(pData->Port, &gpio);

			HAL_GPIO_WritePin(pData->Port, pData->Pin, (GPIO_PinState)pData->wData);
			break;

		case BSP_GPIO_MODE_OUTPUT_PU:
			gpio.Pin		= pData->Pin;
			gpio.Mode		= GPIO_MODE_OUTPUT_PP;
			gpio.Pull		= GPIO_PULLUP;
			gpio.Speed		= GPIO_SPEED_HIGH;

			HAL_GPIO_Init(pData->Port, &gpio);

			HAL_GPIO_WritePin(pData->Port, pData->Pin, (GPIO_PinState)pData->wData);
			break;

		case BSP_GPIO_MODE_OUTPUT_NP:
			gpio.Pin		= pData->Pin;
			gpio.Mode		= GPIO_MODE_OUTPUT_PP;
			gpio.Pull		= GPIO_NOPULL;
			gpio.Speed		= GPIO_SPEED_HIGH;

			HAL_GPIO_Init(pData->Port, &gpio);

			HAL_GPIO_WritePin(pData->Port, pData->Pin, (GPIO_PinState)pData->wData);
			break;

		case BSP_GPIO_MODE_OUTPUT_PD:
			gpio.Pin		= pData->Pin;
			gpio.Mode		= GPIO_MODE_OUTPUT_PP;
			gpio.Pull		= GPIO_PULLDOWN;
			gpio.Speed		= GPIO_SPEED_HIGH;

			HAL_GPIO_Init(pData->Port, &gpio);

			HAL_GPIO_WritePin(pData->Port, pData->Pin, (GPIO_PinState)pData->wData);
			break;

		default:
			result = 0;
			break;
	}

	return result;
}

u8 BSP_GPIO_Deinit(void *pVoid)
{
	u8	result = 0;
	bspGpio_t	*pData;

	pData = (bspGpio_t*)pVoid;

	HAL_GPIO_DeInit(pData->Port, pData->Pin);

	result = 1;

	return result;
}

u8 BSP_GPIO_ReadPin(void *pVoid)
{
	bspGpio_t	*pData;

	pData = (bspGpio_t*)pVoid;

	if(pData->Mode == BSP_GPIO_MODE_INPUT)
	{
		pData->rData = HAL_GPIO_ReadPin(pData->Port, pData->Pin) & 0x01;
	}
	else
	{
		pData->rData = pData->wData;
	}

	return pData->rData;
}

u8 BSP_GPIO_WritePin(void *pVoid, u8 state)
{
	u8 result = 1;
	bspGpio_t	*pData;

	pData = (bspGpio_t*)pVoid;

	pData->wData = state & 0x01;

	if(pData->Mode != BSP_GPIO_MODE_INPUT)
	{
		HAL_GPIO_WritePin(pData->Port, pData->Pin, (GPIO_PinState)pData->wData);
	}

	return result;
}

u8 BSP_GPIO_Toggle(void *pVoid)
{
	bspGpio_t	*pData;

	pData = (bspGpio_t*)pVoid;

	if(pData->wData == 0)
	{
		return BSP_GPIO_WritePin(pVoid, 1);
	}
	else
	{
		return BSP_GPIO_WritePin(pVoid, 0);
	}
}

u8 BSP_GPIO_RegisterInterface(u16 gpioPin, void *pVoid)
{
	u8	result = 1;

	if(gpioPin == GPIO_PIN_0)			memcpy(&bspGpioExtiFops[0], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_1)		memcpy(&bspGpioExtiFops[1], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_2)		memcpy(&bspGpioExtiFops[2], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_3)		memcpy(&bspGpioExtiFops[3], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_4)		memcpy(&bspGpioExtiFops[4], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_5)		memcpy(&bspGpioExtiFops[5], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_6)		memcpy(&bspGpioExtiFops[6], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_7)		memcpy(&bspGpioExtiFops[7], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_8)		memcpy(&bspGpioExtiFops[8], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_9)		memcpy(&bspGpioExtiFops[9], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_10)		memcpy(&bspGpioExtiFops[10], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_11)		memcpy(&bspGpioExtiFops[11], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_12)		memcpy(&bspGpioExtiFops[12], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_13)		memcpy(&bspGpioExtiFops[13], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_14)		memcpy(&bspGpioExtiFops[14], pVoid, sizeof(bspGpioExtiFops_t));
	else if(gpioPin == GPIO_PIN_15)		memcpy(&bspGpioExtiFops[15], pVoid, sizeof(bspGpioExtiFops_t));
	else								result = 0;
	
	return result;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	u8	cnt = 0;

	if(GPIO_Pin == GPIO_PIN_0)			cnt = 0;
	else if(GPIO_Pin == GPIO_PIN_1)		cnt = 1;
	else if(GPIO_Pin == GPIO_PIN_2)		cnt = 2;
	else if(GPIO_Pin == GPIO_PIN_3)		cnt = 3;
	else if(GPIO_Pin == GPIO_PIN_4)		cnt = 4;
	else if(GPIO_Pin == GPIO_PIN_5)		cnt = 5;
	else if(GPIO_Pin == GPIO_PIN_6)		cnt = 6;
	else if(GPIO_Pin == GPIO_PIN_7)		cnt = 7;
	else if(GPIO_Pin == GPIO_PIN_8)		cnt = 8;
	else if(GPIO_Pin == GPIO_PIN_9)		cnt = 9;
	else if(GPIO_Pin == GPIO_PIN_10)	cnt = 10;
	else if(GPIO_Pin == GPIO_PIN_11)	cnt = 11;
	else if(GPIO_Pin == GPIO_PIN_12)	cnt = 12;
	else if(GPIO_Pin == GPIO_PIN_13)	cnt = 13;
	else if(GPIO_Pin == GPIO_PIN_14)	cnt = 14;
	else if(GPIO_Pin == GPIO_PIN_15)	cnt = 15;
	else								return;

	if(bspGpioExtiFops[cnt].ExtiFunc != NULL)
	{
		bspGpioExtiFops[cnt].ExtiFunc();
	}


}


