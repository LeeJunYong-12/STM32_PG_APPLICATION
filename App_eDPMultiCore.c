#include	"App_eDPMultiCore.h"

u8 eDPMC_GetVersion(u8 *pData)
{
	u8	result = 0, rtn;
	u8	adrs = EDPMC_REG_VERSION;
	u8	version;

	rtn = eDP_RegRead(adrs, &version, 1);
	if(rtn)
	{
		sprintf((char*)pData, "%d.%d\0", ((version >> 4) & 0x0F), ((version >> 0) & 0x0F));

		result = 1;
	}

	return result;
}

u8 eDPMC_GetVideoInputType(u8 *pData)
{
	u8	result = 0, rtn;
	u8	adrs = EDPMC_REG_INPUT_MODE;
	u8	inputType;

	rtn = eDP_RegRead(adrs, &inputType, 1);
	if(rtn == 0)		return result;

	result = 1;

	if(inputType == 0)			//	Parallel
	{
	}
	else if(inputType == 1)		//	LVDS
	{
	}
	else
	{
		result = 0;
	}

	return result;
}

u8 eDPMC_SetEdidReadEnable(u8 channel)
{
	u8	result = 0, rtn;
	u8	adrs = EDPMC_REG_EDID_READ;
	u8	regData, mask;

	if(channel >= 5)		return result;

	rtn = eDP_RegRead(adrs, &regData, 1);
	if(rtn == 0)			return result;

	if(channel < 5)			mask = 0x01 << channel;
	else					mask = 0x1f;

	regData &= ~mask;
	regData |= mask;

	rtn = eDP_RegWrite(adrs, &regData, 1);
	if(rtn == 0)			return result;

	result = 1;

	return result;
}

u8 eDPMC_SetCoreReset(u8 channel)
{
	u8	result = 0, rtn;
	u8	adrs = EDPMC_REG_CORE_RESET;
	u8	regData, mask;

	if(channel >= 5)		return result;

	rtn = eDP_RegRead(adrs, &regData, 1);
	if(rtn == 0)			return result;

	if(channel < 5)			mask = 0x01 << channel;
	else					mask = 0xFF;

	regData &= ~mask;
	regData |= mask;

	rtn = eDP_RegWrite(adrs, &regData, 1);
	if(rtn == 0)			return result;

	result = 1;

	return result;
}

u8 eDPMC_GetLinkStatus(u8 channel, u8 *pData)
{
	u8	result = 0, rtn;
	u8	adrs, statData;

	switch(channel)
	{
		case 0:
		case 1:
			adrs = EDPMC_REG_LINK_STATUS_0;
			break;

		case 2:
		case 3:
			adrs = EDPMC_REG_LINK_STATUS_1;
			break;

		case 4:
			adrs = EDPMC_REG_LINK_STATUS_2;
			break;

		default:
			return result;
			break;
	}

	rtn = eDP_RegRead(adrs, &statData, 1);
	if(rtn == 0)				return result;

	if((channel % 2) == 0)		*pData = statData & 0x0F;
	else						*pData = (statData >> 4) & 0x0F;

	result = 1;

	return result;
}

u8 eDPMC_SetInputResolution(void *pVoid)
{
	u8	result = 0, rtn;
	u8	adrs, wData;
	u32	blankValue;
	fpgaPgVideo_t	*pVideoData;

	pVideoData = (fpgaPgVideo_t*)pVoid;

	//	H Pixel
	//	Lower
	adrs = EDPMC_REG_H_RESOLUTION_LOW;
	wData = (pVideoData->hPixel >> 0) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;
	//	Higher
	adrs = EDPMC_REG_H_RESOLUTION_HIGH;
	wData = (pVideoData->hPixel >> 8) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;

	//	V Line
	//	Lower
	adrs = EDPMC_REG_V_RESOLUTION_LOW;
	wData = (pVideoData->vLine >> 0) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;
	//	Higher
	adrs = EDPMC_REG_V_RESOLUTION_HIGH;
	wData = (pVideoData->vLine >> 8) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;

	//	Input Framerate
	adrs = EDPMC_REG_INPUT_FRAMERATE;
	wData = (pVideoData->framerate >> 0) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;

	//	Output Framerate
	adrs = EDPMC_REG_OUTPUT_FRAMERATE;
	wData = (pVideoData->framerate >> 0) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;

	//	H Blank
	blankValue = pVideoData->hfp + pVideoData->hbp + pVideoData->hSync;
	//	Lower
	adrs = EDPMC_REG_H_BLANK_LOW;
	wData = (blankValue >> 0) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;
	//	Higher
	adrs = EDPMC_REG_H_BLANK_HIGH;
	wData = (blankValue >> 8) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;

	//	V Blank
	blankValue = pVideoData->vfp + pVideoData->vbp + pVideoData->vSync;
	//	Lower
	adrs = EDPMC_REG_V_BLANK_LOW;
	wData = (blankValue >> 0) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;
	//	Higher
	adrs = EDPMC_REG_V_BLANK_HIGH;
	wData = (blankValue >> 8) & 0xFF;
	rtn = eDP_RegWrite(adrs, &wData, 1);
	if(rtn == 0)		return result;

	result = 1;

	return result;
}

u8 eDPMC_GetEDID(u8 channel, u8 *pData)
{
	u8	result = 0, rtn;
	u8	wData, rData;
	u16	cnt;

	if(channel >= 5)			return result;

	wData = channel;
	rtn = eDP_RegWrite(EDPMC_REG_EDID_ADDRESS_SPACE_SELECT, &wData, 1);
	if(rtn == 0)				return result;

	for(cnt = 0; cnt < 128; cnt++)
	{
		rtn = eDP_RegRead((EDPMC_REG_EDID_START_ADRS + cnt), &rData, 1);
		if(rtn == 0)			return result;

		pData[cnt] = rData;
	}

	result = 1;

	return result;
}

u8 eDPMC_ReadAUX(u8 channel, u32 adrs, u8 *pData, u8 size)
{
	u8	result = 0, rtn;
	u8	regAdrs, wData, rData, cnt;
	u32	sTime, rTime;

	if(channel > 4)			return result;
	if(size == 0)			return result;
	if(size > 16)			return result;

	//	AUX Transaction Initialize
	regAdrs	= EDPMC_REG_AUX_INIT;
	wData = 0x00;
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	Address Write
	regAdrs	= EDPMC_REG_ADDRESS_BYTE_0;
	wData = (u8)((adrs >> 0) & 0xFF);
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	regAdrs	= EDPMC_REG_ADDRESS_BYTE_1;
	wData = (u8)((adrs >> 8) & 0xFF);
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	regAdrs	= EDPMC_REG_ADDRESS_BYTE_2;
	wData = (u8)((adrs >> 16) & 0x0F);
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	regAdrs	= EDPMC_REG_ADDRESS_BYTE_3;
	wData = 0x00;
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	Mode Write
	regAdrs	= EDPMC_REG_SINK_REG_ACCESS_DPCD;
	wData = 0x00;
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	Size Write
	regAdrs	= EDPMC_REG_DATA_SIZE;
	wData = size - 1;
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	AUX Transaction Write
	regAdrs	= EDPMC_REG_AUX_INIT;
	wData = (0x01 << channel);
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	AUX Transaction Complete Check
	sTime = HAL_GetTick();

	regAdrs = EDPMC_REG_AUX_TRANSACTION_STATUS;

	do{
		rtn = eDP_RegRead(regAdrs, &rData, 1);
		if(rData & (1 << channel))	break;

		rTime = HAL_GetTick();
		if((rTime - sTime) > 100)	return result;
	}while(1);


	//	Get Data
	for(cnt = 0; cnt < size; cnt++)
	{
		regAdrs	= EDPMC_REG_DATA_BYTE_0 + cnt;
		rtn = eDP_RegRead(regAdrs, &rData, 1);
		if(rtn == 0)				return result;

		pData[cnt] = rData;
	}

	result = 1;

	return result;
}

u8 eDPMC_WriteAUX(u8 channel, u32 adrs, u8 *pData, u8 size)
{
	u8	result = 0, rtn;
	u8	regAdrs, wData, rData, cnt;
	u32	sTime, rTime;

	if(channel > 4)			return result;
	if(size == 0)			return result;
	if(size > 16)			return result;

	//	AUX Transaction Initialize
	regAdrs	= EDPMC_REG_AUX_INIT;
	wData = 0x00;
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	Address Write
	regAdrs	= EDPMC_REG_ADDRESS_BYTE_0;
	wData = (u8)((adrs >> 0) & 0xFF);
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	regAdrs	= EDPMC_REG_ADDRESS_BYTE_1;
	wData = (u8)((adrs >> 8) & 0xFF);
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	regAdrs	= EDPMC_REG_ADDRESS_BYTE_2;
	wData = (u8)((adrs >> 16) & 0x0F);
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	regAdrs	= EDPMC_REG_ADDRESS_BYTE_3;
	wData = 0x00;
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	Mode Write
	regAdrs	= EDPMC_REG_SINK_REG_ACCESS_DPCD;
	wData = 0x01;
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	Size Write
	regAdrs	= EDPMC_REG_DATA_SIZE;
	wData = size - 1;
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	Set Data
	for(cnt = 0; cnt < size; cnt++)
	{
		regAdrs	= EDPMC_REG_DATA_BYTE_0 + cnt;
		wData = pData[cnt];
		rtn = eDP_RegWrite(regAdrs, &wData, 1);
		if(rtn == 0)				return result;
	}

	//	AUX Transaction Write
	regAdrs	= EDPMC_REG_AUX_INIT;
	wData = (0x01 << channel);
	rtn = eDP_RegWrite(regAdrs, &wData, 1);
	if(rtn == 0)			return result;

	//	AUX Transaction Complete Check
	sTime = HAL_GetTick();

	regAdrs = EDPMC_REG_AUX_TRANSACTION_STATUS;

	do{
		rtn = eDP_RegRead(regAdrs, &rData, 1);
		if(rData & (1 << channel))	break;

		rTime = HAL_GetTick();
		if((rTime - sTime) > 100)	return result;
	}while(1);

	result = 1;

	return result;
}

