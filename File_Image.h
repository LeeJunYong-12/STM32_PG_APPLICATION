#ifndef	_FILE_IMAGE_H
#define	_FILE_IMAGE_H

#include	"E7602_PG_Application.h"

/*
	SDRAM Memory MAP
*/

#define MDT_BD_SDRAM_SIZE				((u32)0x08000000)
#define MDT_BD_SDRAM_LAST_ADRS			(SDRAM_DEVICE_ADDR + MDT_BD_SDRAM_SIZE)
#define IMG_FILE_MAX_SIZE				((u32)0x02000000)
#define KEY_FILE_SIZE					((u32)0x00200000)
#define LOG_FILE_SIZE					((u32)0x00200000)

#define	IMG_FILE_1_DUMP_ADRS			SDRAM_DEVICE_ADDR
#define IMG_FILE_2_DUMP_ADRS			(IMG_FILE_1_DUMP_ADRS + IMG_FILE_MAX_SIZE)
#define IMG_FILE_DSC_ADRS				(IMG_FILE_2_DUMP_ADRS + IMG_FILE_MAX_SIZE)

#define KEY_FILE_DUMP_ADRS				(MDT_BD_SDRAM_LAST_ADRS - KEY_FILE_SIZE - LOG_FILE_SIZE)
#define	LOG_FILE_DUMP_ADRS				(MDT_BD_SDRAM_LAST_ADRS - LOG_FILE_SIZE)

typedef	union{
	u8 data[20];
	struct{
		u32	size;
		u32	width;
		u32	heigth;
		u32	bitCnt;
		u32	pixelSize;
	};
}imageBufferInfo_t;

typedef	union{
	u8	data[14];
	__packed struct{
	u8	fType[2];
	u32	fSize;
	u8	fReserve1[2];
	u8	fReserve2[2];
	u32	fOffBits;
	};
}bmpFileHeader_t;

typedef	union{
	u8	data[40];
	__packed struct{
	u32	iSize;
	u32	iWidth;
	u32	iHeight;
	u16	iPlanes;
	u16	iBitCnt;
	u32	iCompression;
	u32	iSizeImage;
	u32	iXppm;
	u32	iYppm;
	u32	iColorUsed;
	u32	iColorImportant;
	};
}bmpImageHeader_t;

typedef	union{
	u8	data[60];
	__packed struct{
	bmpFileHeader_t		hFile;
	bmpImageHeader_t	hImage;
	u32					*iData;
	u8					dummy[2];
	};
}bmpHeader_t;

typedef	union{
	u8	u8data[12];
	u32	u32Data[3];
	__packed struct{
		u8	r1;
		u8	b1;
		u8	g1;
		u8	r2;
		u8	b2;
		u8	g2;
		u8	r3;
		u8	g3;
		u8	b3;
		u8	r4;
		u8	g4;
		u8	b4;
	};
}bmpConvert_t;

void FIMG_MatchBMP(bmpHeader_t *pFile, void *pData);
u8 FIMG_BmpHeaderChecker(imageBufferInfo_t *pImgInfo, bmpHeader_t *pBmp);
u32 FIMG_BMPHeaderMaker(JPEG_ConfTypeDef *imgInfo, bmpHeader_t *pBmp, u32 iAdrs);
void FIMG_ImageCpy(u32 dData, u32 sData, u32 size);
void FIMG_bmp2rgb(imageBufferInfo_t *pInfo, u32 dstAdrs, u32 srcAdrs);
void FIMG_dsc2rgb(imageBufferInfo_t *pInfo, u32 dstAdrs, u32 srcAdrs, u32 mode);

#endif	// _FILE_IMAGE_H