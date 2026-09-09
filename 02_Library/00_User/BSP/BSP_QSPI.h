#ifndef	_BSP_QSPI_H
#define	_BSP_QSPI_H

#include	"BSP.h"

typedef	struct{
	void	(* MspInit)						(void);
	void	(* MspDeInit)					(void);
	void	(* ErrorCallback)				(void);
	void	(* AbortCpltCallback)			(void);
	void	(* FifoThresholdCallback)		(void);
	void	(* CmdCpltCallback)				(void);
	void	(* RxCpltCallback)				(void);
	void	(* TxCpltCallback)				(void);
	void	(* RxHalfCpltCallback)			(void);
	void	(* TxHalfCpltCallback)			(void);
	void	(* StatusMatchCallback)			(void);
	void	(* TimeOutCallback)				(void);
}bspQspiFops_t;

extern	QSPI_HandleTypeDef	hQSPI;

u32 BSP_QSPI_GetHandle();
u8 BSP_QSPI_RegisterInterface(QSPI_HandleTypeDef *pHandle, void *pVoid);

#endif	//	_BSP_QSPI_H
