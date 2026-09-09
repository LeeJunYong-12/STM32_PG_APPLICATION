#ifndef	_APP_SBC_H
#define	_APP_SBC_H

#include	"E7602_PG_Application.h"

#define	SBC_UART_BUFFER_SIZE				4096

u8 SBC_UartInit();
u8 SBC_IoInit();
u8 SBC_UartChannelSelect(u16 channel);
u8 SBC_UartReceiveStart();
u8 SBC_UartGetChar(u8 *pData);
u8 SBC_UartTransmitData(u8 *pData, u16 size);

#endif	//	_APP_SBC_H