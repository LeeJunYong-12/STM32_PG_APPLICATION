#include	"App_VideoInput.h"

static u8 parameterSet;
videoInput_t videoInput;

u8 VI_ResetCtrl(u32 data)
{
	u8	result = 0;

	if(data > 1)			return result;

	FPGA_WriteSingle(FPGA_CMD_EDP_RECEIVER_NRESET, data);

	result = 1;

	return result;
}

u8 VI_Init()
{
	u8	result = 0;

	parameterSet = 0;

	memset(&videoInput, NULL, sizeof(videoInput));

	result = 1;

	return result;
}

u8 VI_ParameterSet(u8 mode, u16 hPixel, u16 vLines)
{
	u8	result = 0;

	videoInput.type = mode;

	videoInput.hPixel = hPixel;

	videoInput.vLines = vLines;

	parameterSet = 1;

	result = 1;

	return result;
}

u8 VI_LvdsRxDataMap(u32 data)
{
	u8	result = 0;

	if(data > 2)	return result;

	FPGA_WriteSingle(FPGA_CMD_LVDS_RX_DATA_MAP, data);

	result = 1;

	return result;
}

u8 VI_LvdsInputSyncPolarity(u32 vPol, u32 hPol)
{
	u8	result = 0;
	u32 data;

	if(vPol > 1)	return result;
	if(hPol > 1)	return result;

	data = 0;
	data |= (vPol << 1);
	data |= hPol;

	FPGA_WriteSingle(FPGA_CMD_LVDS_RX_INPUT_SYNC_POL, data);

	result = 1;

	return result;
}

u8 VI_StreamInputSkip(u32 data)
{
	u8	result = 0;

	if(data >= 60)			return result;

	FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_SKIP, data);

	result = 1;

	return result;
}

u8 VI_ModuleReset()
{
	u8	result = 0;

	VI_ResetCtrl(LOW);

	HAL_Delay(10);

	VI_ResetCtrl(HIGH);

	HAL_Delay(10);

	result = 1;

	return result;
}

u8 VI_Enable()
{
	u8	result = 0, rtn;
	u32	imageNo, maxCount;

	if(parameterSet == 0)		return result;

	rtn = IMG_PreloadListGetMaxCount(&maxCount);

	imageNo = maxCount - 1;

	rtn = IMG_PreloadListGetSlotAdrs(imageNo, &videoInput.memAdrs);
	if(rtn == 0)				return result;

	FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_ST_ADDR,			videoInput.memAdrs);

	FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_WIDTH,		videoInput.hPixel);
	FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_HEIGHT,	videoInput.vLines);

	
	switch(videoInput.type)
	{
		case VIDEOINPUT_DVI_SINGLE:
			FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_SEL,		0);
			FPGA_WriteSingle(FPGA_CMD_VIDEO_DVI_DUAL_LINK,			0);
			break;

		case VIDEOINPUT_DVI_DUAL:
			FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_SEL,		0);
			FPGA_WriteSingle(FPGA_CMD_VIDEO_DVI_DUAL_LINK,			1);
			break;

		case VIDEOINPUT_DP:
		case VIDEOINPUT_HDMI:
			FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_SEL,		1);
			FPGA_WriteSingle(FPGA_CMD_VIDEO_DVI_DUAL_LINK,			0);
			break;
	}
	
	FPGA_WriteSingle(FPGA_CMD_VIDEO_INPUT_MODE,				1);

	result = 1;

	return result;	
}

u8 VI_Disable()
{
	u8	result = 0, rtn;
	u32	imageNo, maxCount;

	VI_Init();

	rtn = IMG_PreloadListGetMaxCount(&maxCount);

	imageNo = maxCount - 1;

	rtn = IMG_PreloadListGetSlotAdrs(imageNo, &videoInput.memAdrs);
	if(rtn == 0)				return result;

	FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_ST_ADDR,			videoInput.memAdrs);
	FPGA_WriteSingle(FPGA_CMD_VIDEO_INPUT_MODE,				0);
	FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_SEL,		0);
	FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_WIDTH,		0);
	FPGA_WriteSingle(FPGA_CMD_VIDEO_STREAM_INPUT_HEIGHT,	0);
	FPGA_WriteSingle(FPGA_CMD_VIDEO_DVI_DUAL_LINK,			0);

	result = 1;

	return result;
}

u8 VI_Display()
{
	u8	result = 0;

	FPGA_WriteSingle(FPGA_CMD_DDR_READ_ADRS, videoInput.memAdrs);

	FPGA_WriteSingle(FPGA_CMD_DDR_READ_START, 0);

	result = 1;

	return result;
}
