#ifndef	_APP_IMAGE_H
#define	_APP_IMAGE_H

#include	"E7602_PG_Application.h"

#define	IMAGE_LIST_MAX_COUNT		200

enum{
	IMG_ARGB8888 = 0,
	IMG_RGB101010
};

enum{
	IMG_COLOR_TYPE_RGB_8B = 0,
	IMG_COLOR_TYPE_RGB_10B,
	IMG_COLOR_TYPE_YCBCR_8B_TYPE1,
};

typedef	struct{
	u32 mode;
	u32 colorData;
}imgColor_t;

typedef	union{
	u32 colorData;

	struct{
		u32 blue	:8;
		u32	green	:8;
		u32	red		:8;
		u32	dc		:8;
	}rgb8;

	struct{
		u32	blue	:10;
		u32	green	:10;
		u32	red		:10;
		u32	dc		:2;
	}rgb10;

	struct{
		u32	y		:8;
		u32	cb		:8;
		u32	cr		:8;
		u32	dc		:8;
	}ycbcr8;
}imgColorData_t;

typedef	union{
	u32	data[4];

	struct{
		//	data[0]
		u32	cr0			:8;
		u32	y0			:8;
		u32	cb0			:8;
		u32	dc0			:8;
		//	data[1]
		u32	y2			:8;
		u32	cb2			:8;
		u32	y1			:8;
		u32	dc1			:8;
		//	data[2]
		u32	cb4			:8;
		u32	y3			:8;
		u32	cr2			:8;
		u32	dc2			:8;
		//	data[3]
		u32	y5			:8;
		u32	cr4			:8;
		u32	y4			:8;
		u32	dc3			:8;
	};
}imgYCbCr422_96_t;

typedef	struct{
	u32	mode;
	double	y_R;
	double	y_G;
	double	y_B;
	double	cb_R;
	double	cb_G;
	double	cb_B;
	double	cr_R;
	double	cr_G;
	double	cr_B;

	double	y_Offset;
	double	cb_Offset;
	double	cr_Offset;
}imgYCbCrColor_t;

typedef	struct{
	u32	hPixel;
	u32	vLine;
	u32	colorDepth;
}imgLcmInfo_t;

typedef	struct{
	u16 indexNo;
	u16	status;
	u32	size;
	u32	startAdrs;
}imgInfo_t;

typedef	struct{
	u32			maxCount;
	u32			hSize;
	u32			vSize;
	u32			lcmSize;
	u32			realSize;
	imgInfo_t	image[IMAGE_LIST_MAX_COUNT];
}imgListFpga_t;

typedef	struct{
	u32	status;
	u32	indexNo;
	u32	hSize;
	u32	vSize;
	u32	imgSize;
	u32	startAdrs;
	u8	path[288];
}imgPreloadInfo_t;

typedef	struct{
	u32					status;
	u32					memorySize;
	u32					hSize_Max;
	u32					vSize_Max;
	u32					slotSize;
	u32					maxCount;
	u32					hSize_Current;
	u32					vSize_Current;
	imgPreloadInfo_t	list[IMAGE_LIST_MAX_COUNT];
}imgPreloadList_t;

typedef	struct{
	u32	enable;
	u32	duration;
	u32	startTime;
	u32	startImg;
	u32	endImg;
	u32	repeatCnt;
}imgRepeat_t;

typedef	struct{
	u32	mode;
	u32	gMode;
	u32	color;
	u32	hPixel;
	u32	vLine;
	u32	boxSize;
	u32	lineColor;
	u32	lineSize;
	u32	horizontal;
	u32	colorCnt;
	u32	*pColor;
#if 1 // yeom_20200916 [start]
	u32	apl_width;
	u32	apl_height;
#endif // yeom_20200916 [end]
}imgPattern_t;

enum{
	IMG_PATTERN_MODE_FILL		= 0,
	IMG_PATTERN_MODE_GRADATION_V,
	IMG_PATTERN_MODE_GRADATION_H,
	IMG_PATTERN_MODE_GRADATION_D,
	IMG_PATTERN_MODE_CHECKBOX,
	IMG_PATTERN_MODE_EDGE,
	IMG_PATTERN_MODE_COLORBAR,
	IMG_PATTERN_MODE_PIXEL_TEST,
	IMG_PATTERN_MODE_LINE_CONFIRM,
	IMG_PATTERN_MODE_APL,
	IMG_PATTERN_MODE_APLETC,
	IMG_PATTERN_MODE_GRADATION_HH,
	IMG_PATTERN_MODE_HLL,
	IMG_PATTERN_MODE_VLL,
};

typedef	union{
	u32	rawData;

	struct{
		u32	blue	:10;
		u32	green	:10;
		u32	red		:10;
		u32	dummy	:2;// ddrrrrrrrrrrggggggggggbbbbbbbbbb
	};
}imgPixelColor_t;

typedef	struct{
	u32	imgAdrs;
	u16	h;
	u16	v;
	u16	pv;
	u16	mag;
	u32	color;
	u16	font;
}imgStringInfo_t;

typedef	struct{
	u8	**pString;
	u32	strCnt;
	u32	bgColor;
	u32	firstSize;
	u32	otherSize;
	u32	firstColor;
	u32	otherColor;
}imgString_t;

typedef	struct{
	u32	frameMode;
	u32	currentDisplayNo;
	u32	refleshDelay;
	u32	refleshEnable;
	u32	delayDouble;
	u32	vhReverse;
	u32	vReverse;
	u32	hReverse;
	u32	ycbcrEnable;
	u32	ycbcrRound;
}imageDisplayInfo_t;

enum{
	IMAGE_DISPLAY_FRAME_MODE_DEFAULT		= 0,
	IMAGE_DISPLAY_FRAME_MODE_CUSTOM,
	IMAGE_DISPLAY_FRAME_MODE_END,
};

u8 IMG_PreloadListDataInit();
u8 IMG_PreloadListInit();
u8 IMG_PreloadListSetMemorySize(u32 memorySize);
u8 IMG_PreloadListGetMaxResolution(u32 *phSize, u32 *pvSize);
u8 IMG_PreloadListSetMaxResolution(u32 hSize, u32 vSize);
u8 IMG_PreloadListGetCurrentResolution(u32 *phSize, u32 *pvSize);
u8 IMG_PreloadListSetCurrentResolution(u32 hSize, u32 vSize);
u8 IMG_PreloadListGetSlotAdrs(u32 slotNo, u32 *pAdrs);
u8 IMG_PreloadListGetMaxCount(u32 *pMaxCount);
u8 IMG_PreloadListGetSlotStatus(u32 slotNo, u8 *pStatus);
u8 IMG_PreloadListCheckImage(u32 *pCheckCnt);
u8 IMG_PreloadListGetSlotFileName(u32 slotNo, u8 *pFileName);
u8 IMG_PreloadListSetSlotFileName(u32 slotNo, u8 *pFileName);
u8 IMG_PreloadListGetSlotImageSize(u32 slotNo, u32 *phSize, u32 *pvSize);
u8 IMG_PreloadListSetSlotImageSize(u32 slotNo, u32 hSize, u32 vSize);
u8 IMG_PreloadListCheckSlotImageSize(u32 slotNo, u8 *pStatus);
u8 IMG_PreloadListDeleteSlot(u8 slotNo);
u8 IMG_PreloadListClearSlot();
u8 IMG_VLineReverse(u32 mode);
u8 IMG_LoadBmpFile(u8 *pPath);
u8 IMG_LoadJpgFile(u8 *pPath);
u8 IMG_LoadDscFile(u32 vReverse, u8 *pPath);
u8 IMG_ReverseEnable();
u8 IMG_ReverseDisable();
u8 IMG_YCbCrEnable();
u8 IMG_YCbCrDisable();
u8 IMG_ImageReverse3Byte(u8 *pData, u32 vSize, u32 hSize);
u8 IMG_ImageReverse4Byte(u32 *pData, u32 vSize, u32 hSize);
u8 IMG_ImageSend_24b4Byte();
u8 IMG_ImageSend_32b4Byte(u8 mode);
u8 IMG_YCbCrConversionData(u8 mode);
u8 IMG_YCbCrConversionDataInit();
u32	IMG_RGB24b2YCbCr24b(u32 src);
u32 IMG_RGB30b2YCbCr24b(u32 src);
u8 IMG_ImageSend_24b4Byte_YCbCr422();
u8 IMG_ImageSend_32b4Byte_YCbCr422(u8 mode);
u8 IMG_Preload(u16 indexNo, u16 slotNo);
u8 IMG_PatternFill(u32 hPixel, u32 vLine, u32 color);
u8 IMG_PatternGradationVertical(u32 hPixel, u32 vLine, u32 mode);
u8 IMG_PatternGradationHorizontal(u32 hPixel, u32 vLine, u32 mode);
u8 IMG_PatternGradationVerticalMax(u32 hPixel, u32 vLine, u32 mode);
u8 IMG_PatternGradationHorizontalMax(u32 hPixel, u32 vLine, u32 mode);
u8 IMG_PatternGradationDiagonal(u32 hPixel, u32 vLine, u32 mode);
u8 IMG_PatternCheckBox(u32 hPixel, u32 vLine, u32 boxPixel);
u8	IMG_PatternEdge(u32	hPixel, u32	vLine, u32 bgColor, u32	lineColor, u32 lineSize);
u32 IMG_ColorSelectForColorBar(u32 total, u32 pixelCnt, u8 colorCnt, u32 *pColor);
u8 IMG_PatternColorBar(u32	hPixel, u32	vLine, u32 horizontal, u32 colorCnt, u32 *pColor);
u8 IMG_PatternCoreTest(u32 hPixel, u32 vLine, u8 mode, u32 errBit);
u8 IMG_PatternPixelTest(u32 hPixel, u32 vLine, u8 mode, u32 color);
u8 IMG_PreloadLineConfirmPattern(u32 hPixel, u32 vLine);
u8 IMG_PreloadTestPattern(void *pVoid, u16 slotNo);
u8 IMG_PreloadFile(u16 slotNo, u8 *pPath);

u8 IMG_PatternApl(u32 hPixel, u32 vLine, u32 mode);
u8 IMG_PatternAplEtc(u32 hPixel, u32 vLine, u32 mode, u32 width, u32 height);

u8 IMG_PatternGradationHorizontalH(u32 hPixel, u32 vLine);
u8 IMG_PatternVll(u32	hPixel, u32	vLine);
u8 IMG_PatternHll(u32	hPixel, u32	vLine);

u8 IMG_DisplayRefleshAll();
u8 IMG_DisplayRefleshSelect(u8 channel);
u8 IMG_SetFrameMode(u32 frameMode);
u8 IMG_GetFrameMode(u32 *pFrameMode);
u8 IMG_SetRefleshDelay(u32 delay);
u8 IMG_GetRefleshDelay(u32 *pDelay);
u8 IMG_SetRefleshEnable(u8 enable);
u8 IMG_SetRefleshDelayDouble(u8 mode);
u8 IMG_Display(u32 slotNo);
u8 IMG_SetFrameBank(u8 channel, u16 bankNo);
u8 IMG_DisplayCustom(u8 channel, u32 slotNo);
u8 IMG_DisplayPrev();
u8 IMG_DisplayNext();
u8 IMG_RepeatEnable(u32 duration);
u8 IMG_RepeatEnableSelect(u32 duration, u32 sImg, u32 eImg);
u8 IMG_RepeatDisable();
u8 IMG_DisplayNext_ForRepeat();
u8 IMG_RepeatProcess();
u8 IMG_WriteChar(u32 adrs, u16 h, u16 v, u16 ph, u16 pv, u8 mag, u32 color, u8 font, u8 data);
u8 IMG_WriteString(u32 adrs, u16 h, u16 v, u16 ph, u16 pv, u8 mag, u32 color, u8 font, u8 *pData, u16 length);
u8 IMG_StringPrintf(void *pVoid, const char *pData, ...);
u8 IMG_PreloadString(u16 slotNo, void *pVoid);
u8 DBVConfig(u8 mode, u8 type, u32 value);
u8 DBVSet(u8 slot, u32 value);
u8 DBVClear();
void rolling_start(u32 slot);
void sdp_rolling(u16 sdp_value);
void aux_rolling(u16 aux_value);

u8 SetTriggerMode(u8 channel, u8 state);
u8 TriggerStart(u8 channel);

u8 DisplayTimeoutInit(u8 channel, u32 time);
u8 DisplayTimerSet(u8 channel, u8 mode);
void DisplayTimerFunction();

#endif	// _APP_IMAGE_H
