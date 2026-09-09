#ifndef	_APP_GPIO_R_H
#define	_APP_GPIO_R_H

#include	"E7602_PG_Application.h"

u8 GPIO_R_Init(u8 position);
u8 GPIO_R_Direction(u8 position, u8 state);
u8 GPIO_R_Write(u8 position, u8 state);
u8 GPIO_R_Read(u8 *pState);
u8 GPIO_R_Set(u8 position, u8 data);
u8 GPIO_R_Clear(u8 position, u8 data);

#endif	// _APP_GPIO_R_H