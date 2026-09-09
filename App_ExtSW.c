#include	"App_ExtSW.h"

extSW_t	extSW;

u8 ExtSW_DataInit()
{
	u8	result = 0;

	memset(&extSW, NULL, sizeof(extSW));

	extSW.effective = 6;

	result = 1;

	return result;
}

u8 ExtSW_GpioInit()
{
	u8	result = 0, rtn, errCnt = 0;

	extSW.effective	= 6;

	extSW.io[0].Port		= GPIOK;
	extSW.io[0].Pin			= GPIO_PIN_0;
	extSW.io[0].Mode		= BSP_GPIO_MODE_INPUT;

	extSW.io[1].Port		= GPIOK;
	extSW.io[1].Pin			= GPIO_PIN_1;
	extSW.io[1].Mode		= BSP_GPIO_MODE_INPUT;

	extSW.io[2].Port		= GPIOK;
	extSW.io[2].Pin			= GPIO_PIN_2;
	extSW.io[2].Mode		= BSP_GPIO_MODE_INPUT;

	extSW.io[3].Port		= GPIOK;
	extSW.io[3].Pin			= GPIO_PIN_3;
	extSW.io[3].Mode		= BSP_GPIO_MODE_INPUT;

	extSW.io[4].Port		= GPIOK;
	extSW.io[4].Pin			= GPIO_PIN_4;
	extSW.io[4].Mode		= BSP_GPIO_MODE_INPUT;

	extSW.io[5].Port		= GPIOK;
	extSW.io[5].Pin			= GPIO_PIN_5;
	extSW.io[5].Mode		= BSP_GPIO_MODE_INPUT;

	rtn = BSP_GPIO_Init(&extSW.io[0]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&extSW.io[1]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&extSW.io[2]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&extSW.io[3]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&extSW.io[4]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&extSW.io[5]);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 ExtSW_IoRead()
{
	u8	cnt, status = 0;

	for(cnt = 0; cnt < extSW.effective; cnt++)
	{
		status |= !BSP_GPIO_ReadPin(&extSW.io[cnt]) << cnt;
	}

	extSW.status = status;
	
	return extSW.status;
}

static void ExtSW_Process()
{
	static	u16	exeTick = 0, tickCount = 0;
	static	u8	swData_c = 0, swData_l = 0;

	if(exeTick == 0)
	{
		swData_c = ExtSW_IoRead();

		if((swData_c == 0x00) && (swData_c != swData_l))
		{
			if(extSW.flag[0] != 0)
			{
			}
			else if((tickCount >= 200) && (tickCount < 2000))
			{
				if(swData_l == 0x01)			extSW.flag[0] = 1;
				else if(swData_l == 0x02)		extSW.flag[0] = 2;
				else if(swData_l == 0x04)		extSW.flag[0] = 3;
				else if(swData_l == 0x08)		extSW.flag[0] = 4;
				else if(swData_l == 0x10)		extSW.flag[0] = 5;
				else if(swData_l == 0x20)		extSW.flag[0] = 6;
				else							extSW.flag[0] = 0;
			}
		}
		else if((swData_c != 0x00) && (swData_c != swData_l))
		{
			tickCount = 0;
		}
		else
		{
			tickCount += 10;
		}

		swData_l = swData_c;
	}

	exeTick += 1;
	if(exeTick >= 10)		exeTick -= 10;
}

u8 ExtSW_Init()
{
	u8	result = 0, rtn, errCnt = 0;
	bspSysTickFops_t	fops;

	rtn = ExtSW_DataInit();
	if(rtn == 0)				errCnt += 1;

	rtn = ExtSW_GpioInit();
	if(rtn == 0)				errCnt += 1;

	fops.SysTickFunction = ExtSW_Process;

	rtn = BSP_SysTick_RegisterInterface(2, &fops);
	if(rtn == 0)				errCnt += 1;

	if(errCnt == 0)				result = 1;

	return result;
}

u8 ExtSW_GetFlag()
{
	return extSW.flag[0];
}

u8 ExtSW_ClearFlag()
{
	u8	result = 0;

	extSW.flag[0] = 0;

	result = 1;

	return result;
}

