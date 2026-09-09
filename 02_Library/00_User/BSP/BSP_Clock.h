#ifndef	_BSP_CLOCK_H
#define	_BSP_CLOCK_H

#include	"BSP.h"

#define	BSP_CLOCK_MPU_ENABLE		1

#define	BSP_CLOCK_ICACHE_ENABLE		1
#define	BSP_CLOCK_DCACHE_ENABLE		1

u8 BSP_Clock_Init();

#endif	//	_BSP_CLOCK_H