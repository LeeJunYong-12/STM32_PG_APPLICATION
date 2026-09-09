#ifndef __USBD_DESC_H
#define __USBD_DESC_H

#include	"BSP.h"

#include	"usbd_def.h"

#define	USBD_DESC_SERIAL_HEADER		0xE7502A00

#define	USB_SIZ_STRING_SERIAL		0x1A

extern	USBD_DescriptorsTypeDef	VCP_Desc;

uint8_t	UsbDesc_SetSerialNumber(uint32_t serial);

#endif	//	__USBD_DESC_H