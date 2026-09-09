#include	"HSP_Led.h"

static	bspGpio_t	hspLedIO[3];

static u8 HSP_LedGpioInit()
{
	u8	result = 0, rtn, errCnt = 0;

	hspLedIO[0].Port	= GPIOE;
	hspLedIO[0].Pin		= GPIO_PIN_4;
	hspLedIO[0].Mode	= BSP_GPIO_MODE_OUTPUT_PU;
	hspLedIO[0].wData	= LOW;

	hspLedIO[1].Port	= GPIOE;
	hspLedIO[1].Pin		= GPIO_PIN_3;
	hspLedIO[1].Mode	= BSP_GPIO_MODE_OUTPUT_PU;
	hspLedIO[1].wData	= LOW;

	hspLedIO[2].Port	= GPIOE;
	hspLedIO[2].Pin		= GPIO_PIN_6;
	hspLedIO[2].Mode	= BSP_GPIO_MODE_OUTPUT_PU;
	hspLedIO[2].wData	= LOW;

	rtn = BSP_GPIO_Init(&hspLedIO[0]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&hspLedIO[1]);
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_Init(&hspLedIO[2]);
	if(rtn == 0)		errCnt += 1;

	if(errCnt == 0)		result = 1;

	return result;
}

u8 HSP_LedCtrl(u8 position, u8 data)
{
	u8	result = 0;

	if(position > HSP_LED_COUNT)	return result;
	if(data > 1)					return result;

	result = BSP_GPIO_WritePin(&hspLedIO[position], !data);

	return result;
}

u8 HSP_LedToggle(u8 position)
{
	u8	result = 0;

	if(position > HSP_LED_COUNT)	return result;

	result = BSP_GPIO_Toggle(&hspLedIO[position]);

	return result;
}

static void HSP_LedBlinkProcess()
{
	static	u32	tick = 0;

	tick += 1;

	if(tick > HSP_LED_TICK_COUNT)
	{
		tick -= HSP_LED_TICK_COUNT;

		HSP_LedToggle(0);
	}
}

u8 HSP_LedInit()
{
	u8	result = 0, rtn;
	bspSysTickFops_t	fops;

	rtn = HSP_LedGpioInit();
	if(rtn == 0)				return result;

	fops.SysTickFunction		= HSP_LedBlinkProcess;

	rtn = BSP_SysTick_RegisterInterface(0, &fops);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

