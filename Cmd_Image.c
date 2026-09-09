#include	"Cmd_Image.h"

static	u8	imgName[256];

static u8 Cmd_ImageGetSlot(void *pVoid)
{
	u8	result = 0, rtn;
	u32	slotNo, maxCount;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)					return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);

	if(slotNo == 0)							return result;

	slotNo -= 1;

	rtn = IMG_PreloadListGetMaxCount(&maxCount);
	if(rtn == 0)							return result;

	if(slotNo >= maxCount)					return result;

	memset(imgName, NULL, sizeof(imgName));

	rtn = IMG_PreloadListGetSlotFileName(slotNo, imgName);

	if(rtn)	CMD_Printf(";%s", imgName);
	else
	{
		CMD_Printf(";NO_IMAGE");
		return result;
	}

	result = 1;

	return result;
}

static u8 Cmd_ImageMaxCount(void *pVoid)
{
	u8	result = 0;
	u32	maxCount;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = IMG_PreloadListGetMaxCount(&maxCount);

	if(result)
	{
		CMD_Printf(";%d", maxCount);
	}

	return result;
}

static u8 Cmd_ImagePreLoad(void *pVoid)
{
	u8	result = 0, rtn;
	u8 indexNo, slotNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	indexNo	= CMD_StrToUL(pData->pArgv[0]);
	slotNo	= CMD_StrToUL(pData->pArgv[1]);

	indexNo -= 1;
	slotNo -= 1;

	rtn = IMG_Preload(indexNo, slotNo);
	if(rtn == 0)	return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadFill(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u16 red, green, blue;
	u32	hSize, vSize;
	imgPattern_t pattern;
	imgPixelColor_t color;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	red		= CMD_StrToUL(pData->pArgv[1]);
	if(red > 0xFF)				return result;
	green	= CMD_StrToUL(pData->pArgv[2]);
	if(green > 0xFF)			return result;
	blue	= CMD_StrToUL(pData->pArgv[3]);
	if(blue > 0xFF)				return result;

	color.rawData = 0;

	color.red	= red	& 0x00ff;
	color.green	= green	& 0x00ff;
	color.blue	= blue	& 0x00ff;

	color.red	<<= 2;
	color.green	<<= 2;
	color.blue	<<= 2;

	pattern.color	= color.rawData;
	pattern.mode	= IMG_PATTERN_MODE_FILL;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadFill10(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u16 red, green, blue;
	u32	hSize, vSize;
	imgPattern_t pattern;
	imgPixelColor_t color;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	red		= CMD_StrToUL(pData->pArgv[1]);
	if(red > 0x3FF)				return result;
	green	= CMD_StrToUL(pData->pArgv[2]);
	if(green > 0x3FF)			return result;
	blue	= CMD_StrToUL(pData->pArgv[3]);
	if(blue > 0x3FF)			return result;

	color.rawData = 0;

	color.red	= red	& 0x03ff;
	color.green	= green	& 0x03ff;
	color.blue	= blue	& 0x03ff;

	pattern.color	= color.rawData;
	pattern.mode	= IMG_PATTERN_MODE_FILL;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadGradation(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	if(CMD_Compare(pData->pArgv[1], "Horizontal"))
	{
		pattern.mode = IMG_PATTERN_MODE_GRADATION_H;
	}
	else if(CMD_Compare(pData->pArgv[1], "Vertical"))
	{
		pattern.mode = IMG_PATTERN_MODE_GRADATION_V;
	}
	else if(CMD_Compare(pData->pArgv[1], "Diagonal"))
	{
		pattern.mode = IMG_PATTERN_MODE_GRADATION_D;
	}
	else							return result;

	if(CMD_Compare(pData->pArgv[2], "white"))
	{
		pattern.gMode = 0;
	}
	else if(CMD_Compare(pData->pArgv[2], "red"))
	{
		pattern.gMode = 1;
	}
	else if(CMD_Compare(pData->pArgv[2], "green"))
	{
		pattern.gMode = 2;
	}
	else if(CMD_Compare(pData->pArgv[2], "blue"))
	{
		pattern.gMode = 3;
	}
	else							return result;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)					return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadCheckBox(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	boxSize;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	boxSize = CMD_StrToUL(pData->pArgv[1]);

	if(boxSize < 1)				return result;
	if(boxSize > 1024)			return result;

	pattern.mode	= IMG_PATTERN_MODE_CHECKBOX;
	pattern.boxSize	= boxSize;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadEdge(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	bgColor, lineColor, lineSize;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)				return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	rtn = CMD_ColorConverter(pData->pArgv[1], &bgColor);

	if(rtn == 0)						return result;

	rtn = CMD_ColorConverter(pData->pArgv[2], &lineColor);

	if(rtn == 0)						return result;

	lineSize = CMD_StrToUL(pData->pArgv[3]);

	pattern.mode	= IMG_PATTERN_MODE_EDGE;

	pattern.color		= bgColor;
	pattern.lineColor	= lineColor;
	pattern.lineSize	= lineSize;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)						return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadColorBar(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	colorList[15];
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc  < 4)								return result;
	if(pData->argc  > 17)								return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	memset(&pattern, NULL, sizeof(pattern));

	pattern.mode = IMG_PATTERN_MODE_COLORBAR;

	pattern.colorCnt = (pData->argc - 2);

	if(CMD_Compare(pData->pArgv[1], "horizontal"))		pattern.horizontal = 1;
	else if(CMD_Compare(pData->pArgv[1], "vertical"))	pattern.horizontal = 0;
	else												return result;

	for(u32 cnt = 0; cnt < pattern.colorCnt; cnt++)
	{
		rtn = CMD_ColorConverter(pData->pArgv[cnt + 2], &colorList[cnt]);

		if(rtn == 0)									return result;
	}

	pattern.pColor = colorList;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)										return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadCoreTest(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if((pData->argc != 1) || (pData->argc != 3))	return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	memset(&pattern, NULL, sizeof(pattern));

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)									return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	if(pData->argc == 1)
	{
		pattern.mode	= 0xffffffff;

		pattern.gMode	= 0;
	}
	else if(pData->argc == 3)
	{
		pattern.mode = 0xffffffff;

		if(CMD_Compare(pData->pArgv[1], "and"))
		{
			pattern.gMode	= 1;
		}
		else if(CMD_Compare(pData->pArgv[1], "or"))
		{
			pattern.gMode	= 2;
		}
		else										return result;

		pattern.color		= CMD_StrToUL(pData->pArgv[2]);
	}

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)									return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadPixelTest(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	memset(&pattern, NULL, sizeof(pattern));

	pattern.mode	= IMG_PATTERN_MODE_PIXEL_TEST;
	pattern.gMode	= CMD_StrToUL(pData->pArgv[1]);

	pattern.color	= CMD_StrToUL(pData->pArgv[2]);

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

static u8 Cmd_IamgePreLoadLConfirm(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	memset(&pattern, NULL, sizeof(pattern));

	pattern.mode	= IMG_PATTERN_MODE_LINE_CONFIRM;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadString(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo, *pString[5];
	imgString_t imgString;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 5)			return result;

	slotNo						= CMD_StrToUL(pData->pArgv[0]);

	rtn							= CMD_ColorConverter(pData->pArgv[1], &imgString.bgColor);
	if(rtn == 0)				return result;

	imgString.firstSize			= CMD_StrToUL(pData->pArgv[2]);

	rtn							= CMD_ColorConverter(pData->pArgv[3], &imgString.firstColor);
	if(rtn == 0)				return result;

	pString[0] = pData->pArgv[4];

	imgString.strCnt = 1;

	if(pData->argc >= 8)
	{
		imgString.otherSize		= CMD_StrToUL(pData->pArgv[5]);

		rtn						= CMD_ColorConverter(pData->pArgv[6], &imgString.otherColor);
		if(rtn == 0)			return result;

		pString[1] = pData->pArgv[7];

		imgString.strCnt += 1;
	}

	if(pData->argc >= 9)
	{
		pString[2] = pData->pArgv[8];

		imgString.strCnt += 1;
	}

	if(pData->argc >= 10)
	{
		pString[3] = pData->pArgv[9];

		imgString.strCnt += 1;
	}

	if(pData->argc >= 11)
	{
		pString[4] = pData->pArgv[10];

		imgString.strCnt += 1;
	}

	if(pData->argc > 12)		return result;

	if((pData->argc > 5) && (pData->argc < 6))
	{
		return result;
	}

	slotNo -= 1;

	imgString.pString = pString;

	rtn = IMG_PreloadString(slotNo, (void*)&imgString);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadFile(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u8	*filePath;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	filePath = pData->pArgv[1];

	slotNo -= 1;

	rtn = IMG_PreloadFile(slotNo, filePath);
	if(rtn == 0)	return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadClear(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = IMG_PreloadListClearSlot();

	return result;
}

#if 1 // yeom_20200828 [start]
static u8 Cmd_ImagePreLoadApl(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	if(CMD_Compare(pData->pArgv[1], "apl5"))
	{
		pattern.gMode = 0;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl10"))
	{
		pattern.gMode = 1;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl15"))
	{
		pattern.gMode = 2;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl20"))
	{
		pattern.gMode = 3;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl25"))
	{
		pattern.gMode = 4;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl30"))
	{
		pattern.gMode = 5;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl35"))
	{
		pattern.gMode = 6;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl40"))
	{
		pattern.gMode = 7;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl45"))
	{
		pattern.gMode = 8;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl50"))
	{
		pattern.gMode = 9;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl55"))
	{
		pattern.gMode = 10;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl60"))
	{
		pattern.gMode = 11;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl65"))
	{
		pattern.gMode = 12;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl70"))
	{
		pattern.gMode = 13;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl75"))
	{
		pattern.gMode = 14;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl80"))
	{
		pattern.gMode = 15;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl85"))
	{
		pattern.gMode = 16;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl90"))
	{
		pattern.gMode = 17;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl95"))
	{
		pattern.gMode = 18;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl100"))
	{
		pattern.gMode = 19;
	}
	else	return result;

	pattern.mode	= IMG_PATTERN_MODE_APL;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)					return result;

	result = 1;

	return result;
}
#endif // yeom_20200828 [end]

#if 1 // yeom_20200916 [start]
static u8 Cmd_ImagePreLoadAplEtc(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 4)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	if(CMD_Compare(pData->pArgv[1], "apl5"))
	{
		pattern.gMode = 0;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl10"))
	{
		pattern.gMode = 1;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl15"))
	{
		pattern.gMode = 2;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl20"))
	{
		pattern.gMode = 3;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl25"))
	{
		pattern.gMode = 4;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl30"))
	{
		pattern.gMode = 5;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl35"))
	{
		pattern.gMode = 6;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl40"))
	{
		pattern.gMode = 7;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl45"))
	{
		pattern.gMode = 8;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl50"))
	{
		pattern.gMode = 9;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl55"))
	{
		pattern.gMode = 10;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl60"))
	{
		pattern.gMode = 11;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl65"))
	{
		pattern.gMode = 12;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl70"))
	{
		pattern.gMode = 13;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl75"))
	{
		pattern.gMode = 14;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl80"))
	{
		pattern.gMode = 15;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl85"))
	{
		pattern.gMode = 16;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl90"))
	{
		pattern.gMode = 17;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl95"))
	{
		pattern.gMode = 18;
	}
	else if(CMD_Compare(pData->pArgv[1], "apl100"))
	{
		pattern.gMode = 19;
	}
	else	return result;

	pattern.apl_width = CMD_StrToUL(pData->pArgv[2]);
	pattern.apl_height = CMD_StrToUL(pData->pArgv[3]);

	pattern.mode	= IMG_PATTERN_MODE_APLETC;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)					return result;

	result = 1;

	return result;
}
#endif // yeom_20200916 [end]

static u8 Cmd_ImagePreLoadGradationH(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	pattern.mode	= IMG_PATTERN_MODE_GRADATION_HH;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)					return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadHll(void *pVoid)//width
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	pattern.mode	= IMG_PATTERN_MODE_HLL;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)					return result;

	result = 1;

	return result;
}

static u8 Cmd_ImagePreLoadVll(void *pVoid)//height
{
	u8	result = 0, rtn;
	u8	slotNo;
	u32	hSize, vSize;
	imgPattern_t pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	slotNo = CMD_StrToUL(pData->pArgv[0]);
	slotNo -= 1;

	pattern.mode	= IMG_PATTERN_MODE_VLL;

	rtn = IMG_PreloadListGetCurrentResolution(&hSize, &vSize);
	if(rtn == 0)				return result;

	pattern.hPixel	= hSize;
	pattern.vLine	= vSize;

	rtn = IMG_PreloadTestPattern((void*)&pattern, slotNo);
	if(rtn == 0)					return result;

	result = 1;

	return result;
}

#if 1 // yeom_20201005 [start]
static u8 Cmd_ImageDisplayApl(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32 mode;
	u32 width;
	u32 height;
	u32 value, value2;
	u32 w_start, h_start;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "apl5"))			mode = 0;
	else if(CMD_Compare(pData->pArgv[0], "apl10"))		mode = 1;
	else if(CMD_Compare(pData->pArgv[0], "apl15"))		mode = 2;
	else if(CMD_Compare(pData->pArgv[0], "apl20"))		mode = 3;
	else if(CMD_Compare(pData->pArgv[0], "apl25"))		mode = 4;
	else if(CMD_Compare(pData->pArgv[0], "apl30"))		mode = 5;
	else if(CMD_Compare(pData->pArgv[0], "apl35"))		mode = 6;
	else if(CMD_Compare(pData->pArgv[0], "apl40"))		mode = 7;
	else if(CMD_Compare(pData->pArgv[0], "apl45"))		mode = 8;
	else if(CMD_Compare(pData->pArgv[0], "apl50"))		mode = 9;
	else if(CMD_Compare(pData->pArgv[0], "apl55"))		mode = 10;
	else if(CMD_Compare(pData->pArgv[0], "apl60"))		mode = 11;
	else if(CMD_Compare(pData->pArgv[0], "apl65"))		mode = 12;
	else if(CMD_Compare(pData->pArgv[0], "apl70"))		mode = 13;
	else if(CMD_Compare(pData->pArgv[0], "apl75"))		mode = 14;
	else if(CMD_Compare(pData->pArgv[0], "apl80"))		mode = 15;
	else if(CMD_Compare(pData->pArgv[0], "apl85"))		mode = 16;
	else if(CMD_Compare(pData->pArgv[0], "apl90"))		mode = 17;
	else if(CMD_Compare(pData->pArgv[0], "apl95"))		mode = 18;
	else if(CMD_Compare(pData->pArgv[0], "apl100"))		mode = 19;
	else	return result;

	switch(mode)
	{
		case 0:		// apl5
			height = 380;
			width = 380;
			h_start = 1076;
			w_start = 395;
			break;
		case 1:		// apl10
			height = 537;
			width = 537;
			h_start = 997;
			w_start = 316;
			break;
		case 2:		// apl15
			height = 658;
			width = 658;
			h_start = 937;
			w_start = 256;
			break;
		case 3:		// apl20
			height = 760;
			width = 760;
			h_start = 886;
			w_start = 205;
			break;
		case 4:		// apl25
			height = 850;
			width = 850;
			h_start = 841;
			w_start = 160;
			break;
		case 5:		// apl30
			height = 742;
			width = 1170;
			h_start = 895;
			w_start = 1;
			break;
		case 6:		// apl35
			height = 816;
			width = 1170;
			h_start = 883;
			w_start = 1;
			break;
		case 7:		// apl40
			height = 990;
			width = 1170;
			h_start = 771;
			w_start = 1;
			break;
		case 8:		// apl45
			height = 1113;
			width = 1170;
			h_start = 709;
			w_start = 1;
			break;
		case 9:		// apl50
			height = 1237;
			width = 1170;
			h_start = 647;
			w_start = 1;
			break;
		case 10:		// apl55
			height = 1361;
			width = 1170;
			h_start = 585;
			w_start = 1;
			break;
		case 11:		// apl60
			height = 1485;
			width = 1170;
			h_start = 523;
			w_start = 1;
			break;
		case 12:		// apl65
			height = 1609;
			width = 1170;
			h_start = 461;
			w_start = 1;
			break;
		case 13:		// apl70
			height = 1733;
			width = 1170;
			h_start = 399;
			w_start = 1;
			break;
		case 14:		// apl75
			height = 1856;
			width = 1170;
			h_start = 338;
			w_start = 1;
			break;
		case 15:		// apl80
			height = 1980;
			width = 1170;
			h_start = 276;
			w_start = 1;
			break;
		case 16:		// apl85
			height = 2104;
			width = 1170;
			h_start = 214;
			w_start = 1;
			break;
		case 17:		// apl90
			height = 2228;
			width = 1170;
			h_start = 152;
			w_start = 1;
			break;
		case 18:		// apl95
			height = 2352;
			width = 1170;
			h_start = 90;
			w_start = 1;
			break;
		case 19:		// apl100
			height = 2796;
			width = 1290;
			h_start = 1;
			w_start = 1;
			break;
	}

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, 0x3fffffff);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, 0x3fffffff);
		if(rtn == 0)			errCnt += 1;

		value = width << 16 | height;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
		if(rtn == 0)			errCnt += 1;

		value2 = w_start << 16 | h_start;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_START, value2);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_START, value2);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, 0x3fffffff);
			if(rtn == 0)			errCnt += 1;

			value = width << 16 | height;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
			if(rtn == 0)			errCnt += 1;

			value2 = w_start << 16 | h_start;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_START, value2);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, 0x3fffffff);
			if(rtn == 0)			errCnt += 1;

			value = width << 16 | height;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
			if(rtn == 0)			errCnt += 1;

			value2 = w_start << 16 | h_start;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_START, value2);
			if(rtn == 0)			errCnt += 1;
		}
	}

	result = 1;

	return result;
}

static u8 Cmd_ImageDisplayAplEtc(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32 width;
	u32 height;
	u32 value, value2;
	u32	wSize, hSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	width	= CMD_StrToUL(pData->pArgv[0]);
	height	= CMD_StrToUL(pData->pArgv[1]);

	rtn = IMG_PreloadListGetCurrentResolution(&wSize, &hSize);
	if(rtn == 0)				return result;

	if(pData->option == NULL)
	{
		value = width << 16 | height;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
		if(rtn == 0)			errCnt += 1;

		value2 = (((wSize - width)/2) + 1) << 16 | (((hSize - height)/2) + 1);
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_START, value2);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_START, value2);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			value = width << 16 | height;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
			if(rtn == 0)			errCnt += 1;

			value2 = (((wSize - width)/2) + 1) << 16 | (((hSize - height)/2) + 1);
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_START, value2);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			value = width << 16 | height;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
			if(rtn == 0)			errCnt += 1;

			value2 = (((wSize - width)/2) + 1) << 16 | (((hSize - height)/2) + 1);
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_START, value2);
			if(rtn == 0)			errCnt += 1;
		}
	}

	return 1;
}

static u8 Cmd_ImageDisplayAplEtcColor(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	color;
	u16 red, green, blue;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	red		= CMD_StrToUL(pData->pArgv[0]);
	green	= CMD_StrToUL(pData->pArgv[1]);
	blue	= CMD_StrToUL(pData->pArgv[2]);

	color = red << 22 | green << 12 | blue << 2;

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, color);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, color);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, color);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, color);
			if(rtn == 0)			errCnt += 1;
		}
	}

	return 1;
}

static u8 Cmd_ImageDisplayAplEtcEnable(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x02);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_CONFIG, 0x1f);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_START, 0x01);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
		if(rtn == 0)			errCnt += 1;

		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x02);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_CONFIG, 0x1f);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_START, 0x01);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if(pData->option == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x02);
			if(rtn == 0)			errCnt += 1;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_CONFIG, 0x1f);
			if(rtn == 0)			errCnt += 1;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_START, 0x01);
			if(rtn == 0)			errCnt += 1;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
			if(rtn == 0)			errCnt += 1;
		}
		else if(pData->option == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x02);
			if(rtn == 0)			errCnt += 1;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_CONFIG, 0x1f);
			if(rtn == 0)			errCnt += 1;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_START, 0x01);
			if(rtn == 0)			errCnt += 1;
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
			if(rtn == 0)			errCnt += 1;
		}
	}

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayAplEtcDisable(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_START, 0x00);
		if(rtn == 0)			errCnt += 1;
		HAL_Delay(100);
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x00);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_START, 0x00);
		if(rtn == 0)			errCnt += 1;
		HAL_Delay(100);
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x00);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if(pData->option == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_START, 0x00);
			if(rtn == 0)			errCnt += 1;
			HAL_Delay(100);
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x00);
			if(rtn == 0)			errCnt += 1;
		}
		else
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_START, 0x00);
			if(rtn == 0)			errCnt += 1;
			HAL_Delay(100);
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x00);
			if(rtn == 0)			errCnt += 1;
		}
	}

	if(errCnt == 0)			result = 1;

	return 1;
}
#endif // yeom_20201005 [end]


static u8 Cmd_ImageReflesh(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = IMG_DisplayRefleshAll();

	return result;
}

static u8 Cmd_ImageDisplay(void *pVoid)
{
	u8	result = 0;
	u32	slotNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)							return result;

	slotNo	= CMD_StrToUL(pData->pArgv[0]);

	slotNo -= 1;

	result = IMG_Display(slotNo);

	return result;
}

static u8 Cmd_ImageDisplayFill(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	color;
	u16 red, green, blue;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)							return result;

	red		= CMD_StrToUL(pData->pArgv[0]);
	green	= CMD_StrToUL(pData->pArgv[1]);
	blue	= CMD_StrToUL(pData->pArgv[2]);

	color = red << 22 | green << 12 | blue << 2;

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_RGB, color);
		if(rtn == 0)			errCnt += 1;

		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_RGB, color);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_RGB, color);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_RGB, color);
			if(rtn == 0)			errCnt += 1;
		}
	}

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayFillEnable(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x02);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_CONFIG, 0x17);
	if(rtn == 0)			errCnt += 1;
	/*
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_MEMORY_ADDRESS, 0x1F);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_OUTPUT_MEMORY_ADDRESS, 0x1F);
	if(rtn == 0)			errCnt += 1;
	*/
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_START, 0x01);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
	if(rtn == 0)			errCnt += 1;

	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x02);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_CONFIG, 0x17);
	if(rtn == 0)			errCnt += 1;
	/*
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_MEMORY_ADDRESS, 0x1F);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_OUTPUT_MEMORY_ADDRESS, 0x1F);
	if(rtn == 0)			errCnt += 1;
	*/
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_START, 0x01);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayFillDisable(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	//rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x00);
	//if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_START, 0x00);
	if(rtn == 0)			errCnt += 1;
	HAL_Delay(100);
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x00);
	if(rtn == 0)			errCnt += 1;
	/*
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_MEMORY_ADDRESS, 0x00);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_OUTPUT_MEMORY_ADDRESS, 0x00);
	if(rtn == 0)			errCnt += 1;
	*/
	//rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x00);
	//if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_START, 0x00);
	if(rtn == 0)			errCnt += 1;
	HAL_Delay(100);
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x00);
	if(rtn == 0)			errCnt += 1;
	/*
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_MEMORY_ADDRESS, 0x00);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_OUTPUT_MEMORY_ADDRESS, 0x00);
	if(rtn == 0)			errCnt += 1;
	*/
	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayPattern(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	pattern;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)							return result;

	pattern		= CMD_StrToUL(pData->pArgv[0]);

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_CONFIG, pattern);
		if(rtn == 0)			errCnt += 1;

		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_CONFIG, pattern);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_CONFIG, pattern);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_CONFIG, pattern);
			if(rtn == 0)			errCnt += 1;
		}
	}

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayPatternEnable(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x02);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_START, 0x01);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
	if(rtn == 0)			errCnt += 1;

	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x02);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_START, 0x01);
	if(rtn == 0)			errCnt += 1;
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_OUTPUT_START, 0x01);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayPatternDisable(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_START, 0x00);
	if(rtn == 0)			errCnt += 1;
	HAL_Delay(100);
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x00);
	if(rtn == 0)			errCnt += 1;

	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_START, 0x00);
	if(rtn == 0)			errCnt += 1;
	HAL_Delay(100);
	rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_VIDEO_STREAM_INPUT_DATA_TYPE, 0x00);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayCustom(void *pVoid)
{
	u8	result = 0;
	u8	channel, slotNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)			return result;

	channel	= CMD_StrToUL(pData->pArgv[0]);
	slotNo	= CMD_StrToUL(pData->pArgv[1]);

	//channel	-= 1;
	channel += 1;	// channel 1 = BOARD_POSITION_3 = 2, channel 2 = BOARD_POSITION_4 = 3
	slotNo	-= 1;

	result = IMG_DisplayCustom(channel, slotNo);

	return result;
}

static u8 Cmd_ImageDisplayPrev(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	rtn = IMG_DisplayPrev();
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

static u8 Cmd_ImageDisplayNext(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	rtn = IMG_DisplayNext();
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

static u8 Cmd_ImageRepeatEnable(void *pVoid)
{
	u8	result = 0;
	u32	duration;
	float time;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	time = CMD_AToF(pData->pArgv[0]);

	if(time <= 0)				return result;

	time *= 1000;

	duration = (u32)time;

	result = IMG_RepeatEnable(duration);

	return result;
}

static u8 Cmd_ImageRepeatSelect(void *pVoid)
{
	u8	result = 0, rtn;
	u32	maxCount, duration;
	u32	sImg, eImg;
	float time;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)				return result;

	sImg = CMD_StrToUL(pData->pArgv[0]);
	eImg = CMD_StrToUL(pData->pArgv[1]);
	time = CMD_AToF(pData->pArgv[2]);

	sImg -= 1;
	eImg -= 1;

	rtn = IMG_PreloadListGetMaxCount(&maxCount);
	if(rtn == 0)						return result;

	if(sImg >= maxCount)				return result;
	if(eImg >= maxCount)				return result;
	if(sImg >= eImg)					return result;
	if(time <= 0)						return result;

	time *= 1000;

	duration = (u32)time;

	if(duration == 0)					return result;

	result = IMG_RepeatEnableSelect(duration, sImg, eImg);

	return result;
}

static u8 Cmd_ImageRepeatDisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = IMG_RepeatDisable();

	return result;
}

static u8 Cmd_ImageReverseEnable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	IMG_ReverseEnable();

	result = 1;

	return result;
}

static u8 Cmd_ImageReverseDisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	IMG_ReverseDisable();

	result = 1;

	return result;
}

static u8 Cmd_ImageYCbCrType(void *pVoid)
{
	u8	result = 0;
	u8	type;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "type1"))			type = 0;
	else if(CMD_Compare(pData->pArgv[0], "type2"))		type = 1;
	else if(CMD_Compare(pData->pArgv[0], "type3"))		type = 2;
	else if(CMD_Compare(pData->pArgv[0], "type4"))		type = 3;
	else												return result;

	IMG_YCbCrConversionData(type);

	result = 1;

	return result;
}

static u8 Cmd_ImageYCbCrEnable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = IMG_YCbCrEnable();

	return result;
}

static u8 Cmd_ImageYCbCrDisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)								return result;

	result = IMG_YCbCrDisable();

	return result;
}

static u8 Cmd_ImageResolutionMax(void *pVoid)
{
	u8	result = 0;
	u32	hSize, vSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)			return result;

	vSize = CMD_StrToUL(pData->pArgv[0]);
	hSize = CMD_StrToUL(pData->pArgv[1]);

	result = IMG_PreloadListSetMaxResolution(hSize, vSize);

	return result;
}

static u8 Cmd_ImageResolutionCurrent(void *pVoid)
{
	u8	result = 0;
	u32	hSize, vSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)			return result;

	vSize = CMD_StrToUL(pData->pArgv[0]);
	hSize = CMD_StrToUL(pData->pArgv[1]);

	result = IMG_PreloadListSetCurrentResolution(hSize, vSize);

	return result;
}

static u8 Cmd_ImageFrameMode(void *pVoid)
{
	u8	result = 0;
	u32	mode;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)								return result;

	if(CMD_Compare(pData->pArgv[0], "Default"))			mode = IMAGE_DISPLAY_FRAME_MODE_DEFAULT;
	else if(CMD_Compare(pData->pArgv[0], "Custom"))		mode = IMAGE_DISPLAY_FRAME_MODE_CUSTOM;
	else												return result;

	result = IMG_SetFrameMode(mode);

	return result;
}

static u8 Cmd_ImageDisplayAdjboxSize(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32 width;
	u32 height;
	u32 value;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	width	= CMD_StrToUL(pData->pArgv[0]);
	height	= CMD_StrToUL(pData->pArgv[1]);

	value = width << 16 | height;

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_SIZE, value);
			if(rtn == 0)			errCnt += 1;
		}
	}

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayAdjboxStart(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32 width;
	u32 height;
	u32 value;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	width	= CMD_StrToUL(pData->pArgv[0]);
	height	= CMD_StrToUL(pData->pArgv[1]);

	value = width << 16 | height;


	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_START, value);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_START, value);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_START, value);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_START, value);
			if(rtn == 0)			errCnt += 1;
		}
	}

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayAdjboxPtcolor(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	color;
	u16 red, green, blue;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	red		= CMD_StrToUL(pData->pArgv[0]);
	green	= CMD_StrToUL(pData->pArgv[1]);
	blue	= CMD_StrToUL(pData->pArgv[2]);

	color = red << 22 | green << 12 | blue << 2;

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, color);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, color);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, color);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, color);
			if(rtn == 0)			errCnt += 1;
		}
	}

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayAdjboxBgcolor(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	color;
	u16 red, green, blue;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	red		= CMD_StrToUL(pData->pArgv[0]);
	green	= CMD_StrToUL(pData->pArgv[1]);
	blue	= CMD_StrToUL(pData->pArgv[2]);

	color = red << 22 | green << 12 | blue << 2;

	if(pData->option == NULL)
	{
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_BGCOLOR, color);
		if(rtn == 0)			errCnt += 1;
		rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_BGCOLOR, color);
		if(rtn == 0)			errCnt += 1;
	}
	else
	{
		if((pData->option) == 0x01)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_BGCOLOR, color);
			if(rtn == 0)			errCnt += 1;
		}
		if((pData->option) == 0x02)
		{
			rtn = DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_BGCOLOR, color);
			if(rtn == 0)			errCnt += 1;
		}
	}

	if(errCnt == 0)			result = 1;

	return 1;
}

static u8 Cmd_ImageDisplayAdjboxfree(void *pVoid)
{
	u8	result = 0;
	u16 red, green, blue;
	u32 bg_color;
	u32 box_color;
	u32 start_xpos;
	u32 start_ypos;
	u32 pos;
	u32 end_xpos;
	u32 end_ypos;
	u32 size;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 10)		return result;

	red		= CMD_StrToUL(pData->pArgv[0]);
	green	= CMD_StrToUL(pData->pArgv[1]);
	blue	= CMD_StrToUL(pData->pArgv[2]);

	bg_color = red << 22 | green <<12 | blue<<2;

	red		= CMD_StrToUL(pData->pArgv[3]);
	green	= CMD_StrToUL(pData->pArgv[4]);
	blue	= CMD_StrToUL(pData->pArgv[5]);

	box_color = red << 22 | green <<12 | blue<<2;

	start_xpos	= CMD_StrToUL(pData->pArgv[6]);
	start_ypos	= CMD_StrToUL(pData->pArgv[7]);
	pos			= (start_xpos+1)<<16 | (start_ypos+1);

	end_xpos	= CMD_StrToUL(pData->pArgv[8]);
	end_ypos	= CMD_StrToUL(pData->pArgv[9]);
	size = (end_xpos-start_xpos) <<16 | (end_ypos-start_ypos);

	if(pData->option == NULL)
	{
		DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_BGCOLOR, bg_color);

		DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, box_color);

		DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_START, pos);

		DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_SIZE, size);

		DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_BGCOLOR, bg_color);

		DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, box_color);

		DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_START, pos);

		DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_SIZE, size);
	}
	else
	{
		if((pData->option) == 0x01)
		{
			DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_BGCOLOR, bg_color);

			DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, box_color);

			DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_START, pos);

			DPC_DirectRegWriteSingle(BOARD_POSITION_3, CORE_REG_INT_TPG_ADJ_BOX_SIZE, size);
		}
		if((pData->option) == 0x02)
		{
			DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_BGCOLOR, bg_color);

			DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_PTCOLOR, box_color);

			DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_START, pos);

			DPC_DirectRegWriteSingle(BOARD_POSITION_4, CORE_REG_INT_TPG_ADJ_BOX_SIZE, size);
		}
	}

	return 1;
}

static u8 Cmd_ImageDBVConfig(void *pVoid)
{
	u8	result = 0;
	u8 DBV_mode = 0, DBV_format = 0;
	u32 DBV_Value = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)			return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))				DBV_mode = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))			DBV_mode = 1;
	else								 					return result;

	if(CMD_Compare(pData->pArgv[1], "SDP"))					DBV_format = 1;
	else if(CMD_Compare(pData->pArgv[1], "AUX"))			DBV_format = 2;
	else								 					return result;

	DBV_Value = CMD_StrToUL(pData->pArgv[2]);

	result = DBVConfig(DBV_mode, DBV_format, DBV_Value);

	return result;
}

static u8 Cmd_ImageDBVSet(void *pVoid)
{
	u8	result = 0;
	u8  DBV_slot = 0;
	u32 DBV_slotvalue = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)			return result;

	DBV_slot = CMD_StrToUL(pData->pArgv[0]);

	if(DBV_slot > 32)			return result;
	if(DBV_slot == 0)			return result;

	DBV_slotvalue = CMD_StrToUL(pData->pArgv[1]);

	result = DBVSet(DBV_slot, DBV_slotvalue);

	return result;
}

static u8 Cmd_ImageDBVClear(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)			return result;

	result = DBVClear();

	return result;
}

static u8 Cmd_ImageTriggerEnable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)			return result;

	result = SetTriggerMode(pData->option, ON);

	return result;
}

static u8 Cmd_ImageTriggerDisable(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)			return result;

	result = SetTriggerMode(pData->option, OFF);

	return result;
}

static u8 Cmd_ImageTriggerOn(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)			return result;

	result = TriggerStart(pData->option);

	return result;
}

static u8 Cmd_ImageTimeoutInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	u32 time = 0;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)			return result;

	time = CMD_StrToUL(pData->pArgv[0]);

	result = DisplayTimeoutInit(pData->option, time);

	return result;
}

static u8 Cmd_ImageTimerOn(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)			return result;

	result = DisplayTimerSet(pData->option, ON);

	return result;
}

static u8 Cmd_ImageTimerOff(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)			return result;

	result = DisplayTimerSet(pData->option, OFF);

	return result;
}

const	commandList_t	cmdList_IMAGE[] =	{
//	{	Command,					Function,						Option,		Mode,		RootMessage					Note}
	{	"help",						Cmd_Help,						NULL,		0x01,		";HELP",					NULL},
	{	"getslot",					Cmd_ImageGetSlot,				NULL,		NULL,		";GETSLOT",					NULL},
	{	"maxcount",					Cmd_ImageMaxCount,				NULL,		NULL,		";MAXCOUNT",				NULL},
	{	"preload",					Cmd_ImagePreLoad,				NULL,		NULL,		";PRELOAD",					NULL},
	{	"preload.fill",				Cmd_ImagePreLoadFill,			NULL,		NULL,		";PRELOAD.FILL",			NULL},
	{	"preload.fill10",			Cmd_ImagePreLoadFill10,			NULL,		NULL,		";PRELOAD.FILL10",			NULL},
	{	"preload.gradation",		Cmd_ImagePreLoadGradation,		NULL,		NULL,		";PRELOAD.GRADATION",		NULL},
	{	"preload.checkbox",			Cmd_ImagePreLoadCheckBox,		NULL,		NULL,		";PRELOAD.CHECKBOX",		NULL},
	{	"preload.edge",				Cmd_ImagePreLoadEdge,			NULL,		NULL,		";PRELOAD.EDGE",			NULL},
	{	"preload.colorbar",			Cmd_ImagePreLoadColorBar,		NULL,		NULL,		";PRELOAD.COLORBAR",		NULL},
	{	"preload.coretest",			Cmd_ImagePreLoadCoreTest,		NULL,		NULL,		";PRELOAD.CORETEST",		NULL},
	{	"preload.pixeltest",		Cmd_ImagePreLoadPixelTest,		NULL,		NULL,		";PRELOAD.PIXELTEST",		NULL},
	{	"preload.lineconfirm",		Cmd_IamgePreLoadLConfirm,		NULL,		NULL,		";PRELOAD.LCONFIRM",		NULL},
	{	"preload.string",			Cmd_ImagePreLoadString,			NULL,		NULL,		";PRELOAD.STRING",			NULL},
	{	"preload.file",				Cmd_ImagePreLoadFile,			NULL,		NULL,		";PRELOAD.FILE",			NULL},
	{	"preload.clear",			Cmd_ImagePreLoadClear,			NULL,		NULL,		";PRELOAD.CLEAR",			NULL},

	{	"preload.apl",				Cmd_ImagePreLoadApl,			NULL,		NULL,		";PRELOAD.APL",				NULL},
	{	"preload.apletc",			Cmd_ImagePreLoadAplEtc,			NULL,		NULL,		";PRELOAD.APLETC",			NULL},

	{	"preload.gradationh",		Cmd_ImagePreLoadGradationH,		NULL,		NULL,		";PRELOAD.GRADATIONH",		NULL},
	{	"preload.hll",				Cmd_ImagePreLoadHll,			NULL,		NULL,		";PRELOAD.HLL",				NULL},
	{	"preload.vll",				Cmd_ImagePreLoadVll,			NULL,		NULL,		";PRELOAD.VLL",				NULL},

	{	"reflesh",					Cmd_ImageReflesh,				NULL,		NULL,		";REFLESH",					NULL},
	{	"display",					Cmd_ImageDisplay,				NULL,		NULL,		";DISPLAY",					NULL},

	{	"display.apl",				Cmd_ImageDisplayApl,			NULL,		NULL,		";DISPLAY.APL",				NULL},
	{	"display.apl1",				Cmd_ImageDisplayApl,			0x01,		NULL,		";DISPLAY.APL1",			NULL},
	{	"display.apl2",				Cmd_ImageDisplayApl,			0x02,		NULL,		";DISPLAY.APL2",			NULL},
	{	"display.apl.enable",		Cmd_ImageDisplayAplEtcEnable,	NULL,		NULL,		";DISPLAY.APL.ENABLE",		NULL},
	{	"display.apl1.enable",		Cmd_ImageDisplayAplEtcEnable,	0x01,		NULL,		";DISPLAY.APL1.ENABLE",		NULL},
	{	"display.apl2.enable",		Cmd_ImageDisplayAplEtcEnable,	0x02,		NULL,		";DISPLAY.APL2.ENABLE",		NULL},
	{	"display.apl.disable",		Cmd_ImageDisplayAplEtcDisable,	NULL,		NULL,		";DISPLAY.APL.DISABLE",		NULL},
	{	"display.apl1.disable",		Cmd_ImageDisplayAplEtcDisable,	0x01,		NULL,		";DISPLAY.APL1.DISABLE",	NULL},
	{	"display.apl2.disable",		Cmd_ImageDisplayAplEtcDisable,	0x02,		NULL,		";DISPLAY.APL2.DISABLE",	NULL},
	{	"display.apletc",			Cmd_ImageDisplayAplEtc,			NULL,		NULL,		";DISPLAY.APLETC",			NULL},
	{	"display.apletc1",			Cmd_ImageDisplayAplEtc,			0x01,		NULL,		";DISPLAY.APLETC1",			NULL},
	{	"display.apletc2",			Cmd_ImageDisplayAplEtc,			0x02,		NULL,		";DISPLAY.APLETC2",			NULL},
	{	"display.apletc.color",		Cmd_ImageDisplayAplEtcColor,	NULL,		NULL,		";DISPLAY.APLETC.Color",	NULL},
	{	"display.apletc.color1",	Cmd_ImageDisplayAplEtcColor,	0x01,		NULL,		";DISPLAY.APLETC.Color1",	NULL},
	{	"display.apletc.color2",	Cmd_ImageDisplayAplEtcColor,	0x02,		NULL,		";DISPLAY.APLETC.Color2",	NULL},

	{	"display.apletc.enable",	Cmd_ImageDisplayAplEtcEnable,	NULL,		NULL,		";DISPLAY.APLETC.ENABLE",	NULL},
	{	"display.apletc1.enable",	Cmd_ImageDisplayAplEtcEnable,	0x01,		NULL,		";DISPLAY.APLETC1.ENABLE",	NULL},
	{	"display.apletc2.enable",	Cmd_ImageDisplayAplEtcEnable,	0x02,		NULL,		";DISPLAY.APLETC2.ENABLE",	NULL},
	{	"display.apletc.disable",	Cmd_ImageDisplayAplEtcDisable,	NULL,		NULL,		";DISPLAY.APLETC.DISABLE",	NULL},
	{	"display.apletc1.disable",	Cmd_ImageDisplayAplEtcDisable,	0x01,		NULL,		";DISPLAY.APLETC1.DISABLE",	NULL},
	{	"display.apletc2.disable",	Cmd_ImageDisplayAplEtcDisable,	0x02,		NULL,		";DISPLAY.APLETC2.DISABLE",	NULL},
	{	"display.adjbox.enable",	Cmd_ImageDisplayAplEtcEnable,	NULL,		NULL,		";DISPLAY.ADJBOX.ENABLE",	NULL},
	{	"display.adjbox1.enable",	Cmd_ImageDisplayAplEtcEnable,	0x01,		NULL,		";DISPLAY.ADJBOX1.ENABLE",	NULL},
	{	"display.adjbox2.enable",	Cmd_ImageDisplayAplEtcEnable,	0x02,		NULL,		";DISPLAY.ADJBOX2.ENABLE",	NULL},
	{	"display.adjbox.disable",	Cmd_ImageDisplayAplEtcDisable,	NULL,		NULL,		";DISPLAY.ADJBOX.DISABLE",	NULL},
	{	"display.adjbox1.disable",	Cmd_ImageDisplayAplEtcDisable,	0x01,		NULL,		";DISPLAY.ADJBOX1.DISABLE",	NULL},
	{	"display.adjbox2.disable",	Cmd_ImageDisplayAplEtcDisable,	0x02,		NULL,		";DISPLAY.ADJBOX2.DISABLE",	NULL},
	{	"display.adjbox.boxsize",	Cmd_ImageDisplayAdjboxSize,		NULL,		NULL,		";DISPLAY.ADJBOX.SIZE",		NULL},
	{	"display.adjbox.boxsize1",	Cmd_ImageDisplayAdjboxSize,		0x01,		NULL,		";DISPLAY.ADJBOX.SIZE1",	NULL},
	{	"display.adjbox.boxsize2",	Cmd_ImageDisplayAdjboxSize,		0x02,		NULL,		";DISPLAY.ADJBOX.SIZE2",	NULL},
	{	"display.adjbox.coordinate",	Cmd_ImageDisplayAdjboxStart,	NULL,		NULL,		";DISPLAY.ADJBOX.START",	NULL},
	{	"display.adjbox.coordinate1",	Cmd_ImageDisplayAdjboxStart,	0x01,		NULL,		";DISPLAY.ADJBOX.START1",	NULL},
	{	"display.adjbox.coordinate2",	Cmd_ImageDisplayAdjboxStart,	0x02,		NULL,		";DISPLAY.ADJBOX.START2",	NULL},
	{	"display.adjbox.boxcolor",	Cmd_ImageDisplayAdjboxPtcolor,	NULL,		NULL,		";DISPLAY.ADJBOX.PTCOLOR",	NULL},
	{	"display.adjbox.boxcolor1",	Cmd_ImageDisplayAdjboxPtcolor,	0x01,		NULL,		";DISPLAY.ADJBOX.PTCOLOR1",	NULL},
	{	"display.adjbox.boxcolor2",	Cmd_ImageDisplayAdjboxPtcolor,	0x02,		NULL,		";DISPLAY.ADJBOX.PTCOLOR2",	NULL},
	{	"display.adjbox.bgcolor",	Cmd_ImageDisplayAdjboxBgcolor,	NULL,		NULL,		";DISPLAY.ADJBOX.BGCOLOR",	NULL},
	{	"display.adjbox.bgcolor1",	Cmd_ImageDisplayAdjboxBgcolor,	0x01,		NULL,		";DISPLAY.ADJBOX.BGCOLOR1",	NULL},
	{	"display.adjbox.bgcolor2",	Cmd_ImageDisplayAdjboxBgcolor,	0x02,		NULL,		";DISPLAY.ADJBOX.BGCOLOR2",	NULL},
	{	"display.aplfree",			Cmd_ImageDisplayAdjboxfree,		NULL,		NULL,		";DISPLAY.APL.FREE",		NULL},
	{	"display.aplfree1",			Cmd_ImageDisplayAdjboxfree,		0x01,		NULL,		";DISPLAY.APL.FREE1",	NULL},
	{	"display.aplfree2",			Cmd_ImageDisplayAdjboxfree,		0x02,		NULL,		";DISPLAY.APL.FREE2",	NULL},

	{	"display.fill",				Cmd_ImageDisplayFill,			NULL,		NULL,		";DISPLAY.FILL",			NULL},
	{	"display.fill1",			Cmd_ImageDisplayFill,			0x01,		NULL,		";DISPLAY.FILL1",			NULL},
	{	"display.fill2",			Cmd_ImageDisplayFill,			0x02,		NULL,		";DISPLAY.FILL2",			NULL},
	{	"display.fill.enable",		Cmd_ImageDisplayFillEnable,		NULL,		NULL,		";DISPLAY.FILL.ENABLE",		NULL},
	{	"display.fill.disable",		Cmd_ImageDisplayFillDisable,	NULL,		NULL,		";DISPLAY.FILL.DISABLE",	NULL},

	{	"display.pattern",			Cmd_ImageDisplayPattern,		NULL,		NULL,		";DISPLAY.PATTERN",			NULL},
	{	"display.pattern1",			Cmd_ImageDisplayPattern,		0x01,		NULL,		";DISPLAY.PATTERN1",		NULL},
	{	"display.pattern2",			Cmd_ImageDisplayPattern,		0x02,		NULL,		";DISPLAY.PATTERN2",		NULL},
	{	"display.pattern.enable",	Cmd_ImageDisplayPatternEnable,	NULL,		NULL,		";DISPLAY.PATTERN.ENABLE",	NULL},
	{	"display.pattern.disable",	Cmd_ImageDisplayPatternDisable,	NULL,		NULL,		";DISPLAY.PATTERN.DISABLE",	NULL},

	{	"display.custom",			Cmd_ImageDisplayCustom,			NULL,		NULL,		";DISPLAY.CUSCOM",			NULL},
	{	"display.prev",				Cmd_ImageDisplayPrev,			NULL,		NULL,		";DISPLAY.PREV",			NULL},
	{	"display.next",				Cmd_ImageDisplayNext,			NULL,		NULL,		";DISPLAY.NEXT",			NULL},
	{	"repeat.enable",			Cmd_ImageRepeatEnable,			NULL,		NULL,		";REPEAT.ENABLE",			NULL},
	{	"repeat.select",			Cmd_ImageRepeatSelect,			NULL,		NULL,		";REPEAT.SELECT",			NULL},
	{	"repeat.disable",			Cmd_ImageRepeatDisable,			NULL,		NULL,		";REPEAT.DISABLE",			NULL},
	{	"reverse.enable",			Cmd_ImageReverseEnable,			NULL,		NULL,		";REVERSE.ENABLE",			NULL},
	{	"reverse.disable",			Cmd_ImageReverseDisable,		NULL,		NULL,		";REVERSE.DISABLE",			NULL},
	{	"ycbcr.type",				Cmd_ImageYCbCrType,				NULL,		NULL,		";YCBCR.TYPE",				NULL},
	{	"ycbcr.enable",				Cmd_ImageYCbCrEnable,			NULL,		NULL,		";YCBCR.ENABLE",			NULL},
	{	"ycbcr.disable",			Cmd_ImageYCbCrDisable,			NULL,		NULL,		";YCBCR.DISABLE",			NULL},
	{	"resolution.max",			Cmd_ImageResolutionMax,			NULL,		NULL,		";RESOLUTION.MAX",			NULL},
	{	"resolution.current",		Cmd_ImageResolutionCurrent,		NULL,		NULL,		";RESOLUTION.CURRENT",		NULL},	// e7602하고 cmd param 순서 다름
	{	"Frame.Mode",				Cmd_ImageFrameMode,				NULL,		NULL,		";FRAME.MODE",				NULL},
	{	"repeat.dbv.config",		Cmd_ImageDBVConfig,				NULL,		NULL,		";DBV.CONFIG",				NULL},
	{	"repeat.dbv.set",			Cmd_ImageDBVSet,				NULL,		NULL,		";DBV.SET",					NULL},
	{	"repeat.dbv.clear",			Cmd_ImageDBVClear,				NULL,		NULL,		";DBV.CLEAR",				NULL},

	{	"trigger.enable",			Cmd_ImageTriggerEnable,			NULL,		NULL,		";TRIGGER.ENABLE",			NULL},
	{	"trigger1.enable",			Cmd_ImageTriggerEnable,			0x01,		NULL,		";TRIGGER.ENABLE",			NULL},
	{	"trigger2.enable",			Cmd_ImageTriggerEnable,			0x02,		NULL,		";TRIGGER.ENABLE",			NULL},
	{	"trigger.disable",			Cmd_ImageTriggerDisable,		NULL,		NULL,		";TRIGGER.DISABLE",			NULL},
	{	"trigger1.disable",			Cmd_ImageTriggerDisable,		0x01,		NULL,		";TRIGGER.DISABLE",			NULL},
	{	"trigger2.disable",			Cmd_ImageTriggerDisable,		0x02,		NULL,		";TRIGGER.DISABLE",			NULL},
	{	"trigger.on",				Cmd_ImageTriggerOn,				NULL,		NULL,		";TRIGGER.ON",				NULL},
	{	"trigger1.on",				Cmd_ImageTriggerOn,				0x01,		NULL,		";TRIGGER.ON",				NULL},
	{	"trigger2.on",				Cmd_ImageTriggerOn,				0x02,		NULL,		";TRIGGER.ON",				NULL},

	{	"display.timer1.init",		Cmd_ImageTimeoutInit,			0x01,		NULL,		";DISPLAY.TIMEOUT.INIT",	NULL},
	{	"display.timer2.init",		Cmd_ImageTimeoutInit,			0x02,		NULL,		";DISPLAY.TIMEOUT.INIT",	NULL},
	{	"display.timer1.on",		Cmd_ImageTimerOn,				0x01,		NULL,		";DISPLAY.TIMER1.ON",		NULL},
	{	"display.timer2.on",		Cmd_ImageTimerOn,				0x02,		NULL,		";DISPLAY.TIMER2.ON",		NULL},
	{	"display.timer1.off",		Cmd_ImageTimerOff,				0x01,		NULL,		";DISPLAY.TIMER1.OFF",		NULL},
	{	"display.timer2.off",		Cmd_ImageTimerOff,				0x02,		NULL,		";DISPLAY.TIMER2.OFF",		NULL},
	{	NULL,						NULL,							NULL,		NULL,		NULL,						NULL},
};

