#include	"App_BASE_S.h"

static u8 BASE_S_MipiGetVersion(u8 *pData)
{
	u8	result = 0, rtn;
	u8	buffer[32];
	u16	wSize = 0;

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
	memset(buffer, NULL, sizeof(buffer));
	
	rtn = 0;
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";CORE_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";CORE_ERROR");

//	POWER Version
	memset(buffer, NULL, sizeof(buffer));
	
	rtn = 0;
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PWR_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";PWR_ERROR");

	result = 1;

	return result;
}

static u8 BASE_S_MipiGetBuildDate(u8 *pData)
{
	u8	result = 0, rtn;
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
	memset(buffer, NULL, sizeof(buffer));
	
	rtn = 0;
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";CORE_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";CORE_ERROR");

//	POWER Build Date
	memset(buffer, NULL, sizeof(buffer));
	
	rtn = 0;
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PWR_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";PWR_ERROR");

	result = 1;

	return result;
}

static u8 BASE_S_eDPGetVersion(u8 *pData)
{
	u8	result = 0, rtn;
	u8	buffer[32];
	u16	wSize = 0;

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
	memset(buffer, NULL, sizeof(buffer));
	
	rtn = 0;
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";CORE_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";CORE_ERROR");

//	POWER Version
	memset(buffer, NULL, sizeof(buffer));
	
	rtn = 0;
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PWR_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";PWR_ERROR");

	result = 1;

	return result;
}

static u8 BASE_S_eDPGetBuildDate(u8 *pData)
{
	u8	result = 0, rtn;
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
	memset(buffer, NULL, sizeof(buffer));
	
	rtn = 0;
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";CORE_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";CORE_ERROR");

//	POWER Build Date
	memset(buffer, NULL, sizeof(buffer));
	
	rtn = 0;
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PWR_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";PWR_ERROR");

	result = 1;

	return result;
}

u8 BASE_S_GetVersion(u8 *pData)
{
	u8	result = 0, rtn;
	u32	bType;

	rtn = Board_GetPhyType(&bType);
	if(rtn == 0)		return result;

	switch(bType)
	{
		case BOARD_PHY_TYPE_MIPI:
			result = BASE_S_MipiGetVersion(pData);
			break;

		case BOARD_PHY_TYPE_EDP:
			result = BASE_S_eDPGetVersion(pData);
			break;

		default:
			return result;
			break;
	}

	return result;
}

u8 BASE_S_GetBuildDate(u8 *pData)
{
	u8	result = 0, rtn;
	u32	bType;

	rtn = Board_GetPhyType(&bType);
	if(rtn == 0)		return result;

	switch(bType)
	{
		case BOARD_PHY_TYPE_MIPI:
			result = BASE_S_MipiGetBuildDate(pData);
			break;

		case BOARD_PHY_TYPE_EDP:
			result = BASE_S_eDPGetBuildDate(pData);
			break;

		default:
			return result;
			break;
	}

	return result;
}

