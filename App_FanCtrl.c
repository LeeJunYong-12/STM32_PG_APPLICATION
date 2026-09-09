#include	"App_FanCtrl.h"

static	bspGpio_t	*pFanIO[FAN_MAX_COUNT];

u8 FanCtrl_IoInit(u8 position, u32 ioAdrs)
{
	u8	result = 0;

	if(position >= FAN_MAX_COUNT)			return result;
	if(ioAdrs == NULL)
	{
		pFanIO[position] = NULL;

		result = 1;

		return result;
	}

	pFanIO[position] = (bspGpio_t*)ioAdrs;

	pFanIO[position]->Mode	= BSP_GPIO_MODE_OUTPUT_PU;
	pFanIO[position]->wData	= HIGH;

	result = BSP_GPIO_Init(pFanIO[position]);

	return result;
}

u8 FanCtrl_SetStatus(u8 position, u8 status)
{
	u8	result = 0;

	if(position >= FAN_MAX_COUNT)			return result;
	if(status > 1)							return result;

	if(pFanIO[position] == NULL)			return result;

	result = BSP_GPIO_WritePin(pFanIO[position], status);

	return result;
}

