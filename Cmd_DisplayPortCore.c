#include	"Cmd_DisplayPortCore.h"

static uc8	cmdNote_dpCorePowerOFF[] = "E7602 Core Power OFF | A-Cnt : 0\0";
static u8 Cmd_DPC_PowerOFF(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	result = Core_PowerCtrl(pData->position, LOW);

	return result;
}

static uc8	cmdNote_dpCorePowerON[] = "E7602 Core Power ON | A-Cnt : 0\0";
static u8 Cmd_DPC_PowerON(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	result = Core_PowerCtrl(pData->position, HIGH);

	return result;
}

static uc8	cmdNote_dpCoreReset[] = "E7602 Core SW Reset | A-Cnt : 0\0";
static u8 Cmd_DPC_Reset(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	result = DPC_CoreSoftReset(pData->position);

	return result;
}

static uc8	cmdNote_dpCoreNiosWorkingPeriod[] = "\0";
static u8 Cmd_DPC_NiosWorkingPeriod(void *pVoid)
{
	u8	result = 0;
	u32	period;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	result = DPC_NiosWorkingPeriod(pData->position, &period);
	if(result != 0)						CMD_Printf(";%duSec", period);

	return result;
}

static uc8	cmdNote_dpCoreTemperature[] = "\0";
static u8 Cmd_DPC_Temperature(void *pVoid)
{
	u8	result = 0;
	float	temperature;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)				return result;

	result = DPC_GetTemperature(pData->position, &temperature);
	if(result)							CMD_Printf(";%.03fDegree C", temperature);

	return result;
}

static uc8	cmdNote_dpCoreRegWrite[] = "E7602 Core Register Write | A-Cnt : 2 | A-Value : Address, Data\0";
static u8 Cmd_DPC_RegWrite(void *pVoid)
{
	u8	result = 0;
	u16	adrs;
	u32	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)			return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	data = CMD_StrToUL(pData->pArgv[1]);

	result = DPC_DirectRegWriteSingle(pData->position, adrs, data);

	return result;
}

static uc8	cmdNote_dpCoreRegRead[] = "E7602 Core Register Read | A-Cnt : 1 | A-Value : Address\0";
static u8 Cmd_DPC_RegRead(void *pVoid)
{
	u8	result = 0;
	u16	adrs;
	u32	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)			return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);

	result = DPC_DirectRegReadSingle(pData->position, adrs, &data);
	if(result)
	{
		CMD_Printf(";DATA=0x%08X", data);
	}

	return result;
}

static uc8	cmdNote_dpCoreRegWriteBurst[] = "E7602 Core Register Write Burst | A-Cnt : 1 + N | A-Value : Address, Data0 ... DataN\0";
static u8 Cmd_DPC_RegWriteBurst(void *pVoid)
{
	u8	result = 0;
	u16	adrs, size;
	u32	*pBurstData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 2)			return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	size = pData->argc - 1;
	if(size > 256)				return result;

	pBurstData = (u32*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	for(u16 cnt = 0; cnt < size; cnt++)
	{
		pBurstData[cnt] = CMD_StrToUL(pData->pArgv[cnt + 1]);
	}

	result = DPC_DirectRegWrite(pData->position, adrs, pBurstData, size);

	return result;
}

static uc8	cmdNote_dpCoreRegReadBurst[] = "E7602 Core Register Read Burst | A-Cnt : 2 | A-Value : Address, Count\0";
static u8 Cmd_DPC_RegReadBurst(void *pVoid)
{
	u8	result = 0;
	u16	adrs, size;
	u32	*pBurstData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)			return result;

	adrs = CMD_StrToUL(pData->pArgv[0]);
	size = CMD_StrToUL(pData->pArgv[1]);
	if(size > 256)					return result;

	pBurstData = (u32*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	result = DPC_DirectRegRead(pData->position, adrs, pBurstData, size);
	if(result)
	{
		CMD_Printf(";DATA=");

		for(u16 cnt = 0; cnt < size; cnt++)
		{
			if(cnt != (size - 1))		CMD_Printf("0x%08X ", pBurstData[cnt]);
			else						CMD_Printf("0x%08X", pBurstData[cnt]);
		}
	}

	return result;
}

static uc8	cmdNote_dpCoreTxMainLinkConfig[] = "E7602 DisplayPort Tx Link Config | A-Cnt : 2 | A-Value : LinkRate, LaneCount\0";
static u8 Cmd_DPC_TxMainLinkConfig(void *pVoid)
{
	u8	result = 0;
	u32	linkRate, laneCount;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)			return result;

	linkRate	= CMD_StrToUL(pData->pArgv[0]);
	laneCount	= CMD_StrToUL(pData->pArgv[1]);

	result = DP_TxMainLinkConfig(pData->position, linkRate, laneCount);

	return result;
}

static uc8	cmdNote_dpCoreTxHpdConfig[] = "E7602 DisplayPort Tx HPD Config | A-Cnt : 3 | A-Value : LinkTraining Mode, EDID Read, Video Output\0";
static u8 Cmd_DPC_TxHpdEventConfig(void *pVoid)
{
	u8	result = 0;
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

	result = DP_TxHpdEventConfig(pData->position, ltMode, edidReadDisable, videoOutputDisable);

	return result;
}

static uc8	cmdNote_dpCoreTxColorSpaceConfig[] = "E7602 DisplayPort Tx Color Space Config | A-Cnt : 5 | A-Value : Format, BPC, Range, Use VSC SDP, Colorimetry\0";
static u8 Cmd_DPC_TxColorSpaceConfig(void *pVoid)
{
	u8	result = 0;
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

	result = DP_TxColorSpaceConfig(pData->position, format, bpc, range, useVscSdp, colorimetry);

	return result;
}

static uc8	cmdNote_dpCoreTxVConfigALPDP[] = "E7602 DisplayPort Tx ALP-DP Config | A-Cnt : 3 | A-Value : Enable, VFP, VBP\0";
static u8 Cmd_DPC_TxVideoConfigALPDP(void *pVoid)
{
	u8	result = 0;
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

	result = DP_TxVideoConfigDummyVideo(pData->position, ctrl, be, vfp, vbp);

	return result;
}

static uc8	cmdNote_dpCoreTxBlankConfig[] = "\0";
static u8 Cmd_DPC_TxBlankConfig(void *pVoid)
{
	u8	result = 0;
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

	result = DP_TxBlankConfig(pData->position, sof, eof);

	return result;
}

static uc8	cmdNote_dpCoreTxAuxConfig[] = "\0";
static u8 Cmd_DPC_TxAuxConfig(void *pVoid)
{
	u8	result = 0;
	u32	preamble, precharge, AUX_INVERT;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)								return result;

	preamble	= CMD_StrToUL(pData->pArgv[0]);
	precharge	= CMD_StrToUL(pData->pArgv[1]);
	if(CMD_Compare(pData->pArgv[2], "disable"))			AUX_INVERT = 0x00;
	else if(CMD_Compare(pData->pArgv[2], "enable"))		AUX_INVERT = 0x1f;

	result = DP_TxAuxConfig(pData->position, preamble, precharge, AUX_INVERT);

	return result;
}

static uc8	cmdNote_dpCoreTxSdpBurstConfig[] = "\0";
static u8 Cmd_DPC_TxSdpBurstConfig(void *pVoid)
{
	u8	result = 0;
	u32	sdpBurstCtrl, sdpBurstCount, sdpBurstMode, sdpBurstHBlankSLine;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)								return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))
	{
		if(pData->argc != 1)							return result;

		sdpBurstCtrl = 0;
		sdpBurstCount = 0;
		sdpBurstMode = 0;
		sdpBurstHBlankSLine = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "enable"))
	{
		if(pData->argc != 4)							return result;

		sdpBurstCtrl = 1;

		sdpBurstCount = CMD_StrToUL(pData->pArgv[1]);
		if(sdpBurstCount > 5)							return result;

		if(CMD_Compare(pData->pArgv[2], "vBlank"))		sdpBurstMode = 0;
		else if(CMD_Compare(pData->pArgv[2], "hBlank"))	sdpBurstMode = 1;
		else											return result;

		sdpBurstHBlankSLine = CMD_StrToUL(pData->pArgv[3]);
	}
	else												return result;

	result = DP_TxSdpBurstConfig(pData->position, sdpBurstCtrl, sdpBurstCount, sdpBurstMode, sdpBurstHBlankSLine);

	return result;
}

static uc8	cmdNote_dpCoreTxLaneConfig[] = "\0";
static u8 Cmd_DPC_TxLaneConfig(void *pVoid)
{
	u8	result = 0;
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

	result = DPC_MainLinkConfig(pData->position, channel, laneOrder, pnSwap, voltRange);

	return result;
}


static uc8	cmdNote_dpCoreTxAlpmSignal[] = "\0";
static u8 Cmd_DPC_TxAlpmSignal(void *pVoid)
{
	u8	result = 0;
	u32 mainLinkSignal, wakeUpSignal, symbolLockSignal, auxAckCheck;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)										return result;

	if(CMD_Compare(pData->pArgv[0], "ML_PHY_SLEEP"))			mainLinkSignal = 0;
	else if(CMD_Compare(pData->pArgv[0], "SLEEP"))				mainLinkSignal = 0;
	else if(CMD_Compare(pData->pArgv[0], "ML_PHY_STANDBY"))		mainLinkSignal = 1;
	else if(CMD_Compare(pData->pArgv[0], "STANDBY"))			mainLinkSignal = 1;
	else														return result;

	if(CMD_Compare(pData->pArgv[1], "AUX_PHY_WAKE"))			wakeUpSignal = 0;
	else if(CMD_Compare(pData->pArgv[1], "APW"))				wakeUpSignal = 0;
	else if(CMD_Compare(pData->pArgv[1], "WAKE_F_CHANGE"))		wakeUpSignal = 1;
	else if(CMD_Compare(pData->pArgv[1], "WFC"))				wakeUpSignal = 1;
	else														return result;

	if(CMD_Compare(pData->pArgv[2], "TPS2"))					symbolLockSignal = 0;
	else if(CMD_Compare(pData->pArgv[2], "TP2"))				symbolLockSignal = 0;
	else if(CMD_Compare(pData->pArgv[2], "TPS3"))				symbolLockSignal = 1;
	else if(CMD_Compare(pData->pArgv[2], "TP3"))				symbolLockSignal = 1;
	else														return result;

	if(CMD_Compare(pData->pArgv[3], "Disable"))					auxAckCheck = 0;
	else if(CMD_Compare(pData->pArgv[3], "Enable"))				auxAckCheck = 1;
	else														return result;

	result = DP_TxAlpmSignal(pData->position, mainLinkSignal, wakeUpSignal, symbolLockSignal, auxAckCheck);

	return result;
}

static uc8	cmdNote_dpCoreTxAlpmTiming[] = "\0";
static u8 Cmd_DPC_TxAlpmTiming(void *pVoid)
{
	u8	result = 0;
	u32 timing_T12, timing_N1_vb, timing_N2_vb, timing_N3_vb, timing_N5b_vb;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)									return result;

	timing_T12		= CMD_StrToUL(pData->pArgv[0]);
	timing_N1_vb	= CMD_StrToUL(pData->pArgv[1]);
	timing_N2_vb 	= CMD_StrToUL(pData->pArgv[2]);
	timing_N3_vb	= CMD_StrToUL(pData->pArgv[3]);
	timing_N5b_vb	= CMD_StrToUL(pData->pArgv[4]);

	result = DP_TxAlpmTiming(pData->position, timing_T12, timing_N1_vb, timing_N2_vb, timing_N3_vb, timing_N5b_vb);

	return result;
}

static uc8	cmdNote_dpCoreTxAlpmClk[] = "\0";
static u8 Cmd_DPC_TxAlpmClk(void *pVoid)
{
	u8	result = 0;
	float rate =0.0;
	u32 rate_u=0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)									return result;

	rate = (float)CMD_AToF(pData->pArgv[0]);
	if(rate>10.0)
	{
		rate = 10.0;
	}
	rate_u = (u32)(rate*1000);

	result = DP_TxAlpmClk(pData->position, rate_u);

	return result;
}

static uc8	cmdNote_dpCoreTxACVRRCtrl[] = "\0";
static u8 Cmd_DPC_TxACVRRCtrl(void *pVoid)
{
	u8	result = 0;
	u32 ACVRR_Enable=0, ACVRR_Range=0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)									return result;

	ACVRR_Range			= CMD_StrToUL(pData->pArgv[0]);
	if(ACVRR_Range>256)
	{
		return result;
	}

	if(CMD_Compare(pData->pArgv[1], "Disable"))					ACVRR_Enable = 0;
	else if(CMD_Compare(pData->pArgv[1], "Enable"))				ACVRR_Enable = 1;
	else														return result;

	result = DP_TxACVRRCtrl(pData->position, ACVRR_Range, ACVRR_Enable);

	return result;
}

static uc8	cmdNote_dpCoreTxACVRRSet[] = "\0";
static u8 Cmd_DPC_TxACVRRSetting(void *pVoid)
{
	u8	result = 0;
	u32 ACVRR_num = 0, ACVRR_hz = 0, ACVRR_cnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)									return result;

	ACVRR_num			= CMD_StrToUL(pData->pArgv[0]);
	ACVRR_hz			= CMD_StrToUL(pData->pArgv[1]);
	ACVRR_cnt 			= CMD_StrToUL(pData->pArgv[2]);

	if(ACVRR_num>128)
	{
		return result;
	}
	if(ACVRR_hz>120)
	{
		return result;
	}
	if(ACVRR_cnt>256)
	{
		return result;
	}

	result = DP_TxACVRRSet(pData->position, ACVRR_num, ACVRR_hz, ACVRR_cnt);

	return result;
}

#if 0	// EDID NOT USE IN 2CH _ 210226_lyh
static uc8	cmdNote_dpCoreRxInit[] = "\0";
static u8 Cmd_DPC_RxInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)									return result;

	// DP RxInit (EDID Read & CoreReg Write)
	result = DP_RxInit(pData->position);

	return result;
}
#endif

static uc8	cmdNote_dpCoreTxAUXINVERT[] = "\0";
static u8 Cmd_DPC_TxAUXinverting(void *pVoid)
{
	u8	result = 0;
	u32 AUX_INVERT =0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)									return result;

	AUX_INVERT	= CMD_StrToUL(pData->pArgv[0]);

	result = DP_TxAUXINV(pData->position, AUX_INVERT);

	return result;
}

static uc8	cmdNote_dpCorexsync[] = "\0";
static u8 Cmd_DPC_xsync(void *pVoid)
{
	u8	result = 0;
	u32 xsync_enable = 0, xsync_duty = 0, xsync_shift = 0, xsync_sel = 2;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 3 || pData->argc > 4)					return result;

	if(CMD_Compare(pData->pArgv[0], "Disable"))					xsync_enable = 0;
	else if(CMD_Compare(pData->pArgv[0], "Enable"))				xsync_enable = 1;

	xsync_duty = CMD_StrToUL(pData->pArgv[1]);
	xsync_shift = CMD_StrToUL(pData->pArgv[2]);

	if(xsync_duty < 1 || xsync_duty > 9)		return result;
	if(xsync_shift > 4095)	return result;

	if(pData->argc == 4)
	{
		xsync_sel = CMD_StrToUL(pData->pArgv[3]);
		if(xsync_sel > 1) 			return result;
	}

	result = DP_XSYNC(pData->position, xsync_enable, xsync_duty, xsync_shift, xsync_sel);

	return result;
}

static uc8	cmdNote_dpCorebsync[] = "\0";
static u8 Cmd_DPC_bsync(void *pVoid)
{
	u8	result = 0;
	u32 bsync_enable = 0, bsync_width = 0, bsync_shift = 0, bsync_sel = 2;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 3 || pData->argc > 4)					return result;

	if(CMD_Compare(pData->pArgv[0], "Disable"))					bsync_enable = 0;
	else if(CMD_Compare(pData->pArgv[0], "Enable"))				bsync_enable = 1;

	bsync_width = CMD_StrToUL(pData->pArgv[1]);
	bsync_shift = CMD_StrToUL(pData->pArgv[2]);

	if(bsync_shift > 4095) 	return result;

	if(pData->argc == 4)
	{
		bsync_sel = CMD_StrToUL(pData->pArgv[3]);
		if(bsync_sel > 1) 			return result;
	}

	result = DP_BSYNC(pData->position, bsync_enable, bsync_width, bsync_shift, bsync_sel);

	return result;
}

static u8 Cmd_VideoStreamSemiAuto(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	coreVideoInfo_t		videoInfo;

	pData = (cmdFunctionData_t*)pVoid;

	if((pData->argc != 9) && (pData->argc != 10))	return result;

	memset(&videoInfo, NULL, sizeof(videoInfo));

	videoInfo.mode			= CORE_VIDEO_MODE_SEMI_AUTO;

	if(pData->argc == 9)
	{
		videoInfo.hActive		= CMD_StrToUL(pData->pArgv[0]);
		videoInfo.vActive		= CMD_StrToUL(pData->pArgv[1]);
		videoInfo.cFrameRate	= CMD_StrToUL(pData->pArgv[2]);
		videoInfo.hfp			= CMD_StrToUL(pData->pArgv[3]);
		videoInfo.hbp			= CMD_StrToUL(pData->pArgv[4]);
		videoInfo.vfp			= CMD_StrToUL(pData->pArgv[5]);
		videoInfo.vbp			= CMD_StrToUL(pData->pArgv[6]);
		videoInfo.hSync			= CMD_StrToUL(pData->pArgv[7]);
		videoInfo.vSync			= CMD_StrToUL(pData->pArgv[8]);
	}
	else
	{
		videoInfo.hActive		= CMD_StrToUL(pData->pArgv[0]);
		videoInfo.vActive		= CMD_StrToUL(pData->pArgv[1]);
		videoInfo.mFrameRate	= CMD_StrToUL(pData->pArgv[2]);
		videoInfo.cFrameRate	= CMD_StrToUL(pData->pArgv[3]);
		videoInfo.hfp			= CMD_StrToUL(pData->pArgv[4]);
		videoInfo.hbp			= CMD_StrToUL(pData->pArgv[5]);
		videoInfo.vfp			= CMD_StrToUL(pData->pArgv[6]);
		videoInfo.vbp			= CMD_StrToUL(pData->pArgv[7]);
		videoInfo.hSync			= CMD_StrToUL(pData->pArgv[8]);
		videoInfo.vSync			= CMD_StrToUL(pData->pArgv[9]);
	}

	result = CoreVC_VideoStreamConfig(pData->position, &videoInfo);

	rtn = DPC_DirectRegWriteSingle(pData->position, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
	if(rtn == 0)                    return 0;

	return result;
}

static u8 Cmd_VideoStreamFormat(void *pVoid)
{
	u8	result = 0;
	u32	toggle, mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)									return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))				toggle = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))			toggle = 1;

	if(CMD_Compare(pData->pArgv[1], "RGB"))					mode = 0;
	else if(CMD_Compare(pData->pArgv[1], "YCbCr444"))		mode = 0;
	else if(CMD_Compare(pData->pArgv[1], "YCbCr422_T1"))	mode = 1;
	else if(CMD_Compare(pData->pArgv[1], "YCbCr422_T2"))	mode = 2;
	else													return result;

	result = CoreVC_VideoStreamFormat(pData->position, toggle, mode);

	return result;
}

static u8 Cmd_DPC_UpdateCheck(void *pVoid)
{
	u8	result = 0;
	u8	data;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)									return result;

	result = DPC_UpdateCheck(pData->position, &data);

	if(result)
	{
		CMD_Printf(";DATA_CH%d=%d", pData->channel, data);
	}

	return result;
}

static u8 Cmd_DPC_HpdEvent(void *pVoid)
{
	u8	result = 0;
	u8  enable;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)									return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))				enable = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))			enable = 2;

	result = HPDEVENT_Enable(pData->position, enable);

	return result;
}

static u8 Cmd_DPC_ExtClkCntStart(void *pVoid)
{
	u8	result = 0;
	u32	time;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)									return result;

	time = CMD_StrToUL(pData->pArgv[0]);
	if(time > 4095)										return result;

	result = EXT_CLK_CNT_START(pData->position, time);

	return result;
}

static u8 Cmd_DPC_ExtClkCount(void *pVoid)
{
	u8	result = 0;
	float cnt_value;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)									return result;

	result = EXT_CLK_CNT_VALUE(pData->position, &cnt_value);
	if(result)				CMD_Printf(";COUNT=%.06f", cnt_value);

	return result;
}

static u8 Cmd_DPC_ExtClkPeriod(void *pVoid)
{
	u8	result = 0;
	u32 period;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)									return result;

	result = EXT_CLK_PERIOD(pData->position, &period);
	if(result)				CMD_Printf(";PERIOD=%d usec", period);

	return result;
}

static u8 Cmd_DPC_PPAEnable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)									return result;

	result = PPA_Enable(pData->position);

	return result;
}

static u8 Cmd_DPC_PPADisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)									return result;

	result = PPA_Disable(pData->position);

	return result;
}

static u8 Cmd_DPC_PPAMode(void *pVoid)
{
	u8	result = 0;
	u8	pnl_mode, simple_en, green, avg_mode, end_mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)									return result;

	pnl_mode	= CMD_StrToUL(pData->pArgv[0]);
	simple_en	= CMD_StrToUL(pData->pArgv[1]);
	green		= CMD_StrToUL(pData->pArgv[2]);
	avg_mode	= CMD_StrToUL(pData->pArgv[3]);
	end_mode	= CMD_StrToUL(pData->pArgv[4]);

	result = PPA_Mode(pData->position, pnl_mode, simple_en, green, avg_mode, end_mode);

	return result;
}

static u8 Cmd_DPC_OSDEnable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)									return result;

	result = OSD_Enable(pData->position, ON);

	return result;
}


static u8 Cmd_DPC_OSDDisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)									return result;

	result = OSD_Enable(pData->position, OFF);

	return result;
}

static u8 Cmd_DPC_OSDSetting(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	u16	red, green, blue, size = 0, thick = 2;
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 5)									return result;

	red		= CMD_StrToUL(pData->pArgv[0]);
	if(red > 0xFF)					return result;

	green	= CMD_StrToUL(pData->pArgv[1]);
	if(green > 0xFF)				return result;

	blue	= CMD_StrToUL(pData->pArgv[2]);
	if(blue > 0xFF)					return result;

	size		= CMD_StrToUL(pData->pArgv[3]);
	if(size > 501)					return result;

	thick		= CMD_StrToUL(pData->pArgv[4]);
	if(thick > 15)					return result;

	result = OSD_Setting(pData->position, red, green, blue, size, thick);

	return result;
}

static u8 Cmd_DPC_OSDBox(void *pVoid)
{
	u8	result = 0, box;
	cmdFunctionData_t	*pData;
	u16	xpos, ypos;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)									return result;

	if(CMD_Compare(pData->pArgv[0], "1"))						box = 0;
	else if(CMD_Compare(pData->pArgv[0], "2"))					box = 1;
	else if(CMD_Compare(pData->pArgv[0], "3"))					box = 2;
	else if(CMD_Compare(pData->pArgv[0], "4"))					box = 3;
	else if(CMD_Compare(pData->pArgv[0], "5"))					box = 4;
	else														return result;

	xpos	= CMD_StrToUL(pData->pArgv[1]);
	ypos	= CMD_StrToUL(pData->pArgv[2]);

	result = OSD_Box(pData->position, box, xpos, ypos);

	return result;
}

const	commandList_t	cmdList_DisplayPortCore[] = {
//		{	Command,				Function,						Option,		Mode,			RootMessage				Note}
		{	"help",					Cmd_Help,						NULL,		CMD_MODE_NONE,	";HELP",				NULL									},
		{	"power.off",			Cmd_DPC_PowerOFF,				NULL,		CMD_MODE_NONE,	";POWER.OFF",			(u8*)cmdNote_dpCorePowerOFF				},
		{	"power.on",				Cmd_DPC_PowerON,				NULL,		CMD_MODE_NONE,	";POWER.ON",			(u8*)cmdNote_dpCorePowerON				},
		{	"reset",				Cmd_DPC_Reset,					NULL,		CMD_MODE_NONE,	";RESET",				(u8*)cmdNote_dpCoreReset				},
		{	"NIOS.optime",			Cmd_DPC_NiosWorkingPeriod,		NULL,		CMD_MODE_NONE,	";NIOS.OPTIME",			(u8*)cmdNote_dpCoreNiosWorkingPeriod	},
		{	"Temperature",			Cmd_DPC_Temperature,			NULL,		CMD_MODE_NONE,	";TEMPERATURE",			(u8*)cmdNote_dpCoreTemperature			},
		{	"Reg.Write",			Cmd_DPC_RegWrite,				NULL,		CMD_MODE_NONE,	";REG.WRITE",			(u8*)cmdNote_dpCoreRegWrite				},
		{	"Reg.Read",				Cmd_DPC_RegRead,				NULL,		CMD_MODE_NONE,	";REG.READ",			(u8*)cmdNote_dpCoreRegRead				},
		{	"Reg.Write_b",			Cmd_DPC_RegWriteBurst,			NULL,		CMD_MODE_NONE,	";REG.WRITE_B",			(u8*)cmdNote_dpCoreRegWriteBurst		},
		{	"Reg.Read_b",			Cmd_DPC_RegReadBurst,			NULL,		CMD_MODE_NONE,	";REG.READ_B",			(u8*)cmdNote_dpCoreRegReadBurst			},
		{	"Tx.LinkConfig",		Cmd_DPC_TxMainLinkConfig,		NULL,		CMD_MODE_NONE,	";TX.LINKCONFIG",		(u8*)cmdNote_dpCoreTxMainLinkConfig		},
		{	"Tx.HpdConfig",			Cmd_DPC_TxHpdEventConfig,		NULL,		CMD_MODE_NONE,	";TX.HPDCONFIG",		(u8*)cmdNote_dpCoreTxHpdConfig			},
		{	"Tx.ColorConfig",		Cmd_DPC_TxColorSpaceConfig,		NULL,		CMD_MODE_NONE,	";TX.COLORCONFIG",		(u8*)cmdNote_dpCoreTxColorSpaceConfig	},
		{	"Tx.DummyVideo",		Cmd_DPC_TxVideoConfigALPDP,		NULL,		CMD_MODE_NONE,	";TX.DUMMYVIDEO",		(u8*)cmdNote_dpCoreTxVConfigALPDP		},
		{	"Tx.BlankConfig",		Cmd_DPC_TxBlankConfig,			NULL,		CMD_MODE_NONE,	";TX.BLANKCONFIG",		(u8*)cmdNote_dpCoreTxBlankConfig		},
		{	"Tx.AuxConfig",			Cmd_DPC_TxAuxConfig,			NULL,		CMD_MODE_NONE,	";TX.AUXCONFIG",		(u8*)cmdNote_dpCoreTxAuxConfig			},
		{	"Tx.SdpBurstConfig",	Cmd_DPC_TxSdpBurstConfig,		NULL,		CMD_MODE_NONE,	";TX.SDPBURSTCONFIG",	(u8*)cmdNote_dpCoreTxSdpBurstConfig		},
		{	"Tx.LaneConfig",		Cmd_DPC_TxLaneConfig,			NULL,		CMD_MODE_NONE,	";TX.LANECONFIG",		(u8*)cmdNote_dpCoreTxLaneConfig			},
		{	"ALPM.Signal",			Cmd_DPC_TxAlpmSignal,			NULL,		CMD_MODE_NONE,	";ALPM.SIGNAL",			(u8*)cmdNote_dpCoreTxAlpmSignal			},
		{	"ALPM.Timing",			Cmd_DPC_TxAlpmTiming,			NULL,		CMD_MODE_NONE,	";ALPM.TIMING",			(u8*)cmdNote_dpCoreTxAlpmTiming			},
		{	"ALPM.Clk",				Cmd_DPC_TxAlpmClk,				NULL,		CMD_MODE_NONE,	";ALPM.CLK",			(u8*)cmdNote_dpCoreTxAlpmClk			},
		{	"ACVRR.Ctrl",			Cmd_DPC_TxACVRRCtrl,			NULL,		CMD_MODE_NONE,	";ACVRR.CTRL",			(u8*)cmdNote_dpCoreTxACVRRCtrl			},
		{	"ACVRR.Setting",		Cmd_DPC_TxACVRRSetting,			NULL,		CMD_MODE_NONE,	";ACVRR.SET",			(u8*)cmdNote_dpCoreTxACVRRSet			},
		{	"AUX.Inverting",		Cmd_DPC_TxAUXinverting,			NULL,		CMD_MODE_NONE,	";AUX.INVERT",			(u8*)cmdNote_dpCoreTxAUXINVERT			},
		{	"xsync",				Cmd_DPC_xsync,					NULL,		CMD_MODE_NONE,	";XSYNC",				(u8*)cmdNote_dpCorexsync				},
		{	"bsync",				Cmd_DPC_bsync,					NULL,		CMD_MODE_NONE,	";BSYNC",				(u8*)cmdNote_dpCorebsync				},
		{	"Video",				Cmd_VideoStreamSemiAuto,		NULL,		CMD_MODE_NONE,	";VIDEO",				NULL									},
		{	"video.Format",			Cmd_VideoStreamFormat,			NULL,		CMD_MODE_NONE,	";VIDEO.FORMAT",		NULL									},
		{	"update.check",			Cmd_DPC_UpdateCheck,			NULL,		CMD_MODE_NONE,	";UPDATE.CHECK",		NULL									},
		{	"hpdevent",				Cmd_DPC_HpdEvent,				NULL,		CMD_MODE_NONE,	";HPD.EVENT.CHECK",		NULL									},
		{	"pulse.init",			Cmd_DPC_ExtClkCntStart,			NULL,		CMD_MODE_NONE,	";TEBSYNC.COUNT.INIT",	NULL									},
		{	"pulse.count",			Cmd_DPC_ExtClkCount,			NULL,		CMD_MODE_NONE,	";TEBSYNC.COUNT.READ",	NULL									},
		{	"pulse.period",			Cmd_DPC_ExtClkPeriod,			NULL,		CMD_MODE_NONE,	";TEBSYNC.PERIOD.READ",	NULL									},
		{	"ppa.enable",			Cmd_DPC_PPAEnable,				NULL,		CMD_MODE_NONE,	";PPA.ENABLE",			NULL									},
		{	"ppa.disable",			Cmd_DPC_PPADisable,				NULL,		CMD_MODE_NONE,	";PPA.DISABLE",			NULL									},
		{	"ppa.mode",				Cmd_DPC_PPAMode,				NULL,		CMD_MODE_NONE,	";PPA.MODE",			NULL									},
		{	"osd.enable",			Cmd_DPC_OSDEnable,				NULL,		CMD_MODE_NONE,	";OSD.ENABLE",			NULL									},
		{	"osd.disable",			Cmd_DPC_OSDDisable,				NULL,		CMD_MODE_NONE,	";OSD.DISABLE",			NULL									},
		{	"osd.setting",			Cmd_DPC_OSDSetting,				NULL,		CMD_MODE_NONE,	";OSD.SETTING",			NULL									},
		{	"osd.box",				Cmd_DPC_OSDBox,					NULL,		CMD_MODE_NONE,	";OSD.BOX",				NULL									},

		{	NULL,					NULL,							NULL,		NULL,			NULL,					NULL									},
};


