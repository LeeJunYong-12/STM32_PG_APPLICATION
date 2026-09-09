#ifndef	_BSP_DMA_H
#define	_BSP_DMA_H

#include	"BSP.h"

enum{
	BSP_DMA1_S0 = 0,
	BSP_DMA1_S1,
	BSP_DMA1_S2,
	BSP_DMA1_S3,
	BSP_DMA1_S4,
	BSP_DMA1_S5,
	BSP_DMA1_S6,
	BSP_DMA1_S7,

	BSP_DMA2_S0,
	BSP_DMA2_S1,
	BSP_DMA2_S2,
	BSP_DMA2_S3,
	BSP_DMA2_S4,
	BSP_DMA2_S5,
	BSP_DMA2_S6,
	BSP_DMA2_S7,
};

enum{
	BSP_DMA_MODE_I2C_TX = 0,
	BSP_DMA_MODE_I2C_RX,
	BSP_DMA_MODE_END,
};

extern	DMA_HandleTypeDef	hDMA[16];

u32 BSP_DMA_GetHandle(u8 data);
u8 BSP_DMA_Link(u8 mode, void *pHandle, void *pDmaHandle);

#endif	//	_BSP_DMA_H
