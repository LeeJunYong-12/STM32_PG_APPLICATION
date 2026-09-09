#include	"App_Core.h"

static	vu32	*pCoreData	= (vu32*)MEMORY_MAP_FPGA_DATA_ADRS;

u8 Core_Init()
{
	u8	result = 0, rtn, errCnt = 0;
	u8	cnt = 0;

	for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
	{
		rtn = CoreReg_PageInit(cnt);
		if(rtn == 0)		errCnt += 1;
	}

	rtn = CoreSPI_IoInit();
	if(rtn == 0)		errCnt += 1;

	rtn = CoreSPI_Init();
	if(rtn == 0)		errCnt += 1;

	if(errCnt == 0)		result = 1;

	return result;
}

u8 Core_PowerCtrl(u8 channel, u8 status)
{
	u8	result = 0;

	if(status > 1)			return result;

	result = ExtGPIO_WritePin(channel, status);

	return result;
}

u8 Core_Reset(u8 position)
{
	u8	result = 0, rtn, errCnt = 0;

	if(position >= BOARD_CHANNEL_MAX_COUNT)		return result;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_RESET, 0);
	if(rtn == 0)		errCnt += 1;

	HAL_Delay(5);

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_RESET, 1);
	if(rtn == 0)		errCnt += 1;

	HAL_Delay(5);

	if(errCnt == 0)		result = 1;

	return result;
}

u8 Core_ConfigClock(u8 position, u32 clk)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	vd, rd = 98, od = 6;
	u16	temp, regData;
	double	clock;
	double	odValue;

	switch(od)
	{
		case 0:
			odValue = 6;
			break;

		case 1:
			odValue = 2;
			break;

		case 2:
			odValue = 8;
			break;

		case 3:
			odValue = 4;
			break;

		case 4:
			odValue = 5;
			break;

		case 5:
			odValue = 7;
			break;

		case 6:
			odValue = 1;
			break;

		case 7:
			odValue = 3;
			break;
	}

	clock = (double)clk;

	clock /= 100000000;		// iClk * 2
	clock *= (rd + 2);		// RDW + 2
	clock *= odValue;		// OD(1)

	clock -= 8;				// VDW - 8

	vd = (u16)clock;

	regData = CoreReg_RegisterReadSingle(position, CORE_CMD_OUTPUT_PG_CLK_CONFIG_1);
	regData &= 0xfe00;
	regData |= (0x01ff & vd);
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_OUTPUT_PG_CLK_CONFIG_1, regData);
	if(rtn == 0)			errCnt += 1;

	regData = CoreReg_RegisterReadSingle(position, CORE_CMD_OUTPUT_PG_CLK_CONFIG_2);
	temp = (od << 8);
	temp |= (0x007f & rd);
	regData &= 0xf000;
	regData |= (0x077f & temp);
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_OUTPUT_PG_CLK_CONFIG_2, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 Core_PixelClockConfig(u8 position, u32 clk)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	data, range, regData;
	u32	sTick, eTick;

	if (clk < 50000000)
	{
		range = 2;
	}
	else if (clk < 100000000)
	{
		range = 1;
	}
	else
	{
		range = 0;
	}

	regData = CoreReg_RegisterReadSingle(position, CORE_CMD_OUTPUT_PG_CLK_CONFIG_1);

	regData &= 0xcfff;
	regData |= (0x3000 & (range << 12));
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_OUTPUT_PG_CLK_CONFIG_1, regData);
	if(rtn == 0)		errCnt += 1;

	rtn = Core_ConfigClock(position, clk);
	if(rtn == 0)		errCnt += 1;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_OUTPUT_PG_CLK_RECONFIG, 1);
	if(rtn == 0)		errCnt += 1;

	sTick = HAL_GetTick();

	do{
		data = CoreReg_RegisterReadSingle(position, CORE_CMD_OUTPUT_PG_CLK_RECONFIG);

		eTick = HAL_GetTick();

		if(data & 0x0001)			break;

		if((eTick - sTick) > 1000)	break;
	} while (1);

	if(errCnt == 0)			result = 1;

	return result;
}

u8 Core_DsiDatarateConfig(u8 position, u32 datarate, double *pPhase)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	multi2x = 0;
	u16	regData, chkr;
	u32	clkMulti, clkFrac, clkDivide;
	u32	phaseData[2], frac;
	u32	sTime, rTime;
	double	refClock, laneSpeed, multi;

	refClock = 50;

	phaseData[0] = (u32)(pPhase[0] * 1000);
	phaseData[1] = (u32)(pPhase[1] * 1000);

	laneSpeed = (double)datarate;

	multi = laneSpeed / refClock;

	if(datarate < 600)		multi *= 2;

	clkMulti	= (u16)multi;
	frac		= (u32)(multi * 1000);
	clkFrac		= (u16)(frac % 1000);

	if((clkMulti % 2) == 0)
	{
		if((clkFrac > 500) && (clkFrac <= 875))
		{
			multi2x = 1;

			multi *= 2;

			clkMulti	= (u16)multi;
			frac		= (u32)(multi * 1000);
			clkFrac		= (u16)(frac % 1000);
		}
	}

	if(clkMulti % 2)
	{
		if((clkFrac > 124) && (clkFrac < 187))	clkFrac = 124;

		if((clkFrac > 186) && (clkFrac < 250))	clkFrac = 250;

		if(clkFrac > 875)	clkFrac = 875;
	}
	else
	{
		if(clkFrac > 875)	clkFrac = 875;
	}

	if(multi2x == 0)
	{
		if(datarate > 1200)	clkDivide = 2;
		else				clkDivide = 1;
	}
	else
	{
		if(datarate > 1200)	clkDivide = 4;
		else				clkDivide = 2;
	}

	regData = (u16)clkDivide;
	regData <<= 8;
	regData |= (u16)(clkMulti & 0x00ff);
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_1, regData);
	if(rtn == 0)			errCnt += 1;

	if(clkFrac == 0)	regData = 0;
	else				regData = 1;
	regData <<= 12;
	regData |= (u16)(clkFrac & 0x03ff);
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_2, regData);
	if(rtn == 0)			errCnt += 1;

	if(datarate < 600)
	{
		regData = 4;
		regData <<= 8;
		regData |= 16;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_3, regData);
		if(rtn == 0)			errCnt += 1;
		
		regData = 16;
		regData <<= 8;
		regData |= 4;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_4, regData);
		if(rtn == 0)			errCnt += 1;

		regData = 4;
		regData <<= 8;
		regData |= 4;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_5, regData);
		if(rtn == 0)			errCnt += 1;
	}
	else if(datarate <= 1200)
	{
		regData = 2;
		regData <<= 8;
		regData |= 8;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_3, regData);
		if(rtn == 0)			errCnt += 1;

		regData = 8;
		regData <<= 8;
		regData |= 2;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_4, regData);
		if(rtn == 0)			errCnt += 1;

		regData = 2;
		regData <<= 8;
		regData |= 2;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_5, regData);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		regData = 1;
		regData <<= 8;
		regData |= 4;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_3, regData);
		if(rtn == 0)			errCnt += 1;

		regData = 4;
		regData <<= 8;
		regData |= 1;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_4, regData);
		if(rtn == 0)			errCnt += 1;

		regData = 1;
		regData <<= 8;
		regData |= 1;
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CONFIG_5, regData);
		if(rtn == 0)			errCnt += 1;
	}

	regData = (u16)(phaseData[0] >> 0);
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CH0_PHASE_L, regData);
	if(rtn == 0)			errCnt += 1;
	regData = (u16)(phaseData[0] >> 16);
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CH0_PHASE_H, regData);
	if(rtn == 0)			errCnt += 1;

	regData = (u16)(phaseData[1] >> 0);
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CH1_PHASE_L, regData);
	if(rtn == 0)			errCnt += 1;
	regData = (u16)(phaseData[1] >> 16);
	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_CH1_PHASE_H, regData);
	if(rtn == 0)			errCnt += 1;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CLK_RECONFIG, 1);
	if(rtn == 0)			errCnt += 1;

	sTime = HAL_GetTick();

	do{
		chkr = CoreReg_RegisterReadSingle(position, CORE_CMD_DSI_CLK_RECONFIG);

		if((chkr & 0x0001) == 0x0001)	break;

		rTime = HAL_GetTick();

		if((rTime - sTime) > 1000)		break;
	}while(1);

	if(errCnt == 0)						result = 1;

	return result;
}

u8 Core_GetCoreType(u8 position, u8 *pData)
{
	u8	result = 1;
	u16	regData;

	regData = CoreReg_RegisterReadSingle(position, CORE_CMD_FPGA_MODEL);
	if(regData == CORE_MODEL_KINTEX_ULTRASCALE)
	{
		*pData = 1;
	}
	else if(regData == CORE_MODEL_KINTEX_7)
	{
		*pData = 2;
	}
	else	result = 0;

	return result;
}

u8 Core_VideoInputConfig(u8 position, u32 mode)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	regData;
	u32	frameMode;

	rtn = IMG_GetFrameMode(&frameMode);
	if(rtn == 0)			return result;

	if(frameMode == IMAGE_DISPLAY_FRAME_MODE_DEFAULT)
	{
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_EXT_RGB_INPUT_FRAME_BUFFER_BANK_SELECT, 0x0000);
	}
	else if(frameMode == IMAGE_DISPLAY_FRAME_MODE_CUSTOM)
	{
		rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_EXT_RGB_INPUT_FRAME_BUFFER_BANK_SELECT, 0x0001);
	}
	if(rtn == 0)			errCnt += 1;

	regData = CoreReg_RegisterReadSingle(position, CORE_CMD_EXT_RGB_INPUT_CTRL);

	regData &= 0xff00;

	//	RGB Input Mode : DUAL/SINGLE
	if(mode == 1)			regData &= ~0x0010;
	else if(mode == 2)		regData |= 0x0010;
	else					errCnt += 1;

	//	RGB Input Clock Phase : 0
	regData &= ~0x0080;

	//	RGB Input Color Bit : 10 Bit
	regData &= ~0x0003;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_EXT_RGB_INPUT_CTRL, regData);
	if(rtn == 0)			errCnt += 1;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_EXT_RGB_INPUT_ENABLE, 0x0001);
	if(rtn == 0)			errCnt += 1;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_FRAME_BUFFER_BANK_SELECT, 0x0000);
	if(rtn == 0)			errCnt += 1;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_FRAME_BUFFER_READ_START, 0x0001);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 Core_PTGEnable(u8 position)
{
	return CoreReg_RegisterWriteSingle(position, CORE_CMD_OUTPUT_PG_ENABLE, 1);
}

u8 Core_PTGDisable(u8 position)
{
	return CoreReg_RegisterWriteSingle(position, CORE_CMD_OUTPUT_PG_ENABLE, 0);
}

u8 Core_RgbInterfaceTest(u8 position, void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	regData;
	u32	sTime, rTime;
	coreRgbIfTest_t *pData;

	pData = (coreRgbIfTest_t*)pVoid;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_RGB_IF_TEST_ENABLE, 0x0001);
	if(rtn == 0)			errCnt += 1;

	sTime = HAL_GetTick();

	do{
		regData = CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_TEST_ENABLE);

		rTime = HAL_GetTick();

		if((regData & 0x0001) == 0)				break;
	
		if((rTime - sTime) > pData->waitTime)	break;
	}while(1);

	if((regData & 0x0001) != 0)					return result;

	regData					= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERROR_STATUS);

	pData->errPos			= (u8)regData;
	pData->errCnt			= (u8)(regData >> 8);

//	CH0 RED
	pData->ch0ErrRef[0]		= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_REF_CH0_R) & 0x03ff;
	pData->ch0ErrValue[0]	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_VALUE_CH0_R) & 0x03ff;
//	CH0 GREEN
	pData->ch0ErrRef[1]		= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_REF_CH0_G) & 0x03ff;
	pData->ch0ErrValue[1]	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_VALUE_CH0_G) & 0x03ff;
//	CH0 BLUE
	pData->ch0ErrRef[2] 	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_REF_CH0_B) & 0x03ff;
	pData->ch0ErrValue[2]	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_VALUE_CH0_B) & 0x03ff;

//	CH0 RED
	pData->ch1ErrRef[0] 	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_REF_CH1_R) & 0x03ff;
	pData->ch1ErrValue[0]	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_VALUE_CH1_R) & 0x03ff;
//	CH0 GREEN
	pData->ch1ErrRef[1] 	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_REF_CH1_G) & 0x03ff;
	pData->ch1ErrValue[1]	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_VALUE_CH1_G) & 0x03ff;
//	CH0 BLUE
	pData->ch1ErrRef[2] 	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_REF_CH1_B) & 0x03ff;
	pData->ch1ErrValue[2]	= CoreReg_RegisterReadSingle(position, CORE_CMD_RGB_IF_ERR_VALUE_CH1_B) & 0x03ff;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 Core_DebugLedCtrl(u8 position, u8 data)
{
	u8	result = 0;

	data &= 0x01;

	if(data)	// ON
	{
		result = CoreReg_RegisterWriteSingle(position, CORE_CMD_DEBUG_LED_OFF, 0x0000);
	}
	else		// OFF
	{
		result = CoreReg_RegisterWriteSingle(position, CORE_CMD_DEBUG_LED_OFF, 0x0001);
	}

	return result;
}

u8 Core_FrameBufferBankSelect_Input(u8 position, u16 slot)
{
	u8	result = 0, rtn;

	if(slot >= CORE_FRAME_BUFFER_MAX_COUNT)					return result;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_EXT_RGB_INPUT_FRAME_BUFFER_BANK_SELECT, slot);
	if(rtn != 0)				result = 1;

	return result;
}

u8 Core_FrameBufferBankSelect_Output(u8 position, u16 slot)
{
	u8	result = 0, rtn;

	if(slot >= CORE_FRAME_BUFFER_MAX_COUNT)					return result;

	rtn = CoreReg_RegisterWriteSingle(position, CORE_CMD_FRAME_BUFFER_BANK_SELECT, slot);
	if(rtn != 0)				result = 1;

	return result;
}

u8 Core_VideoReflesh(u8 position)
{
	return CoreReg_RegisterWriteSingle(position, CORE_CMD_DSI_CMD_MODE_REFRESH, 1);
}

u8 Core_GetVersion(u8 position, u8 *pData)
{
	u8	result = 0;
	u8	msb, lsb;
	u16	regData;

	regData = CoreReg_RegisterReadSingle(position, CORE_CMD_VERSION);

	msb = (u8)(regData >> 8);
	lsb = (u8)(regData >> 0);

	sprintf((char*)pData, "%02X.%02X", msb, lsb);

	result = 1;

	return result;
}

u8 Core_GetBuildDate(u8 position, u8 *pData)
{
	u8	result = 0;
	u8	month, day;
	u16	year;
	u16	regData;
	u32	build;

	regData = CoreReg_RegisterReadSingle(position, CORE_CMD_BUILD_DATE_H);
	build = regData;
	build <<= 16;
	regData = CoreReg_RegisterReadSingle(position, CORE_CMD_BUILD_DATE_L);
	build |= regData;

	year	= (u16)(build >> 16);
	month	= (u8)(build >> 8);
	day		= (u8)(build >> 0);

	sprintf((char*)pData, "%04X.%02X.%02X", year, month, day);

	result = 1;

	return result;
}

void Core_WriteBurst(u32 *pData, u32 size)
{
	for(u32 cnt = 0; cnt < size; cnt++)
	{
		*pCoreData = pData[cnt];
	}
}
