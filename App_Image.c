#include	"App_Image.h"

static	imgPreloadList_t	*pImgPreloadList = (imgPreloadList_t*)MEMORY_MAP_SDRAM_IMAGE_PRELOAD_LIST_ADRS;
static	imageBufferInfo_t	imageBufferInfo;
static	imageDisplayInfo_t	imageDisplayInfo;
static	imgYCbCrColor_t		ycbcrColor;
static	imgRepeat_t			imgRepeat;

static u8 rolling_mode = 0;
static u8 rolling_type = 0;
static u16 rolling_value = 0;
static u16 rolling_set[32] = {0,};
static u8 sdp_packet[36] = {0x00, 0x08, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xBA, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static u8 aux_packet[32] = {0x0F, 0x00, 0x00, 0x00, 0xFF, 0xBA, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
dptxRegData_t	dptxRegData_aux;

static	u8 Trigger = OFF;

static	u32 Timeout[2]	= {0, 0};
static	u8	TimerSet[2]	= {OFF, OFF};
static	u32 Timer_start[2] = {0, 0};

u8 IMG_PreloadListDataInit()
{
	u8	result = 0;

	memset(pImgPreloadList, NULL, sizeof(imgPreloadList_t));
	memset(&imageBufferInfo, NULL, sizeof(imageBufferInfo));
	memset(&imageDisplayInfo, NULL, sizeof(imageDisplayInfo));
	memset(&ycbcrColor, NULL, sizeof(ycbcrColor));

	result = 1;

	return result;
}

u8 IMG_PreloadListInit()
{
	u8	result = 0;

	memset(pImgPreloadList, NULL, sizeof(imgPreloadList_t));

	result = 1;

	return result;
}

u8 IMG_PreloadListSetMemorySize(u32 memorySize)
{
	u8	result = 0;

	pImgPreloadList->memorySize = memorySize;

	result = 1;

	return result;
}

u8 IMG_PreloadListGetMaxResolution(u32 *phSize, u32 *pvSize)
{
	u8	result = 0;

	if(pImgPreloadList->status == 0)
	{
		*phSize = 0;
		*pvSize = 0;

		return result;
	}

	*phSize	= pImgPreloadList->hSize_Max;
	*pvSize	= pImgPreloadList->vSize_Max;

	result = 1;

	return result;
}

u8 IMG_PreloadListSetMaxResolution(u32 hSize, u32 vSize)
{
	u8	result = 0;
	u32	imageCount;
	u32	slotSize = 0;
	imgPreloadInfo_t	*pInfo;

	if((hSize == 0) || (vSize == 0))				return result;

	if((pImgPreloadList->hSize_Max == hSize) && (pImgPreloadList->vSize_Max == vSize))
	{
		result = 1;

		return result;
	}

	slotSize = hSize * vSize * 4;
	if(slotSize % 8)	slotSize += (8 - (slotSize % 8));

	if(slotSize > (36 * 1024 * 1024))				return result;

	if(pImgPreloadList->memorySize < slotSize)		return result;

	pImgPreloadList->hSize_Max	= hSize;
	pImgPreloadList->vSize_Max	= vSize;

	pImgPreloadList->slotSize	= slotSize;

	pImgPreloadList->maxCount	= pImgPreloadList->memorySize / pImgPreloadList->slotSize;
	if(pImgPreloadList->maxCount > IMAGE_LIST_MAX_COUNT)
	{
		pImgPreloadList->maxCount	= IMAGE_LIST_MAX_COUNT;
	}

	for(imageCount = 0; imageCount < IMAGE_LIST_MAX_COUNT; imageCount++)
	{
		pInfo = (imgPreloadInfo_t*)&(pImgPreloadList->list[imageCount]);

		memset(pInfo, NULL, sizeof(imgPreloadInfo_t));

		if(imageCount < pImgPreloadList->maxCount)
		{
			pInfo->indexNo		= imageCount;
			pInfo->startAdrs	= imageCount * slotSize;
		}
	}

	pImgPreloadList->hSize_Current = pImgPreloadList->hSize_Max;
	pImgPreloadList->vSize_Current = pImgPreloadList->vSize_Max;

	pImgPreloadList->status	= 1;

	result = 1;

	return result;
}

u8 IMG_PreloadListGetCurrentResolution(u32 *phSize, u32 *pvSize)
{
	u8	result = 0;

	if(pImgPreloadList->status == 0)
	{
		*phSize	= 0;
		*pvSize	= 0;

		return result;
	}

	*phSize	= pImgPreloadList->hSize_Current;
	*pvSize	= pImgPreloadList->vSize_Current;

	result = 1;

	return result;
}

u8 IMG_PreloadListSetCurrentResolution(u32 hSize, u32 vSize)
{
	u8	result = 0, rtn, chkr;
	u32	hSize_Max, vSize_Max, maxSize, currentSize;

	if((hSize == 0) || (vSize == 0))		return result;

	rtn = IMG_PreloadListGetMaxResolution(&hSize_Max, &vSize_Max);
	if(rtn == 0)
	{
		chkr = IMG_PreloadListSetMaxResolution(hSize, vSize);
		if(chkr == 0)						return result;
	}
	else
	{
		maxSize		= hSize_Max * vSize_Max;
		currentSize	= hSize * vSize;

		if(maxSize < currentSize)
		{
			chkr = IMG_PreloadListSetMaxResolution(hSize, vSize);
			if(chkr == 0)					return result;
		}
	}

	pImgPreloadList->hSize_Current	= hSize;
	pImgPreloadList->vSize_Current	= vSize;

	result = 1;

	return result;
}

u8 IMG_PreloadListGetSlotAdrs(u32 slotNo, u32 *pAdrs)
{
	u8	result = 0;

	if(slotNo >= pImgPreloadList->maxCount)			return result;

	if(pImgPreloadList->list[slotNo].status != 0)
	{
		*pAdrs = pImgPreloadList->list[slotNo].startAdrs;

		result = 1;
	}
	else
	{
		*pAdrs = 0;
	}

	return result;
}

u8 IMG_PreloadListGetMaxCount(u32 *pMaxCount)
{
	u8	result = 0;

	*pMaxCount = pImgPreloadList->maxCount;

	result = 1;

	return result;
}

u8 IMG_PreloadListGetSlotStatus(u32 slotNo, u8 *pStatus)
{
	u8	result = 0;

	if(slotNo >= pImgPreloadList->maxCount)			return result;

	*pStatus = pImgPreloadList->list[slotNo].status;

	result = 1;

	return result;
}

u8 IMG_PreloadListCheckImage(u32 *pCheckCnt)
{
	u8	result = 0;
	u32	checkCnt = 0;

	for(u32 cnt = 0; cnt < pImgPreloadList->maxCount; cnt++)
	{
		if(pImgPreloadList->list[cnt].status != 0)		checkCnt += 1;
	}

	if(checkCnt != 0)
	{
		*pCheckCnt = checkCnt;

		result = 1;
	}

	return result;
}

u8 IMG_PreloadListGetSlotFileName(u32 slotNo, u8 *pFileName)
{
	u8	result = 0;

	if(slotNo >= pImgPreloadList->maxCount)			return result;

	if(pImgPreloadList->list[slotNo].status != 0)
	{
		strcpy((char*)pFileName, (char*)pImgPreloadList->list[slotNo].path);

		result = 1;
	}

	return result;
}

u8 IMG_PreloadListSetSlotFileName(u32 slotNo, u8 *pFileName)
{
	u8	result = 0;

	if(slotNo >= pImgPreloadList->maxCount)			return result;

	memset(pImgPreloadList->list[slotNo].path, NULL, 288);

	pImgPreloadList->list[slotNo].status	= 1;

	strcpy((char*)pImgPreloadList->list[slotNo].path, (char*)pFileName);

	result = 1;

	return result;
}

u8 IMG_PreloadListGetSlotImageSize(u32 slotNo, u32 *phSize, u32 *pvSize)
{
	u8	result = 0;

	if(slotNo >= pImgPreloadList->maxCount)			return result;

	if(pImgPreloadList->list[slotNo].status == 0)	return result;

	*phSize = pImgPreloadList->list[slotNo].hSize;
	*pvSize = pImgPreloadList->list[slotNo].vSize;

	result = 1;

	return result;
}

u8 IMG_PreloadListSetSlotImageSize(u32 slotNo, u32 hSize, u32 vSize)
{
	u8	result = 0;

	if(slotNo >= pImgPreloadList->maxCount)			return result;

	pImgPreloadList->list[slotNo].hSize	= hSize;
	pImgPreloadList->list[slotNo].vSize	= vSize;

	result = 1;

	return result;
}

u8 IMG_PreloadListCheckSlotImageSize(u32 slotNo, u8 *pStatus)
{
	u8	result = 0;
	u8	status = 1;

	if(slotNo >= pImgPreloadList->maxCount)			return result;

	if(pImgPreloadList->list[slotNo].status == 0)	return result;

	if(pImgPreloadList->hSize_Current != pImgPreloadList->list[slotNo].hSize)	status = 0;
	if(pImgPreloadList->vSize_Current != pImgPreloadList->list[slotNo].vSize)	status = 0;

	*pStatus = status;

	result = 1;

	return result;
}

u8 IMG_PreloadListDeleteSlot(u8 slotNo)
{
	u8	result = 0;

	if(slotNo >= pImgPreloadList->maxCount)			return result;

	pImgPreloadList->list[slotNo].status = 0;

	result = 1;

	return result;
}

u8 IMG_PreloadListClearSlot()
{
	u8	result = 0;

	for(u32 cnt = 0; cnt < IMAGE_LIST_MAX_COUNT; cnt++)
	{
		pImgPreloadList->list[cnt].status = 0;
	}

	result = 1;

	return result;
}

u8 IMG_VLineReverse(u32 mode)
{
	u8	result = 0;

	imageDisplayInfo.vReverse = (mode & 0x01);

	return result;
}

u8 IMG_LoadBmpFile(u8 *pPath)
{
	u8	result = 0, rtn;
	FIL	bmpFile;
	bmpHeader_t bmpHeader;
	imageBufferInfo_t imgInfo;

	// bmp file open

	rtn = FatFS_FileOpen(&bmpFile, pPath);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FilePtrWrite(&bmpFile, 0);

	// bmp header read

	memset(bmpHeader.data, NULL, sizeof(bmpHeader));

	rtn = FatFS_FileRead(&bmpFile, (u32)bmpHeader.data, sizeof(bmpHeader_t));

	// bmp file check

	rtn = FIMG_BmpHeaderChecker(&imgInfo, &bmpHeader);

	if(rtn != 1)
	{
		FatFS_FileClose(&bmpFile);
		return result;
	}

	rtn = FatFS_FilePtrWrite(&bmpFile, 0);

	rtn = FatFS_FileReadAll(&bmpFile, MEMORY_MAP_SDRAM_FILE_LOAD_ADRS);

	FatFS_FileClose(&bmpFile);

	FIMG_bmp2rgb(&imgInfo, MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS, MEMORY_MAP_SDRAM_FILE_LOAD_ADRS + bmpHeader.hFile.fOffBits);

	imageBufferInfo.heigth		= imgInfo.heigth;
	imageBufferInfo.width		= imgInfo.width;
	imageBufferInfo.size		= imgInfo.size;
	imageBufferInfo.bitCnt		= imgInfo.bitCnt;

	result = 1;

	return result;
}

u8 IMG_LoadJpgFile(u8 *pPath)
{
	u8	result = 0, rtn;
	u32 fileSize;
	FIL	jpgFile;
	u32 *dumpAdrs;
	JPEG_ConfTypeDef info;

	dumpAdrs = (u32*)MEMORY_MAP_SDRAM_FILE_LOAD_ADRS;
	memset(dumpAdrs, NULL, (32 * 1024 * 1024));

	// bmp file open

	rtn = FatFS_FileOpen(&jpgFile, pPath);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FilePtrWrite(&jpgFile, 0);

	fileSize = FatFS_FileSize(&jpgFile);

	rtn = FatFS_FileReadAll(&jpgFile, MEMORY_MAP_SDRAM_FILE_LOAD_ADRS);

	FatFS_FileClose(&jpgFile);

	JPEG_DecodeFromSDRAM(MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS, MEMORY_MAP_SDRAM_FILE_LOAD_ADRS, fileSize);

	JPEG_GetInfo(&info);

	imageBufferInfo.heigth		= info.ImageHeight;
	imageBufferInfo.width		= info.ImageWidth;
	imageBufferInfo.size		= info.ImageHeight * info.ImageWidth * 3;
	imageBufferInfo.bitCnt		= 24;

	result = 1;

	return result;
}

u8 IMG_ReverseEnable()
{
	u8	result = 0;

	imageDisplayInfo.vhReverse = 1;

	return result;
}

u8 IMG_ReverseDisable()
{
	u8	result = 0;

	imageDisplayInfo.vhReverse = 0;

	return result;
}

u8 IMG_YCbCrEnable()
{
	u8	result = 0;

	imageDisplayInfo.ycbcrEnable = 1;

	return result;
}

u8 IMG_YCbCrDisable()
{
	u8	result = 0;

	imageDisplayInfo.ycbcrEnable = 0;

	return result;
}

u8 IMG_ImageReverse3Byte(u8 *pData, u32 vSize, u32 hSize)
{
	u8	result = 0;
	u8	dummy[3];
	u32	pixelSize, rCnt;

	pixelSize = (vSize * hSize) - 1;

	rCnt = (pixelSize + 1) / 2;

	for(u32	cnt = 0; cnt < rCnt; cnt++)
	{
		dummy[0]				= pData[(cnt * 3) + 0];
		dummy[1]				= pData[(cnt * 3) + 1];
		dummy[2]				= pData[(cnt * 3) + 2];

		pData[(cnt * 3) + 0]	= pData[(pixelSize * 3) + 0];
		pData[(cnt * 3) + 1]	= pData[(pixelSize * 3) + 1];
		pData[(cnt * 3) + 2]	= pData[(pixelSize * 3) + 2];

		pData[(pixelSize * 3) + 0]	= dummy[0];
		pData[(pixelSize * 3) + 1]	= dummy[1];
		pData[(pixelSize * 3) + 2]	= dummy[2];

		pixelSize -= 1;
	}

	result = 1;

	return result;
}

u8 IMG_ImageReverse4Byte(u32 *pData, u32 vSize, u32 hSize)
{
	u8	result = 0;
	u32	dummy;
	u32	pixelSize, rCnt;

	pixelSize = (vSize * hSize) - 1;

	rCnt = (pixelSize + 1) / 2;

	for(u32	cnt = 0; cnt < rCnt; cnt++)
	{
		dummy = pData[cnt];

		pData[cnt] = pData[pixelSize];

		pData[pixelSize] = dummy;

		pixelSize -= 1;
	}

	result = 1;

	return result;
}

u8 IMG_ImageSend_24b4Byte()
{
	u8	result = 0;
	u8	*pImgData;
	u32	imgAdrs;
	u32	hSize, hCnt;
	u32	vCnt;
	bitCtrl32_t	pixelData;

	hSize = imageBufferInfo.width * 3;

	pixelData.u32Data = 0;

	pImgData = (u8*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	if(imageDisplayInfo.vhReverse)
	{
		IMG_ImageReverse3Byte(pImgData, imageBufferInfo.heigth, imageBufferInfo.width);
	}

	FPGA_WriteSingle(FPGA_CMD_INPUT_RGB10_MODE, 0x00000000);

	if((pImgPreloadList->hSize_Current != imageBufferInfo.width) || (pImgPreloadList->vSize_Current != imageBufferInfo.heigth))
	{
		imgAdrs = MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		for(vCnt = 0; vCnt < pImgPreloadList->vSize_Current; vCnt++)
		{
			if(vCnt < imageBufferInfo.heigth)
			{
				pixelData.u32Data = 0;

				for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
				{
					if(hCnt < imageBufferInfo.width)
					{
						pixelData.u8Data[0] = *pImgData;
						pImgData++;
						pixelData.u8Data[1] = *pImgData;
						pImgData++;
						pixelData.u8Data[2] = *pImgData;
						pImgData++;
					}
					else
					{
						pixelData.u32Data = 0x00ffffff;
					}

					FPGA_WriteDATA(pixelData.u32Data);

				}
			}
			else
			{
				pixelData.u32Data = 0x00ffffff;

				for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
				{
					FPGA_WriteDATA(pixelData.u32Data);
				}
			}

			imgAdrs += hSize;

			pImgData = (u8*)imgAdrs;
		}

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);
	}
	else
	{
		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		for(vCnt = 0; vCnt < pImgPreloadList->vSize_Current; vCnt++)
		{
			for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
			{
				pixelData.u8Data[0] = *pImgData;
				pImgData++;
				pixelData.u8Data[1] = *pImgData;
				pImgData++;
				pixelData.u8Data[2] = *pImgData;
				pImgData++;

				FPGA_WriteDATA(pixelData.u32Data);
			}
		}

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);
	}

	result = 1;

	return result;
}

u8 IMG_ImageSend_32b4Byte(u8 mode)
{
	u8	result = 0;
	u32 *pImgData;
	u32	iSize;
	u32	hCnt, vCnt;

	pImgData = (u32*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	if(imageDisplayInfo.vhReverse)
	{
		IMG_ImageReverse4Byte(pImgData, imageBufferInfo.heigth, imageBufferInfo.width);
	}

	if(mode == IMG_ARGB8888)
	{
		FPGA_WriteSingle(FPGA_CMD_INPUT_RGB10_MODE, 0x00000000);
	}
	else if(mode == IMG_RGB101010)
	{
		FPGA_WriteSingle(FPGA_CMD_INPUT_RGB10_MODE, 0x00000001);
	}

	if((pImgPreloadList->hSize_Current != imageBufferInfo.width) || (pImgPreloadList->vSize_Current != imageBufferInfo.heigth))
	{
		iSize = imageBufferInfo.width;// * 4;
		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		for(vCnt = 0; vCnt < pImgPreloadList->vSize_Current; vCnt++)
		{
			if(vCnt < imageBufferInfo.heigth)
			{
				for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
				{
					if(hCnt < imageBufferInfo.width)
					{
						FPGA_WriteDATA(pImgData[hCnt]);
					}
					else
					{
						FPGA_WriteDATA(0xffffffff);
					}
				}
			}
			else
			{
				for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
				{
					FPGA_WriteDATA(0xffffffff);
				}
			}

			pImgData += iSize;
		}

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);
	}
	else
	{
		iSize = imageBufferInfo.width * imageBufferInfo.heigth;

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

		FPGA_Write(FPGA_CMD_DDR_WRITE_DATA, pImgData, iSize);

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);
	}

	result = 1;

	return result;
}

u8 IMG_YCbCrConversionData(u8 mode)
{
	u8	result = 1;

	switch(mode)
	{
		case 0:
			ycbcrColor.mode			= 0;

			ycbcrColor.y_R			= 0.257;
			ycbcrColor.y_G			= 0.504;
			ycbcrColor.y_B			= 0.098;
			ycbcrColor.cb_R			= -0.148;
			ycbcrColor.cb_G			= -0.291;
			ycbcrColor.cb_B			= 0.439;
			ycbcrColor.cr_R			= 0.439;
			ycbcrColor.cr_G			= -0.368;
			ycbcrColor.cr_B			= -0.071;
			ycbcrColor.y_Offset		= 16;
			ycbcrColor.cb_Offset	= 128;
			ycbcrColor.cr_Offset	= 128;
			break;

		case 1:
			ycbcrColor.mode			= 1;

			ycbcrColor.y_R			= 0.299;
			ycbcrColor.y_G			= 0.587;
			ycbcrColor.y_B			= 0.114;
			ycbcrColor.cb_R			= -0.169;
			ycbcrColor.cb_G			= -0.331;
			ycbcrColor.cb_B			= 0.5;
			ycbcrColor.cr_R			= 0.5;
			ycbcrColor.cr_G			= -0.419;
			ycbcrColor.cr_B			= -0.081;
			ycbcrColor.y_Offset		= 0;
			ycbcrColor.cb_Offset	= 128;
			ycbcrColor.cr_Offset	= 128;
			break;

		case 2:
			ycbcrColor.mode			= 2;

			ycbcrColor.y_R			= 0.257;
			ycbcrColor.y_G			= 0.504;
			ycbcrColor.y_B			= 0.098;
			ycbcrColor.cb_R			= -0.148;
			ycbcrColor.cb_G			= -0.291;
			ycbcrColor.cb_B			= 0.439;
			ycbcrColor.cr_R			= 0.439;
			ycbcrColor.cr_G			= -0.368;
			ycbcrColor.cr_B			= -0.071;
			ycbcrColor.y_Offset		= 16;
			ycbcrColor.cb_Offset	= 0;
			ycbcrColor.cr_Offset	= 0;
			break;

		case 3:
			ycbcrColor.mode			= 3;

			ycbcrColor.y_R			= 0.299;
			ycbcrColor.y_G			= 0.587;
			ycbcrColor.y_B			= 0.114;
			ycbcrColor.cb_R			= -0.169;
			ycbcrColor.cb_G			= -0.331;
			ycbcrColor.cb_B			= 0.5;
			ycbcrColor.cr_R			= 0.5;
			ycbcrColor.cr_G			= -0.419;
			ycbcrColor.cr_B			= -0.081;
			ycbcrColor.y_Offset		= 0;
			ycbcrColor.cb_Offset	= 0;
			ycbcrColor.cr_Offset	= 0;
			break;

		default:
			result = 0;
			break;
	}

	return result;
}

u8 IMG_YCbCrConversionDataInit()
{
	u8	result = 0;

	memset(&ycbcrColor, NULL, sizeof(ycbcrColor));

	result = IMG_YCbCrConversionData(0);

	return result;
}

u32	IMG_RGB24b2YCbCr24b(u32 src)
{
	double	r, g, b;
	double	y, cb, cr;
	imgColorData_t	input, output;

	input.colorData = src;

	r	= (double)input.rgb8.red;
	g	= (double)input.rgb8.green;
	b	= (double)input.rgb8.blue;

	y	= (r * ycbcrColor.y_R)	+ (g * ycbcrColor.y_G)	+ (b * ycbcrColor.y_B)	+ ycbcrColor.y_Offset;
	cb	= (r * ycbcrColor.cb_R) + (g * ycbcrColor.cb_G) + (b * ycbcrColor.cb_B) + ycbcrColor.cb_Offset;
	cr	= (r * ycbcrColor.cr_R) + (g * ycbcrColor.cr_G) + (b * ycbcrColor.cr_B) + ycbcrColor.cr_Offset;

	if(imageDisplayInfo.ycbcrRound)
	{
		y	= round(y);
		cb	= round(cb);
		cr	= round(cr);
	}

	output.ycbcr8.y		= (u8)y;
	output.ycbcr8.cb	= (u8)cb;
	output.ycbcr8.cr	= (u8)cr;
	output.ycbcr8.dc	= 0;

	return output.colorData;
}

u32 IMG_RGB30b2YCbCr24b(u32 src)
{
	double	r, g, b;
	double	y, cb, cr;
	imgColorData_t	input, output;

	input.colorData = src;

	r	= (double)(input.rgb10.red		>> 2);
	g	= (double)(input.rgb10.green	>> 2);
	b	= (double)(input.rgb10.blue		>> 2);

	y	= (r * ycbcrColor.y_R)	+ (g * ycbcrColor.y_G)	+ (b * ycbcrColor.y_B)	+ ycbcrColor.y_Offset;
	cb	= (r * ycbcrColor.cb_R) + (g * ycbcrColor.cb_G) + (b * ycbcrColor.cb_B) + ycbcrColor.cb_Offset;
	cr	= (r * ycbcrColor.cr_R) + (g * ycbcrColor.cr_G) + (b * ycbcrColor.cr_B) + ycbcrColor.cr_Offset;

	if(imageDisplayInfo.ycbcrRound)
	{
		y	= round(y);
		cb	= round(cb);
		cr	= round(cr);
	}

	output.ycbcr8.y		= (u8)y;
	output.ycbcr8.cb	= (u8)cb;
	output.ycbcr8.cr	= (u8)cr;
	output.ycbcr8.dc	= 0;

	return output.colorData;
}

u8 IMG_ImageSend_24b4Byte_YCbCr422()
{
	u8	result = 0;

	u8	*pImgData;
	u32	imgAdrs;
	u32	hSize, hCnt;
	u32	vCnt;
	bitCtrl32_t	pixelData;
	imgColorData_t	pxlData;
	imgYCbCr422_96_t	ycbcr422_96;
	u8	txCount = 0;

	hSize = imageBufferInfo.width * 3;

	pixelData.u32Data = 0;

	pImgData = (u8*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	if(imageDisplayInfo.vhReverse)
	{
		IMG_ImageReverse3Byte(pImgData, imageBufferInfo.heigth, imageBufferInfo.width);
	}

	FPGA_WriteSingle(FPGA_CMD_INPUT_RGB10_MODE, 0x00000000);

	if((pImgPreloadList->hSize_Current != imageBufferInfo.width) || (pImgPreloadList->vSize_Current != imageBufferInfo.heigth))
	{
		imgAdrs = MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		for(vCnt = 0; vCnt < pImgPreloadList->vSize_Current; vCnt++)
		{
			if(vCnt < imageBufferInfo.heigth)
			{
				pixelData.u32Data = 0;

				for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
				{
					if(hCnt < imageBufferInfo.width)
					{
						pixelData.u8Data[0] = *pImgData;
						pImgData++;
						pixelData.u8Data[1] = *pImgData;
						pImgData++;
						pixelData.u8Data[2] = *pImgData;
						pImgData++;
					}
					else
					{
						pixelData.u32Data = 0x00ffffff;
					}

					pxlData.colorData = IMG_RGB24b2YCbCr24b(pixelData.u32Data);

					switch(txCount)
					{
						case 0:
							ycbcr422_96.y0	= pxlData.ycbcr8.y;
							ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
							ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
							txCount++;
							break;

						case 1:
							ycbcr422_96.y1	= pxlData.ycbcr8.y;
							txCount++;
							break;

						case 2:
							ycbcr422_96.y2	= pxlData.ycbcr8.y;
							ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
							ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
							txCount++;
							break;

						case 3:
							ycbcr422_96.y3	= pxlData.ycbcr8.y;
							txCount++;
							break;

						case 4:
							ycbcr422_96.y4	= pxlData.ycbcr8.y;
							ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
							ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
							txCount++;
							break;

						case 5:
							ycbcr422_96.y5	= pxlData.ycbcr8.y;
							FPGA_WriteDATA(ycbcr422_96.data[0]);
							FPGA_WriteDATA(ycbcr422_96.data[1]);
							FPGA_WriteDATA(ycbcr422_96.data[2]);
							FPGA_WriteDATA(ycbcr422_96.data[3]);
							txCount = 0;
							break;
					}
				}
			}
			else
			{
				pixelData.u32Data = 0x00ffffff;

				for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
				{
					pxlData.colorData = IMG_RGB24b2YCbCr24b(pixelData.u32Data);

					switch(txCount)
					{
						case 0:
							ycbcr422_96.y0	= pxlData.ycbcr8.y;
							ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
							ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
							txCount++;
							break;

						case 1:
							ycbcr422_96.y1	= pxlData.ycbcr8.y;
							txCount++;
							break;

						case 2:
							ycbcr422_96.y2	= pxlData.ycbcr8.y;
							ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
							ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
							txCount++;
							break;

						case 3:
							ycbcr422_96.y3	= pxlData.ycbcr8.y;
							txCount++;
							break;

						case 4:
							ycbcr422_96.y4	= pxlData.ycbcr8.y;
							ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
							ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
							txCount++;
							break;

						case 5:
							ycbcr422_96.y5	= pxlData.ycbcr8.y;
							FPGA_WriteDATA(ycbcr422_96.data[0]);
							FPGA_WriteDATA(ycbcr422_96.data[1]);
							FPGA_WriteDATA(ycbcr422_96.data[2]);
							FPGA_WriteDATA(ycbcr422_96.data[3]);
							txCount = 0;
							break;
					}
				}
			}

			imgAdrs += hSize;

			pImgData = (u8*)imgAdrs;
		}

		if(txCount != 0)
		{
			FPGA_WriteDATA(ycbcr422_96.data[0]);
			FPGA_WriteDATA(ycbcr422_96.data[1]);
			FPGA_WriteDATA(ycbcr422_96.data[2]);
			FPGA_WriteDATA(ycbcr422_96.data[3]);
		}

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);
	}
	else
	{
		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		for(vCnt = 0; vCnt < pImgPreloadList->vSize_Current; vCnt++)
		{
			for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
			{
				pixelData.u8Data[0] = *pImgData;
				pImgData++;
				pixelData.u8Data[1] = *pImgData;
				pImgData++;
				pixelData.u8Data[2] = *pImgData;
				pImgData++;

				pxlData.colorData = IMG_RGB24b2YCbCr24b(pixelData.u32Data);

				switch(txCount)
				{
					case 0:
						ycbcr422_96.y0	= pxlData.ycbcr8.y;
						ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 1:
						ycbcr422_96.y1	= pxlData.ycbcr8.y;
						txCount++;
						break;

					case 2:
						ycbcr422_96.y2	= pxlData.ycbcr8.y;
						ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 3:
						ycbcr422_96.y3	= pxlData.ycbcr8.y;
						txCount++;
						break;

					case 4:
						ycbcr422_96.y4	= pxlData.ycbcr8.y;
						ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 5:
						ycbcr422_96.y5	= pxlData.ycbcr8.y;
						FPGA_WriteDATA(ycbcr422_96.data[0]);
						FPGA_WriteDATA(ycbcr422_96.data[1]);
						FPGA_WriteDATA(ycbcr422_96.data[2]);
						FPGA_WriteDATA(ycbcr422_96.data[3]);
						txCount = 0;
						break;
				}
			}
		}

		if(txCount != 0)
		{
			FPGA_WriteDATA(ycbcr422_96.data[0]);
			FPGA_WriteDATA(ycbcr422_96.data[1]);
			FPGA_WriteDATA(ycbcr422_96.data[2]);
			FPGA_WriteDATA(ycbcr422_96.data[3]);
		}

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);
	}

	result = 1;

	return result;
}

u8 IMG_ImageSend_32b4Byte_YCbCr422(u8 mode)
{
	u8	result = 0;
	u32 *pImgData;
	u32	iSize;
	u32	hCnt, vCnt;
	imgColorData_t	pxlData;
	imgYCbCr422_96_t	ycbcr422_96;
	u8	txCount = 0;

	pImgData = (u32*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	if(imageDisplayInfo.vhReverse)
	{
		IMG_ImageReverse4Byte(pImgData, imageBufferInfo.heigth, imageBufferInfo.width);
	}

	FPGA_WriteSingle(FPGA_CMD_INPUT_RGB10_MODE, 0x00000000);

	if((pImgPreloadList->hSize_Current != imageBufferInfo.width) || (pImgPreloadList->vSize_Current != imageBufferInfo.heigth))
	{
		iSize = imageBufferInfo.width;// * 4;
		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		for(vCnt = 0; vCnt < pImgPreloadList->vSize_Current; vCnt++)
		{
			if(vCnt < imageBufferInfo.heigth)
			{
				if(mode == IMG_ARGB8888)
				{
					for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
					{
						if(hCnt < imageBufferInfo.width)
						{
							pxlData.colorData = IMG_RGB24b2YCbCr24b(pImgData[hCnt]);

							switch(txCount)
							{
								case 0:
									ycbcr422_96.y0	= pxlData.ycbcr8.y;
									ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 1:
									ycbcr422_96.y1	= pxlData.ycbcr8.y;
									txCount++;
									break;

								case 2:
									ycbcr422_96.y2	= pxlData.ycbcr8.y;
									ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 3:
									ycbcr422_96.y3	= pxlData.ycbcr8.y;
									txCount++;
									break;

								case 4:
									ycbcr422_96.y4	= pxlData.ycbcr8.y;
									ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 5:
									ycbcr422_96.y5	= pxlData.ycbcr8.y;
									FPGA_WriteDATA(ycbcr422_96.data[0]);
									FPGA_WriteDATA(ycbcr422_96.data[1]);
									FPGA_WriteDATA(ycbcr422_96.data[2]);
									FPGA_WriteDATA(ycbcr422_96.data[3]);
									txCount = 0;
									break;
							}
						}
						else
						{
							pxlData.colorData = IMG_RGB24b2YCbCr24b(0xffffffff);

							switch(txCount)
							{
								case 0:
									ycbcr422_96.y0	= pxlData.ycbcr8.y;
									ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 1:
									ycbcr422_96.y1	= pxlData.ycbcr8.y;
									txCount++;
									break;

								case 2:
									ycbcr422_96.y2	= pxlData.ycbcr8.y;
									ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 3:
									ycbcr422_96.y3	= pxlData.ycbcr8.y;
									txCount++;
									break;

								case 4:
									ycbcr422_96.y4	= pxlData.ycbcr8.y;
									ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 5:
									ycbcr422_96.y5	= pxlData.ycbcr8.y;
									FPGA_WriteDATA(ycbcr422_96.data[0]);
									FPGA_WriteDATA(ycbcr422_96.data[1]);
									FPGA_WriteDATA(ycbcr422_96.data[2]);
									FPGA_WriteDATA(ycbcr422_96.data[3]);
									txCount = 0;
									break;
							}
						}
					}
				}
				else if(mode == IMG_RGB101010)
				{
					for(hCnt = 0; hCnt < pImgPreloadList->hSize_Current; hCnt++)
					{
						if(hCnt < imageBufferInfo.width)
						{
							pxlData.colorData = IMG_RGB30b2YCbCr24b(pImgData[hCnt]);

							switch(txCount)
							{
								case 0:
									ycbcr422_96.y0	= pxlData.ycbcr8.y;
									ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 1:
									ycbcr422_96.y1	= pxlData.ycbcr8.y;
									txCount++;
									break;

								case 2:
									ycbcr422_96.y2	= pxlData.ycbcr8.y;
									ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 3:
									ycbcr422_96.y3	= pxlData.ycbcr8.y;
									txCount++;
									break;

								case 4:
									ycbcr422_96.y4	= pxlData.ycbcr8.y;
									ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 5:
									ycbcr422_96.y5	= pxlData.ycbcr8.y;
									FPGA_WriteDATA(ycbcr422_96.data[0]);
									FPGA_WriteDATA(ycbcr422_96.data[1]);
									FPGA_WriteDATA(ycbcr422_96.data[2]);
									FPGA_WriteDATA(ycbcr422_96.data[3]);
									txCount = 0;
									break;
							}
						}
						else
						{
							pxlData.colorData = IMG_RGB30b2YCbCr24b(0xffffffff);

							switch(txCount)
							{
								case 0:
									ycbcr422_96.y0	= pxlData.ycbcr8.y;
									ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 1:
									ycbcr422_96.y1	= pxlData.ycbcr8.y;
									txCount++;
									break;

								case 2:
									ycbcr422_96.y2	= pxlData.ycbcr8.y;
									ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 3:
									ycbcr422_96.y3	= pxlData.ycbcr8.y;
									txCount++;
									break;

								case 4:
									ycbcr422_96.y4	= pxlData.ycbcr8.y;
									ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
									ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
									txCount++;
									break;

								case 5:
									ycbcr422_96.y5	= pxlData.ycbcr8.y;
									FPGA_WriteDATA(ycbcr422_96.data[0]);
									FPGA_WriteDATA(ycbcr422_96.data[1]);
									FPGA_WriteDATA(ycbcr422_96.data[2]);
									FPGA_WriteDATA(ycbcr422_96.data[3]);
									txCount = 0;
									break;
							}
						}
					}
				}
			}
			else
			{
				pxlData.colorData = IMG_RGB30b2YCbCr24b(0xffffffff);

				switch(txCount)
				{
					case 0:
						ycbcr422_96.y0	= pxlData.ycbcr8.y;
						ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 1:
						ycbcr422_96.y1	= pxlData.ycbcr8.y;
						txCount++;
						break;

					case 2:
						ycbcr422_96.y2	= pxlData.ycbcr8.y;
						ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 3:
						ycbcr422_96.y3	= pxlData.ycbcr8.y;
						txCount++;
						break;

					case 4:
						ycbcr422_96.y4	= pxlData.ycbcr8.y;
						ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 5:
						ycbcr422_96.y5	= pxlData.ycbcr8.y;
						FPGA_WriteDATA(ycbcr422_96.data[0]);
						FPGA_WriteDATA(ycbcr422_96.data[1]);
						FPGA_WriteDATA(ycbcr422_96.data[2]);
						FPGA_WriteDATA(ycbcr422_96.data[3]);
						txCount = 0;
						break;
				}
			}

			pImgData += iSize;
		}

		if(txCount != 0)
		{
			FPGA_WriteDATA(ycbcr422_96.data[0]);
			FPGA_WriteDATA(ycbcr422_96.data[1]);
			FPGA_WriteDATA(ycbcr422_96.data[2]);
			FPGA_WriteDATA(ycbcr422_96.data[3]);
		}

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);
	}
	else
	{
		iSize = imageBufferInfo.width * imageBufferInfo.heigth;

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		if(mode == IMG_ARGB8888)
		{
			for(u32 cnt = 0; cnt < iSize; cnt++)
			{
				pxlData.colorData = IMG_RGB24b2YCbCr24b(pImgData[cnt]);

				switch(txCount)
				{
					case 0:
						ycbcr422_96.y0	= pxlData.ycbcr8.y;
						ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 1:
						ycbcr422_96.y1	= pxlData.ycbcr8.y;
						txCount++;
						break;

					case 2:
						ycbcr422_96.y2	= pxlData.ycbcr8.y;
						ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 3:
						ycbcr422_96.y3	= pxlData.ycbcr8.y;
						txCount++;
						break;

					case 4:
						ycbcr422_96.y4	= pxlData.ycbcr8.y;
						ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 5:
						ycbcr422_96.y5	= pxlData.ycbcr8.y;
						FPGA_WriteDATA(ycbcr422_96.data[0]);
						FPGA_WriteDATA(ycbcr422_96.data[1]);
						FPGA_WriteDATA(ycbcr422_96.data[2]);
						FPGA_WriteDATA(ycbcr422_96.data[3]);
						txCount = 0;
						break;
				}
			}
		}
		else if(mode == IMG_RGB101010)
		{
			for(u32 cnt = 0; cnt < iSize; cnt++)
			{
				pxlData.colorData = IMG_RGB30b2YCbCr24b(pImgData[cnt]);

				switch(txCount)
				{
					case 0:
						ycbcr422_96.y0	= pxlData.ycbcr8.y;
						ycbcr422_96.cb0	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr0	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 1:
						ycbcr422_96.y1	= pxlData.ycbcr8.y;
						txCount++;
						break;

					case 2:
						ycbcr422_96.y2	= pxlData.ycbcr8.y;
						ycbcr422_96.cb2	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr2	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 3:
						ycbcr422_96.y3	= pxlData.ycbcr8.y;
						txCount++;
						break;

					case 4:
						ycbcr422_96.y4	= pxlData.ycbcr8.y;
						ycbcr422_96.cb4	= pxlData.ycbcr8.cb;
						ycbcr422_96.cr4	= pxlData.ycbcr8.cr;
						txCount++;
						break;

					case 5:
						ycbcr422_96.y5	= pxlData.ycbcr8.y;
						FPGA_WriteDATA(ycbcr422_96.data[0]);
						FPGA_WriteDATA(ycbcr422_96.data[1]);
						FPGA_WriteDATA(ycbcr422_96.data[2]);
						FPGA_WriteDATA(ycbcr422_96.data[3]);
						txCount = 0;
						break;
				}
			}
		}

		if(txCount != 0)
		{
			FPGA_WriteDATA(ycbcr422_96.data[0]);
			FPGA_WriteDATA(ycbcr422_96.data[1]);
			FPGA_WriteDATA(ycbcr422_96.data[2]);
			FPGA_WriteDATA(ycbcr422_96.data[3]);
		}

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);
	}

	result = 1;

	return result;
}

u8 IMG_Preload(u16 indexNo, u16 slotNo)
{
	u8	result = 0, rtn;
	u8	jpgUse = 0;
	u8	filePath[256];
	u8	filePathCpy[256];
	u32	memAdrs;

	if(slotNo >= pImgPreloadList->maxCount)		return result;

	FM_IndexRead(FM_FILE_IMAGE, indexNo, &fmIndexImage);

	if(fmIndexImage.fState == 0)				return result;

	memcpy(filePath, fmIndexImage.fName, sizeof(filePath));
	memcpy(filePathCpy, fmIndexImage.fName, sizeof(filePathCpy));

	CMD_StrLwr(filePathCpy);

	if(strstr((char*)filePathCpy, ".jpg") != NULL)
	{
		rtn = IMG_LoadJpgFile(filePath);
		jpgUse = 1;
	}
	else if(strstr((char*)filePathCpy, ".bmp") != NULL)
	{
		rtn = IMG_LoadBmpFile(filePath);
	}
#if 0	// dsc_210302_lyh
	else if(strstr((char*)filePathCpy, ".dsc") != NULL)
	{
		rtn = IMG_LoadDscFile(imageDisplayInfo.vReverse, filePath);
	}
#endif
	else
	{
		return result;
	}

	if(rtn == 0)								return result;

	rtn = IMG_PreloadListSetSlotFileName(slotNo, filePath);
	if(rtn == 0)								return result;

	rtn = IMG_PreloadListSetSlotImageSize(slotNo, pImgPreloadList->hSize_Current, pImgPreloadList->vSize_Current);
	if(rtn == 0)								return result;

	rtn = IMG_PreloadListGetSlotAdrs(slotNo, &memAdrs);
	if(rtn == 0)								return result;

	FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_ADRS, memAdrs);

	rtn = CoreVC_VideoStreamInputEnable(2);
	if(rtn == 0)								return result;

	rtn = CoreVC_VideoStreamInputEnable(3);
	if(rtn == 0)								return result;

	if(imageDisplayInfo.ycbcrEnable)
	{
		if(imageBufferInfo.bitCnt == 24)
		{
			if(jpgUse)	rtn = IMG_ImageSend_32b4Byte_YCbCr422(IMG_ARGB8888);
			else		rtn = IMG_ImageSend_24b4Byte_YCbCr422();
		}
		else if(imageBufferInfo.bitCnt == 32)
		{
			rtn = IMG_ImageSend_32b4Byte_YCbCr422(IMG_RGB101010);
		}
	}
	else
	{
		if(imageBufferInfo.bitCnt == 24)
		{
			if(jpgUse)	rtn = IMG_ImageSend_32b4Byte(IMG_ARGB8888);
			else		rtn = IMG_ImageSend_24b4Byte();
		}
		else if(imageBufferInfo.bitCnt == 32)
		{
			rtn = IMG_ImageSend_32b4Byte(IMG_RGB101010);
		}
	}

	if(rtn == 1)
	{
		result = 1;
	}

	return result;
}

u8 IMG_PatternFill(u32 hPixel, u32 vLine, u32 color)
{
	u8	result = 0;
	u32p pImgData;
	u32	imageSize;
	u32	cnt;

	pImgData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	imageSize = hPixel * vLine;

	for(cnt = 0; cnt < imageSize; cnt++)
	{
		pImgData[cnt] = color;
	}

	result = 1;

	return result;
}

u8 IMG_PatternGradationVertical(u32 hPixel, u32 vLine, u32 mode)
{
	u8	result = 0;
	u32p pImgData;
	imgPixelColor_t pixel;
	u32	hCnt, vCnt;

	if(mode > 3)	return result;

	pImgData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	pixel.rawData = 0;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		switch(mode)
		{
			case 0:		// White
				pixel.red = pixel.green = pixel.blue = vCnt & 0x000003ff;
				break;

			case 1:		// Red
				pixel.red	= vCnt & 0x000003ff;
				break;

			case 2:		// Green
				pixel.green	= vCnt & 0x000003ff;
				break;

			case 3:		// Blue
				pixel.blue	= vCnt & 0x000003ff;
				break;
		}

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			pImgData[(vCnt * hPixel) + hCnt] = pixel.rawData;
		}
	}

	result = 1;

	return result;
}

u8 IMG_PatternGradationHorizontal(u32 hPixel, u32 vLine, u32 mode)
{
	u8	result = 0;
	u32p pImgData;
	imgPixelColor_t pixel;
	u32	hCnt, vCnt;

	if(mode > 3)	return result;

	pImgData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	pixel.rawData = 0;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			switch(mode)
			{
				case 0:		// White
					pixel.red = pixel.green = pixel.blue = hCnt & 0x000003ff;
					break;

				case 1:		// Red
					pixel.red	= hCnt & 0x000003ff;
					break;

				case 2:		// Green
					pixel.green	= hCnt & 0x000003ff;
					break;

				case 3:		// Blue
					pixel.blue	= hCnt & 0x000003ff;
					break;
			}

			pImgData[(vCnt * hPixel) + hCnt] = pixel.rawData;
		}
	}

	result = 1;

	return result;
}

u8 IMG_PatternGradationVerticalMax(u32 hPixel, u32 vLine, u32 mode)
{
	u8	result = 0;
	u32	hCnt, vCnt;
	u32	*pImgData;
	double	gain;
	imgPixelColor_t pixel;

	if(mode > 3)	return result;

	pImgData = (u32*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	gain = (double)1024 / (double)vLine;

	pixel.rawData = 0;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		switch(mode)
		{
			case 0:		// White
				pixel.red = pixel.green = pixel.blue = (u32)round((double)vCnt * gain) & 0x000003ff;
				break;

			case 1:		// Red
				pixel.red	= (u32)round((double)vCnt * gain) & 0x000003ff;
				break;

			case 2:		// Green
				pixel.green	= (u32)round((double)vCnt * gain) & 0x000003ff;
				break;

			case 3:		// Blue
				pixel.blue	= (u32)round((double)vCnt * gain) & 0x000003ff;
				break;
		}

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			pImgData[(vCnt * hPixel) + hCnt] = pixel.rawData;
		}
	}

	result = 1;

	return result;
}

u8 IMG_PatternGradationHorizontalMax(u32 hPixel, u32 vLine, u32 mode)
{
	u8	result = 0;
	u32	hCnt, vCnt;
	u32	*pImgData;
	double	gain;
	imgPixelColor_t pixel;

	if(mode > 3)	return result;

	pImgData = (u32*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	gain = (double)1024 / (double)hPixel;

	pixel.rawData = 0;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			switch(mode)
			{
				case 0:		// White
					pixel.red = pixel.green = pixel.blue = (u32)round((double)hCnt * gain) & 0x000003ff;
					break;

				case 1:		// Red
					pixel.red	= (u32)round((double)hCnt * gain) & 0x000003ff;
					break;

				case 2:		// Green
					pixel.green	= (u32)round((double)hCnt * gain) & 0x000003ff;
					break;

				case 3:		// Blue
					pixel.blue	= (u32)round((double)hCnt * gain) & 0x000003ff;
					break;
			}

			pImgData[(vCnt * hPixel) + hCnt] = pixel.rawData;
		}
	}

	result = 1;

	return result;
}

u8 IMG_PatternGradationDiagonal(u32 hPixel, u32 vLine, u32 mode)
{
	u8	result = 0;
	u32	hCnt, vCnt;
	u32	*pImgData;
	double	hGain, vGain, offset;

	imgPixelColor_t	pixel;

	if(mode > 3)			return result;

	pImgData = (u32*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	hGain = (double)1024 / (double)hPixel;
	hGain /= 2;

	vGain = (double)1024 / (double)vLine;
	vGain /= 2;

	pixel.rawData = 0;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		offset = (double)vCnt * vGain;

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			switch(mode)
			{
				case 0:		// White
					pixel.red = pixel.green = pixel.blue = (u32)round(((double)hCnt * hGain) + offset) & 0x3FF;
					break;

				case 1:		// Red
					pixel.red	= (u32)round(((double)hCnt * hGain) + offset) & 0x3FF;
					break;

				case 2:		// Green
					pixel.green	= (u32)round(((double)hCnt * hGain) + offset) & 0x3FF;
					break;

				case 3:		// Blue
					pixel.blue	= (u32)round(((double)hCnt * hGain) + offset) & 0x3FF;
					break;
			}

			pImgData[(vCnt * hPixel) + hCnt] = pixel.rawData;
		}
	}

	result = 1;

	return result;
}

u8 IMG_PatternCheckBox(u32 hPixel, u32 vLine, u32 boxPixel)
{
	u8	result = 0;
	u32p pImgData;
	u32	pixel;
	u32	hCnt, vCnt;
	u8	hColor, vColor;

	vColor = hColor = 0;

	pImgData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		if(((vCnt / boxPixel) % 2) == 0)		vColor = 0;
		else									vColor = 1;

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			if(((hCnt / boxPixel) % 2) == 0)	hColor = 0;
			else								hColor = 1;

			if(vColor == hColor)				pixel = 0x00000000;
			else if(vColor != hColor)			pixel = 0x3fffffff;

			pImgData[(vCnt * hPixel) + hCnt] =	pixel;
		}
	}

	result = 1;

	return result;
}

u8	IMG_PatternEdge(u32	hPixel, u32	vLine, u32 bgColor, u32	lineColor, u32 lineSize)
{
	u8	result = 0, rtn;
	u32p pData;
	u32	vCnt, hCnt;
	u32	lineCnt;

	rtn = IMG_PatternFill(hPixel, vLine, bgColor);

	if(rtn == 0)	return result;

	pData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			if(hCnt < lineSize)
			{
				pData[hCnt] = lineColor;
			}

			if(hCnt >= (hPixel - lineSize))
			{
				pData[hCnt] = lineColor;
			}
		}

		pData = &pData[hPixel];
	}

	pData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	for(lineCnt = 0; lineCnt < lineSize; lineCnt++)
	{
		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			pData[hCnt] = lineColor;
		}

		pData = &pData[hPixel];
	}

	for(lineCnt = (vLine - lineSize); lineCnt < vLine; lineCnt++)
	{
		pData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

		pData = &pData[hPixel * lineCnt];

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			pData[hCnt] = lineColor;
		}
	}

	result = 1;

	return result;
}

u32 IMG_ColorSelectForColorBar(u32 total, u32 pixelCnt, u8 colorCnt, u32 *pColor)
{
	u32	result;
	u32	size;
	u32	value;

	size = total / colorCnt;

	value = pixelCnt / size;

	if(value >= colorCnt)	value = colorCnt - 1;

	result = pColor[value];

	return result;
}

u8 IMG_PatternColorBar(u32	hPixel, u32	vLine, u32 horizontal, u32 colorCnt, u32 *pColor)
{
	u8		result = 0;
	u32p	pPixel;
	u32		hCnt, vCnt;
	u32		pixelColor;

	pPixel = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	if(horizontal)
	{
		for(vCnt = 0; vCnt < vLine; vCnt++)
		{
			pixelColor = IMG_ColorSelectForColorBar(vLine, vCnt, colorCnt, pColor);

			for(hCnt = 0; hCnt < hPixel; hCnt++)
			{
				pPixel[hCnt] = pixelColor;
			}

			pPixel = &pPixel[hPixel];
		}
	}
	else
	{
		for(vCnt = 0; vCnt < vLine; vCnt++)
		{
			for(hCnt = 0; hCnt < hPixel; hCnt++)
			{
				pixelColor = IMG_ColorSelectForColorBar(hPixel, hCnt, colorCnt, pColor);

				pPixel[hCnt] = pixelColor;
			}

			pPixel = &pPixel[hPixel];
		}
	}

	result = 1;

	return result;
}

u8 IMG_PatternCoreTest(u32 hPixel, u32 vLine, u8 mode, u32 errBit)
{
	u8	result = 0;
	u8	pixelMode;
	u16	colorCnt;
	u32	vCnt, hCnt;
	u32p pImgData;
	imgPixelColor_t	pixel;

	pImgData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		pixelMode	= vCnt % 6;

		pixel.rawData	= 0;
		colorCnt	= 0;

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			switch(pixelMode)
			{
				case 0:		// EVEN_R
					if((hCnt % 2) == 0)		pixel.blue	= colorCnt++;
					else					pixel.rawData	= 0;
					break;

				case 1:		// EVEN_G
					if((hCnt % 2) == 0)		pixel.green	= colorCnt++;
					else					pixel.rawData	= 0;
					break;

				case 2:		// EVEN_B
					if((hCnt % 2) == 0)		pixel.red	= colorCnt++;
					else					pixel.rawData	= 0;
					break;

				case 3:		// ODD_R
					if((hCnt % 2) == 1)		pixel.blue	= colorCnt++;
					else					pixel.rawData	= 0;
					break;

				case 4:		// ODD_G
					if((hCnt % 2) == 1)		pixel.green	= colorCnt++;
					else					pixel.rawData	= 0;
					break;

				case 5:		// ODD_B
					if((hCnt % 2) == 1)		pixel.red	= colorCnt++;
					else					pixel.rawData	= 0;
					break;

				default:
					break;
			}

			if(mode == 1)		// and
			{
				pixel.rawData &= errBit;
			}
			else if(mode == 2)	// or
			{
				pixel.rawData |= errBit;
			}

			pImgData[hCnt]	= pixel.rawData;
		}

		pImgData += hPixel;
	}

	result = 1;

	return result;
}

u8 IMG_PatternPixelTest(u32 hPixel, u32 vLine, u8 mode, u32 color)
{
	u8	result = 0;
	u8	hMode, vEO;
	u32	hCnt, vCnt;
	u32	*pImgData;
	u32	writeColor;

	switch(mode)
	{
		case 0:
			hMode	= 0;
			vEO		= 0;
			break;

		case 1:
			hMode	= 0;
			vEO		= 1;
			break;

		case 2:
			hMode	= 1;
			vEO		= 0;
			break;

		case 3:
			hMode	= 1;
			vEO		= 1;
			break;

		case 4:
			hMode	= 2;
			vEO		= 0;
			break;

		case 5:
			hMode	= 2;
			vEO		= 1;
			break;

		default:
			return result;
			break;
	}

	pImgData = (u32*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			if(vEO)
			{
				if(vCnt % 2)
				{
					switch(hMode)
					{
						case 0:		// LINE
							writeColor = color;
							break;

						case 1:		// EVEN
							if(hCnt % 2)	writeColor = 0;
							else			writeColor = color;
							break;

						case 2:		// ODD
							if(hCnt % 2)	writeColor = color;
							else			writeColor = 0;
							break;
					}
				}
				else
				{
					writeColor = 0;
				}
			}
			else
			{
				if(vCnt % 2)
				{
					writeColor = 0;
				}
				else
				{
					switch(hMode)
					{
						case 0:		// LINE
							writeColor = color;
							break;

						case 1:		// EVEN
							if(hCnt % 2)	writeColor = 0;
							else			writeColor = color;
							break;

						case 2:		// ODD
							if(hCnt % 2)	writeColor = color;
							else			writeColor = 0;
							break;
					}
				}
			}

			pImgData[hCnt] = writeColor;
		}

		pImgData = &pImgData[hPixel];
	}

	result = 1;

	return result;
}

u8 IMG_PreloadLineConfirmPattern(u32 hPixel, u32 vLine)
{
	u8	result = 0;
	u32	hCnt, vCnt;
	u32	*pImgData;
	imgPixelColor_t	pixelColor;

	pImgData = (u32*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	pixelColor.rawData = 0;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			if((vCnt % 3) == 0)				//	RED
			{
				pixelColor.red = (hCnt & 0x000003FF);
			}
			else if((vCnt % 3) == 1)		//	GREEN
			{
				pixelColor.green = (hCnt & 0x000003FF);
			}
			else if((vCnt % 3) == 2)		//	BLUE
			{
				pixelColor.blue = (hCnt & 0x000003FF);
			}

			pImgData[hCnt] = pixelColor.rawData;
		}

		pImgData = &pImgData[hPixel];
	}

	result = 1;

	return result;
}

#if 1 // yeom_20210401 [start]
u8 IMG_PatternGradationHorizontalH(u32 hPixel, u32 vLine)
{
	u8	result = 0;
	u32p pImgData;
	imgPixelColor_t pixel;
	u32	hCnt, vCnt;

	pImgData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	pixel.rawData = 0;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
		//0x3fffffff;//white
		//0x00000000;//black
#if 1 //left black
		if(hCnt < (hPixel-1023))
			pixel.red = pixel.green = pixel.blue = 0x00000000;
		else
			pixel.red = pixel.green = pixel.blue = (hCnt-(hPixel-1023)) & 0x000003ff;
			
#else //right white
		if(hCnt < 1024)
			pixel.red = pixel.green = pixel.blue = hCnt & 0x000003ff;
		else
			pixel.red = pixel.green = pixel.blue = 0x000003ff;
#endif

			pImgData[(vCnt * hPixel) + hCnt] = pixel.rawData;
		}
	}

	result = 1;

	return result;
}

u8 IMG_PatternVll(u32	hPixel, u32	vLine)
{
	u8		result = 0;
	u32p	pPixel;
	u32		hCnt, vCnt;
	u32		pixelColor;

	pPixel = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			if((hCnt % 2) == 0)		pixelColor = 0x00000000;//black
			else					pixelColor = 0x3fffffff;//white

			pPixel[hCnt] = pixelColor;
		}

		pPixel = &pPixel[hPixel];
	}

	result = 1;

	return result;
}

u8 IMG_PatternHll(u32	hPixel, u32	vLine)
{
	u8		result = 0;
	u32p	pPixel;
	u32		hCnt, vCnt;
	u32		pixelColor;

	pPixel = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		if((vCnt % 2) == 0)		pixelColor = 0x00000000;//black
		else					pixelColor = 0x3fffffff;//white

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			pPixel[hCnt] = pixelColor;
		}

		pPixel = &pPixel[hPixel];
	}

	result = 1;

	return result;
}
#endif // yeom_20210401 [end]

u8 IMG_PreloadTestPattern(void *pVoid, u16 slotNo)
{
	u8	result = 0, rtn;
	u8	nameBuffer[256];
	u32	memAdrs;
	imgPattern_t *pData;

	pData = (imgPattern_t*)pVoid;

	if(slotNo >= pImgPreloadList->maxCount)		return result;

	memset(nameBuffer, NULL, sizeof(nameBuffer));

	switch(pData->mode)
	{
		case IMG_PATTERN_MODE_FILL:				// Image Fill
			rtn = IMG_PatternFill(pData->hPixel, pData->vLine, pData->color);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=FILL,COLOR=0x%08X\0", pData->color);
			break;

		case IMG_PATTERN_MODE_GRADATION_V:		// Image Gradation Vertical
			//rtn = IMG_PatternGradationVertical(pData->hPixel, pData->vLine, pData->gMode);
			rtn = IMG_PatternGradationVerticalMax(pData->hPixel, pData->vLine, pData->gMode);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=GRADATION_V,COLOR=%d\0", pData->gMode);
			break;

		case IMG_PATTERN_MODE_GRADATION_H:		// Image Gradation Horizontal
			//rtn = IMG_PatternGradationHorizontal(pData->hPixel, pData->vLine, pData->gMode);
			rtn = IMG_PatternGradationHorizontalMax(pData->hPixel, pData->vLine, pData->gMode);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=GRADATION_H,COLOR=%d\0", pData->gMode);
			break;

		case IMG_PATTERN_MODE_GRADATION_D:
			rtn = IMG_PatternGradationDiagonal(pData->hPixel, pData->vLine, pData->gMode);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=GRADATION_D,COLOR=%d\0", pData->gMode);
			break;

		case IMG_PATTERN_MODE_CHECKBOX:			// Image CheckBox
			rtn = IMG_PatternCheckBox(pData->hPixel, pData->vLine, pData->boxSize);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=CHECKBOX,BOXSIZE=%d\0", pData->boxSize);
			break;

		case IMG_PATTERN_MODE_EDGE:				// Image Edge
			rtn = IMG_PatternEdge(pData->hPixel, pData->vLine, pData->color, pData->lineColor, pData->lineSize);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=EDGE,BGCOLOR=0x%08X,LINECOLOR=0x%08X,LINESIZE=%d\0", pData->color, pData->lineColor, pData->lineSize);
			break;

		case IMG_PATTERN_MODE_COLORBAR:			// Image ColorBar
			rtn = IMG_PatternColorBar(pData->hPixel, pData->vLine, pData->horizontal, pData->colorCnt, pData->pColor);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=COLORBAR,DIRECTION=%d,COLORCNT=%d\0", pData->horizontal, pData->colorCnt);
			break;

		case IMG_PATTERN_MODE_PIXEL_TEST:
			rtn = IMG_PatternPixelTest(pData->hPixel, pData->vLine, (u8)pData->gMode, pData->color);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=PIXELTEST\0");
			break;

		case IMG_PATTERN_MODE_LINE_CONFIRM:
			rtn = IMG_PreloadLineConfirmPattern(pData->hPixel, pData->vLine);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=LINECONFIRM\0");
			break;

		case IMG_PATTERN_MODE_APL:		// Image Apl
			rtn = IMG_PatternApl(pData->hPixel, pData->vLine, pData->gMode);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=APL,COLOR=%d\0", pData->gMode);
			break;

		case IMG_PATTERN_MODE_APLETC:		// Image Apl Etc
			rtn = IMG_PatternAplEtc(pData->hPixel, pData->vLine, pData->gMode, pData->apl_width, pData->apl_height);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=APLETC,COLOR=%d\0", pData->gMode);
			break;

		case IMG_PATTERN_MODE_GRADATION_HH:		// Image Gradation Horizontal
			rtn = IMG_PatternGradationHorizontalH(pData->hPixel, pData->vLine);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=GRADATION_H\0");
			break;
			
		case IMG_PATTERN_MODE_HLL:		// Image Hll
			rtn = IMG_PatternHll(pData->hPixel, pData->vLine);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=HLL\0");
			break;
			
		case IMG_PATTERN_MODE_VLL:		// Image Vll
			rtn = IMG_PatternVll(pData->hPixel, pData->vLine);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=VLL\0");
			break;

		case 0xffffffff:
			rtn = IMG_PatternCoreTest(pData->hPixel, pData->vLine, (u8)pData->gMode, pData->color);
			sprintf((char*)nameBuffer, "INSIDE_PATTERN_IMAGE,TYPE=CORETEST\0");
			break;

		default:
			return result;
			break;
	}

	if(rtn == 0)		return result;

	imageBufferInfo.heigth		= pData->vLine;
	imageBufferInfo.width		= pData->hPixel;
	imageBufferInfo.size		= pData->vLine * pData->hPixel * 4;
	imageBufferInfo.bitCnt		= 32;

	rtn = IMG_PreloadListSetSlotFileName(slotNo, nameBuffer);
	if(rtn == 0)		return result;

	rtn = IMG_PreloadListSetSlotImageSize(slotNo, pImgPreloadList->hSize_Current, pImgPreloadList->vSize_Current);
	if(rtn == 0)		return result;

	rtn = IMG_PreloadListGetSlotAdrs(slotNo, &memAdrs);
	if(rtn == 0)		return result;

	FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_ADRS, memAdrs);

	if(imageDisplayInfo.ycbcrEnable)
	{
		rtn = IMG_ImageSend_32b4Byte_YCbCr422(IMG_RGB101010);
	}
	else
	{
		rtn = IMG_ImageSend_32b4Byte(IMG_RGB101010);
	}

	rtn = CoreVC_VideoStreamInputEnable(2);
	if(rtn == 0)								return result;
	rtn = CoreVC_VideoStreamInputEnable(3);
	if(rtn == 0)								return result;

	if(rtn == 1)
	{
		result = 1;
	}

	return result;
}

u8 IMG_PreloadFile(u16 slotNo, u8 *pPath)
{
	u8	result = 0, rtn;
	u32	iSize;
	u32	cnt;
	u32	memAdrs;
	FIL	file;
	bitCtrl32_t	pixelData;
	u8 *pSrcData;

	if(slotNo >= pImgPreloadList->maxCount)		return result;

	rtn = FatFS_FileOpen(&file, pPath);

	if(rtn != FR_OK)							return result;

	iSize = FatFS_FileSize(&file);

	rtn = FatFS_FilePtrWrite(&file, 0);

	rtn = FatFS_FileReadAll(&file, MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS);

	rtn = FatFS_FileClose(&file);

	pSrcData = (u8*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	rtn = IMG_PreloadListSetSlotFileName(slotNo, pPath);
	if(rtn == 0)								return result;

	FPGA_WriteSingle(FPGA_CMD_INPUT_RGB10_MODE, 0x00000000);

	rtn = IMG_PreloadListGetSlotAdrs(slotNo, &memAdrs);
	if(rtn == 0)								return result;

	FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_ADRS, memAdrs);

	FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);

	pixelData.u32Data = 0;
	cnt = 0;

	FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

	do{
		pixelData.u8Data[0] = *pSrcData;
		pSrcData++;
		pixelData.u8Data[1] = *pSrcData;
		pSrcData++;
		pixelData.u8Data[2] = *pSrcData;
		pSrcData++;

		FPGA_WriteDATA(pixelData.u32Data);

		cnt += 3;

		if(cnt >= iSize)	break;
	}while(1);

	FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);

	result = 1;

	return result;
}

#if 1 // yeom_20200828 [start]
u8 IMG_PatternApl(u32 hPixel, u32 vLine, u32 mode)
{
	u8	result = 0;
	u32p pImgData;
	u32	pixel;
	u32	hCnt, vCnt;
	u8	hColor, vColor;
	u32 row_start;
	u32 row_end;
	u32 column_start;
	u32 column_end;

	vColor = hColor = 0;

	if(mode > 19)	return result;

	pImgData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	switch(mode)
	{
		case 0:		// apl5
			row_start = 1076;
			row_end = 1456;
			column_start = 395;
			column_end = 775;
			break;
		case 1:		// apl10
			row_start = 997;
			row_end = 1534;
			column_start = 316;
			column_end = 853;
			break;
		case 2:		// apl15
			row_start = 937;
			row_end = 1595;
			column_start = 256;
			column_end = 914;
			break;
		case 3:		// apl20
			row_start = 886;
			row_end = 1646;
			column_start = 205;
			column_end = 965;
			break;
		case 4:		// apl25
			row_start = 841;
			row_end = 1691;
			column_start = 160;
			column_end = 1010;
			break;
		case 5:		// apl30
			row_start = 895;
			row_end = 1637;
			column_start = 1;
			column_end = 1170;
			break;
		case 6:		// apl35
			row_start = 883;
			row_end = 1699;
			column_start = 1;
			column_end = 1170;
			break;
		case 7:		// apl40
			row_start = 771;
			row_end = 1761;
			column_start = 1;
			column_end = 1170;
			break;
		case 8:		// apl45
			row_start = 709;
			row_end = 1822;
			column_start = 1;
			column_end = 1170;
			break;
		case 9:		// apl50
			row_start = 647;
			row_end = 1884;
			column_start = 1;
			column_end = 1170;
			break;
		case 10:		// apl55
			row_start = 585;
			row_end = 1946;
			column_start = 1;
			column_end = 1170;
			break;
		case 11:		// apl60
			row_start = 523;
			row_end = 2008;
			column_start = 1;
			column_end = 1170;
			break;
		case 12:		// apl65
			row_start = 461;
			row_end = 2070;
			column_start = 1;
			column_end = 1170;
			break;
		case 13:		// apl70
			row_start = 399;
			row_end = 2132;
			column_start = 1;
			column_end = 1170;
			break;
		case 14:		// apl75
			row_start = 338;
			row_end = 2194;
			column_start = 1;
			column_end = 1170;
			break;
		case 15:		// apl80
			row_start = 276;
			row_end = 2256;
			column_start = 1;
			column_end = 1170;
			break;
		case 16:		// apl85
			row_start = 214;
			row_end = 2318;
			column_start = 1;
			column_end = 1170;
			break;
		case 17:		// apl90
			row_start = 152;
			row_end = 2380;
			column_start = 1;
			column_end = 1170;
			break;
		case 18:		// apl95
			row_start = 90;
			row_end = 2442;
			column_start = 1;
			column_end = 1170;
			break;
		case 19:		// apl100
			row_start = 1;
			row_end = 2532;
			column_start = 1;
			column_end = 1170;
			break;
	}

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		if((vCnt < row_start) || (vCnt > row_end))				vColor = 0;
		else													vColor = 1;

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			if((hCnt < column_start) || (hCnt > column_end))	hColor = 0;
			else												hColor = 1;

			if((vColor == 1) && (hColor == 1))					pixel = 0x3fffffff;//white
			else												pixel = 0x00000000;//black

			pImgData[(vCnt * hPixel) + hCnt] =	pixel;
		}
	}
	result = 1;

	return result;
}
#endif // yeom_20200828 [end]

#if 1 // yeom_20200916 [start]
u8 IMG_PatternAplEtc(u32 hPixel, u32 vLine, u32 mode, u32 width, u32 height)
{
	u8	result = 0;
	u32p pImgData;
	u32	pixel;
	u32	hCnt, vCnt;
	u8	hColor, vColor;
	u32 row_start;
	u32 row_end;
	u32 column_start;
	u32 column_end;

	vColor = hColor = 0;

	if(mode > 19)	return result;

	pImgData = (u32p)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	switch(mode)
	{
		case 0:		// apl5
			row_start = 1076;
			row_end = row_start + height;
			column_start = 395;
			column_end = column_start + width;
			break;
		case 1:		// apl10
			row_start = 997;
			row_end = row_start + height;
			column_start = 316;
			column_end = column_start + width;
			break;
		case 2:		// apl15
			row_start = 937;
			row_end = row_start + height;
			column_start = 256;
			column_end = column_start + width;
			break;
		case 3:		// apl20
			row_start = 886;
			row_end = row_start + height;
			column_start = 205;
			column_end = column_start + width;
			break;
		case 4:		// apl25
			row_start = 841;
			row_end = row_start + height;
			column_start = 160;
			column_end = column_start + width;
			break;
		case 5:		// apl30
			row_start = 895;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 6:		// apl35
			row_start = 883;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 7:		// apl40
			row_start = 771;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 8:		// apl45
			row_start = 709;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 9:		// apl50
			row_start = 647;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 10:		// apl55
			row_start = 585;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 11:		// apl60
			row_start = 523;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 12:		// apl65
			row_start = 461;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 13:		// apl70
			row_start = 399;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 14:		// apl75
			row_start = 338;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 15:		// apl80
			row_start = 276;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 16:		// apl85
			row_start = 214;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 17:		// apl90
			row_start = 152;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 18:		// apl95
			row_start = 90;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
		case 19:		// apl100
			row_start = 1;
			row_end = row_start + height;
			column_start = 1;
			column_end = column_start + width;
			break;
	}

	for(vCnt = 0; vCnt < vLine; vCnt++)
	{
		if((vCnt < row_start) || (vCnt > row_end))				vColor = 0;
		else													vColor = 1;

		for(hCnt = 0; hCnt < hPixel; hCnt++)
		{
			if((hCnt < column_start) || (hCnt > column_end))	hColor = 0;
			else												hColor = 1;

			if((vColor == 1) && (hColor == 1))					pixel = 0x3fffffff;//white
			else												pixel = 0x00000000;//black

			pImgData[(vCnt * hPixel) + hCnt] =	pixel;
		}
	}
	result = 1;

	return result;
}
#endif // yeom_20200916 [end]


u8 IMG_DisplayRefleshAll()
{
  u8	result = 0, errCnt = 0;

  if(errCnt == 0)		result = 1;

  return result;
}

u8 IMG_DisplayRefleshSelect(u8 channel)
{
  u8	result = 0, errCnt = 0;

  if(errCnt == 0)		result = 1;

  return result;
}

u8 IMG_SetFrameMode(u32 frameMode)
{
	u8	result = 0;

	if(frameMode == IMAGE_DISPLAY_FRAME_MODE_DEFAULT)
	{
		IMG_SetFrameBank(2, 0);
		IMG_SetFrameBank(3, 0);
	}
	else if(frameMode == IMAGE_DISPLAY_FRAME_MODE_CUSTOM)
	{
		IMG_SetFrameBank(2, 1);
		IMG_SetFrameBank(3, 1);
	}
	else								return result;

	imageDisplayInfo.frameMode = frameMode;

	result = 1;

	return result;
}

u8 IMG_GetFrameMode(u32 *pFrameMode)
{
	u8	result = 0;

	*pFrameMode = imageDisplayInfo.frameMode;

	result = 1;

	return result;
}

u8 IMG_SetRefleshDelay(u32 delay)
{
	u8	result = 0;

//	if(delay >= 1000)		return result;

	imageDisplayInfo.refleshDelay	= delay;

	result = 1;

	return result;
}

u8 IMG_GetRefleshDelay(u32 *pDelay)
{
	u8	result = 0;

	*pDelay = imageDisplayInfo.refleshDelay;

	result = 1;

	return result;
}

u8 IMG_SetRefleshEnable(u8 enable)
{
	u8	result = 0;

	if(enable > 1)			return result;

	imageDisplayInfo.refleshEnable = enable;

	result = 1;

	return result;
}

u8 IMG_SetRefleshDelayDouble(u8 mode)
{
	u8	result = 0;

	if(mode > 1)			return result;

	imageDisplayInfo.delayDouble	= mode;

	result = 1;

	return result;
}

u8 IMG_Display(u32 slotNo)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	status;
	u32	memAdrs, delayTime;

	if(imageDisplayInfo.frameMode != IMAGE_DISPLAY_FRAME_MODE_DEFAULT)		return result;

	if(slotNo >= pImgPreloadList->maxCount)									return result;

	rtn = IMG_PreloadListCheckSlotImageSize(slotNo, &status);
	if(rtn == 0)															return result;
	if(status == 0)															return result;

	rtn = IMG_PreloadListGetSlotAdrs(slotNo, &memAdrs);
	if(rtn == 0)															return result;

	FPGA_WriteSingle(FPGA_CMD_DDR_READ_ADRS, memAdrs);

	FPGA_WriteSingle(FPGA_CMD_DDR_READ_START, 0);

	imageDisplayInfo.currentDisplayNo = slotNo;

	if(imageDisplayInfo.refleshEnable)
	{
		delayTime = (imageDisplayInfo.refleshDelay * 2) + 1;

		if(imageDisplayInfo.delayDouble)	delayTime *= 2;

		HAL_Delay(delayTime);

		rtn = IMG_DisplayRefleshAll();
		if(rtn == 0)														errCnt += 1;
	}

	if(errCnt == 0)															result = 1;

	return result;
}

u8 IMG_SetFrameBank(u8 channel, u16 bankNo)
{
	u8	result = 0, rtn, errCnt = 0;

	rtn = DPC_DirectRegWriteSingle(channel, CORE_REG_VIDEO_STREAM_INPUT_MEMORY_ADDRESS, bankNo);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 IMG_DisplayCustom(u8 channel, u32 slotNo)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	status;
	u32	memAdrs, regData;

	if(imageDisplayInfo.frameMode != IMAGE_DISPLAY_FRAME_MODE_CUSTOM)		return result;

	if(channel >= BOARD_CHANNEL_MAX_COUNT)									return result;

	rtn = IMG_PreloadListCheckSlotImageSize(slotNo, &status);
	if(rtn == 0)															return result;
	if(status == 0)															return result;

	rtn = IMG_PreloadListGetSlotAdrs(slotNo, &memAdrs);
	if(rtn == 0)															return result;

	rtn = IMG_SetFrameBank(channel, 0);
	if(rtn == 0)			errCnt += 1;

	//	PG Image Output
	FPGA_WriteSingle(FPGA_CMD_DDR_READ_ADRS, memAdrs);

	FPGA_WriteSingle(FPGA_CMD_DDR_READ_START, 0);

	if(Trigger == ON)	// Trigger Mode
	{
		FPGA_WriteSingle(FPGA_CMD_PG_1FRAME_TRIGGER, 1);	// Trigger ON

		do{
			regData = FPGA_ReadSingle(FPGA_CMD_PG_1FRAME_TRIGGER);

			if(regData == 1)		break;

		}while(1);
		FPGA_WriteSingle(FPGA_CMD_PG_1FRAME_TRIGGER, 0x02);
	}
	else
		HAL_Delay((imageDisplayInfo.refleshDelay * 2) + 1);

	rtn = IMG_SetFrameBank(channel, 1);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 IMG_DisplayPrev()
{
	u8	result = 0, rtn, status, remain = 0;
	u32	imgCnt;
	u32	imageNo, maxCount;

	imageNo = imageDisplayInfo.currentDisplayNo;

	IMG_PreloadListGetMaxCount(&maxCount);

	rtn = IMG_PreloadListCheckImage(&imgCnt);
	if(rtn == 0)			return result;

	if(imgCnt == 0)			return result;

	do{
		if(imageNo == 0)
		{
			if(remain != 0)		break;

			imageNo = maxCount;
			remain = 1;
		}

		imageNo -= 1;

		rtn = IMG_PreloadListCheckSlotImageSize(imageNo, &status);
		if((rtn == 1) && (status == 1))		break;
	}while(1);

	result = IMG_Display(imageNo);

	if(rolling_mode == 1)  	    rolling_start(imageNo);

	return result;
}

u8 IMG_DisplayNext()
{
	u8	result = 0, rtn, status, remain = 0;
	u32	imgCnt;
	u32	imageNo, maxCount;

	imageNo = imageDisplayInfo.currentDisplayNo;

	IMG_PreloadListGetMaxCount(&maxCount);

	rtn = IMG_PreloadListCheckImage(&imgCnt);
	if(rtn == 0)			return result;

	if(imgCnt == 0)			return result;

	do{
		imageNo += 1;

		if(imageNo >= maxCount)
		{
			imageNo = 0;

			if(remain != 0)		break;
			else				remain = 1;
		}

		rtn = IMG_PreloadListCheckSlotImageSize(imageNo, &status);
		if((rtn == 1) && (status == 1))		break;
	}while(1);

	result = IMG_Display(imageNo);

	if(rolling_mode == 1)  	    rolling_start(imageNo);

	return result;
}

u8 IMG_RepeatEnable(u32 duration)
{
	u8	result = 0;

	if(duration > 30000)			return result;

	imgRepeat.startTime	= HAL_GetTick();

	imgRepeat.enable	= 1;

	imgRepeat.duration	= duration;

	imgRepeat.startImg	= 0xffffffff;

	imgRepeat.endImg	= 0xffffffff;

	result = 1;

	return result;
}

u8 IMG_RepeatEnableSelect(u32 duration, u32 sImg, u32 eImg)
{
	u8	result = 0;

	if(duration > 30000)			return result;

	imgRepeat.startTime	= HAL_GetTick();

	imgRepeat.enable	= 1;

	imgRepeat.duration	= duration;

	imgRepeat.startImg	= sImg;

	imgRepeat.endImg	= eImg;

	return result;
}

u8 IMG_RepeatDisable()
{
	u8	result = 0;

	imgRepeat.startTime	= 0;

	imgRepeat.enable	= 0;

	imgRepeat.duration	= 0;

	imgRepeat.startImg	= 0xffffffff;

	imgRepeat.endImg	= 0xffffffff;

	result = 1;

	return result;
}

u8 IMG_DisplayNext_ForRepeat()
{
	u8	result = 0, rtn, status, remain = 0;
	u32	imageChkr, imageNo, maxCount;

	rtn = IMG_PreloadListCheckImage(&imageChkr);
	if(rtn == 0)
	{
		IMG_RepeatDisable();

		return result;
	}

	imageNo = imageDisplayInfo.currentDisplayNo;

	rtn = IMG_PreloadListGetMaxCount(&maxCount);
	if(rtn == 0)				return result;

	if((imgRepeat.startImg == 0xFFFFFFFF) && (imgRepeat.endImg == 0xFFFFFFFF))
	{
		do{
			imageNo += 1;

			if(imageNo >= maxCount)
			{
				imageNo = 0;

				if(remain != 0)		break;
				else				remain = 1;
			}

			rtn = IMG_PreloadListGetSlotStatus(imageNo, &status);
			if(rtn == 0)				return result;

			if(status != 0)				break;
		}while(1);
	}
	else
	{
		do{
			imageNo += 1;

			if(imageNo > imgRepeat.endImg)
			{
				imageNo = imgRepeat.startImg;

				if(remain != 0)		break;
				else				remain = 1;
			}

			rtn = IMG_PreloadListGetSlotStatus(imageNo, &status);
			if(rtn == 0)				return result;

			if(status != 0)				break;
		}while(1);
	}

	result = IMG_Display(imageNo);

	return result;
}

u8 IMG_RepeatProcess()
{
	u8	result = 0;
	u32	rTime;

	if(imgRepeat.enable == 0)	return result;

	rTime = HAL_GetTick();

	if((rTime - imgRepeat.startTime) < imgRepeat.duration)	return result;

	IMG_DisplayNext_ForRepeat();

	imgRepeat.startTime = HAL_GetTick();

	result = 1;

	return result;
}

u8 IMG_WriteChar(u32 adrs, u16 h, u16 v, u16 ph, u16 pv, u8 mag, u32 color, u8 font, u8 data)
{
	u8	result = 0;
	u8p	pFontData;
	u32p	pImgData, pWriteData;
	u32	startPos;
	u32	vMag, hMag;
	u32	vMax, hMax;
	u32	vCnt, hCnt;
	u32	vPos, hPos;
	u32	fontLine0 = 0, fontLine1 = 0;
	u8	fontShift0, fontShift1;

	if(mag == 0)	mag = 1;
	if(data < 0x20)	data = 0x20;

	data -= 0x20;

	if(font == 0)
	{
		pFontData = (u8p)&fontConsolas_32.fontData[data * fontConsolas_32.height * (fontConsolas_32.width / 8)];

		vMax = fontConsolas_32.height;
		hMax = fontConsolas_32.width;

		startPos = (h * pv) + ph;

		pImgData = (u32p)adrs;

		vPos = startPos;

		for(vCnt = 0; vCnt < vMax; vCnt++)
		{
			fontLine0 = pFontData[vCnt * 4];
			fontLine0 <<= 8;
			fontLine0 |= pFontData[vCnt * 4 + 1];
			fontLine0 <<= 8;
			fontLine0 |= pFontData[vCnt * 4 + 2];
			fontLine0 <<= 8;
			fontLine0 |= pFontData[vCnt * 4 + 3];

			pWriteData = &pImgData[vPos];

			hPos = 0;

			for(vMag = 0; vMag < mag; vMag++)
			{
				fontShift0 = 31;

				for(hCnt = 0; hCnt < hMax; hCnt++)
				{
					for(hMag = 0; hMag < mag; hMag++)
					{
						if((fontLine0 >> fontShift0) & 0x00000001)
						{
							pWriteData[hPos + (hCnt * mag) + hMag] = color;
						}
					}

					fontShift0--;
				}

				hPos += h;
			}
			vPos += (h * mag);
		}
	}
	else
	{
		pFontData = (u8p)&fontVerdana_64.fontData[data * fontVerdana_64.height * (fontVerdana_64.width / 8)];

		vMax = fontVerdana_64.height;
		hMax = fontVerdana_64.width;

		startPos = (h * pv) + ph;

		pImgData = (u32p)adrs;

		vPos = startPos;

		for(vCnt = 0; vCnt < vMax; vCnt++)
		{
			fontLine0 = pFontData[vCnt * 8];
			fontLine0 <<= 8;
			fontLine0 |= pFontData[vCnt * 8 + 1];
			fontLine0 <<= 8;
			fontLine0 |= pFontData[vCnt * 8 + 2];
			fontLine0 <<= 8;
			fontLine0 |= pFontData[vCnt * 8 + 3];

			fontLine1 = pFontData[vCnt * 8 + 4];
			fontLine1 <<= 8;
			fontLine1 |= pFontData[vCnt * 8 + 5];
			fontLine1 <<= 8;
			fontLine1 |= pFontData[vCnt * 8 + 6];
			fontLine1 <<= 8;
			fontLine1 |= pFontData[vCnt * 8 + 7];

			pWriteData = &pImgData[vPos];

			hPos = 0;

			for(vMag = 0; vMag < mag; vMag++)
			{
				fontShift0 = 31;
				fontShift1 = 31;

				for(hCnt = 0; hCnt < hMax; hCnt++)
				{
					for(hMag = 0; hMag < mag; hMag++)
					{
						if(hCnt < 32)
						{
							if((fontLine0 >> fontShift0) & 0x00000001)
							{
								pWriteData[hPos + (hCnt * mag) + hMag] = color;
							}
						}
						else
						{
							if((fontLine1 >> fontShift1) & 0x00000001)
							{
								pWriteData[hPos + (hCnt * mag) + hMag] = color;
							}
						}
					}

					if(hCnt < 32)		fontShift0--;
					else				fontShift1--;
				}

				hPos += h;
			}
			vPos += (h * mag);
		}
	}

	result = 1;

	return result;
}

u8 IMG_WriteString(u32 adrs, u16 h, u16 v, u16 ph, u16 pv, u8 mag, u32 color, u8 font, u8 *pData, u16 length)
{
	u8	result = 0, rtn;
	u16	cnt;
	u16	hPos;

	if(mag == 0)	mag = 1;

	hPos = ph;

	for(cnt = 0; cnt < length; cnt++)
	{
		rtn = IMG_WriteChar(adrs, h, v, hPos, pv, mag, color, font, pData[cnt]);

		if(font == 0)		hPos += (fontConsolas_32.width * mag);
		else				hPos += (fontVerdana_64.width * mag);
	}

	result = rtn;

	return result;
}

u8 IMG_StringPrintf(void *pVoid, const char *pData, ...)
{
	u8	result = 0, rtn;
	va_list	ap;
	char	string[128];
	u16		length;
	imgStringInfo_t *pInfo;
	u16	hPos;

	pInfo = (imgStringInfo_t*)pVoid;

	memset(string, NULL, sizeof(string));

	va_start(ap, pData);

	length = vsprintf(string, pData, ap);

	va_end(ap);

	if(pInfo->font == 0)
	{
		hPos = (pInfo->h / 2) - ((length * (fontConsolas_32.width * pInfo->mag)) / 2);
	}
	else
	{
		hPos = (pInfo->h / 2) - ((length * (fontVerdana_64.width * pInfo->mag)) / 2);
	}

	rtn = IMG_WriteString(pInfo->imgAdrs, pInfo->h, pInfo->v, hPos, pInfo->pv, pInfo->mag, pInfo->color, pInfo->font, (u8*)string, length);

	result = rtn;

	return result;
}

u8 IMG_PreloadString(u16 slotNo, void *pVoid)
{
	u8	result = 0, rtn;
	u8	cnt;
	u16	vPos;
	u32	memAdrs;
	imgString_t *pData;
	imgStringInfo_t info;

	pData = (imgString_t*)pVoid;

	if(slotNo >= pImgPreloadList->maxCount)				return result;

	if(pData->strCnt < 1)								return result;
	if(pData->strCnt > 5)								return result;

	rtn = IMG_PatternFill(pImgPreloadList->hSize_Current, pImgPreloadList->vSize_Current, pData->bgColor);
	if(rtn == 0)										return result;

	if(pData->strCnt == 1)
	{
		vPos = (pImgPreloadList->vSize_Current / 2) - ((pData->firstSize * fontVerdana_64.height) / 2);
	}
	else
	{
		vPos = (pImgPreloadList->vSize_Current / 4) - ((pData->firstSize * fontVerdana_64.height) / 2);
	}

	if(vPos > (pImgPreloadList->vSize_Current / 2))		return result;

	info.imgAdrs	= MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;
	info.h			= pImgPreloadList->hSize_Current;
	info.v			= pImgPreloadList->vSize_Current;
	info.pv			= vPos;
	info.mag		= pData->firstSize;
	info.color		= pData->firstColor;
	info.font		= 1;

	rtn = IMG_StringPrintf(&info, (char*)pData->pString[0]);


	vPos = (pImgPreloadList->vSize_Current / 2);

	if(pData->strCnt != 1)
	{
		vPos += (pImgPreloadList->vSize_Current / 10);
	}

	info.mag		= pData->otherSize;
	info.color		= pData->otherColor;
	info.font		= 0;

	for(cnt = 1; cnt < pData->strCnt; cnt++)
	{
		info.pv			= vPos;

		rtn = IMG_StringPrintf(&info, (char*)pData->pString[cnt]);

		vPos += (pData->otherSize * fontConsolas_32.height);
	}

	if(rtn == 0)		return result;

	imageBufferInfo.heigth		= pImgPreloadList->vSize_Current;
	imageBufferInfo.width		= pImgPreloadList->hSize_Current;
	imageBufferInfo.size		= pImgPreloadList->vSize_Current * pImgPreloadList->hSize_Current * 4;
	imageBufferInfo.bitCnt		= 32;

	rtn = IMG_PreloadListSetSlotFileName(slotNo, "INSIDE_PATTERN_IMAGE,TYPE=STRING\0");
	if(rtn == 0)		return result;

	rtn = IMG_PreloadListSetSlotImageSize(slotNo, pImgPreloadList->hSize_Current, pImgPreloadList->vSize_Current);
	if(rtn == 0)		return result;

	rtn = IMG_PreloadListGetSlotAdrs(slotNo, &memAdrs);
	if(rtn == 0)		return result;

	FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_ADRS, memAdrs);

	rtn = IMG_ImageSend_32b4Byte(IMG_RGB101010);
	if(rtn == 1)
	{
		result = 1;
	}

	return result;
}

u8 DBVConfig(u8 mode, u8 type, u32 value)
{
	u8	result = 0;

	rolling_mode = mode;
	rolling_type = type;
	rolling_value = (u16)value;

	result = 1;

	return result;
}

u8 DBVSet(u8 slot, u32 value)
{
	u8 result = 0;

	slot = slot - 1;

	rolling_set[slot] = value;

	result = 1;

	return result;
}

u8 DBVClear()
{
	u8 result = 0;

	rolling_mode = 0;
	rolling_type = 0;
	rolling_value = 0;

	for(u8 km = 0; km < 32; km++)
	{
		rolling_set[km] = 0;
	}

	result = 1;

	return result;
}

void rolling_start(u32 slot)
{
	if(rolling_set[slot] != 0)
	{
		if(rolling_type == 1)
			sdp_rolling(rolling_set[slot]);
		else if(rolling_type == 2)
			aux_rolling(rolling_set[slot]);
	}
	else if(rolling_set[slot] == 0)
	{
		if(rolling_type==1)
			sdp_rolling(rolling_value);
		else if(rolling_type==2)
			aux_rolling(rolling_value);
	}
}

void sdp_rolling(u16 sdp_value)
{
	sdp_packet[9] = (u8)((sdp_value&0x00ff));
	sdp_packet[10] = (u8)((sdp_value&0xff00)>>8);

	for(u8 kk = 3; kk < 13; kk++)
	{
		DPTX_SecondaryDataPacketWrite(kk, sdp_packet, 36);
	}
}

void aux_rolling(u16 aux_value)
{
	dptxRegData_aux.regAdrs		= 0x51;
	dptxRegData_aux.dataAdrs	= 0x00;
	dptxRegData_aux.dataSize	= 32;

	aux_packet[5] = (u8)((aux_value&0x00ff));
	aux_packet[6] = (u8)((aux_value&0xff00)>>8);

	dptxRegData_aux.pBuffer=  aux_packet;
	for(u8 kk = 3; kk < 13; kk++)
	{
		dptxRegData_aux.channel		= kk;
		DPTX_RegDataWriteDIC(&dptxRegData_aux);
	}
}

u8 SetTriggerMode(u8 channel, u8 state)
{
	u8	result = 0;

	if(state == ON)
	{
		Trigger = ON;
		if(channel == 0x00)
		{
			for(int i = 2; i < 4; i++)
			{
				DPC_DirectRegWriteSingle(i, CORE_REG_TRIGGER_MODE, 0x01);
			}
		}
		else
		{
			DPC_DirectRegWriteSingle(channel + 1, CORE_REG_TRIGGER_MODE, 0x01);
		}
		FPGA_WriteSingle(FPGA_CMD_PG_1FRAME_TRIGGER_MODE, 1);
	}
	else
	{
		Trigger = OFF;
		if(channel == 0x00)
		{
			for(int i = 2; i < 4; i++)
			{
				DPC_DirectRegWriteSingle(i, CORE_REG_TRIGGER_MODE, 0x00);
			}
		}
		else
		{
			DPC_DirectRegWriteSingle(channel + 1, CORE_REG_TRIGGER_MODE, 0x00);
		}
		FPGA_WriteSingle(FPGA_CMD_PG_1FRAME_TRIGGER_MODE, 0);
	}

	result = 1;

	return result;
}

u8 TriggerStart(u8 channel)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData, mask = 0x00000002;

	if(channel == 0x00)
	{
		for(int i = 2; i < 4; i++)
		{
			rtn = DPC_DirectRegReadSingle(i, CORE_REG_TRIGGER_MODE, &regData);
			if(rtn == 0)				errCnt += 1;

			regData |= mask;

			rtn = DPC_DirectRegWriteSingle(i, CORE_REG_TRIGGER_MODE, regData);
			if(rtn == 0)					errCnt += 1;
		}
	}
	else
	{
		rtn = DPC_DirectRegReadSingle(channel + 1, CORE_REG_TRIGGER_MODE, &regData);
		if(rtn == 0)				errCnt += 1;

		regData |= mask;

		rtn = DPC_DirectRegWriteSingle(channel + 1, CORE_REG_TRIGGER_MODE, regData);
		if(rtn == 0)					errCnt += 1;
	}

	FPGA_WriteSingle(FPGA_CMD_PG_1FRAME_TRIGGER, 1);

	if(errCnt == 0)					result = 1;

	return result;
}

u8 DisplayTimeoutInit(u8 channel, u32 time)
{
	u8 result = 0;

	if(channel == 1)
	{
		Timeout[0] = time * 1000;	// msec
		TimerSet[0] = OFF;
	}
	else if(channel == 2)
	{
		Timeout[1] = time * 1000;	// msec
		TimerSet[1] = OFF;
	}

	result = 1;

	return result;
}

u8 DisplayTimerSet(u8 channel, u8 mode)
{
	u8 result = 0;

	if(channel == 1)
	{
		TimerSet[0] = mode;
		if(mode == 0)	Timer_start[0] = 0;
		else 			Timer_start[0] = HAL_GetTick();
	}		
	else if(channel == 2)
	{
		TimerSet[1] = mode;
		if(mode == 0)	Timer_start[1] = 0;
		else 			Timer_start[1] = HAL_GetTick();
	}
	else					return result;	

	result = 1;

	return result;
}

void DisplayTimerFunction()
{
	u32 cur_time;

	if(TimerSet[0] == OFF && TimerSet[1] == OFF)		return;

	cur_time = HAL_GetTick();

	if(TimerSet[0] == ON)
	{
		if(cur_time - Timer_start[0] > Timeout[0])
		{
			PWR_OutputInit(2, PWR_INIT_TYPE_ALL); 
			
			TimerSet[0] = OFF;
		}
	}
	
	if(TimerSet[1] == ON)
	{
		if(cur_time - Timer_start[1] > Timeout[1])
		{
			PWR_OutputInit(3, PWR_INIT_TYPE_ALL);

			TimerSet[1] = OFF;
		}
	}	
}