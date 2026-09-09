#ifndef	_BSP_SYSTICK_H
#define	_BSP_SYSTICK_H

#include	"BSP.h"

#define	BSP_SYSTICK_FUNCTION_MAX_COUNT			5

typedef	struct{
	void (* SysTickFunction)		(void);
}bspSysTickFops_t;

u8 BSP_SysTick_RegisterInterface(u8 pos, void *pVoid);
void BSP_SysTick_Process();

#endif	//	_BSP_SYSTICK_H
