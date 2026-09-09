#include	"App_IR.h"

static	u8	irFlag, irDecodeData[4];
static	u32	irSOF[16], irRcvTime[128];
static	irData_NEC_t	irData_NEC;
static	irResult_NEC_t	irResult_NEC;

u8 IR_GpioInit()
{
	u8	result = 0;
	GPIO_InitTypeDef	gpio;

	gpio.Pin		= GPIO_PIN_5;
	gpio.Mode		= GPIO_MODE_IT_RISING_FALLING;
	gpio.Pull		= GPIO_PULLUP;
	gpio.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOE, &gpio);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	result = 1;

	return result;
}

u8 IR_IoRead()
{
	return (u8)HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5);
}

static void AAAAAA()
{
	IR_SignalReceive();
}

u8 IR_Init()
{
	u8	result = 0, rtn;
	bspGpioExtiFops_t	fops;

	memset(irSOF, NULL, sizeof(irSOF));
	memset(irRcvTime, NULL, sizeof(irRcvTime));
	memset(&irData_NEC, NULL, sizeof(irData_NEC));
	memset(&fops, NULL, sizeof(fops));

	fops.ExtiFunc = AAAAAA;

	rtn = BSP_GPIO_RegisterInterface(GPIO_PIN_5, &fops);
	if(rtn == 0)						return result;

	irData_NEC.type						= IR_TYPE_NEC_DEFAULT;
	irData_NEC.state					= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
	irData_NEC.pSOF						= irSOF;
	irData_NEC.pRcvTime					= irRcvTime;
	irData_NEC.pDecodeData				= irDecodeData;

	irData_NEC.sofLeadTime				= 9000;
	irData_NEC.sofLeadTimeMargin		= 500;
	irData_NEC.sofSpaceTime				= 2250;
	irData_NEC.sofSpaceTimeMargin		= 250;
	irData_NEC.dataPulseTime			= 560;
	irData_NEC.dataPulseTimeMargin		= 100;
	irData_NEC.dataSpaceTime			= 560;
	irData_NEC.dataSpaceTimeMargin		= 100;

	irFlag = 0;

	Timer_Init();

	IR_GpioInit();

	Timer_Start();

	return result;
}

u8 IR_ByteDecode(u32	*pTimeData, u8 *pDecodeData)
{
	u8	result = 0, errCnt = 0;
	u8	cnt, bitData = 0, decodeData = 0;

	for(cnt = 0; cnt < 8; cnt++)
	{
		decodeData <<= 1;
		if((pTimeData[(cnt * 2) + 1] -pTimeData[(cnt * 2)]) > (irData_NEC.dataPulseTime + irData_NEC.dataPulseTimeMargin))
		{
			errCnt += 1;

			continue;
		}

		if((pTimeData[(cnt * 2) + 1] -pTimeData[(cnt * 2)]) < (irData_NEC.dataPulseTime - irData_NEC.dataPulseTimeMargin))
		{
			errCnt += 1;

			continue;
		}

		if((pTimeData[(cnt * 2) + 2] - pTimeData[(cnt * 2) + 1]) < (irData_NEC.dataSpaceTime - irData_NEC.dataSpaceTimeMargin))
		{
			errCnt += 1;

			continue;
		}
		else if((pTimeData[(cnt * 2) + 2] - pTimeData[(cnt * 2) + 1]) > (irData_NEC.dataSpaceTime + irData_NEC.dataSpaceTimeMargin))
		{
			bitData = 1;
		}
		else bitData = 0;

		decodeData |= bitData;
	}

	if(errCnt == 0)		result = 1;

	*pDecodeData = decodeData;

	return result;
}

u8 IR_SignalDecode()
{
	u8	result = 0, rtn, errCnt = 0;
	u8	cnt;

	for(cnt = 0; cnt < 4; cnt++)
	{
		rtn = IR_ByteDecode(&irData_NEC.pRcvTime[cnt * 16], &irData_NEC.pDecodeData[cnt]);
		if(rtn == 0)	errCnt += 1;
	}

	if(errCnt == 0)		result = 1;

	return result;
}

u8 IR_CmdDecode()
{
	u8	result = 0;
	u8	cmp1, cmp2;

	if(irData_NEC.type == IR_TYPE_NEC_DEFAULT)
	{
		//	Address DATA Compare
		cmp1	= irData_NEC.pDecodeData[0];
		cmp2	= irData_NEC.pDecodeData[1];
		cmp2	= ~cmp2;

		if(cmp1 != cmp2)
		{
			return result;
		}

		//	Command DATA Compare
		cmp1	= irData_NEC.pDecodeData[2];
		cmp2	= irData_NEC.pDecodeData[3];
		cmp2	= ~cmp2;

		if(cmp1 != cmp2)
		{
			return result;
		}

		irResult_NEC.type	= IR_TYPE_NEC_DEFAULT;
		irResult_NEC.cmd	= irData_NEC.pDecodeData[2];
		irResult_NEC.adrs	= irData_NEC.pDecodeData[0];

		result = 1;
	}
	else if(irData_NEC.type == IR_TYPE_NEC_EXTENDED)
	{
		//	Command DATA Compare
		cmp1	= irData_NEC.pDecodeData[2];
		cmp2	= irData_NEC.pDecodeData[3];
		cmp2	= ~cmp2;

		if(cmp1 != cmp2)
		{
			return result;
		}

		irResult_NEC.type	= IR_TYPE_NEC_EXTENDED;
		irResult_NEC.cmd	= irData_NEC.pDecodeData[2];
		irResult_NEC.adrs	= irData_NEC.pDecodeData[1];
		irResult_NEC.adrs	<<= 8;
		irResult_NEC.adrs	|= irData_NEC.pDecodeData[0];

		result = 1;
	}

	return result;
}

u8 IR_SignalReceive()
{
	u8	result = 0, rtn;
	u8	ioState;
	u32	tick;
	static	vu32	tick_old = 0xffffffff;

	if(irFlag)		return result;

	tick	= Timer_GetTick();
	ioState	= !IR_IoRead();

	switch(irData_NEC.state)
	{
		case IR_STATE_NEC_GET_SOF_LEAD_HIGH:
			//	Check IO Rising Edge
			if(ioState != HIGH)		break;
			irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_LOW;
			irData_NEC.pSOF[0]	= tick;
			break;

		case IR_STATE_NEC_GET_SOF_LEAD_LOW:
			//	Check IO Falling Edge
			if(ioState != LOW)
			{
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
				break;
			}

			irData_NEC.pSOF[1]	= tick;

			if((irData_NEC.pSOF[1] - irData_NEC.pSOF[0]) > (irData_NEC.sofLeadTime + irData_NEC.sofLeadTimeMargin))
			{
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
				break;
			}

			if((irData_NEC.pSOF[1] - irData_NEC.pSOF[0]) < (irData_NEC.sofLeadTime - irData_NEC.sofLeadTimeMargin))
			{
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
				break;
			}

			irData_NEC.state	= IR_STATE_NEC_GET_SOF_SPACE_HIGH;
			break;

		case IR_STATE_NEC_GET_SOF_SPACE_HIGH:
			//	Check IO Rising Edge
			if(ioState != HIGH)
			{
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
				break;
			}

			irData_NEC.pSOF[2]	= tick;

			if((irData_NEC.sofSpaceTime + irData_NEC.sofSpaceTimeMargin) < (irData_NEC.pSOF[2] - irData_NEC.pSOF[1]))
			{
				//	Command
				irData_NEC.pRcvTime[0]	= tick;

				irData_NEC.rcvCnt			= 1;

				irData_NEC.state			= IR_STATE_NEC_GET_COMMAND_LOW;
			}
			else if((irData_NEC.sofSpaceTime - irData_NEC.sofSpaceTimeMargin) > (irData_NEC.pSOF[2] - irData_NEC.pSOF[1]))
			{
				//	Repeat
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
			}
			else
			{
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
			}
			break;

		case IR_STATE_NEC_GET_COMMAND_HIGH:
			//	Check IO Rising Edge
			if(ioState != HIGH)
			{
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
				break;
			}

			irData_NEC.pRcvTime[irData_NEC.rcvCnt]	= tick;

			irData_NEC.rcvCnt	+= 1;

			if(irData_NEC.rcvCnt >= 65)		irData_NEC.state	= IR_STATE_NEC_RCV_DATA_DECODE;
			else							irData_NEC.state	= IR_STATE_NEC_GET_COMMAND_LOW;

			break;

		case IR_STATE_NEC_GET_COMMAND_LOW:
			//	Check IO Falling Edge
			if(ioState != LOW)
			{
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
				break;
			}

			irData_NEC.pRcvTime[irData_NEC.rcvCnt]	= tick;

			irData_NEC.rcvCnt	+= 1;

			irData_NEC.state	= IR_STATE_NEC_GET_COMMAND_HIGH;
			break;

		case IR_STATE_NEC_RCV_DATA_DECODE:
			//	Check IO Falling Edge
			if(ioState != LOW)
			{
				irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
				break;
			}

			rtn = IR_SignalDecode();
			if(rtn != 0)		irFlag = IR_CmdDecode();

			irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
			break;

		default:
			irData_NEC.state	= IR_STATE_NEC_GET_SOF_LEAD_HIGH;
			break;
	}

	tick_old = tick;

	return result;
}

u8 IR_Process()
{
	u8	result = 0;

	if(irFlag == 0)		return result;

	switch(irResult_NEC.cmd)
	{
		case 0x88:			//	1
			Script_SetFlag(0);
			break;

		case 0x48:			//	2
			Script_SetFlag(1);
			break;

		case 0xc8:			//	3
			Script_SetFlag(2);
			break;

		case 0x28:			//	4
			Script_SetFlag(3);
			break;

		case 0xa8:			//	5
			Script_SetFlag(4);
			break;

		case 0x68:			//	6
			Script_SetFlag(5);
			break;

		case 0xe8:			//	7
			Script_SetFlag(6);
			break;

		case 0x18:			//	8
			Script_SetFlag(7);
			break;
	}

	irFlag = 0;

	result = 1;

	return result;
}

