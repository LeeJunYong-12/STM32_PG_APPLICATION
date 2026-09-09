#ifndef	_APP_USB_H
#define	_APP_USB_H

#include	"E7602_PG_Application.h"

extern	u8	usbRxBuffer[512], usbRxFlag;
extern	u32	usbRxSize;

void USB_RxRecover();
u8 USB_Init();
u8 USB_TxData(u8 *pData, u16 size);

#endif	// _APP_USB_H