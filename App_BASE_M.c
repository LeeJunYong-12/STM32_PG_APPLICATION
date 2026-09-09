#include	"App_BASE_M.h"

static u8 BASE_M_eDPGetVersion(u8 *pData)
{
	u8	result = 0, rtn;
	u8	cnt;
	u8	buffer[32];
	u32	wSize = 0;

//	MCU Firmware Version
	memset(buffer, NULL, sizeof(buffer));
	rtn = Board_GetVersion(buffer);
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";MCU_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";MCU_ERROR");

//	PG FPGA Version
	memset(buffer, NULL, sizeof(buffer));
	rtn = FPGA_GetVersion(buffer);
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PG_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";PG_ERROR");

//	Core FPGA Version
	for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
	{
		if(Board_GetHwConstruct(cnt) == 0)		continue;
		memset(buffer, NULL, sizeof(buffer));
		// rtn = Core_GetVersion(boardChannel[cnt].position, buffer);
		rtn = DPC_GetFpgaVersion(boardChannel[cnt].position, buffer);
		if(rtn)		wSize += sprintf((char*)&pData[wSize], ";CORE%d_%s", (cnt + 1), buffer);
		else		wSize += sprintf((char*)&pData[wSize], ";CORE%d_ERROR", (cnt + 1));

//	Core NIOS Version
		memset(buffer, NULL, sizeof(buffer));
		rtn = DPC_GetNiosVersion(boardChannel[cnt].position, buffer);
		if(rtn)		wSize += sprintf((char*)&pData[wSize], ";FMCU%d_%s", (cnt + 1), buffer);
		else		wSize += sprintf((char*)&pData[wSize], ";FMCU%d_ERROR", (cnt + 1));
	}

//	POWER Version
	for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
	{
		if(Board_GetHwConstruct(cnt) == 0)		continue;
		memset(buffer, NULL, sizeof(buffer));
		rtn = PWR_M_GetVersion(boardChannel[cnt].position, buffer);
		if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PWR%d_%s", (cnt + 1), buffer);
		else		wSize += sprintf((char*)&pData[wSize], ";PWR%d_ERROR", (cnt + 1));
	}

	result = 1;

	return result;
}

static u8 BASE_M_eDPGetBuildDate(u8 *pData)
{
	u8	result = 0, rtn;
	u8	cnt;
	u8	buffer[32];
	u16	wSize = 0;

//	MCU Firmware Build Date
	memset(buffer, NULL, sizeof(buffer));
	rtn = Board_GetBuildDate(buffer);
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";MCU_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";MCU_ERROR");

//	PG FPGA Build Date
	memset(buffer, NULL, sizeof(buffer));
	rtn = FPGA_GetBuildDate(buffer);
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PG_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";PG_ERROR");

//	Core FPGA Build Date
	for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
	{
		if(Board_GetHwConstruct(cnt) == 0)		continue;
		memset(buffer, NULL, sizeof(buffer));
		//rtn = Core_GetBuildDate(boardChannel[cnt].position, buffer);
		rtn = DPC_GetFpgaBuild(boardChannel[cnt].position, buffer);
		if(rtn)		wSize += sprintf((char*)&pData[wSize], ";CORE%d_%s", (cnt + 1), buffer);
		else		wSize += sprintf((char*)&pData[wSize], ";CORE%d_ERROR", (cnt + 1));
//	Core NIOS Build Date
		memset(buffer, NULL, sizeof(buffer));
		rtn = DPC_GetNiosBuild(boardChannel[cnt].position, buffer);
		if(rtn)		wSize += sprintf((char*)&pData[wSize], ";FMCU%d_%s", (cnt + 1), buffer);
		else		wSize += sprintf((char*)&pData[wSize], ";FMCU%d_ERROR", (cnt + 1));
	}

//	POWER Build Date
	for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
	{
		if(Board_GetHwConstruct(cnt) == 0)		continue;
		memset(buffer, NULL, sizeof(buffer));
		rtn = PWR_M_GetBuildDate(boardChannel[cnt].position, buffer);
		if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PWR%d_%s", (cnt + 1), buffer);
		else		wSize += sprintf((char*)&pData[wSize], ";PWR%d_ERROR", (cnt + 1));
	}

	result = 1;

	return result;
}

u8 BASE_M_GetVersion(u8 *pData)
{
	u8	result = 0, rtn;
	u32	bType;

	rtn = Board_GetPhyType(&bType);
	if(rtn == 0)		return result;

	switch(bType)
	{
		case BOARD_PHY_TYPE_EDP:
			result = BASE_M_eDPGetVersion(pData);
			break;

		default:
			return result;
			break;
	}

	return result;
}

u8 BASE_M_GetBuildDate(u8 *pData)
{
	u8	result = 0, rtn;
	u32	bType;

	rtn = Board_GetPhyType(&bType);
	if(rtn == 0)		return result;

	switch(bType)
	{
		case BOARD_PHY_TYPE_EDP:
			result = BASE_M_eDPGetBuildDate(pData);
			break;

		default:
			return result;
			break;
	}

	return result;
}

