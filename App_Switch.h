#ifndef	_APP_SWITCH_H
#define	_APP_SWITCH_H

#include	"E7602_PG_Application.h"

u8 Switch_GpioInit();
u8 Switch_GetIoStatus();
u32	Switch_GetFlag();
u8 Switch_ClearFlag(u8 position);
u8 Switch_SetFlag(u8 position);
u8 Switch_Init();

#endif	// _APP_SWITCH_H