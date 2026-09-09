#ifndef	_APP_EXTGPIO_H
#define	_APP_EXTGPIO_H

#include	"E7602_PG_Application.h"

enum{
	EXTGPIO_PIN_0			= 0,
	EXTGPIO_PIN_1,
	EXTGPIO_PIN_2,
	EXTGPIO_PIN_3,
	EXTGPIO_PIN_4,
	EXTGPIO_PIN_5,
	EXTGPIO_PIN_6,
	EXTGPIO_PIN_7,
	EXTGPIO_PIN_8,
	EXTGPIO_PIN_END,
};

u8 ExtGPIO_Init();
u32 ExtGPIO_GetHandle(u8 position);
u8 ExtGPIO_Direction(u8 position, u32 direction);
u8 ExtGPIO_WritePin(u8 position, u8 state);
u8 ExtGPIO_ReadPin(u8 position, u8 *pData);
u8 Polling_HPD();
void HPD_EVENT();

#endif	// _APP_EXTGPIO_H