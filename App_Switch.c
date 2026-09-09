#include	"App_Switch.h"

static	bspGpio_t	switchIO[6];
static	u32	switchFlag = 0;

u8 Switch_GpioInit()
{
	u8	result = 0, rtn, errCnt = 0;

	switchIO[0].Port	= GPIOK;
	switchIO[0].Pin		= GPIO_PIN_0;
	switchIO[0].Mode	= BSP_GPIO_MODE_INPUT;

	switchIO[1].Port	= GPIOK;
	switchIO[1].Pin		= GPIO_PIN_1;
	switchIO[1].Mode	= BSP_GPIO_MODE_INPUT;

	switchIO[2].Port	= GPIOK;
	switchIO[2].Pin		= GPIO_PIN_2;
	switchIO[2].Mode	= BSP_GPIO_MODE_INPUT;

	switchIO[3].Port	= GPIOK;
	switchIO[3].Pin		= GPIO_PIN_3;
	switchIO[3].Mode	= BSP_GPIO_MODE_INPUT;

	switchIO[4].Port	= GPIOK;
	switchIO[4].Pin		= GPIO_PIN_4;
	switchIO[4].Mode	= BSP_GPIO_MODE_INPUT;

	switchIO[5].Port	= GPIOK;
	switchIO[5].Pin		= GPIO_PIN_5;
	switchIO[5].Mode	= BSP_GPIO_MODE_INPUT;

	rtn = BSP_GPIO_Init(&switchIO[0]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&switchIO[1]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&switchIO[2]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&switchIO[3]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&switchIO[4]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&switchIO[5]);
	if(rtn == 0)		errCnt += 1;

	if(errCnt == 0)		result = 1;

	return result;
}

u8 Switch_GetIoStatus()
{
	bitCtrl8_t	swData;

	swData.u8Data	= 0;

	swData.b0		= !BSP_GPIO_ReadPin(&switchIO[0]);
	swData.b1		= !BSP_GPIO_ReadPin(&switchIO[1]);
	swData.b2		= !BSP_GPIO_ReadPin(&switchIO[2]);
	swData.b3		= !BSP_GPIO_ReadPin(&switchIO[3]);
	swData.b4		= !BSP_GPIO_ReadPin(&switchIO[4]);
	swData.b5		= !BSP_GPIO_ReadPin(&switchIO[5]);

	return swData.u8Data;
}

u32	Switch_GetFlag()
{
	return switchFlag;
}

u8 Switch_ClearFlag(u8 position)
{
	u8	result = 0;

	if(position >= 32)		return result;

	switchFlag &= ~(0x00000001 << position);

	result = 1;

	return result;
}

u8 Switch_SetFlag(u8 position)
{
	u8	result = 0;

	if(position >= 32)		return result;

	switchFlag |= (0x00000001 << position);

	result = 1;

	return result;
}

static void Switch_Checker()
{
	static	u8	swDataLegacy = 0xff;
	static	u16	swTime = 0;

	u8	swData;

	swData = Switch_GetIoStatus();

	if((swData != 0) && (Script_GetFlag() == 0))
	{
		if(swData == swDataLegacy)
		{
			swTime += 1;

			if((swTime >= 200) && (swTime < 1500))
			{
				if(swData == 0x01)			Script_SetFlag(0);
				else if(swData == 0x02)		Script_SetFlag(1);
				else if(swData == 0x04)		Script_SetFlag(2);
				else if(swData == 0x08)		Script_SetFlag(3);
				else if(swData == 0x10)		Script_SetFlag(4);
				else if(swData == 0x20)		Script_SetFlag(5);
				else if(swData == 0x40)		Script_SetFlag(6);
				else if(swData == 0x80)		Script_SetFlag(7);
			}
		}
		else
		{
			swTime = 0;
		}
	}
	else
	{
		swTime = 0;
	}

	swDataLegacy = swData;
}

u8 Switch_Init()
{
	u8	result = 0, rtn;
	bspSysTickFops_t	fops;

	memset(&fops, NULL, sizeof(fops));

	fops.SysTickFunction		= Switch_Checker;

	rtn = BSP_SysTick_RegisterInterface(1, &fops);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

