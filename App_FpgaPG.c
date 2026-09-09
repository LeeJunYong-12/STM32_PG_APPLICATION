#include	"App_FpgaPG.h"

static	fpgaPgVideo_t	fpgaPgVideo;
static	u8	fpgaPgStatus = 0;
static	const u32 fpgaPgErrorCLK[33] =	{	127, 131, 133, 137, 139,
											143, 149, 151, 157, 161,
											163, 167, 169, 173, 179,
											181, 187, 191, 193, 197,
											199, 203, 209, 211, 217,
											221, 223, 227, 229, 233,
											239, 241, 247
										};

u8 FpgaPG_DataInit()
{
	u8	result = 0;

	memset(&fpgaPgVideo, NULL, sizeof(fpgaPgVideo));

	result = 1;

	return result;
}

u8 FpgaPG_GetStatus()
{
	return fpgaPgStatus;
}

u8 FpgaPG_SetStatus(u8 status)
{
	u8	result = 0;

	if(status > 1)		return result;

	fpgaPgStatus = status;

	result = 1;

	return result;
}

u8 FpgaPG_ClearSatatus()
{
	u8	result = 0;

	fpgaPgStatus = 0;

	FPGA_WriteSingle(FPGA_CMD_PTG_ENABLE, 0x00000000);

	result = 1;

	return result;
}

u8 FpgaPG_ConfigPCLK_ICS525(u32 pixelClk)
{
	u8	result = 0;
	u32	rd, vd, od, odValue;

	if(pixelClk < 20000000)			return result;
	if(pixelClk > 250000000)		return result;

	rd		= 98;
	od		= 6;
	odValue	= 1;

	vd		= pixelClk / 1000000;
	vd		+= 1;
	vd		*= odValue;
	vd		-= 8;

	FPGA_WriteSingle(FPGA_CMD_PCLK_VD,	vd);
	FPGA_WriteSingle(FPGA_CMD_PCLK_R,	rd);
	FPGA_WriteSingle(FPGA_CMD_PCLK_S,	od);

	result = 1;

	return result;
}

u8 FpgaPG_ConfigPCLK_MMCM(u32 pixelClk)
{
	u8 result = 0;
	u32	regData, makeClk_MHz;
	u32	m = 0, f = 0, fEnable = 0, d0 = 0, d1 = 0;
	u32	sTime, eTime;
	double	baseClk, makeClk;
	double	multi, divide0, divide1;

	if(pixelClk < 20000000)			return result;
	if(pixelClk > 250000000)		return result;

	regData = FPGA_ReadSingle(FPGA_CMD_PCLK_BASE_CLK);
	if(regData != 60000000)			return result;

	baseClk = (double)regData;

	makeClk_MHz = pixelClk / 1000000;

	makeClk_MHz += 1;

	for(u8 cnt = 0; cnt < 33; cnt++)
	{
		if(makeClk_MHz == fpgaPgErrorCLK[cnt])
		{
			makeClk_MHz += 1;

			break;
		}
	}

	makeClk = (double)(makeClk_MHz * 1000000);

	if(makeClk_MHz < 50)
	{
		divide0 = 2;
		divide1 = 30;

		multi	= makeClk / (baseClk / (divide0 * divide1));
	}
	else if((makeClk_MHz % 5) == 0)
	{
		if(makeClk_MHz < 135)
		{
			divide0 = 1;
			divide1 = 12;
		}
		else
		{
			divide0 = 2;
			divide1 = 6;
		}

		multi	= makeClk / (baseClk / (divide0 * divide1));
	}
	else if((makeClk_MHz % 4) == 0)
	{
		if(makeClk_MHz < 124)
		{
			divide0 = 1;
			divide1 = 15;
		}
		else
		{
			divide0 = 3;
			divide1 = 5;
		}

		multi	= makeClk / (baseClk / (divide0 * divide1));
	}
	else if((makeClk_MHz % 3) == 0)
	{
		if(makeClk_MHz < 81)
		{
			divide0 = 1;
			divide1 = 20;
		}
		else if(makeClk_MHz < 162)
		{
			divide0 = 2;
			divide1 = 10;
		}
		else if(makeClk_MHz < 189)
		{
			divide0 = 4;
			divide1 = 5;
		}
		else
		{
			divide0 = 2;
			divide1 = 5;
		}

		multi	= makeClk / (baseClk / (divide0 * divide1));
	}
	else
	{
		if(makeClk_MHz < 67)
		{
			divide0 = 2;
			divide1 = 15;
		}
		if(makeClk_MHz < 134)
		{
			divide0 = 3;
			divide1 = 10;
		}
		else
		{
			divide0 = 3;
			divide1 = 5;
		}

		multi	= makeClk / (baseClk / (divide0 * divide1));
	}

	m = (u32)multi;

	if((multi - m) != 0)
	{
		f = 500;
		fEnable = 1;
	}

	d0 = (u32)divide0;
	d1 = (u32)divide1;

	FPGA_WriteSingle(FPGA_CMD_PCLK_MULTI,				m);
	FPGA_WriteSingle(FPGA_CMD_PCLK_MULTI_FRAC_ENABLE,	fEnable);
	FPGA_WriteSingle(FPGA_CMD_PCLK_MULTI_FRAC,			f);
	FPGA_WriteSingle(FPGA_CMD_PCLK_DIVIDE_0,			d0);
	FPGA_WriteSingle(FPGA_CMD_PCLK_DIVIDE_1,			d1);

	FPGA_WriteSingle(FPGA_CMD_PCLK_ENABLE,				1);

	sTime = HAL_GetTick();

	do{
		regData = FPGA_ReadSingle(FPGA_CMD_PCLK_ENABLE);
		if(regData & 0x00000001)		break;

		eTime = HAL_GetTick();
		if((eTime - sTime) > 1000)		break;
	}while(1);

	result = 1;

	return result;
}


u8 FpgaPG_ConfigPixelClock(u32 pixelClk)
{
	u8	result = 0;
	u32	regData;

	if(pixelClk > 250000000)			return result;

	regData = FPGA_ReadSingle(FPGA_CMD_ID);

	if((regData & 0x00F00000) == 0x00100000)
	{
		result = FpgaPG_ConfigPCLK_MMCM(pixelClk);
	}
	else
	{
		result = FpgaPG_ConfigPCLK_ICS525(pixelClk);
	}

	return result;
}

u8 FpgaPG_ConfigVideo(void *pVoid)
{
	u8	result = 0, rtn;
	u32	pixelClk, refleshDelay;
	fpgaPgVideo_t	*pData;

	pData = (fpgaPgVideo_t*)pVoid;

	if(pData->txMode != 1)
	{
		if((pData->hPixel % 2) == 0)	pData->txMode = 2;		//	1 CLK 2 PIXEL
		else							pData->txMode = 1;		//	1 CLK 1 PIXEL
	}

	if(pData->forceInit != 1)
	{
		if(FpgaPG_GetStatus() != 0)
		{
			if((fpgaPgVideo.hPixel == pData->hPixel) && (fpgaPgVideo.vLine == pData->vLine) && (fpgaPgVideo.txMode == pData->txMode))
			{
				result = 1;

				return result;
			}
		}
	}

	rtn = FpgaPG_ClearSatatus();
	if(rtn == 0)						return result;

	if(pData->pgOnly == 0)
	{
		if(pData->framerate > 30)
		{
			pData->framerate /= 2;

			if(pData->framerate < 30)	pData->framerate = 30;
		}
	}

	if(pData->forceActive == 0)
	{
		if(pData->hfp	== 0)				pData->hfp		= 4;
		if(pData->hbp	== 0)				pData->hbp		= 4;
		if(pData->hSync	== 0)				pData->hSync	= 4;
		if(pData->vfp	== 0)				pData->vfp		= 2;
		if(pData->vbp	== 0)				pData->vbp		= 2;
		if(pData->vSync	== 0)				pData->vSync	= 2;
	}

	if(pData->txMode == 2)
	{
		pixelClk = ((pData->hfp + pData->hPixel + pData->hbp + pData->hSync) / 2) * (pData->vfp + pData->vLine + pData->vbp + pData->vSync) * pData->framerate;

		FPGA_WriteSingle(FPGA_CMD_SPLIT_MODE_ENABLE, 0x00000001);
	}
	else
	{
		pixelClk = (pData->hfp + pData->hPixel + pData->hbp + pData->hSync) * (pData->vfp + pData->vLine + pData->vbp + pData->vSync) * pData->framerate;

		FPGA_WriteSingle(FPGA_CMD_SPLIT_MODE_ENABLE, 0x00000000);
	}

	rtn = FpgaPG_ConfigPixelClock(pixelClk);
	if(rtn == 0)						return result;

	FPGA_WriteSingle(FPGA_CMD_CH0_VLINE,	pData->vLine);
	FPGA_WriteSingle(FPGA_CMD_CH0_VSYNC,	pData->vSync);
	FPGA_WriteSingle(FPGA_CMD_CH0_VBP,		pData->vbp);
	FPGA_WriteSingle(FPGA_CMD_CH0_VFP,		pData->vfp);
	FPGA_WriteSingle(FPGA_CMD_CH0_HPIXEL,	pData->hPixel);
	FPGA_WriteSingle(FPGA_CMD_CH0_HSYNC,	pData->hSync);
	FPGA_WriteSingle(FPGA_CMD_CH0_HBP,		pData->hbp);
	FPGA_WriteSingle(FPGA_CMD_CH0_HFP,		pData->hfp);

	FPGA_WriteSingle(FPGA_CMD_CH1_VLINE,	pData->vLine);
	FPGA_WriteSingle(FPGA_CMD_CH1_VSYNC,	pData->vSync);
	FPGA_WriteSingle(FPGA_CMD_CH1_VBP,		pData->vbp);
	FPGA_WriteSingle(FPGA_CMD_CH1_VFP,		pData->vfp);
	FPGA_WriteSingle(FPGA_CMD_CH1_HPIXEL,	pData->hPixel);
	FPGA_WriteSingle(FPGA_CMD_CH1_HSYNC,	pData->hSync);
	FPGA_WriteSingle(FPGA_CMD_CH1_HBP,		pData->hbp);
	FPGA_WriteSingle(FPGA_CMD_CH1_HFP,		pData->hfp);

	FPGA_WriteSingle(FPGA_CMD_PTG_ENABLE,	0x00000003);

	rtn = FpgaPG_SetStatus(1);
	if(rtn != 0)
	{
		memcpy(&fpgaPgVideo, pData, sizeof(fpgaPgVideo_t));

		result = 1;
	}

	if(result)
	{
		refleshDelay = 1000 / pData->framerate;
		if(1000 % pData->framerate)		refleshDelay += 1;

		rtn = IMG_PreloadListSetCurrentResolution(pData->hPixel, pData->vLine);
		if(rtn == 0)					result = 0;

		rtn = IMG_SetRefleshDelay(refleshDelay);
		if(rtn == 0)					result = 0;
	}

	return result;
}

u8 FpgaPG_Reset()
{
	u8	result = 0;

	FPGA_WriteSingle(FPGA_CMD_RESET, 0);

	HAL_Delay(5);

	FPGA_WriteSingle(FPGA_CMD_RESET, 1);

	HAL_Delay(100);

	FpgaPG_ClearSatatus();

	result = 1;

	return result;
}

void FpgaCore_Reset()
{

	FPGA_WriteSingle(FPGA_CMD_CORE_NRESET, 0);

	HAL_Delay(10);

	FPGA_WriteSingle(FPGA_CMD_CORE_NRESET, 1);

	HAL_Delay(10);

}

u8 FpgaPG_RemoteUpdateControl(u8 data)
{
	u8	result = 0;

	FPGA_WriteSingle(FPGA_CMD_REMOTE_UPDATE_CTRL, 1);

	result = 1;

	return result;
}

u8 FpgaPG_RemoteUpdateStatus(u8 mode)
{
	u8	result = 0, bitShift = 0;
	u32 regData, mask;

	switch(mode)
	{
		case UPDATE_DONE_STATUS:
		mask = 0x00000002;
		bitShift = 1;
		break;

		case UPDATE_STATUS:
		mask = 0x00000004;
		bitShift = 2;
		break;

		case UPDATE_ERROR_STATUS:
		mask = 0x00000080;
		bitShift = 7;
		break;

		default:
		break;
	}

	regData = FPGA_ReadSingle(FPGA_CMD_REMOTE_UPDATE_CTRL);
	regData &= mask;
	regData = regData >> bitShift;

	result = (u8)regData;

	return result;
}