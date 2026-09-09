#ifndef	_APP_FANCTRL_H
#define	_APP_FANCTRL_H

#include	"E7602_PG_Application.h"

#define	FAN_MAX_COUNT				BOARD_CHANNEL_MAX_COUNT

u8 FanCtrl_IoInit(u8 position, u32 ioAdrs);
u8 FanCtrl_SetStatus(u8 position, u8 status);

#endif	// _APP_FANCTRL_H