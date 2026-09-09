#include	"BSP_DMA.h"

DMA_HandleTypeDef	hDMA[16];

u32 BSP_DMA_GetHandle(u8 data)
{
	u32	result = 0;

	switch(data)
	{
		case BSP_DMA1_S0:
			hDMA[BSP_DMA1_S0].Instance	= DMA1_Stream0;
			result = (u32)&hDMA[BSP_DMA1_S0];
			break;

		case BSP_DMA1_S1:
			hDMA[BSP_DMA1_S1].Instance	= DMA1_Stream1;
			result = (u32)&hDMA[BSP_DMA1_S1];
			break;

		case BSP_DMA1_S2:
			hDMA[BSP_DMA1_S2].Instance	= DMA1_Stream2;
			result = (u32)&hDMA[BSP_DMA1_S2];
			break;

		case BSP_DMA1_S3:
			hDMA[BSP_DMA1_S3].Instance	= DMA1_Stream3;
			result = (u32)&hDMA[BSP_DMA1_S3];
			break;

		case BSP_DMA1_S4:
			hDMA[BSP_DMA1_S4].Instance	= DMA1_Stream4;
			result = (u32)&hDMA[BSP_DMA1_S4];
			break;

		case BSP_DMA1_S5:
			hDMA[BSP_DMA1_S5].Instance	= DMA1_Stream5;
			result = (u32)&hDMA[BSP_DMA1_S5];
			break;

		case BSP_DMA1_S6:
			hDMA[BSP_DMA1_S6].Instance	= DMA1_Stream6;
			result = (u32)&hDMA[BSP_DMA1_S6];
			break;

		case BSP_DMA1_S7:
			hDMA[BSP_DMA1_S7].Instance	= DMA1_Stream7;
			result = (u32)&hDMA[BSP_DMA1_S7];
			break;

		case BSP_DMA2_S0:
			hDMA[BSP_DMA2_S0].Instance	= DMA2_Stream0;
			result = (u32)&hDMA[BSP_DMA2_S0];
			break;

		case BSP_DMA2_S1:
			hDMA[BSP_DMA2_S1].Instance	= DMA2_Stream1;
			result = (u32)&hDMA[BSP_DMA2_S1];
			break;

		case BSP_DMA2_S2:
			hDMA[BSP_DMA2_S2].Instance	= DMA2_Stream2;
			result = (u32)&hDMA[BSP_DMA2_S2];
			break;

		case BSP_DMA2_S3:
			hDMA[BSP_DMA2_S3].Instance	= DMA2_Stream3;
			result = (u32)&hDMA[BSP_DMA2_S3];
			break;

		case BSP_DMA2_S4:
			hDMA[BSP_DMA2_S4].Instance	= DMA2_Stream4;
			result = (u32)&hDMA[BSP_DMA2_S4];
			break;

		case BSP_DMA2_S5:
			hDMA[BSP_DMA2_S5].Instance	= DMA2_Stream5;
			result = (u32)&hDMA[BSP_DMA2_S5];
			break;

		case BSP_DMA2_S6:
			hDMA[BSP_DMA2_S6].Instance	= DMA2_Stream6;
			result = (u32)&hDMA[BSP_DMA2_S6];
			break;

		case BSP_DMA2_S7:
			hDMA[BSP_DMA2_S7].Instance	= DMA2_Stream7;
			result = (u32)&hDMA[BSP_DMA2_S7];
			break;
	}

	return result;
}

u8 BSP_DMA_Link(u8 mode, void *pHandle, void *pDmaHandle)
{
	u8	result = 1;
	DMA_HandleTypeDef	*pDMA;

	pDMA = (DMA_HandleTypeDef*)pDmaHandle;

	if(mode == BSP_DMA_MODE_I2C_TX)
	{
		I2C_HandleTypeDef	*pI2C;

		pI2C = (I2C_HandleTypeDef*)pHandle;

		pI2C->hdmatx	= pDmaHandle;
		pDMA->Parent	= pHandle;
	}
	else if(mode == BSP_DMA_MODE_I2C_RX)
	{
		I2C_HandleTypeDef	*pI2C;

		pI2C = (I2C_HandleTypeDef*)pHandle;

		pI2C->hdmarx	= pDmaHandle;
		pDMA->Parent	= pHandle;
	}
	else
	{
		result = 0;
	}

	return result;
}

