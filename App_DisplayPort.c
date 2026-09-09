#include	"App_DisplayPort.h"

static	u32	dpAvailableChannel = NULL;

u8 DP_Init()
{
	u8	result = 0;

	dpAvailableChannel = 0;
	dpAvailableChannel |= DP_AVAILABLE_TX_1;
	dpAvailableChannel |= DP_AVAILABLE_TX_2;
	dpAvailableChannel |= DP_AVAILABLE_RX_1;
	dpAvailableChannel |= DP_AVAILABLE_RX_2;

	result = 1;

	return result;
}
/*
u32 DP_GetAvailableChannel()
{
	return dpAvailableChannel;
}

u8 DP_CheckAvailableChannel(u8 position)
{
	u8	result = 0;

	switch(position)
	{
		case DP_CHANNEL_TX1:
			if((dpAvailableChannel & DP_AVAILABLE_TX_1) && ((dpDeActivateChannel & DP_AVAILABLE_TX_1) == 0))		result = 1;
			break;

		case DP_CHANNEL_TX2:
			if((dpAvailableChannel & DP_AVAILABLE_TX_2) && ((dpDeActivateChannel & DP_AVAILABLE_TX_2) == 0))		result = 1;
			break;
	}

	return result;
}

u8 DP_SetActivateChannel(u8 position, u8 state)
{
	u8	result = 0;

	if(state > 1)			return result;

	switch(position)
	{
		case DP_CHANNEL_TX1:
			if(state)		dpDeActivateChannel	&= ~DP_AVAILABLE_TX_1;
			else			dpDeActivateChannel	|= DP_AVAILABLE_TX_1;
			break;

		case DP_CHANNEL_TX2:
			if(state)		dpDeActivateChannel	&= ~DP_AVAILABLE_TX_2;
			else			dpDeActivateChannel	|= DP_AVAILABLE_TX_2;
			break;

		default:
			return result;
			break;
	}

	result = 1;

	return result;
}
*/
u8 DP_GetFlagPosition(u8 position, u32 *pBitFlag)
{
	u8	result = 0;

	switch(position)
	{
		case DP_CHANNEL_COMMON_1:
		case DP_CHANNEL_COMMON_2:
			*pBitFlag = 0;
			break;

		case DP_CHANNEL_TX1:
		case DP_CHANNEL_TX2:
			*pBitFlag = 4;
			break;
/*
		case DP_CHANNEL_TX2:
		case DP_CHANNEL_TX9:
			*pBitFlag = 8;
			break;

		case DP_CHANNEL_TX3:
		case DP_CHANNEL_TX8:
			*pBitFlag = 12;
			break;

		case DP_CHANNEL_TX4:
		case DP_CHANNEL_TX7:
			*pBitFlag = 16;
			break;

		case DP_CHANNEL_TX5:
		case DP_CHANNEL_TX6:
			*pBitFlag = 20;
			break;
*/
		case DP_CHANNEL_RX1:
		case DP_CHANNEL_RX2:
			*pBitFlag = 24;
			break;

		default:
			return result;
			break;
	}

	result = 1;

	return result;
}

u8 DP_GetChipSelect(u8 position, u8 *pChipSelect)
{
	u8	result = 0;

	switch(position)
	{
		case DP_CHANNEL_COMMON_1:
		case DP_CHANNEL_TX1:
		case DP_CHANNEL_RX1:
			*pChipSelect = 2;
			break;

		case DP_CHANNEL_COMMON_2:
		case DP_CHANNEL_TX2:
		case DP_CHANNEL_RX2:
			*pChipSelect = 3;
			break;

		default:
			return result;
			break;
	}

	result = 1;

	return result;
}

u8 DP_GetChannelInfo(u8 position, u8 *pChipSelect, u16 *pBaseAdrs)
{
	u8	result = 0;

	switch(position)
	{
		case DP_CHANNEL_COMMON_1:
			if(dpAvailableChannel & (DP_AVAILABLE_TX_1 | DP_AVAILABLE_RX_1) == 0)
			{
				return result;
			}
			*pChipSelect	= 2;
			*pBaseAdrs		= 0x0000;
			break;

		case DP_CHANNEL_COMMON_2:
			if(dpAvailableChannel & (DP_AVAILABLE_TX_2 | DP_AVAILABLE_RX_2) == 0)
			{
				return result;
			}
			*pChipSelect	= 3;
			*pBaseAdrs		= 0x0000;
			break;

		case DP_CHANNEL_TX1:
			if((dpAvailableChannel & DP_AVAILABLE_TX_1) == 0)		return result;
			*pChipSelect	= 2;
			*pBaseAdrs		= 0x0100;
			break;

		case DP_CHANNEL_TX2:
			if((dpAvailableChannel & DP_AVAILABLE_TX_2) == 0)		return result;
			*pChipSelect	= 3;
			*pBaseAdrs		= 0x0100;
			break;
/*
		case DP_CHANNEL_TX3:
			if((dpAvailableChannel & DP_AVAILABLE_TX_3) == 0)		return result;
			*pChipSelect	= 0;
			*pBaseAdrs		= 0x0400;
			break;

		case DP_CHANNEL_TX4:
			if((dpAvailableChannel & DP_AVAILABLE_TX_4) == 0)		return result;
			*pChipSelect	= 0;
			*pBaseAdrs		= 0x0800;
			break;

		case DP_CHANNEL_TX5:
			if((dpAvailableChannel & DP_AVAILABLE_TX_5) == 0)		return result;
			*pChipSelect	= 0;
			*pBaseAdrs		= 0x1000;
			break;

		case DP_CHANNEL_TX6:
			if((dpAvailableChannel & DP_AVAILABLE_TX_6) == 0)		return result;
			*pChipSelect	= 1;
			*pBaseAdrs		= 0x1000;
			break;

		case DP_CHANNEL_TX7:
			if((dpAvailableChannel & DP_AVAILABLE_TX_7) == 0)		return result;
			*pChipSelect	= 1;
			*pBaseAdrs		= 0x0800;
			break;

		case DP_CHANNEL_TX8:
			if((dpAvailableChannel & DP_AVAILABLE_TX_8) == 0)		return result;
			*pChipSelect	= 1;
			*pBaseAdrs		= 0x0400;
			break;

		case DP_CHANNEL_TX9:
			if((dpAvailableChannel & DP_AVAILABLE_TX_9) == 0)		return result;
			*pChipSelect	= 1;
			*pBaseAdrs		= 0x0200;
			break;

		case DP_CHANNEL_TX10:
			if((dpAvailableChannel & DP_AVAILABLE_TX_10) == 0)		return result;
			*pChipSelect	= 1;
			*pBaseAdrs		= 0x0100;
			break;
*/
		case DP_CHANNEL_RX1:
			if((dpAvailableChannel & DP_AVAILABLE_RX_1) == 0)		return result;
			*pChipSelect	= 2;
			*pBaseAdrs		= 0x2000;
			break;

		case DP_CHANNEL_RX2:
			if((dpAvailableChannel & DP_AVAILABLE_RX_2) == 0)		return result;
			*pChipSelect	= 3;
			*pBaseAdrs		= 0x2000;
			break;

		default:
			return result;
			break;
	}

	result = 1;

	return result;
}

u8 DP_ProcessStart(u8 chipSelect, u32 mode)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	if(mode == 0x00000000)					return result;
	if(mode == CORE_REG_REGION_FAIL)		return result;
	if(mode == CORE_REG_REGION_BUSY)		return result;

	regData = mode;
	regData &= ~CORE_REG_REGION_FAIL;
	regData &= ~CORE_REG_REGION_BUSY;

	rtn = DPC_DirectRegWriteSingle(chipSelect, CORE_REG_NIOS_PROCESS_STATUS, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DPC_DirectRegWriteSingle(chipSelect, CORE_REG_NIOS_PROCESS_START, 0x00000001);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_ProcessCheck(u8 chipSelect, u32 delay)
{
	u8	result = 0, rtn;
	u32	regData;
	u32	sTick, eTick, waitCnt;

	if(chipSelect >= DPC_CORE_MAX_COUNT)		return result;

	if(delay == 0)			waitCnt = 100;
	else if(delay > 3000)	waitCnt = 3000;
	else					waitCnt = delay;

	sTick = HAL_GetTick();

	do{
		rtn = DPC_DirectRegReadSingle(chipSelect, CORE_REG_NIOS_PROCESS_STATUS, &regData);
		if(rtn == 0)							return result;

		if(regData == 0x00000000)
		{
			result = 1;
			break;
		}
		else if((regData & CORE_REG_REGION_FAIL) && ((regData & CORE_REG_REGION_BUSY) == 0))
		{
			result		= 0;
			break;
		}

		eTick = HAL_GetTick();
		if((eTick - sTick) > waitCnt)
		{
			result = 0;
			break;
		}
	}while(1);

	return result;
}

u8 DP_ProcessCheck_extend(u8 chipSelect)
{
	u8	result = 0, rtn;
	u32	regData;

	if(chipSelect >= DPC_CORE_MAX_COUNT)		return result;

	rtn = DPC_DirectRegReadSingle(chipSelect, CORE_REG_NIOS_PROCESS_STATUS, &regData);
	if(rtn == 0)							return result;

	if(regData == 0x00000000)
	{
		result = 1;
	}
	else if((regData & CORE_REG_REGION_FAIL) && ((regData & CORE_REG_REGION_BUSY) == 0))
	{
		result = 0;
	}

	return result;
}

u8 DP_TxMainLinkConfig(u8 chipSelect, u32 linkRate, u32 laneCount)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;
	adrs = CORE_REG_DP_TX_LANE_CONFIG;
	regData = ((linkRate << 16) & 0xFFFF0000) | ((laneCount << 0) & 0x0000FFFF);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxHpdEventConfig(u8 chipSelect, u32 ltMode, u32 edidReadDisable, u32 videoOutputDisable)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_HPD_EVENT_CONFIG;
	regData = ((videoOutputDisable << 16) & 0x00010000) | ((edidReadDisable << 8) & 0x00000100) | ((ltMode << 0) & 0x0000000F);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxColorSpaceConfig(u8 chipSelect, u32 format, u32 bpc, u32 range, u32 useVscSdp, u32 colorimetry)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_VIDEO_FORMAT_CONFIG;
	regData = ((colorimetry << 24) & 0xFF000000) | ((useVscSdp << 20) & 0x00F00000) | ((range << 16) & 0x000F0000) | ((bpc << 8) & 0x0000FF00) | ((format << 0) & 0x000000FF);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxVideoConfigDummyVideo(u8 chipSelect, u32 alpdpEnable, u32 beMode, u32 dummyVFP, u32 dummyVBP)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_ALPDP_VIDEO_CONFIG;
	regData = ((alpdpEnable & 0x01) << 31) | ((beMode & 0x03) << 28) | ((dummyVBP & 0x1FF) << 16) | ((dummyVFP & 0x1FF) << 0);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxBlankConfig(u8 chipSelect, u32 sof, u32 eof)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_BLANK_CONFIG;
	regData = ((eof & 0x01) << 8) | ((sof & 0x01) << 0);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxAuxConfig(u8 chipSelect, u32	 preamble, u32 precharge, u32 INVT)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;


	adrs = CORE_REG_DP_TX_LANE_CONFIG_3;
	regData = INVT;
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	adrs = CORE_REG_DP_TX_AUX_CONFIG;
	regData = ((precharge & 0xFF) << 16) | ((preamble & 0xFF) << 0);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxSdpBurstConfig(u8 chipSelect, u32	sdpBurstCtrl, u32 sdpBurstCount, u32 sdpBurstMode, u32 sdpBurstHBlankSLine)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_SDP_BURST_CONFIG;
	regData = ((sdpBurstCtrl & 0x01) << 31) | ((sdpBurstCount & 0x07) << 24) | ((sdpBurstMode & 0x01) << 16) | ((sdpBurstHBlankSLine & 0xFFFF) << 0);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxAlpmSignal(u8 chipSelect, u32 mainLinkSignal, u32 wakeUpSignal, u32 symbolLockSignal, u32 auxAckCheck)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_ALPM_CONFIG_1;
	regData = ((mainLinkSignal & 0x01) << 0) | ((wakeUpSignal & 0x01) << 4) | ((symbolLockSignal & 0x01) << 8) | ((auxAckCheck & 0x01) << 12);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxAlpmTiming(u8 chipSelect, u32 timing_T12, u32 timing_N1_vb, u32 timing_N2_vb, u32 timing_N3_vb, u32 timing_N5b_vb)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 50;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_ALPM_CONFIG_2;
	regData = ((timing_T12 & 0xFF) << 0);
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	adrs = CORE_REG_DP_TX_ALPM_CONFIG_3;
	regData = ((timing_N1_vb & 0x1FF) << 0) | ((timing_N5b_vb & 0x1FF) << 16);
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	adrs = CORE_REG_DP_TX_ALPM_CONFIG_4;
	regData = ((timing_N2_vb & 0xFFFF) << 0) | ((timing_N3_vb & 0xFFFF) << 16);
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxAlpmClk(u8 chipSelect, u32 rate)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 300;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_ALPM_CLK;
	regData = ((rate << 16) & 0xFFFF0000);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_CLK);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}


u8 DP_TxAlpmLrc(u8 chipSelect, u32 lrc_enable, u32 lrc_mode, u32 lrc_frame_period)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 100;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_LRC;
	regData = ((lrc_frame_period << 8) & 0x0000FF00) | ((lrc_mode & 0X01) << 1) | ((lrc_enable & 0X01) << 0);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxACVRRCtrl(u8 chipSelect, u32 Range, u32 Enable)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 100;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_ACVRR_CTRL;
	regData = ((Range << 8) & 0x0000FF00) | ((Enable & 0x01) << 16);

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxACVRRSet(u8 chipSelect, u32 NUM, u32 Hz, u32 CNT)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData, readRegData;
	//u32 shift=0;
	//u32	delay = 100;
	u32 NUM_remain=0;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	NUM=NUM-1;
	NUM_remain=NUM;
	NUM=NUM/2;

	adrs = CORE_REG_DP_TX_ACVRR_PARAM+(u16)NUM;
	rtn = DPC_DirectRegReadSingle(chipSelect, adrs, &readRegData);
	if(rtn == 0)							errCnt += 1;

	NUM_remain=NUM_remain%2;
	if(NUM_remain==0)
	{
		regData = ((Hz << 24) & 0xFF000000) | ((CNT << 16) & 0x00FF0000);
		regData =  (readRegData&0x0000FFFF) | regData;
	}
	else
	{
		regData = ((Hz << 8) & 0x0000FF00) | ((CNT << 0) & 0x000000FF);
		regData =  (readRegData&0xFFFF0000) | regData;
	}

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	/*rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;*/

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_TxAUXINV(u8 chipSelect, u32 status)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_DP_TX_LANE_CONFIG_3;

	regData = status;

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

#if 0	// NOT USE IN MC2 _ 210226_lyh
u8 DP_RxInit(u8 chipSelect)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	*pEdidData;
	u16	size = 128;

	pEdidData = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	// VIDEO EDID READ (128 Byte)
	rtn = VI_ReadEDID(pEdidData, size);
	if(rtn == 0)							errCnt += 1;

	// CORE REG WRITE - EDID BUF WRITE START
	rtn = DP_Rx_EDID_CTRL(chipSelect, CORE_REG_DP_RX_CTRL, 1);
	if(rtn == 0)							errCnt += 1;

	// CORE REG WRITE - EDID DATA
	rtn = DP_Rx_EDID_WRITE(chipSelect, CORE_REG_DP_RX_EDID_BUF_BASE, pEdidData, size);
	if(rtn == 0)							errCnt += 1;

	// CORE REG WRITE - EDID BUF WRITE END
	rtn = DP_Rx_EDID_CTRL(chipSelect, CORE_REG_DP_RX_CTRL, 0);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

// 201123_LYH
u8 DP_Rx_EDID_CTRL(u8 chipSelect, u16 adrs, u32 flag)
{
	u8	result = 0, rtn, errCnt = 0;
	
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, flag);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}
// 201123_LYH
u8 DP_Rx_EDID_WRITE(u8 chipSelect, u16 adrs, u8 *pData, u16 size)
{
	u8	result = 0, rtn, errCnt = 0;
	u32 regData;
	u32	delay = 1500;

	CMD_Printf(";REG DATA=");
	for(u16 cnt = 0; cnt < size; cnt += 4)
	{
		regData = pData[cnt] & 0x000000FF;
		regData |= (pData[cnt+1] << 8);
		regData |= (pData[cnt+2] << 16);
		regData |= (pData[cnt+3] << 24);

		CMD_Printf("0x%08X ", regData);

		rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
		if(rtn == 0)							errCnt += 1;
	}
	rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_DP_RX_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(chipSelect, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}
#endif
u8 DP_XSYNC(u8 chipSelect, u32 xsync_enable, u32 duty, u32 shift, u32 option)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData, mask = 0x00000003;
	
	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_REG_SYNC_OUT_CONFIG1;
	rtn = DPC_DirectRegReadSingle(chipSelect, adrs, &regData);
	if(rtn == 0)				errCnt += 1;
	
	if(option == 2)
	{
		mask = 0x00000001;
		regData &= ~mask;
		regData |= xsync_enable;
	}
	else
	{
		regData &= ~mask;
		regData |= ((xsync_enable) | ((option & 0X01) << 1));
	}
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)				errCnt += 1;

	adrs = CORE_REG_REG_SYNC_OUT_CONFIG2;
	regData = ((shift) | ((duty) << 16));
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DP_BSYNC(u8 chipSelect, u32 bsync_enable, u32 width, u32 shift, u32 option)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData, mask = 0x00000030;
	
	if(chipSelect >= DPC_CORE_MAX_COUNT)	return result;

	adrs = CORE_REG_REG_SYNC_OUT_CONFIG1;
	rtn = DPC_DirectRegReadSingle(chipSelect, adrs, &regData);
	if(rtn == 0)				errCnt += 1;

	if(option == 2)
	{
		mask = 0x00000010;
		regData &= ~mask;
		regData |= (bsync_enable << 4);
	}
	else
	{
		regData &= ~mask;
		regData |= ((bsync_enable << 4) | (option << 5));
	}
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)				errCnt += 1;

	adrs = CORE_REG_REG_SYNC_OUT_CONFIG3;
	regData = width;
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	adrs = CORE_REG_REG_SYNC_OUT_CONFIG4;
	regData = shift;
	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}