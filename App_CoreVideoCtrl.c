#include	"App_CoreVideoCtrl.h"

u8 CoreVC_VideoStreamConfig(u8 position, void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	delay = 100;
	coreVideoInfo_t	*pData;

	pData = (coreVideoInfo_t*)pVoid;

	if(pData->mode == CORE_VIDEO_MODE_AUTO)
	{
		adrs	= CORE_REG_VIDEO_STREAM_INFO_MODE;
		regData	= pData->mode & 0x0000000F;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_H_ACTIVE;
		regData	= pData->hActive & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_V_ACTIVE;
		regData	= pData->vActive & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_FRAMERATE;
		regData = pData->cFrameRate & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;
	}
	else if(pData->mode == CORE_VIDEO_MODE_SEMI_AUTO)
	{
		adrs	= CORE_REG_VIDEO_STREAM_INFO_MODE;
		regData	= pData->mode & 0x0000000F;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_H_ACTIVE;
		regData	= pData->hActive & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_V_ACTIVE;
		regData	= pData->vActive & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_H_BLANK_1;
		regData	= (pData->hbp << 16) | pData->hfp;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_H_BLANK_2;
		regData	= pData->hSync & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_V_BLANK_1;
		regData	= (pData->vbp << 16) | pData->vfp;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_V_BLANK_2;
		regData	= pData->vSync & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_FRAMERATE;
		regData = (pData->mFrameRate << 16) | pData->cFrameRate;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;
	}
	else if(pData->mode == CORE_VIDEO_MODE_MANUAL)
	{
		adrs	= CORE_REG_VIDEO_STREAM_INFO_MODE;
		regData	= pData->mode & 0x0000000F;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_H_ACTIVE;
		regData	= pData->hActive & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_V_ACTIVE;
		regData	= pData->vActive & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_H_BLANK_1;
		regData	= (pData->hbp << 16) | pData->hfp;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_H_BLANK_2;
		regData	= pData->hSync & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_V_BLANK_1;
		regData	= (pData->vbp << 16) | pData->vfp;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_V_BLANK_2;
		regData	= pData->vSync & 0x0000FFFF;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;

		adrs	= CORE_REG_VIDEO_STREAM_INFO_VCLK;
		regData = pData->streamClock;
		rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
		if(rtn == 0)			errCnt += 1;
	}
	else							return result;

	delay = 100;

	rtn = DP_ProcessStart(position, CORE_REG_REGION_COMMON_VIDEO_CONFIG);
	if(rtn == 0)				errCnt += 1;

	rtn = DP_ProcessCheck(position, delay);
	if(rtn == 0)				errCnt += 1;
	
	if(errCnt == 0)
	{
		rtn = IMG_PreloadListSetCurrentResolution(pData->hActive, pData->vActive);
		if(rtn != 0)				result = 1;
	}

	return result;
}

u8 CoreVC_VideoStreamInputEnable(u8 position)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000001;

	rtn = DPC_DirectRegReadSingle(position, CORE_REG_VIDEO_STREAM_INPUT_START, &regData);
	if(rtn == 0)					errCnt += 1;

	regData &= ~mask;

	regData |= 0x00000001 & mask;

	rtn = DPC_DirectRegWriteSingle(position, CORE_REG_VIDEO_STREAM_INPUT_START, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoStreamInputDisable(u8 position)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000001;

	rtn = DPC_DirectRegReadSingle(position, CORE_REG_VIDEO_STREAM_INPUT_START, &regData);
	if(rtn == 0)				errCnt += 1;

	regData &= ~mask;

	rtn = DPC_DirectRegWriteSingle(position, CORE_REG_VIDEO_STREAM_INPUT_START, regData);
	if(rtn == 0)				errCnt += 1;

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoStreamInputType(u8 position, u32 state)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000007; // ~mask = 0b1111111111111111......1000

	rtn = DPC_DirectRegReadSingle(position, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, &regData);
	if(rtn == 0)					errCnt += 1;

	regData &= ~mask; // remain other value except input source 

	regData |= state & mask; // change value of the input source, maintaining the other value 

	if(state == CORE_VIDEO_INPUT_SOURCE_MCU_BUS)	regData |= 0x00000100;

	rtn = DPC_DirectRegWriteSingle(position, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, regData);
	if(rtn == 0)					errCnt += 1; 

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoStreamInputMode(u8 position, u32 state)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000070;

	if(state > CORE_VIDEO_CTRL_MODE_ARGB8888)		return result;

	rtn = CoreVC_VideoStreamInputType(position, CORE_VIDEO_INPUT_SOURCE_MCU_BUS);
	if(rtn == 0)					errCnt += 1;

	rtn = DPC_DirectRegReadSingle(position, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, &regData);
	if(rtn == 0)					errCnt += 1;

	regData &= ~mask;

	if(state == CORE_VIDEO_CTRL_MODE_RGB101010)		regData |= ((0x00 << 4) & mask);
	else											regData |= ((0x05 << 4) & mask);

	rtn = DPC_DirectRegWriteSingle(position, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, regData);
	if(rtn == 0)					errCnt += 1;
	
	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_FPGADATAInputMode(u8 position, u32 state) // state = BUS DATA WRITE 7
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000070;

	if(state > BUS_DATA_WRITE)		return result;

	rtn = CoreVC_VideoStreamInputType(position, CORE_VIDEO_INPUT_SOURCE_MCU_BUS);
	if(rtn == 0)					errCnt += 1;

	rtn = DPC_DirectRegReadSingle(position, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, &regData);
	if(rtn == 0)					errCnt += 1;

	regData &= ~mask;
	regData |= ((state << 4) & mask);

	rtn = DPC_DirectRegWriteSingle(position, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)					result = 1;

	return result;
}


u8 CoreVC_VideoFrameWriteSlot(u8 position, u32 slotNo)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData;

//	if(slotNo > 0x3F)				return result;

	rtn = CoreVC_VideoStreamInputDisable(position);
	if(rtn == 0)					errCnt += 1;

	regData = slotNo;

	rtn = DPC_DirectRegWriteSingle(position, CORE_REG_VIDEO_STREAM_INPUT_MEMORY_ADDRESS, regData);
	if(rtn == 0)				errCnt += 1;

	DelayCycles(100);

	rtn = CoreVC_VideoStreamInputEnable(position);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoFrameWriteStart(u8 position)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00010000;

	rtn = DPC_DirectRegReadSingle(position, CORE_REG_VIDEO_STREAM_INPUT_START, &regData);
	if(rtn == 0)					errCnt += 1;

	regData |= mask;

	rtn = DPC_DirectRegWriteSingle(position, CORE_REG_VIDEO_STREAM_INPUT_START, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoFrameWriteStop(u8 position)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00010000;

	rtn = DPC_DirectRegReadSingle(position, CORE_REG_VIDEO_STREAM_INPUT_START, &regData);
	if(rtn == 0)					errCnt += 1;

	regData &= ~mask;

	rtn = DPC_DirectRegWriteSingle(position, CORE_REG_VIDEO_STREAM_INPUT_START, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoFrameReadSlot(u32 slotNo)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData;

//	if(slotNo > 0x3F)				return result;

	for(u8 cnt = 0; cnt < DPC_CORE_MAX_COUNT; cnt++)
	{
//		rtn = DPC_DirectRegReadSingle(cnt, CORE_REG_VIDEO_STREAM_OUTPUT_MEMORY_ADDRESS, &regData);
//		if(rtn == 0)				errCnt += 1;

		regData = slotNo;

		rtn = DPC_DirectRegWriteSingle(cnt, CORE_REG_VIDEO_STREAM_OUTPUT_MEMORY_ADDRESS, regData);
		if(rtn == 0)				errCnt += 1;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoStreamOutputEnable()
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000001;

	for(u8 cnt = 0; cnt < DPC_CORE_MAX_COUNT; cnt++)
	{
		rtn = DPC_DirectRegReadSingle(cnt, CORE_REG_VIDEO_STREAM_OUTPUT_START, &regData);
		if(rtn == 0)				errCnt += 1;

		regData &= ~mask;

		regData |= 0x01 & mask;

		rtn = DPC_DirectRegWriteSingle(cnt, CORE_REG_VIDEO_STREAM_OUTPUT_START, regData);
		if(rtn == 0)				errCnt += 1;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoStreamOutputDisable()
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000001;

	for(u8 cnt = 0; cnt < DPC_CORE_MAX_COUNT; cnt++)
	{
		rtn = DPC_DirectRegReadSingle(cnt, CORE_REG_VIDEO_STREAM_OUTPUT_START, &regData);
		if(rtn == 0)				errCnt += 1;

		regData &= ~mask;

		rtn = DPC_DirectRegWriteSingle(cnt, CORE_REG_VIDEO_STREAM_OUTPUT_START, regData);
		if(rtn == 0)				errCnt += 1;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

u8 CoreVC_VideoStreamFormat(u8 position, u32 toggle, u32 mode)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData, mask = (1 << 8) | (3 << 0);

	if(toggle > 1)				return result;
	if(mode >= 3)				return result;

	adrs = CORE_REG_VIDEO_STREAM_INFO_FORMAT;
	rtn = DPC_DirectRegReadSingle(position, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	regData &= ~mask;
	regData |= (toggle << 8) | (mode << 0);

	rtn = DPC_DirectRegWriteSingle(position, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)				result = 1;

	return result;
}

u8 CoreVC_Init()
{
	u8	result = 0, rtn, errCnt = 0;

	rtn = CoreVC_VideoStreamInputEnable(2);
	if(rtn == 0)			errCnt += 1;

	rtn = CoreVC_VideoStreamInputEnable(3);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

#if 1 // yeom_20201117 [start]
u8 CoreVC_VideoStreamInputDataTypeInputSource(u32 state)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000007;

	if(state > INPUT_SOURCE_MCU_BUS)		return result;

	for(u8 cnt = 0; cnt < DPC_CORE_MAX_COUNT; cnt++)
	{
		rtn = DPC_DirectRegReadSingle(cnt, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, &regData);
		if(rtn == 0)	errCnt += 1;

		regData &= ~mask;

		regData |= state & mask;

		rtn = DPC_DirectRegWriteSingle(cnt, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, regData);
		if(rtn == 0)	errCnt += 1; 
	}

	if(errCnt == 0)		result = 1;

	return result;
}

u8 CoreVC_VideoStreamInputDataTypeInputPortMap(u32 state)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000070;

	if(state > INPUT_PORT_MAP_BUS_DATA_WRITE)		return result;

	for(u8 cnt = 0; cnt < DPC_CORE_MAX_COUNT; cnt++)
	{
		rtn = DPC_DirectRegReadSingle(cnt, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, &regData);
		if(rtn == 0)	errCnt += 1;

		regData &= ~mask;

		regData |= state & mask;

		rtn = DPC_DirectRegWriteSingle(cnt, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, regData);
		if(rtn == 0)	errCnt += 1;
	}

	if(errCnt == 0)		result = 1;

	return result;
}

u8 CoreVC_VideoStreamInputDataTypeSinglePortEnable(u32 state)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000100;

	if(state > SINGLE_PORT_ENABLE_SINGLE_PORT_ENABLE)		return result;

	for(u8 cnt = 0; cnt < DPC_CORE_MAX_COUNT; cnt++)
	{
		rtn = DPC_DirectRegReadSingle(cnt, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, &regData);
		if(rtn == 0)	errCnt += 1;

		regData &= ~mask;

		regData |= state & mask;

		rtn = DPC_DirectRegWriteSingle(cnt, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, regData);
		if(rtn == 0)	errCnt += 1;
	}

	if(errCnt == 0)		result = 1;

	return result;
}
#endif // yeom_20201117 [end]



