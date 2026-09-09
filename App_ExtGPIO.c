#include	"App_ExtGPIO.h"

static	bspGpio_t	extGpio[9];
static 	bspGpio_t	hpdgpio[2];

u8 ExtGPIO_Init()
{
	u8	result = 0, rtn, errCnt = 0;

	extGpio[0].Port		= GPIOJ;
	extGpio[0].Pin		= GPIO_PIN_8;
	extGpio[0].Mode		= BSP_GPIO_MODE_INPUT;
	extGpio[0].wData	= LOW;

	extGpio[1].Port		= GPIOJ;
	extGpio[1].Pin		= GPIO_PIN_9;
	extGpio[1].Mode		= BSP_GPIO_MODE_INPUT;
	extGpio[1].wData	= LOW;

	extGpio[2].Port		= GPIOJ;				// Core FPGA 1 POWER ON/OFF(LOW/HIGH)
	extGpio[2].Pin		= GPIO_PIN_12;
	extGpio[2].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	extGpio[2].wData	= LOW;

	extGpio[3].Port		= GPIOJ;				// Core FPGA 2 POWER ON/OFF(LOW/HIGH)
	extGpio[3].Pin		= GPIO_PIN_13;
	extGpio[3].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	extGpio[3].wData	= LOW;

	extGpio[4].Port		= GPIOJ;				// Core 1 FAN Control
	extGpio[4].Pin		= GPIO_PIN_14;
	extGpio[4].Mode		= BSP_GPIO_MODE_INPUT;
	extGpio[4].wData	= LOW;

	extGpio[5].Port		= GPIOJ;				// Core 2 FAN Control
	extGpio[5].Pin		= GPIO_PIN_15;
	extGpio[5].Mode		= BSP_GPIO_MODE_INPUT;
	extGpio[5].wData	= LOW;

	extGpio[6].Port		= GPIOB;				// Power Enable CH1 5V
	extGpio[6].Pin		= GPIO_PIN_13;
	extGpio[6].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	extGpio[6].wData	= LOW;

	extGpio[7].Port		= GPIOB;				// Power Enable CH2 5V
	extGpio[7].Pin		= GPIO_PIN_14;
	extGpio[7].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	extGpio[7].wData	= LOW;

	extGpio[8].Port		= GPIOB;				
	extGpio[8].Pin		= GPIO_PIN_15;
	extGpio[8].Mode		= BSP_GPIO_MODE_OUTPUT_PU;
	extGpio[8].wData	= LOW;

	for(u8 cnt = 0; cnt < 9; cnt++)
	{
		rtn = BSP_GPIO_Init(&extGpio[cnt]);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)			result = 1;

	return result;
}

u32 ExtGPIO_GetHandle(u8 position)
{
	u32	result = 0;

	if(position >= EXTGPIO_PIN_END)		return result;

	result = (u32)&extGpio[position];

	return result;
}

u8 ExtGPIO_Direction(u8 position, u32 direction)
{
	u8	result = 0;

	if(position >= EXTGPIO_PIN_END)		return result;
	if(direction >= BSP_GPIO_MODE_END)	return result;

	extGpio[position].Mode = direction;

	result = BSP_GPIO_Init(&extGpio[position]);

	return result;
}

u8 ExtGPIO_WritePin(u8 position, u8 state)
{
	u8	result = 0;

	if(position >= EXTGPIO_PIN_END)		return result;
	if(state > 1)						return result;

	result = BSP_GPIO_WritePin(&extGpio[position], state);

	return result;
}

u8 ExtGPIO_ReadPin(u8 position, u8 *pData)
{
	u8	result = 0, ioStatus;

	if(position >= EXTGPIO_PIN_END)		return result;

	ioStatus = BSP_GPIO_ReadPin(&extGpio[position]);

	*pData = ioStatus;

	result = 1;

	return result;
}

u8 Polling_HPD()
{
	u8	result = 0;
	u8 rtn, errCnt;

	hpdgpio[0].Port			= GPIOJ;
	hpdgpio[0].Pin			= GPIO_PIN_0;
	hpdgpio[0].Mode			= BSP_GPIO_MODE_INPUT;
	hpdgpio[0].wData		= LOW;

	hpdgpio[1].Port			= GPIOJ;
	hpdgpio[1].Pin			= GPIO_PIN_1;
	hpdgpio[1].Mode			= BSP_GPIO_MODE_INPUT;
	hpdgpio[1].wData		= LOW;


	for(u8 cnt = 0; cnt < 2; cnt++)
	{
		rtn = BSP_GPIO_Init(&hpdgpio[cnt]);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)			result = 1;

	return result;
}
#if 0
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	regData;
	u32 regData_status;
	u8 channel;
	u32 mask = 0x00000001;

	adrs = CORE_REG_ADDR_CORE_IRQ_STATUS;

	if(GPIO_Pin == GPIO_PIN_0)
	{
		channel = 2;
		rtn = DPC_DirectRegReadSingle(channel, adrs, &regData_status);
	}
	else if(GPIO_Pin == GPIO_PIN_1)
	{
		channel = 3;
		rtn = DPC_DirectRegReadSingle(channel, adrs, &regData_status);
	}

	regData = regData_status;

	regData = regData>>8;
	regData &= 0x00000003;

	if(regData==2)
	{
		LCM_GpioWrite(channel, 0x00);
		for(u8 swr=0; swr<7;swr++)
		{
			PWR_OutputEnable(channel, swr, 0);
		}
	}

	regData_status &= ~mask;
	regData_status |= 0x00000001;

	DPC_DirectRegWriteSingle(channel, adrs, regData_status);
}
#endif


void HPD_EVENT()
{
	static u32 display_tick = 0;
	u16 rdata[2] ={0,0};
	u16	adrs;
	u8 rtn;
	u32	regData;
	u32 regData_status;
	u8 channel;
	u32 mask = 0x00000001;



	if((HAL_GetTick() - display_tick) > 1)
	{
		display_tick = HAL_GetTick();

		if(hpd_event[0]==1)
		{
			rdata[0]=BSP_GPIO_ReadPin(&hpdgpio[0]);
		}

		if(hpd_event[1]==1)
		{
			rdata[1]=BSP_GPIO_ReadPin(&hpdgpio[1]);
		}

		if(rdata[0]==1)
		{
			adrs = CORE_REG_ADDR_CORE_IRQ_STATUS;

			channel = 2;
			rtn = DPC_DirectRegReadSingle(channel, adrs, &regData_status);
			if(rtn ==0) return;
			regData = regData_status;

			regData = regData>>2;
			regData &= 0x00000003;

			if(regData==2)
			{
				LCM_GpioWrite(channel, 0x00);
				for(u8 swr=0; swr<7;swr++)
				{
					PWR_OutputEnable(channel, swr, 0);
				}
			}

			regData_status &= ~mask;
			regData_status |= 0x00000001;

			rtn = DPC_DirectRegWriteSingle(channel, adrs, regData_status);
			if(rtn ==0) return;
		}

		if(rdata[1]==1)
		{

			adrs = CORE_REG_ADDR_CORE_IRQ_STATUS;

			channel = 3;
			rtn = DPC_DirectRegReadSingle(channel, adrs, &regData_status);
			if(rtn ==0) return;
			regData = regData_status;

			regData = regData>>2;
			regData &= 0x00000003;

			if(regData==2)
			{
				LCM_GpioWrite(channel, 0x00);
				for(u8 swr=0; swr<7;swr++)
				{
					PWR_OutputEnable(channel, swr, 0);
				}
			}

			regData_status &= ~mask;
			regData_status |= 0x00000001;

			rtn = DPC_DirectRegWriteSingle(channel, adrs, regData_status);
			if(rtn ==0) return;
		}
	}


}
