#ifndef	_APP_JPEGDECODE_H
#define	_APP_JPEGDECODE_H

#include	"E7602_PG_Application.h"

enum{
	JPEG_DECODE_NONE = 0,
	JPEG_DECODE_FROM_FILE,
	JPEG_DECODE_FROM_SDRAM
};

typedef struct
{
  uint8_t *DataBuffer;
  uint32_t DataBufferSize;
}JPEG_Data_BufferTypeDef;

/*
typedef struct _JFIFHeader
{
	BYTE SOI[2];				// 00h  Start of Image Marker
	BYTE APP0[2];				// 02h  Application Use Marker
	BYTE Length[2];				// 04h  Length of APP0 Field
	BYTE Identifier[5];			// 06h  "JFIF" (zero terminated) Id String
	BYTE Version[2];			// 07h  JFIF Format Revision
	BYTE Units;					// 09h  Units used for Resolution
	BYTE Xdensity[2];			// 0Ah  Horizontal Resolution
	BYTE Ydensity[2];			// 0Ch  Vertical Resolution
	BYTE XThumbnail;			// 0Eh  Horizontal Pixel Count
	BYTE YThumbnail;			// 0Fh  Vertical Pixel Count
}JFIFHEAD;
*/

#define	JPEG_MEM_IN_ADRS		MSD_FILE_READ_ADDR
#define	JPEG_MEM_OUT_ADRS		(MSD_FILE_READ_ADDR + MSD_FILE_MAX_SIZE)

#define CHUNK_SIZE_IN  ((uint32_t)(4096)) 
#define CHUNK_SIZE_OUT ((uint32_t)(768))

extern	JPEG_HandleTypeDef     JPEG_Handle;
extern	JPEG_ConfTypeDef       JPEG_Info;

extern	u8 jpegDecoded;

u8 JPEG_Init();
uint32_t JPEG_Decode(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress);
u8 JPEG_DecodeFromFile(FIL *pFile, u32 dstAdrs);
u8 JPEG_DecodeFromSDRAM(u32 dstAdrs, u32 srcAdrs, u32 fileSize);
void JPEG_GetInfo(JPEG_ConfTypeDef *pData);

#endif	// _APP_JPEGDECODE_H