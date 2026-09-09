#include	"App_VideoStream.h"

static	videoStreamHandle_t	videoStreamHandle;

u8 VideoStream_Reset()
{
	u8	result = 0;

	memset(&videoStreamHandle, NULL, sizeof(videoStreamHandle));

	FPGA_Reset();

	videoStreamHandle.initialized	= 0x0E760200;

	result = 1;

	return result;
}

u8 VideoStream_GetStatus()
{
	return (u8)videoStreamHandle.status;
}

u8 VideoStream_CheckResolution(u32 hPixel, u32 vLine)
{
	u8	result = 0;

	if(videoStreamHandle.status == 0)				return result;

	if(videoStreamHandle.info.hPixel != hPixel)		return result;
	if(videoStreamHandle.info.vLine != vLine)		return result;

	result = 1;

	return result;
}

u8 VideoStream_ClockConfig(u32 clk)
{
	u8	result = 0;
	u32	rd, vd, od, odValue;

	if(clk < 20000000)			return result;
	if(clk > 250000000)		return result;

	rd		= 98;
	od		= 6;
	odValue	= 1;

	vd		= clk / 1000000;
	vd		+= 1;
	vd		*= odValue;
	vd		-= 8;

	FPGA_WriteSingle(FPGA_CMD_PCLK_VD,	vd);
	FPGA_WriteSingle(FPGA_CMD_PCLK_R,	rd);
	FPGA_WriteSingle(FPGA_CMD_PCLK_S,	od);

	result = 1;

	return result;
}

u8 VideoStream_Output(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	frameTime;
	videoStreamInfo_t	*pData;

	pData = (videoStreamInfo_t*)pVoid;

	FPGA_WriteSingle(FPGA_CMD_PTG_ENABLE,	0x00000000);

	pData->pixelClk = (pData->hPixel + pData->hbp + pData->hfp + pData->hSync) * (pData->vLine + pData->vbp + pData->vfp + pData->vSync) * pData->framerate;
	
	if(pData->framerate == 0)
	{
		pData->pixelClk = 150000000;	// pclk = 150MHz (MAX)
		pData->framerate = pData->pixelClk / ((pData->hPixel + pData->hbp + pData->hfp + pData->hSync) * (pData->vLine + pData->vbp + pData->vfp + pData->vSync));
	}	

	if(pData->hPixel % 2)				rtn = VideoStream_ClockConfig(pData->pixelClk);				// single mode(1 pixel transmit)
	else								rtn = VideoStream_ClockConfig(pData->pixelClk / 2);			// dual mode(2 pixel transmit)
	if(rtn == 0)						errCnt += 1;

	if(pData->hPixel % 2)				FPGA_WriteSingle(FPGA_CMD_SPLIT_MODE_ENABLE, 0x00000000);	// single mode
	else								FPGA_WriteSingle(FPGA_CMD_SPLIT_MODE_ENABLE, 0x00000001);	// dual mode

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

	rtn = IMG_PreloadListSetCurrentResolution(pData->hPixel, pData->vLine);
	if(rtn == 0)						errCnt += 1;

	frameTime = 1000 / pData->framerate;
	if(1000 % pData->framerate)			frameTime += 1;

	rtn = IMG_SetRefleshDelay(frameTime);
	if(rtn == 0)						errCnt += 1;

	memcpy(&videoStreamHandle.info, pVoid, sizeof(videoDisplayInfo_t));

	if(errCnt == 0)
	{
		videoStreamHandle.status	= 1;

		FPGA_WriteSingle(FPGA_CMD_PTG_ENABLE,	0x00000003);

		result = 1;
	}

	return result;
}


