#include	"App_JpegDecode.h"

JPEG_HandleTypeDef     JPEG_Handle;
JPEG_ConfTypeDef       JPEG_Info;

u8 jpegDecoded;

u8 jpegDecodeMethod;

JPEG_YCbCrToRGB_Convert_Function pConvert_Function;

static FIL *pJpegFile;

uint8_t MCU_Data_OutBuffer[CHUNK_SIZE_OUT];
uint8_t JPEG_Data_InBuffer[CHUNK_SIZE_IN];

JPEG_Data_BufferTypeDef JPEG_InBuffer;// = { JPEG_Data_InBuffer , 0};

uint32_t MCU_TotalNb = 0;
uint32_t MCU_BlockIndex = 0;
uint32_t Inputfile_Offset = 0;
uint32_t Jpeg_Decoding_End = 0;

uint32_t FrameBufferAddress;

void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
  /* Enable JPEG clock */
  __HAL_RCC_JPEG_CLK_ENABLE();
}

void OnError_Handler(void)
{
	while(1);
}

u8 JPEG_Init()
{
	u8	result = 0;

	JPEG_InitColorTables(); 
	
	JPEG_Handle.Instance = JPEG;
	HAL_JPEG_Init(&JPEG_Handle);

	jpegDecoded = 0;

	result = 1;

	return result;
}

void JPEG_VariableInit()
{
	MCU_TotalNb = 0;
	MCU_BlockIndex = 0;
	Inputfile_Offset = 0;
	Jpeg_Decoding_End = 0;
	FrameBufferAddress = 0;
}

uint32_t JPEG_Decode(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress)
{
	pJpegFile = file;  
	FrameBufferAddress = DestAddress;

	/* Read from JPG file and fill the input buffer */
	if(f_read (pJpegFile, JPEG_InBuffer.DataBuffer , CHUNK_SIZE_IN, (UINT*)(&JPEG_InBuffer.DataBufferSize)) != FR_OK)
	{
		OnError_Handler();
	}

	/* Update the file Offset*/
	Inputfile_Offset = JPEG_InBuffer.DataBufferSize;

	/* Start JPEG decoding with polling (Blocking) method */
	HAL_JPEG_Decode(hjpeg ,JPEG_InBuffer.DataBuffer ,JPEG_InBuffer.DataBufferSize ,MCU_Data_OutBuffer, CHUNK_SIZE_OUT, HAL_MAX_DELAY);

	return 0;
}

u8 JPEG_DecodeFromFile(FIL *pFile, u32 dstAdrs)
{
	JPEG_VariableInit();
	
	jpegDecodeMethod = JPEG_DECODE_FROM_FILE;

	JPEG_InBuffer.DataBuffer		= JPEG_Data_InBuffer;
	JPEG_InBuffer.DataBufferSize	= 0;

	pJpegFile = pFile;

	FrameBufferAddress = dstAdrs;

	Jpeg_Decoding_End = 0;

	if(f_read (pJpegFile, JPEG_InBuffer.DataBuffer , CHUNK_SIZE_IN, (UINT*)(&JPEG_InBuffer.DataBufferSize)) != FR_OK)
	{
		OnError_Handler();
	}

	Inputfile_Offset = JPEG_InBuffer.DataBufferSize;

	HAL_JPEG_Decode(&JPEG_Handle, JPEG_InBuffer.DataBuffer, JPEG_InBuffer.DataBufferSize, MCU_Data_OutBuffer, CHUNK_SIZE_OUT, HAL_MAX_DELAY);

	f_close(pJpegFile);

	return 0;
}

u8 JPEG_DecodeFromSDRAM(u32 dstAdrs, u32 srcAdrs, u32 fileSize)
{	
	JPEG_VariableInit();
	
	jpegDecodeMethod = JPEG_DECODE_FROM_SDRAM;

	JPEG_InBuffer.DataBuffer		= (uint8_t*)srcAdrs;
	JPEG_InBuffer.DataBufferSize	= CHUNK_SIZE_IN;

	FrameBufferAddress = dstAdrs;

	Jpeg_Decoding_End = 0;

	Inputfile_Offset = JPEG_InBuffer.DataBufferSize;

	HAL_JPEG_Decode(&JPEG_Handle, JPEG_InBuffer.DataBuffer, JPEG_InBuffer.DataBufferSize, MCU_Data_OutBuffer, CHUNK_SIZE_OUT, HAL_MAX_DELAY);

	do{
		if(Jpeg_Decoding_End == 1)	break;
	}while(1);

	return 0;	
}

void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
	if(JPEG_GetDecodeColorConvertFunc(pInfo, &pConvert_Function, &MCU_TotalNb) != HAL_OK)
	{
		OnError_Handler();
	}
}

void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
	switch(jpegDecodeMethod)
	{
		case JPEG_DECODE_FROM_FILE:
			if(NbDecodedData != JPEG_InBuffer.DataBufferSize)
			{
				Inputfile_Offset = Inputfile_Offset - JPEG_InBuffer.DataBufferSize + NbDecodedData;
				f_lseek(pJpegFile, Inputfile_Offset);
			}

			if(f_read (pJpegFile, JPEG_InBuffer.DataBuffer , CHUNK_SIZE_IN, (UINT*)(&JPEG_InBuffer.DataBufferSize)) == FR_OK)
			{
				Inputfile_Offset += JPEG_InBuffer.DataBufferSize;
				HAL_JPEG_ConfigInputBuffer(hjpeg, JPEG_InBuffer.DataBuffer, JPEG_InBuffer.DataBufferSize);
			}
			else
			{
				OnError_Handler();
			}
		break;

		case JPEG_DECODE_FROM_SDRAM:
			if(NbDecodedData != JPEG_InBuffer.DataBufferSize)
			{
				Inputfile_Offset = Inputfile_Offset - JPEG_InBuffer.DataBufferSize + NbDecodedData;
			}

			JPEG_InBuffer.DataBuffer += NbDecodedData;
			Inputfile_Offset += JPEG_InBuffer.DataBufferSize;
			HAL_JPEG_ConfigInputBuffer(hjpeg, JPEG_InBuffer.DataBuffer, JPEG_InBuffer.DataBufferSize);
			break;

		default:
			break;
	}
}

void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
	uint32_t ConvertedDataCount;

	MCU_BlockIndex += pConvert_Function(pDataOut, (uint8_t *)FrameBufferAddress, MCU_BlockIndex, OutDataLength, &ConvertedDataCount);    

	HAL_JPEG_ConfigOutputBuffer(hjpeg, MCU_Data_OutBuffer, CHUNK_SIZE_OUT);   
}

void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	OnError_Handler();
}

void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{
	Jpeg_Decoding_End = 1;
}

void JPEG_GetInfo(JPEG_ConfTypeDef *pData)
{
	HAL_JPEG_GetInfo(&JPEG_Handle, pData);
}

