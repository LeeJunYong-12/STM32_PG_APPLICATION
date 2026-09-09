#ifndef	_HSP_LED_H
#define	_HSP_LED_H

#include	"HSP.h"

#define	HSP_LED_COUNT				3

#ifdef	_E7502_APPLICATION_
#define	HSP_LED_TICK_COUNT			500
#endif

#ifdef	_E7502_BOOTLOADER_
#define	HSP_LED_TICK_COUNT			50
#endif

u8 HSP_LedCtrl(u8 position, u8 data);
u8 HSP_LedToggle(u8 position);
u8 HSP_LedInit();

#endif	//	_HSP_LED_H
