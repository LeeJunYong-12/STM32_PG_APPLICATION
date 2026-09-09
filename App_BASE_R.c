#include	"App_BASE_R.h"

static u8 BASE_R_MipiGetVersion(u8 *pData)
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

static u8 BASE_R_MipiGetBuildDate(u8 *pData)
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

static u8 BASE_R_eDPGetVersion(u8 *pData)
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
	
	rtn = PWR_R_Version(buffer);
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PWR_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";PWR_ERROR");

	result = 1;

	return result;
}

static u8 BASE_R_eDPGetBuildDate(u8 *pData)
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
	
	rtn = PWR_R_BuildDate(buffer);
	if(rtn)		wSize += sprintf((char*)&pData[wSize], ";PWR_%s", buffer);
	else		wSize += sprintf((char*)&pData[wSize], ";PWR_ERROR");

	result = 1;

	return result;
}

u8 BASE_R_GetVersion(u8 *pData)
{
	u8	result = 0, rtn;
	u32	bType;

	rtn = Board_GetPhyType(&bType);
	if(rtn == 0)		return result;

	switch(bType)
	{
		case BOARD_PHY_TYPE_MIPI:
			result = BASE_R_MipiGetVersion(pData);
			break;

		case BOARD_PHY_TYPE_EDP:
			result = BASE_R_eDPGetVersion(pData);
			break;

		default:
			return result;
			break;
	}

	return result;
}

u8 BASE_R_GetBuildDate(u8 *pData)
{
	u8	result = 0, rtn;
	u32	bType;

	rtn = Board_GetPhyType(&bType);
	if(rtn == 0)		return result;

	switch(bType)
	{
		case BOARD_PHY_TYPE_MIPI:
			result = BASE_R_MipiGetBuildDate(pData);
			break;

		case BOARD_PHY_TYPE_EDP:
			result = BASE_R_eDPGetBuildDate(pData);
			break;

		default:
			return result;
			break;
	}

	return result;
}

u8 BASE_R_eDPRedriverSetPwrDown(u8 position, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_PWR_DOWN;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetSync(u8 position, u8 type, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_SYNC;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= type;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetEqMode(u8 position, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_EQ_MODE;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetChannelEnable(u8 position, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_CHANNEL_ENABLE;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetRxGain(u8 position, u8 type, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_RX_GAIN;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= type;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetEqGain(u8 position, u8 type, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_EQ_GAIN;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= type;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetTxGain(u8 position, u8 type, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_TX_GAIN;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= type;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetEqValue(u8 position, u8 type, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_EQ_VALUE;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= type;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetDriveEnable(u8 position, u8 type, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_DRIVER_ENABLE;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= type;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetEqEnable(u8 position, u8 type, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_EQ_ENABLE;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= type;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 BASE_R_eDPRedriverSetDrivePeak(u8 position, u8 type, u8 mode)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EDP_REDRIVER_DRIVER_PEAK;

	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= type;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;
	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

