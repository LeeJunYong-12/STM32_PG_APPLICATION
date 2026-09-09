#include	"Cmd_RXDPCore.h"

static u8 Cmd_RXDPC_Version(void *pVoid)
{
	u8	result = 0, rtn;
	u32	coreVersion, coreBuild, niosVersion, niosBuild;
	u8	type, channel, major, minor, month, day;
	u16	year;

	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_VERSION;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 16)		return result;

	coreVersion = SYS_HexToWord(&sbParsingData.pRxData[0]);
	coreBuild = SYS_HexToWord(&sbParsingData.pRxData[4]);
	niosVersion = SYS_HexToWord(&sbParsingData.pRxData[8]);
	niosBuild = SYS_HexToWord(&sbParsingData.pRxData[12]);

	type	= (coreVersion >> 24) & 0xFF;
	channel	= (coreVersion >> 16) & 0xFF;
	major	= (coreVersion >> 8) & 0xFF;
	minor	= (coreVersion >> 0) & 0xFF;

	year	= (coreBuild >> 16) & 0xFFFF;
	month	= (coreBuild >> 8) & 0xFF;
	day		= (coreBuild >> 0) & 0xFF;

	CMD_Printf(";CORE:%02X.%d.%02X.%02X,%04X.%02X.%02X", type, channel, major, minor, year, month, day);

	type	= (niosVersion >> 24) & 0xFF;
	channel	= (niosVersion >> 16) & 0xFF;
	major	= (niosVersion >> 8) & 0xFF;
	minor	= (niosVersion >> 0) & 0xFF;

	year	= (niosBuild >> 16) & 0xFFFF;
	month	= (niosBuild >> 8) & 0xFF;
	day		= (niosBuild >> 0) & 0xFF;

	CMD_Printf(";FMCU:%d.%d.%d.%c,%04X.%02X.%02X", type, channel, major, minor, year, month, day);

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_Reset(void *pVoid)
{
	u8	result = 0, rtn;

	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_RESET;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_Temperature(void *pVoid)
{
	u8	result = 0, rtn;
	float	temperature;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_TEMPERATURE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	temperature = SYS_HexToFloat(&sbParsingData.pRxData[0]);

	CMD_Printf(";%.03fDegree C", temperature);

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_CoreRegWrite(void *pVoid)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	data = CMD_StrToUL(pData->pArgv[1]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_REG_WRITE;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_WordToHex(data, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_CoreRegRead(void *pVoid)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)				return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_REG_READ;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	data = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf(";DATA=0x%08X", data);

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_VideoStream(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	u16	hActive, vActive, hfp, hbp, vfp, vbp, hsync, vsync, cframerate, mframerate;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)
	{
		hActive		= 1290;
		vActive		= 2796;
		mframerate	= 120;
		cframerate	= 120;
		hfp			= 8;
		hbp			= 8;
		vfp			= 1;
		vbp			= 82;
		hsync		= 16;
		vsync		= 1;
	}
	else if(pData->argc == 10)
	{
		hActive		= CMD_StrToUL(pData->pArgv[0]);
		vActive		= CMD_StrToUL(pData->pArgv[1]);
		mframerate	= CMD_StrToUL(pData->pArgv[2]);
		cframerate	= CMD_StrToUL(pData->pArgv[3]);
		hfp			= CMD_StrToUL(pData->pArgv[4]);
		hbp			= CMD_StrToUL(pData->pArgv[5]);
		vfp			= CMD_StrToUL(pData->pArgv[6]);
		vbp			= CMD_StrToUL(pData->pArgv[7]);
		hsync		= CMD_StrToUL(pData->pArgv[8]);
		vsync		= CMD_StrToUL(pData->pArgv[9]);
	}
	else							return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_VIDEO_STREAM_INFO;

	SYS_HWordToHex(hActive, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(vActive, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(mframerate, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(cframerate, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(hfp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(hbp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(vfp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(vbp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(hsync, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(vsync, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}
static u8 Cmd_RXDPC_VideoFormat(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	u32	toggle, mode;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))				toggle = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))			toggle = 1;

	if(CMD_Compare(pData->pArgv[1], "RGB"))					mode = 0;
	else if(CMD_Compare(pData->pArgv[1], "YCbCr444"))		mode = 0;
	else if(CMD_Compare(pData->pArgv[1], "YCbCr422_T1"))	mode = 1;
	else if(CMD_Compare(pData->pArgv[1], "YCbCr422_T2"))	mode = 2;
	else													return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_VIDEO_FORMAT;

	SYS_WordToHex(toggle, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(mode, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxLaneConfig(void *pVoid)
{
	u8	result = 0, rtn;
	u8	channel, laneOrder[4], pnSwap, voltRange;
	u32	length;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)							return result;

	if(CMD_Compare(pData->pArgv[0], "CH1"))			channel = 0;
	else if(CMD_Compare(pData->pArgv[0], "CH2"))	channel = 1;
	else if(CMD_Compare(pData->pArgv[0], "CH3"))	channel = 2;
	else if(CMD_Compare(pData->pArgv[0], "CH4"))	channel = 3;
	else if(CMD_Compare(pData->pArgv[0], "CH5"))	channel = 4;
	else if(CMD_Compare(pData->pArgv[0], "ALL"))	channel = 0xFF;
	else											return result;

	length		= strlen((char*)pData->pArgv[1]);
	if(length != 4)									return result;
	laneOrder[0] = (pData->pArgv[1][0] - '0') - 1;
	if(laneOrder[0] > 3)							return result;
	laneOrder[1] = (pData->pArgv[1][1] - '0') - 1;
	if(laneOrder[1] > 3)							return result;
	laneOrder[2] = (pData->pArgv[1][2] - '0') - 1;
	if(laneOrder[2] > 3)							return result;
	laneOrder[3] = (pData->pArgv[1][3] - '0') - 1;
	if(laneOrder[3] > 3)							return result;

	pnSwap		= CMD_StrToUL(pData->pArgv[2]);
	voltRange	= CMD_StrToUL(pData->pArgv[3]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_TX_LANE_CONFIG;

	SYS_WordToHex(channel, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;	
	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], laneOrder, 4);
	sbParsingData.txSize 				+= 4;	
	SYS_WordToHex(pnSwap, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;
	SYS_WordToHex(voltRange, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxMainLinkConfig(void *pVoid)
{
	u8	result = 0, rtn;
	u32	linkRate, laneCount;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)			return result;

	linkRate	= CMD_StrToUL(pData->pArgv[0]);
	laneCount	= CMD_StrToUL(pData->pArgv[1]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_TX_LINK_CONFIG;

	SYS_WordToHex(linkRate, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(laneCount, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxHpdEventConfig(void *pVoid)
{
	u8	result = 0, rtn;
	u32	ltMode, edidReadDisable, videoOutputDisable;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)									return result;

	if(CMD_Compare(pData->pArgv[0], "none"))				ltMode = DISPLAYPORT_LINKTRAINING_MODE_NONE;
	else if(CMD_Compare(pData->pArgv[0], "auto"))			ltMode = DISPLAYPORT_LINKTRAINING_MODE_AUTO;
	else if(CMD_Compare(pData->pArgv[0], "normal"))			ltMode = DISPLAYPORT_LINKTRAINING_MODE_NORMAL;
	else if(CMD_Compare(pData->pArgv[0], "fast1"))			ltMode = DISPLAYPORT_LINKTRAINING_MODE_FASE_1;
	else if(CMD_Compare(pData->pArgv[0], "fast2"))			ltMode = DISPLAYPORT_LINKTRAINING_MODE_FASE_2;
	else if(CMD_Compare(pData->pArgv[0], "quick1"))			ltMode = DISPLAYPORT_LINKTRAINING_MODE_QUICK_1;
	else if(CMD_Compare(pData->pArgv[0], "quick2"))			ltMode = DISPLAYPORT_LINKTRAINING_MODE_QUICK_2;
	else													return result;

	if(CMD_Compare(pData->pArgv[1], "enable"))				edidReadDisable = 0;
	else if(CMD_Compare(pData->pArgv[1], "disable"))		edidReadDisable = 1;
	else													return result;

	if(CMD_Compare(pData->pArgv[2], "enable"))				videoOutputDisable = 0;
	else if(CMD_Compare(pData->pArgv[2], "disable"))		videoOutputDisable = 1;
	else													return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_TX_HPD_CONFIG;

	SYS_WordToHex(ltMode, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(edidReadDisable, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(videoOutputDisable, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxColorSpaceConfig(void *pVoid)
{
	u8	result = 0, rtn;
	u32	format, bpc, range, useVscSdp, colorimetry;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)			return result;

	if(CMD_Compare(pData->pArgv[0], "rgb"))					format = DISPLAYPORT_COLORSPACE_FORMAT_RGB;
	else if(CMD_Compare(pData->pArgv[0], "ycbcr444"))		format = DISPLAYPORT_COLORSPACE_FORMAT_YCBCR_444;
	else if(CMD_Compare(pData->pArgv[0], "ycbcr422"))		format = DISPLAYPORT_COLORSPACE_FORMAT_YCBCR_422;
	else if(CMD_Compare(pData->pArgv[0], "ycbcr420"))		format = DISPLAYPORT_COLORSPACE_FORMAT_YCBCR_420;
	else													return result;

	if(CMD_Compare(pData->pArgv[1], "6bpc"))				bpc = DISPLAYPORT_COLORSPACE_BPC_6BIT;
	else if(CMD_Compare(pData->pArgv[1], "7bpc"))			bpc = DISPLAYPORT_COLORSPACE_BPC_7BIT;
	else if(CMD_Compare(pData->pArgv[1], "8bpc"))			bpc = DISPLAYPORT_COLORSPACE_BPC_8BIT;
	else if(CMD_Compare(pData->pArgv[1], "9bpc"))			bpc = DISPLAYPORT_COLORSPACE_BPC_9BIT;
	else if(CMD_Compare(pData->pArgv[1], "10bpc"))			bpc = DISPLAYPORT_COLORSPACE_BPC_10BIT;
	else if(CMD_Compare(pData->pArgv[1], "12bpc"))			bpc = DISPLAYPORT_COLORSPACE_BPC_12BIT;
	else if(CMD_Compare(pData->pArgv[1], "14bpc"))			bpc = DISPLAYPORT_COLORSPACE_BPC_14BIT;
	else if(CMD_Compare(pData->pArgv[1], "16bpc"))			bpc = DISPLAYPORT_COLORSPACE_BPC_16BIT;
	else													return result;

	if(CMD_Compare(pData->pArgv[2], "vesa"))				range = DISPLAYPORT_COLORSPACE_RANGE_VESA;
	else if(CMD_Compare(pData->pArgv[2], "cea"))			range = DISPLAYPORT_COLORSPACE_RANGE_CEA;
	else													return result;

	if(CMD_Compare(pData->pArgv[3], "misc0"))				useVscSdp = 0;
	else if(CMD_Compare(pData->pArgv[3], "vsc_sdp"))		useVscSdp = 1;
	else													return result;

	colorimetry = CMD_StrToUL(pData->pArgv[4]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_TX_COLOR_CONFIG;

	SYS_WordToHex(format, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(bpc, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(range, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(useVscSdp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(colorimetry, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxDummyVideo(void *pVoid)
{
	u8	result = 0, rtn;
	u32	ctrl, be, vfp, vbp;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)								return result;

	if(CMD_Compare(pData->pArgv[0], "Disable"))			ctrl = 0;
	else if(CMD_Compare(pData->pArgv[0], "Enable"))		ctrl = 1;
	else												return result;

	if(CMD_Compare(pData->pArgv[1], "Disable"))			be = 0;
	else if(CMD_Compare(pData->pArgv[1], "Last"))		be = 1;
	else if(CMD_Compare(pData->pArgv[1], "All"))		be = 2;
	else if(CMD_Compare(pData->pArgv[1], "LastFirst"))	be = 3;
	else												return result;

	vfp	= CMD_StrToUL(pData->pArgv[2]);
	vbp	= CMD_StrToUL(pData->pArgv[3]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_TX_DUMMY_VIDEO;

	SYS_WordToHex(ctrl, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(be, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(vfp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(vbp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxBlankConfig(void *pVoid)
{
	u8	result = 0, rtn;
	u32	sof, eof;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)								return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))			sof = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))		sof = 1;
	else												return result;

	if(CMD_Compare(pData->pArgv[1], "disable"))			eof = 0;
	else if(CMD_Compare(pData->pArgv[1], "enable"))		eof = 1;
	else												return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_TX_BLANK_CONFIG;

	SYS_WordToHex(sof, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(eof, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_RxInit(void *pVoid)
{
	u8	result = 0, rtn, dataSize;
	cmdFunctionData_t	*pData;
	u8	buffer[128];
	u8	status = 0;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 128)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_RX_INIT;
	sbParsingData.waitTime				= 5000;

	dataSize = pData->argc;

	for(int cnt = 0; cnt < dataSize; cnt++)
	{
		buffer[cnt] = CMD_StrToUL(pData->pArgv[cnt]);
		SYS_WordToHex(buffer[cnt], &sbParsingData.pTxData[sbParsingData.txSize]);
		sbParsingData.txSize				+= 1;
	}

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 1)		return result;

	status = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf(";DATA=0x%02X", status);

	if(status == 0)						result = 1;

	return result;
}

static u8 Cmd_RXDPC_VideoStreamInput(void *pVoid)
{
	u8	result = 0, rtn, type;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "dvisingle"))			type = 0;
	else if(CMD_Compare(pData->pArgv[0], "dvidual"))		type = 1;
	else if(CMD_Compare(pData->pArgv[0], "dp"))				type = 2;
	else												return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_VIDEO_INPUT_TYPE;

	SYS_WordToHex(type, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_VideoStreamEnable(void *pVoid)
{
	u8	result = 0, rtn, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	if(pData->option == 0x01)			state = ON;
	else								state = OFF;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_VIDEO_INPUT_ENABLE;

	SYS_WordToHex(state, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_VideoDisplay(void *pVoid)
{
	u8	result = 0, rtn, state;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "on"))				state = ON;
	else if(CMD_Compare(pData->pArgv[0], "off"))		state = OFF;
	else												return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_VIDEO_DISPLAY;

	SYS_WordToHex(state, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_DCTestInit(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	u16	testframes, pattern;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)
	{
		testframes		= 30;
		pattern			= 21;
	}
	else if(pData->argc == 2)
	{
		testframes		= CMD_StrToUL(pData->pArgv[0]);
		pattern			= CMD_StrToUL(pData->pArgv[1]);
	}
	else							return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_DCTEST_INIT;

	SYS_HWordToHex(testframes, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(pattern, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_DCTestStart(void *pVoid)
{
	u8	result = 0, rtn, doneState, errorState;
	cmdFunctionData_t	*pData;
	u32 errorCnt;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_DCTEST_START;
	sbParsingData.waitTime				= 5000;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 6)		return result;

	doneState = sbParsingData.pRxData[0];
	errorState = sbParsingData.pRxData[1];
	errorCnt = SYS_HexToWord(&sbParsingData.pRxData[2]);

	CMD_Printf("\r\n\r\n======================= DATA COMPARE TEST =======================\r\n");
	if(doneState)
	{
		CMD_Printf("[Data Compare TEST DONE] : ");
		if(errorState == 1)
		{
			CMD_Printf("NG\r\nNo Video Data\r\n");
		}
		else if(errorState == 2)
		{
			CMD_Printf("NG\r\nCOUNT=%d\r\n", errorCnt);
		}
		else
		{
			CMD_Printf("OK\r\n");
		}
	}
	else
	{
		CMD_Printf("[Data Compare TEST NOT DONE] : ERROR\r\n");
	}								
	CMD_Printf("============================== END ==============================\r\n\r\n");

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_DCTestDoneCheck(void *pVoid)
{
	u8	result = 0, rtn, state;
	cmdFunctionData_t	*pData;
	u32 errorCnt;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_DCTEST_CHECK_DONE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 5)		return result;

	state = sbParsingData.pRxData[0];
	errorCnt = SYS_HexToWord(&sbParsingData.pRxData[1]);

	if(state)							CMD_Printf(";[Data Compare TEST DONE] : NG COUNT = %d", errorCnt);
	else								CMD_Printf(";[ERROR] : Data Compare TEST ERROR");

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_IOTestStart(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	u32 iostate;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_IOTEST_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	iostate = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf("\r\n\r\n============================ IO TEST ============================\r\n");

	if(iostate == 0)								CMD_Printf("[IO TEST DONE] : OK\r\n");

	if(iostate & 0x00000001)						CMD_Printf("[GPIO_MTP_TO_TOUCH_RESET_L_CONN] : NG\r\n");
	if(iostate & 0x00000002)						CMD_Printf("[SPI0_MTP_TO_TOUCH_SCLK_CONN] : NG\r\n");
	if(iostate & 0x00000004)						CMD_Printf("[SPI0_MTP_FROM_TOUCH_MISO_CONN] : NG\r\n");
	if(iostate & 0x00000008)						CMD_Printf("[SPI0_MTP_TO_TOUCH_MOSI_CONN] : NG\r\n");
	if(iostate & 0x00000010)						CMD_Printf("[GPIO_MTP_FROM_TOUCH_INT0_L_CONN] : NG\r\n");
	if(iostate & 0x00000020)						CMD_Printf("[IO_KRAKEN_TO_TOUCH_AID_CONN] : NG\r\n");
	if(iostate & 0x00000040)						CMD_Printf("[SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN] : NG\r\n");
	if(iostate & 0x00000080)						CMD_Printf("[SPI0_MTP_TO_TOUCH_SSIN_CONN] : NG\r\n");
	if(iostate & 0x00000100)						CMD_Printf("[GPIO_SEQ_PMU_TO_DISPLAY_RESET_L] : NG\r\n");
	if(iostate & 0x00000200)						CMD_Printf("[IO_TOUCH_TO_MANY_SCAN_ACTIVE] : NG\r\n");
	if(iostate & 0x00000400)						CMD_Printf("[I2C_SDA_TOUCH_EEPROM_1V8] : NG\r\n");
	if(iostate & 0x00000800)						CMD_Printf("[I2C_SCL_TOUCH_EEPROM_1V8] : NG\r\n");
	if(iostate & 0x00001000)						CMD_Printf("[GPIO_MTP_FROM_TOUCH_INT1_L_CONN] : NG\r\n");
	if(iostate & 0x00002000)						CMD_Printf("[GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN] : NG\r\n");
	if(iostate & 0x00004000)						CMD_Printf("[SWD_NUB_BI_TOUCH_SWDIO_CONN] : NG\r\n");
	if(iostate & 0x00008000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC0_CONN_1] : NG\r\n");
	if(iostate & 0x00010000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC1_CONN_1] : NG\r\n");
	if(iostate & 0x00020000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC0_CONN_2] : NG\r\n");
	if(iostate & 0x00040000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC1_CONN_2] : NG\r\n");
	if(iostate & 0x00080000)						CMD_Printf("[FRAME_GND_MEAS_OUT] : NG\r\n");
	if(iostate & 0x00100000)						CMD_Printf("[FTP_KELVIN_DG_0] : NG\r\n");
	if(iostate & 0x00200000)						CMD_Printf("[FTP_KELVIN_DG_1] : NG\r\n");
	if(iostate & 0x00400000)						CMD_Printf("[POGO_VTP_DUT_GND_0] : NG\r\n");

	CMD_Printf("============================== END ==============================\r\n\r\n");

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_IOTest2Start(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	u32 iostate_open, iostate_short;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_IOTEST2_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 8)		return result;

	iostate_open = SYS_HexToWord(&sbParsingData.pRxData[0]);
	iostate_short = SYS_HexToWord(&sbParsingData.pRxData[4]);

	CMD_Printf("\r\n\r\n========================== IO OPEN TEST ==========================\r\n");
	
	if(iostate_open == 0)								CMD_Printf("[IO TEST DONE] : OK\r\n");

	if(iostate_open & 0x00000001)						CMD_Printf("[GPIO_MTP_TO_TOUCH_RESET_L_CONN] : NG\r\n");
	if(iostate_open & 0x00000002)						CMD_Printf("[SPI0_MTP_TO_TOUCH_SCLK_CONN] : NG\r\n");
	if(iostate_open & 0x00000004)						CMD_Printf("[SPI0_MTP_FROM_TOUCH_MISO_CONN] : NG\r\n");
	if(iostate_open & 0x00000008)						CMD_Printf("[SPI0_MTP_TO_TOUCH_MOSI_CONN] : NG\r\n");
	if(iostate_open & 0x00000010)						CMD_Printf("[GPIO_MTP_FROM_TOUCH_INT0_L_CONN] : NG\r\n");
	if(iostate_open & 0x00000020)						CMD_Printf("[IO_KRAKEN_TO_TOUCH_AID_CONN] : NG\r\n");
	if(iostate_open & 0x00000040)						CMD_Printf("[SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN] : NG\r\n");
	if(iostate_open & 0x00000080)						CMD_Printf("[SPI0_MTP_TO_TOUCH_SSIN_CONN] : NG\r\n");
	if(iostate_open & 0x00000100)						CMD_Printf("[GPIO_SEQ_PMU_TO_DISPLAY_RESET_L] : NG\r\n");
	if(iostate_open & 0x00000200)						CMD_Printf("[IO_TOUCH_TO_MANY_SCAN_ACTIVE] : NG\r\n");
	if(iostate_open & 0x00000400)						CMD_Printf("[I2C_SDA_TOUCH_EEPROM_1V8] : NG\r\n");
	if(iostate_open & 0x00000800)						CMD_Printf("[I2C_SCL_TOUCH_EEPROM_1V8] : NG\r\n");
	if(iostate_open & 0x00001000)						CMD_Printf("[GPIO_MTP_FROM_TOUCH_INT1_L_CONN] : NG\r\n");
	if(iostate_open & 0x00002000)						CMD_Printf("[GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN] : NG\r\n");
	if(iostate_open & 0x00004000)						CMD_Printf("[SWD_NUB_BI_TOUCH_SWDIO_CONN] : NG\r\n");
	if(iostate_open & 0x00008000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC0_CONN_1] : NG\r\n");
	if(iostate_open & 0x00010000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC1_CONN_1] : NG\r\n");
	if(iostate_open & 0x00020000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC0_CONN_2] : NG\r\n");
	if(iostate_open & 0x00040000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC1_CONN_2] : NG\r\n");
	if(iostate_open & 0x00080000)						CMD_Printf("[FRAME_GND_MEAS_OUT] : NG\r\n");
	if(iostate_open & 0x00100000)						CMD_Printf("[FTP_KELVIN_DG_0] : NG\r\n");
	if(iostate_open & 0x00200000)						CMD_Printf("[FTP_KELVIN_DG_1] : NG\r\n");
	if(iostate_open & 0x00400000)						CMD_Printf("[POGO_VTP_DUT_GND_0] : NG\r\n");

	CMD_Printf("============================== END ===============================\r\n");

	CMD_Printf("========================= IO SHORT TEST ==========================\r\n");	
	
	if(iostate_short == 0)								CMD_Printf("[IO TEST DONE] : OK\r\n");

	if(iostate_short & 0x00000001)						CMD_Printf("[GPIO_MTP_TO_TOUCH_RESET_L_CONN] : NG\r\n");
	if(iostate_short & 0x00000002)						CMD_Printf("[SPI0_MTP_TO_TOUCH_SCLK_CONN] : NG\r\n");
	if(iostate_short & 0x00000004)						CMD_Printf("[SPI0_MTP_FROM_TOUCH_MISO_CONN] : NG\r\n");
	if(iostate_short & 0x00000008)						CMD_Printf("[SPI0_MTP_TO_TOUCH_MOSI_CONN] : NG\r\n");
	if(iostate_short & 0x00000010)						CMD_Printf("[GPIO_MTP_FROM_TOUCH_INT0_L_CONN] : NG\r\n");
	if(iostate_short & 0x00000020)						CMD_Printf("[IO_KRAKEN_TO_TOUCH_AID_CONN] : NG\r\n");
	if(iostate_short & 0x00000040)						CMD_Printf("[SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN] : NG\r\n");
	if(iostate_short & 0x00000080)						CMD_Printf("[SPI0_MTP_TO_TOUCH_SSIN_CONN] : NG\r\n");
	if(iostate_short & 0x00000100)						CMD_Printf("[GPIO_SEQ_PMU_TO_DISPLAY_RESET_L] : NG\r\n");
	if(iostate_short & 0x00000200)						CMD_Printf("[IO_TOUCH_TO_MANY_SCAN_ACTIVE] : NG\r\n");
	if(iostate_short & 0x00000400)						CMD_Printf("[I2C_SDA_TOUCH_EEPROM_1V8] : NG\r\n");
	if(iostate_short & 0x00000800)						CMD_Printf("[I2C_SCL_TOUCH_EEPROM_1V8] : NG\r\n");
	if(iostate_short & 0x00001000)						CMD_Printf("[GPIO_MTP_FROM_TOUCH_INT1_L_CONN] : NG\r\n");
	if(iostate_short & 0x00002000)						CMD_Printf("[GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN] : NG\r\n");
	if(iostate_short & 0x00004000)						CMD_Printf("[SWD_NUB_BI_TOUCH_SWDIO_CONN] : NG\r\n");
	if(iostate_short & 0x00008000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC0_CONN_1] : NG\r\n");
	if(iostate_short & 0x00010000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC1_CONN_1] : NG\r\n");
	if(iostate_short & 0x00020000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC0_CONN_2] : NG\r\n");
	if(iostate_short & 0x00040000)						CMD_Printf("[IO_AOP_TO_TOUCH_BSYNC1_CONN_2] : NG\r\n");
	if(iostate_short & 0x00080000)						CMD_Printf("[FRAME_GND_MEAS_OUT] : NG\r\n");
	if(iostate_short & 0x00100000)						CMD_Printf("[FTP_KELVIN_DG_0] : NG\r\n");
	if(iostate_short & 0x00200000)						CMD_Printf("[FTP_KELVIN_DG_1] : NG\r\n");
	if(iostate_short & 0x00400000)						CMD_Printf("[POGO_VTP_DUT_GND_0] : NG\r\n");

	CMD_Printf("============================== END ===============================\r\n\r\n");

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_IOTestXSync(void *pVoid)
{
	u8	result = 0, rtn, data;
	cmdFunctionData_t	*pData;
	u32 iostate;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	// xsync = 1
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x11);
	if(rtn == 0)						return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_IOTEST_XSYNC;

	data = 1;

	SYS_WordToHex(data, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	iostate = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf("\r\n\r\n=========================== XSYNC TEST ==========================\r\n");

	if(iostate)
	{
		CMD_Printf("[XSYNC TEST DONE] : NG\r\n");
		CMD_Printf("============================== END ==============================\r\n\r\n");
		return 1;
	}							

	// xsync = 0
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x10);
	if(rtn == 0)						return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_IOTEST_XSYNC;

	data = 0;

	SYS_WordToHex(data, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	iostate = SYS_HexToWord(&sbParsingData.pRxData[0]);
	if(iostate)
	{
		CMD_Printf("[XSYNC TEST DONE] : NG\r\n");
	}							
	else
	{
		CMD_Printf("[XSYNC TEST DONE] : OK\r\n");
	}

	CMD_Printf("============================== END ==============================\r\n\r\n");

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_PBTestStart(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	u32 iostate;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_PBTEST_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	iostate = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf("\r\n\r\n========================== PANICB TEST ==========================\r\n");
	if(iostate == 0)						CMD_Printf("[PANICB TEST DONE] : OK\r\n");
	else 									CMD_Printf("[DC_IO_AOP_PMU_DPMIC_TO_DISPLAY_PANICB_L] : NG\r\n");

	CMD_Printf("============================== END ==============================\r\n\r\n");

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_CRCTestStart(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	u32 tx_crc_R, tx_crc_G, tx_crc_B, rx_crc_R, rx_crc_G, rx_crc_B;
	dptxSFunction_t	sFunction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_CRCTEST_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 12)		return result;

	rx_crc_R = SYS_HexToWord(&sbParsingData.pRxData[0]);
	rx_crc_G = SYS_HexToWord(&sbParsingData.pRxData[4]);
	rx_crc_B = SYS_HexToWord(&sbParsingData.pRxData[8]);

	memset(&sFunction, NULL, sizeof(sFunction));
	sFunction.channel	= pData->position + 1;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= 0x0030;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)									tx_crc_R = sFunction.buffer[1];
	else									return result;
	sFunction.buffer[0]	= 0x0031;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)									tx_crc_G = sFunction.buffer[1];
	else									return result;
	sFunction.buffer[0]	= 0x0032;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)									tx_crc_B = sFunction.buffer[1];
	else									return result;

	CMD_Printf("\r\n\r\n============================ CRC TEST ===========================\r\n");
	CMD_Printf("[CRC_R] DPTX=0x%08X, DPRX=0x%08X : ", tx_crc_R, rx_crc_R);
	if(tx_crc_R == rx_crc_R)				CMD_Printf("OK\r\n");
	else									CMD_Printf("NG\r\n");
	CMD_Printf("[CRC_G] DPTX=0x%08X, DPRX=0x%08X : ", tx_crc_G, rx_crc_G);
	if(tx_crc_G == rx_crc_G)				CMD_Printf("OK\r\n");
	else									CMD_Printf("NG\r\n");
	CMD_Printf("[CRC_B] DPTX=0x%08X, DPRX=0x%08X : ", tx_crc_B, rx_crc_B);
	if(tx_crc_B == rx_crc_B)				CMD_Printf("OK\r\n");
	else									CMD_Printf("NG\r\n");
	CMD_Printf("============================== END ==============================\r\n\r\n");

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_SymbolTestStart(void *pVoid)
{
	u8	result = 0, rtn, lanecnt;
	cmdFunctionData_t	*pData;
	u32	tx_control, mask = 0x8000000F, regData;
	u16 err_lane[4];
	dptxSFunction_t	sFunction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	memset(&sFunction, NULL, sizeof(sFunction));
	sFunction.channel	= pData->position + 1;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= 0x00;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)								tx_control = sFunction.buffer[1];
	else								return result;

	lanecnt = (tx_control & 0x000003E0) >> 5;	// Lane Count = bit [9:5]
	
	regData = tx_control & ~mask;
	regData |= 0x0B;		// TP = 0x0B(PRBS7)

	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_WRITE;
	sFunction.buffer[0]	= 0x00;
	sFunction.buffer[1]	= regData;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn == 0)						return result;

	HAL_Delay(500);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_SYMBOLTEST_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 8)		return result;

	err_lane[0] = SYS_HexToWord(&sbParsingData.pRxData[0]);
	err_lane[1] = SYS_HexToWord(&sbParsingData.pRxData[2]);
	err_lane[2] = SYS_HexToWord(&sbParsingData.pRxData[4]);
	err_lane[3] = SYS_HexToWord(&sbParsingData.pRxData[6]);

	CMD_Printf("\r\n\r\n========================== SYMBOL TEST ==========================\r\n");
	for(int cnt = 0; cnt < lanecnt; cnt++)
	{
		CMD_Printf("[Lane[%d]] ERR COUNT=%d : ", cnt, err_lane[cnt]);
		if(!err_lane[cnt])							CMD_Printf("OK\r\n");
		else										CMD_Printf("NG\r\n");
	}
	CMD_Printf("============================== END ==============================\r\n\r\n");

	sFunction.buffer[1]	= tx_control;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn == 0)						return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxRegWrite(void *pVoid)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	data = CMD_StrToUL(pData->pArgv[1]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPTX_REG_WRITE;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_WordToHex(data, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxRegRead(void *pVoid)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)			return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPTX_REG_READ;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	data = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf(";DATA=0x%08X", data);

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_RxRegWrite(void *pVoid)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	data = CMD_StrToUL(pData->pArgv[1]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPRX_REG_WRITE;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_WordToHex(data, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_RxRegRead(void *pVoid)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)			return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPRX_REG_READ;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	data = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf(";DATA=0x%08X", data);

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxAuxWrite(void *pVoid)
{
	u8	result = 0, rtn, status;
	u32	adrs;
	u16 dataSize;
	u8	data[256];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)				return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	dataSize = pData->argc - 1;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPTX_AUX_WRITE;
	sbParsingData.waitTime				= 5000;

	SYS_WordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_WordToHex(dataSize, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	for(int cnt = 0; cnt < dataSize; cnt++)
	{
		data[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
		SYS_WordToHex(data[cnt], &sbParsingData.pTxData[sbParsingData.txSize]);
		sbParsingData.txSize				+= 1;
	}

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 1)		return result;

	status = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf(";STATUS=%d", status);

	if(status == 0)						result = 1;

	return result;

}

static u8 Cmd_RXDPC_TxAuxRead(void *pVoid)
{
	u8	result = 0, rtn, status;
	u32	adrs;
	u16 dataSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)						return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	dataSize = CMD_StrToUL(pData->pArgv[1]);

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPTX_AUX_READ;
	sbParsingData.waitTime				= 5000;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;
	SYS_HWordToHex(dataSize, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != dataSize + 1)	return result;

	status = SYS_HexToWord(&sbParsingData.pRxData[0]);

	CMD_Printf(";STATUS=%d,DATA=", status);

	for(u32 cnt = 0; cnt < dataSize; cnt++)
	{
		if(cnt != (dataSize - 1))		CMD_Printf("0x%02X ", sbParsingData.pRxData[cnt + 1]);
		else							CMD_Printf("0x%02X", sbParsingData.pRxData[cnt + 1]);
	}

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxHpdIrqEnable(void *pVoid)
{
	u8	result = 0, rtn, state;

	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPTX_HPD_ENABLE;

	state = 1;

	SYS_HWordToHex(state, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	HAL_Delay(50);

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_TxHpdIrqDisable(void *pVoid)
{
	u8	result = 0, rtn, state;

	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)						return result;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel				= pData->position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPTX_HPD_ENABLE;

	state = 0;

	SYS_HWordToHex(state, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	HAL_Delay(50);

	result = 1;

	return result;
}

static u8 Cmd_RXDPC_ExtendedLaneSupport(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	dptxTransaction_t	dptxTransaction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= pData->position + 1;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_WRITE;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxTransaction.adrs	= 0x00471;	// DPCD_ADDR_EXTENDED_LANE_COUNT_SUPPORT
	dptxTransaction.size	= 1;

	if(pData->option == 0x01)				dptxTransaction.pData[0] = 0x03;
	else									dptxTransaction.pData[0] = 0x00;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		CMD_Printf(";STATUS=%d", dptxTransaction.status);
	}

	return result;
}

static u8 Cmd_RXDPC_ExtendedLaneSet(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	dptxTransaction_t	dptxTransaction;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= pData->position + 1;

	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_WRITE;
	dptxTransaction.pData	= (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	dptxTransaction.adrs	= 0x0035F;	// DPCD_ADDR_LANE_COUNT_SET_EXT
	dptxTransaction.size	= 1;

	if(pData->option == 0x01)				dptxTransaction.pData[0] = 0x01;
	else									dptxTransaction.pData[0] = 0x00;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
		CMD_Printf(";STATUS=%d", dptxTransaction.status);
	}

	return result;
}


const	commandList_t	cmdList_RXDPCore[] = {
//		{	Command,					Function,						Option,		Mode,			RootMessage					Note}
		{	"help",						Cmd_Help,						NULL,		CMD_MODE_NONE,	";HELP",					NULL					},
		{	"version",					Cmd_RXDPC_Version,				NULL,		CMD_MODE_NONE,	";VERSION",					NULL					},
		{	"dpcore.reset",				Cmd_RXDPC_Reset,				NULL,		CMD_MODE_NONE,	";RESET",					NULL					},
		{	"dpcore.temperature",		Cmd_RXDPC_Temperature,			NULL,		CMD_MODE_NONE,	";TEMPERATURE",				NULL					},
		{	"dpcore.reg.write",			Cmd_RXDPC_CoreRegWrite,			NULL,		CMD_MODE_NONE,	";DPC.REG.WRITE",			NULL					},
		{	"dpcore.reg.read",			Cmd_RXDPC_CoreRegRead,			NULL,		CMD_MODE_NONE,	";DPC.REG.READ",			NULL					},
		{	"dpcore.video",				Cmd_RXDPC_VideoStream,			NULL,		CMD_MODE_NONE,	";VIDEO",					NULL					},
		{	"dpcore.video.format",		Cmd_RXDPC_VideoFormat,			NULL,		CMD_MODE_NONE,	";VIDEO.FORMAT",			NULL					},
		{	"dpcore.tx.laneconfig",		Cmd_RXDPC_TxLaneConfig,			NULL,		CMD_MODE_NONE,	";TX.LANECONFIG",			NULL					},
		{	"dpcore.tx.linkconfig",		Cmd_RXDPC_TxMainLinkConfig,		NULL,		CMD_MODE_NONE,	";TX.LINKCONFIG",			NULL					},
		{	"dpcore.tx.hpdconfig",		Cmd_RXDPC_TxHpdEventConfig,		NULL,		CMD_MODE_NONE,	";TX.HPDCONFIG",			NULL					},
		{	"dpcore.tx.colorconfig",	Cmd_RXDPC_TxColorSpaceConfig,	NULL,		CMD_MODE_NONE,	";TX.COLORCONFIG",			NULL					},
		{	"dpcore.tx.dummyvideo",		Cmd_RXDPC_TxDummyVideo,			NULL,		CMD_MODE_NONE,	";TX.DUMMYVIDEO",			NULL					},
		{	"dpcore.tx.blankconfig",	Cmd_RXDPC_TxBlankConfig,		NULL,		CMD_MODE_NONE,	";TX.BLANKCONFIG",			NULL					},
		{	"dpcore.rx.init",			Cmd_RXDPC_RxInit,				NULL,		CMD_MODE_NONE,	";RX.INIT",					NULL					},

		{	"video.input.set",			Cmd_RXDPC_VideoStreamInput,		NULL,		CMD_MODE_NONE,	";VIDEO.INPUT",				NULL					},
		{	"video.input.enable",		Cmd_RXDPC_VideoStreamEnable,	0x01,		CMD_MODE_NONE,	";VIDEO.INPUT.ENABLE",		NULL					},
		{	"video.input.disable",		Cmd_RXDPC_VideoStreamEnable,	0x00,		CMD_MODE_NONE,	";VIDEO.INPUT.DISABLE",		NULL					},
		{	"video.display",			Cmd_RXDPC_VideoDisplay,			NULL,		CMD_MODE_NONE,	";VIDEO.DISPLAY",			NULL					},

		{	"dctest.init",				Cmd_RXDPC_DCTestInit,			NULL,		CMD_MODE_NONE,	";DCTEST.INIT",				NULL					},
		{	"dctest.start",				Cmd_RXDPC_DCTestStart,			NULL,		CMD_MODE_NONE,	";DCTEST.START",			NULL					},
		{	"dctest.donecheck",			Cmd_RXDPC_DCTestDoneCheck,		NULL,		CMD_MODE_NONE,	";DCTEST.DONE.CHECK",		NULL					},
		{	"iotest.start",				Cmd_RXDPC_IOTestStart,			NULL,		CMD_MODE_NONE,	";IOTEST.START",			NULL					},
		{	"iotest2.start",			Cmd_RXDPC_IOTest2Start,			NULL,		CMD_MODE_NONE,	";IOTEST2.START",			NULL					},
		{	"iotest.xsync",				Cmd_RXDPC_IOTestXSync,			NULL,		CMD_MODE_NONE,	";IOTEST.XSYNC",			NULL					},
		{	"pbtest.start",				Cmd_RXDPC_PBTestStart,			NULL,		CMD_MODE_NONE,	";PBTEST.START",			NULL					},
		{	"crctest.start",			Cmd_RXDPC_CRCTestStart,			NULL,		CMD_MODE_NONE,	";CRCTEST.START",			NULL					},
		{	"symboltest.start",			Cmd_RXDPC_SymbolTestStart,		NULL,		CMD_MODE_NONE,	";SYMBOLTEST.START",		NULL					},

		{	"dptx.reg.write",			Cmd_RXDPC_TxRegWrite,			NULL,		CMD_MODE_NONE,	";DPTX.REG.WRITE",			NULL					},
		{	"dptx.reg.read",			Cmd_RXDPC_TxRegRead,			NULL,		CMD_MODE_NONE,	";DPTX.REG.READ",			NULL					},
		{	"dprx.reg.write",			Cmd_RXDPC_RxRegWrite,			NULL,		CMD_MODE_NONE,	";DPRX.REG.WRITE",			NULL					},
		{	"dprx.reg.read",			Cmd_RXDPC_RxRegRead,			NULL,		CMD_MODE_NONE,	";DPRX.REG.READ",			NULL					},
		{	"dptx.aux.write",			Cmd_RXDPC_TxAuxWrite,			NULL,		CMD_MODE_NONE,	";DPTX.AUX.WRITE",			NULL					},
		{	"dptx.aux.read",			Cmd_RXDPC_TxAuxRead,			NULL,		CMD_MODE_NONE,	";DPTX.AUX.READ",			NULL					},
		{	"dptx.hpd.enable",			Cmd_RXDPC_TxHpdIrqEnable,		NULL,		CMD_MODE_NONE,	";DPTX.HPD.ENABLE",			NULL					},
		{	"dptx.hpd.disable",			Cmd_RXDPC_TxHpdIrqDisable,		NULL,		CMD_MODE_NONE,	";DPTX.HPD.DISABLE",		NULL					},

		{	"exlane.support.enable",	Cmd_RXDPC_ExtendedLaneSupport,	0x01,		CMD_MODE_NONE,	";EXLANE.SUPPORT.ENABLE",	NULL					},
		{	"exlane.support.disable",	Cmd_RXDPC_ExtendedLaneSupport,	0x00,		CMD_MODE_NONE,	";EXLANE.SUPPORT.DISABLE",	NULL					},
		{	"exlane.set.enable",		Cmd_RXDPC_ExtendedLaneSet,		0x01,		CMD_MODE_NONE,	";EXLANE.SET.ENABLE",		NULL					},
		{	"exlane.set.disable",		Cmd_RXDPC_ExtendedLaneSet,		0x00,		CMD_MODE_NONE,	";EXLANE.SET.DISABLE",		NULL					},

		{	NULL,						NULL,							NULL,		NULL,			NULL,					NULL									},
};


