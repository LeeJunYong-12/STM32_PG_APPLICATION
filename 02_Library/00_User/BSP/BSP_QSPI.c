#include	"BSP_QSPI.h"

static	bspQspiFops_t	bspQspiFops;

QSPI_HandleTypeDef	hQSPI;

u8 BSP_QSPI_DataInit()
{
	u8	result = 0;

	memset(&bspQspiFops, NULL, sizeof(bspQspiFops));
	memset(&hQSPI, NULL, sizeof(hQSPI));

	result = 1;

	return result;
}

u32 BSP_QSPI_GetHandle()
{
	u32	result = NULL;

	hQSPI.Instance	= QUADSPI;
	result = (u32)&hQSPI;

	return result;
}

u8 BSP_QSPI_RegisterInterface(QSPI_HandleTypeDef *pHandle, void *pVoid)
{
	u8	result = 1;

	if(pHandle->Instance == QUADSPI)						memcpy(&bspQspiFops, pVoid, sizeof(bspQspiFops_t));
	else													result = 0;

	return	result;
}

void HAL_QSPI_MspInit  (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.MspInit != NULL)						bspQspiFops.MspInit();
	}
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.MspDeInit != NULL)					bspQspiFops.MspDeInit();
	}
}

void HAL_QSPI_ErrorCallback        (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.ErrorCallback != NULL)				bspQspiFops.ErrorCallback();
	}
}

void HAL_QSPI_AbortCpltCallback    (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.AbortCpltCallback != NULL)			bspQspiFops.AbortCpltCallback();
	}
}

void HAL_QSPI_FifoThresholdCallback(QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.FifoThresholdCallback != NULL)		bspQspiFops.FifoThresholdCallback();
	}
}

void HAL_QSPI_CmdCpltCallback      (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.CmdCpltCallback != NULL)				bspQspiFops.CmdCpltCallback();
	}
}

void HAL_QSPI_RxCpltCallback       (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.RxCpltCallback != NULL)				bspQspiFops.RxCpltCallback();
	}
}

void HAL_QSPI_TxCpltCallback       (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.TxCpltCallback != NULL)				bspQspiFops.TxCpltCallback();
	}
}

void HAL_QSPI_RxHalfCpltCallback   (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.RxHalfCpltCallback != NULL)			bspQspiFops.RxHalfCpltCallback();
	}
}

void HAL_QSPI_TxHalfCpltCallback   (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.TxHalfCpltCallback != NULL)			bspQspiFops.TxHalfCpltCallback();
	}
}

void HAL_QSPI_StatusMatchCallback  (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.StatusMatchCallback != NULL)			bspQspiFops.StatusMatchCallback();
	}
}

void HAL_QSPI_TimeOutCallback      (QSPI_HandleTypeDef *hqspi)
{
	if(hqspi->Instance == QUADSPI)
	{
		if(bspQspiFops.TimeOutCallback != NULL)				bspQspiFops.TimeOutCallback();
	}
}

