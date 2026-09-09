#include	"BSP_SPI.h"

static	bspSpiFops_t	bspSpiFops[BSP_SPI_COUNT];

SPI_HandleTypeDef	hSPI[BSP_SPI_COUNT];

u8 BSP_SPI_DataInit()
{
	u8	result = 0;

	memset(bspSpiFops, NULL, sizeof(bspSpiFops));
	memset(hSPI, NULL, sizeof(hSPI));

	result = 1;

	return result;
}

u32 BSP_SPI_GetHandle(u8 data)
{
	u32	result = NULL;

	switch(data)
	{
		case BSP_SPI1:
			hSPI[BSP_SPI1].Instance	= SPI1;
			result = (u32)&hSPI[BSP_SPI1];
			break;

		case BSP_SPI2:
			hSPI[BSP_SPI2].Instance	= SPI2;
			result = (u32)&hSPI[BSP_SPI2];
			break;

		case BSP_SPI3:
			hSPI[BSP_SPI3].Instance	= SPI3;
			result = (u32)&hSPI[BSP_SPI3];
			break;

		case BSP_SPI4:
			hSPI[BSP_SPI4].Instance	= SPI4;
			result = (u32)&hSPI[BSP_SPI4];
			break;

		case BSP_SPI5:
			hSPI[BSP_SPI5].Instance	= SPI5;
			result = (u32)&hSPI[BSP_SPI5];
			break;

		case BSP_SPI6:
			hSPI[BSP_SPI6].Instance	= SPI6;
			result = (u32)&hSPI[BSP_SPI6];
			break;
	}

	return result;
}

u8 BSP_SPI_RegisterInterface(SPI_HandleTypeDef *pHandle, void *pVoid)
{
	u8	result = 1;

	if(pHandle->Instance == SPI1)			memcpy(&bspSpiFops[BSP_SPI1], pVoid, sizeof(bspSpiFops_t));
	else if(pHandle->Instance == SPI2)		memcpy(&bspSpiFops[BSP_SPI2], pVoid, sizeof(bspSpiFops_t));
	else if(pHandle->Instance == SPI3)		memcpy(&bspSpiFops[BSP_SPI3], pVoid, sizeof(bspSpiFops_t));
	else if(pHandle->Instance == SPI4)		memcpy(&bspSpiFops[BSP_SPI4], pVoid, sizeof(bspSpiFops_t));
	else if(pHandle->Instance == SPI5)		memcpy(&bspSpiFops[BSP_SPI5], pVoid, sizeof(bspSpiFops_t));
	else if(pHandle->Instance == SPI6)		memcpy(&bspSpiFops[BSP_SPI6], pVoid, sizeof(bspSpiFops_t));
	else									result = 0;

	return	result;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef * hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].MspInit != NULL)
	{
		bspSpiFops[cnt].MspInit();
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef * hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].MspDeInit != NULL)
	{
		bspSpiFops[cnt].MspDeInit();
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].TxCpltCallback != NULL)
	{
		bspSpiFops[cnt].TxCpltCallback();
	}
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].RxCpltCallback != NULL)
	{
		bspSpiFops[cnt].RxCpltCallback();
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].TxRxCpltCallback != NULL)
	{
		bspSpiFops[cnt].TxRxCpltCallback();
	}
}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].TxHalfCpltCallback != NULL)
	{
		bspSpiFops[cnt].TxHalfCpltCallback();
	}
}

void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].RxHalfCpltCallback != NULL)
	{
		bspSpiFops[cnt].RxHalfCpltCallback();
	}
}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].TxRxHalfCpltCallback != NULL)
	{
		bspSpiFops[cnt].TxRxHalfCpltCallback();
	}
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].ErrorCallback != NULL)
	{
		bspSpiFops[cnt].ErrorCallback();
	}
}

void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi)
{
	u8	cnt = 0;

	if(hspi->Instance == SPI1)			cnt = BSP_SPI1;
	else if(hspi->Instance == SPI2)		cnt = BSP_SPI2;
	else if(hspi->Instance == SPI3)		cnt = BSP_SPI3;
	else if(hspi->Instance == SPI4)		cnt = BSP_SPI4;
	else if(hspi->Instance == SPI5)		cnt = BSP_SPI5;
	else if(hspi->Instance == SPI6)		cnt = BSP_SPI6;
	else								return;

	if(bspSpiFops[cnt].AbortCpltCallback != NULL)
	{
		bspSpiFops[cnt].AbortCpltCallback();
	}
}

