#ifndef	_BSP_SDMMC_H
#define	_BSP_SDMMC_H

#include	"BSP.h"

#define	BSP_SD_COUNT			2

typedef	struct{
	void	(* MspInit)					(void);
	void	(* MspDeInit)				(void);
	void	(* TxCpltCallback)			(void);
	void	(* RxCpltCallback)			(void);
	void	(* ErrorCallback)			(void);
	void	(* AbortCallback)			(void);
}bspSdFops_t;

enum{
	BSP_SD1 = 0,
	BSP_SD2,
};

extern	SD_HandleTypeDef	hSD[BSP_SD_COUNT];

u32 BSP_SD_GetHandle(u8 data);
u8 BSP_SD_RegisterInterface(SD_HandleTypeDef *pHandle, void *pVoid);

#endif	//	_BSP_SDMMC_H
