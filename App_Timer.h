#ifndef	_APP_TIMER_H
#define	_APP_TIMER_H

#include	"E7602_PG_Application.h"

u8 Timer_Init();
void Timer_Start();
void Timer_Stop();
void Timer_TickClear();
u32 Timer_GetTick();
void Timer_Delay(u32 cnt);

#endif	// _APP_TIMER_H
