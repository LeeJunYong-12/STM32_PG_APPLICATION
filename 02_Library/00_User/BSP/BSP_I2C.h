#ifndef	_BSP_I2C_H
#define	_BSP_I2C_H

#include	"BSP.h"

#define	BSP_I2C_COUNT			4

typedef	struct{
	void	(* MspInit)					(void);
	void	(* MspDeInit)				(void);
	void	(* MasterTxCpltCallback)	(void);
	void	(* MasterRxCpltCallback)	(void);
	void	(* SlaveTxCpltCallback)		(void);
	void	(* SlaveRxCpltCallback)		(void);
	void	(* AddrCallback)		 	(u8, u16);
	void	(* ListenCpltCallback)		(void);
	void	(* MemTxCpltCallback)		(void);
	void	(* MemRxCpltCallback)		(void);
	void	(* ErrorCallback)			(void);
	void	(* AbortCpltCallback)		(void);
}bspI2CFops_t;

enum{
	BSP_I2C1 = 0,
	BSP_I2C2,
	BSP_I2C3,
	BSP_I2C4,
};

extern	I2C_HandleTypeDef	hI2C[BSP_I2C_COUNT];

u8 BSP_I2C_GetTiming(u16 baudrate, u32 *pTiming);
u32 BSP_I2C_GetHandle(u8 data);
u8 BSP_I2C_RegisterInterface(I2C_HandleTypeDef *pHandle, void *pVoid);

#endif	//	_BSP_I2C_H_
