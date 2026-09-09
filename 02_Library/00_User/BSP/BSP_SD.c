#include	"BSP_SD.h"

static	bspSdFops_t	bspSdFops[BSP_SD_COUNT];

SD_HandleTypeDef	hSD[BSP_SD_COUNT];

u8 BSP_SD_DataInit()
{
	u8	result = 0;

	memset(bspSdFops, NULL, sizeof(bspSdFops_t));
	memset(hSD, NULL, sizeof(hSD));

	result = 1;

	return result;
}

u32 BSP_SD_GetHandle(u8 data)
{
	u32	result = 0;

	switch(data)
	{
		case BSP_SD1:
			hSD[BSP_SD1].Instance	= SDMMC1;
			result = (u32)&hSD[BSP_SD1];
			break;

		case BSP_SD2:
			hSD[BSP_SD2].Instance	= SDMMC2;
			result = (u32)&hSD[BSP_SD2];
			break;
	}

	return result;
}

u8 BSP_SD_RegisterInterface(SD_HandleTypeDef *pHandle, void *pVoid)
{
	u8	result = 1;

	if(pHandle->Instance == SDMMC1)			memcpy(&bspSdFops[BSP_SD1], pVoid, sizeof(bspSdFops_t));
	else if(pHandle->Instance == SDMMC2)	memcpy(&bspSdFops[BSP_SD2], pVoid, sizeof(bspSdFops_t));
	else									result = 0;

	return result;
}


void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
	u8	cnt;

	if(hsd->Instance == SDMMC1)				cnt = BSP_SD1;
	else if(hsd->Instance == SDMMC2)		cnt = BSP_SD2;
	else									return;

	if(bspSdFops[cnt].MspInit != NULL)
	{
		bspSdFops[cnt].MspInit();
	}
}

void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
	u8	cnt;

	if(hsd->Instance == SDMMC1)				cnt = BSP_SD1;
	else if(hsd->Instance == SDMMC2)		cnt = BSP_SD2;
	else									return;

	if(bspSdFops[cnt].MspDeInit != NULL)
	{
		bspSdFops[cnt].MspDeInit();
	}
}

#if 0 // yeom_20210120 [start]
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
	u8	cnt;

	if(hsd->Instance == SDMMC1)				cnt = BSP_SD1;
	else if(hsd->Instance == SDMMC2)		cnt = BSP_SD2;
	else									return;

	if(bspSdFops[cnt].TxCpltCallback != NULL)
	{
		bspSdFops[cnt].TxCpltCallback();
	}
}
#endif // yeom_20210120 [end]

#if 0 // yeom_20210120 [start]
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
	u8	cnt;

	if(hsd->Instance == SDMMC1)				cnt = BSP_SD1;
	else if(hsd->Instance == SDMMC2)		cnt = BSP_SD2;
	else									return;

	if(bspSdFops[cnt].RxCpltCallback != NULL)
	{
		bspSdFops[cnt].RxCpltCallback();
	}
}
#endif // yeom_20210120 [end]

void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
	u8	cnt;

	if(hsd->Instance == SDMMC1)				cnt = BSP_SD1;
	else if(hsd->Instance == SDMMC2)		cnt = BSP_SD2;
	else									return;

	if(bspSdFops[cnt].ErrorCallback != NULL)
	{
		bspSdFops[cnt].ErrorCallback();
	}
}

#if 0 // yeom_20210120 [start]
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
	u8	cnt;

	if(hsd->Instance == SDMMC1)				cnt = BSP_SD1;
	else if(hsd->Instance == SDMMC2)		cnt = BSP_SD2;
	else									return;

	if(bspSdFops[cnt].AbortCallback != NULL)
	{
		bspSdFops[cnt].AbortCallback();
	}
}
#endif // yeom_20210120 [end]

