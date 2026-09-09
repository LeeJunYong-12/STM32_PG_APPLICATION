#ifndef	_USBD_CDC_IF_H
#define	_USBD_CDC_IF_H

#include	"usbd_cdc.h"

#include	"BSP.h"

typedef	struct{
	void (*RxCpltCallback)		(u8*, u32);
}usbCdcCallback_t;

extern	USBD_CDC_ItfTypeDef	usbdCdcFops;

void USB_CDC_DataRegister(void *pHandle, void *pFops);

#endif	//	_USBD_CDC_IF_H