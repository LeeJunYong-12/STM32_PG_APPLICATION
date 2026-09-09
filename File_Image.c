#include	"File_Image.h"

void FIMG_MatchBMP(bmpHeader_t *pFile, void *pData)
{
	memcpy(pFile->data, pData, sizeof(bmpHeader_t));

	pFile->iData = (u32*)((u32*)pData + pFile->hFile.fOffBits);
}

u8 FIMG_BmpHeaderChecker(imageBufferInfo_t *pImgInfo, bmpHeader_t *pBmp)
{
	if((pBmp->hFile.fType[0] != 'B') || (pBmp->hFile.fType[1] != 'M'))	return 0;

	pImgInfo->width		= pBmp->hImage.iWidth;
	pImgInfo->heigth	= pBmp->hImage.iHeight;
	pImgInfo->bitCnt	= pBmp->hImage.iBitCnt;
	pImgInfo->size		= pImgInfo->width * pImgInfo->heigth * pImgInfo->bitCnt / 8;

	return 1;
}

u32 FIMG_BMPHeaderMaker(JPEG_ConfTypeDef *imgInfo, bmpHeader_t *pBmp, u32 iAdrs)
{
	u8 *startAdrs;
	u8 cnt;
	u32	hOffset = 0;

	startAdrs = (u8*)iAdrs;

	pBmp->hFile.fType[0]			= 'B';
	pBmp->hFile.fType[1]			= 'M';

	pBmp->hFile.fOffBits			= sizeof(bmpFileHeader_t) + sizeof(bmpImageHeader_t) + 2;

	pBmp->hImage.iSize				= sizeof(bmpImageHeader_t);
	pBmp->hImage.iWidth				= imgInfo->ImageWidth;
	pBmp->hImage.iHeight			= imgInfo->ImageHeight;
	pBmp->hImage.iPlanes			= 1;
	pBmp->hImage.iBitCnt			= 24;
	pBmp->hImage.iCompression		= 0;
	if(imgInfo->ImageWidth % 4)
	{
		hOffset	= (4 - (imgInfo->ImageWidth % 4));
	}
	pBmp->hImage.iSizeImage			= (pBmp->hImage.iWidth + hOffset) * pBmp->hImage.iHeight * pBmp->hImage.iBitCnt / 8;
	pBmp->hImage.iXppm				= 0;
	pBmp->hImage.iYppm				= 0;
	pBmp->hImage.iColorUsed			= 0;
	pBmp->hImage.iColorImportant	= 0;

	pBmp->hFile.fSize				= pBmp->hImage.iSizeImage + pBmp->hFile.fOffBits;

	pBmp->iData						=(u32*)iAdrs;

	startAdrs -= (u8)pBmp->hFile.fOffBits;

	pBmp->dummy[0]					= 0;
	pBmp->dummy[1]					= 0;

	for(cnt = 0; cnt < pBmp->hFile.fOffBits; cnt++)
	{
		startAdrs[cnt] = pBmp->data[cnt];
	}

	return (u32)startAdrs;
}

void FIMG_ImageCpy(u32 dData, u32 sData, u32 size)
{
	u8 *pdData, *psData;
	u32 cnt = 0;

	pdData = (u8*)dData;
	psData = (u8*)sData;

	for(cnt = 0; cnt < size; cnt++)
	{
		pdData[cnt] = psData[cnt];
	}
}

void FIMG_rgb2bmp(imageBufferInfo_t *pInfo, u32 dstAdrs, u32 srcAdrs)
{
}

void FIMG_bmp2rgb(imageBufferInfo_t *pInfo, u32 dstAdrs, u32 srcAdrs)
{
	u32 imageHeight, imageWidth;
	u32 wCnt, hCnt, hOffset = 0;
	u32 iCnt, pixelCnt;
	u8 *pDstAdrs8b, *pSrcAdrs8b;

	imageHeight	= pInfo->heigth;
	imageWidth	= pInfo->width;

	pixelCnt = imageWidth * 3;

	iCnt = pixelCnt;

	if(pixelCnt % 4)
	{
		hOffset = (4 - (pixelCnt % 4));
	}

	pDstAdrs8b = (u8*)(dstAdrs + pInfo->size);
	pSrcAdrs8b = (u8*)srcAdrs;

	for(hCnt = 0; hCnt < imageHeight; hCnt++)
	{
		pDstAdrs8b -= pixelCnt;

		for(wCnt = 0; wCnt < iCnt; wCnt++)
		{
			pDstAdrs8b[wCnt] =	pSrcAdrs8b[wCnt];
		}

		pSrcAdrs8b += (pixelCnt + hOffset);
	}
}

void FIMG_dsc2rgb(imageBufferInfo_t *pInfo, u32 dstAdrs, u32 srcAdrs, u32 mode)
{
	u32 imageHeight, imageWidth;
	u32 wCnt, hCnt;
	u32 iCnt, pixelCnt;
	u8 *pDstAdrs8b, *pSrcAdrs8b;
	u8	buffer[3];

	if(mode)		// REVERSE
	{
		imageHeight	= pInfo->heigth;
		imageWidth	= pInfo->width;

		pixelCnt = imageWidth * 3;

		iCnt = pixelCnt;

		pDstAdrs8b = (u8*)(dstAdrs + pInfo->size);
		pSrcAdrs8b = (u8*)srcAdrs;

		for(hCnt = 0; hCnt < imageHeight; hCnt++)
		{
			pDstAdrs8b -= pixelCnt;

			for(wCnt = 0; wCnt < iCnt; wCnt++)
			{
				pDstAdrs8b[wCnt] =	pSrcAdrs8b[wCnt];
			}

			pSrcAdrs8b += pixelCnt;
		}
	}
	else
	{
		pDstAdrs8b = (u8*)dstAdrs;
		pSrcAdrs8b = (u8*)srcAdrs;

		imageHeight	= pInfo->heigth;
		imageWidth	= pInfo->width;

		pixelCnt = imageWidth * 3;

		for(hCnt = 0; hCnt < imageHeight; hCnt++)
		{
			for(wCnt = 0; wCnt < imageWidth; wCnt++)
			{
				buffer[0] = pSrcAdrs8b[(wCnt * 3) + 2];
				buffer[1] = pSrcAdrs8b[(wCnt * 3) + 1];
				buffer[2] = pSrcAdrs8b[(wCnt * 3) + 0];
				
				pDstAdrs8b[(wCnt * 3) + 0] = buffer[0];
				pDstAdrs8b[(wCnt * 3) + 1] = buffer[1];
				pDstAdrs8b[(wCnt * 3) + 2] = buffer[2];
			}

			pSrcAdrs8b += pixelCnt;
			pDstAdrs8b += pixelCnt;
		}
	}
}

