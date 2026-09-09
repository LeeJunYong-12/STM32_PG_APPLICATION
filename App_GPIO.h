#ifndef	_APP_GPIO_H
#define	_APP_GPIO_H

#include	"E7602_PG_Application.h"

u8 LCM_GpioInit(u8 position);
u8 LCM_GpioDirection(u8 position, u8 state);
u8 LCM_GpioWrite(u8 position, u8 state);
u8 LCM_GpioRead(u8 position, u8 *pState);
u8 LCM_GpioSet(u8 position, u8 data);
u8 LCM_GpioClear(u8 position, u8 data);
u8 LCM_GpioPulseInit(u8 position);
u8 LCM_GpioPulseDeInit(u8 position);
u8 LCM_GpioPulseRead(u8 position, float *pPulseAverage);
u8 LCM_GpioPulseCountRead(u8 position, u32 *pCurrentCnt, u32 *pLoopCnt);

#endif	// _APP_GPIO_H