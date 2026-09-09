#ifndef	_APP_EXTSW_H
#define	_APP_EXTSW_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u8			effective;
	u8			status;
	u8			flag[8];
	bspGpio_t	io[8];
}extSW_t;

extern	extSW_t	extSW;

u8 ExtSW_DataInit();
u8 ExtSW_GpioInit();
u8 ExtSW_IoRead();
u8 ExtSW_Init();
u8 ExtSW_GetFlag();
u8 ExtSW_ClearFlag();

#endif	// _APP_EXTSW_H
