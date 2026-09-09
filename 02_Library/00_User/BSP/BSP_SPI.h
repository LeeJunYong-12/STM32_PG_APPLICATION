#ifndef	_BSP_SPI_H
#define	_BSP_SPI_H

#include	"BSP.h"

#define	BSP_SPI_COUNT			6

typedef	struct{
	void	(* MspInit)					(void);
	void	(* MspDeInit)				(void);
	void	(* TxCpltCallback)			(void);
	void	(* RxCpltCallback)			(void);
	void	(* TxRxCpltCallback)		(void);
	void	(* TxHalfCpltCallback)		(void);
	void	(* RxHalfCpltCallback)		(void);
	void	(* TxRxHalfCpltCallback)	(void);
	void	(* ErrorCallback)			(void);
	void	(* AbortCpltCallback)		(void);
}bspSpiFops_t;

enum{
	BSP_SPI1 = 0,
	BSP_SPI2,
	BSP_SPI3,
	BSP_SPI4,
	BSP_SPI5,
	BSP_SPI6,
};

extern	SPI_HandleTypeDef	hSPI[BSP_SPI_COUNT];

u32 BSP_SPI_GetHandle(u8 data);
u8 BSP_SPI_RegisterInterface(SPI_HandleTypeDef *pHandle, void *pVoid);

#endif	//	_BSP_SPI_H
