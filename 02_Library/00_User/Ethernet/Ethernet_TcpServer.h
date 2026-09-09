#ifndef	_ETHERNET_TCPSERVER_H
#define	_ETHERNET_TCPSERVER_H

#include	"BSP.h"

typedef	struct{
	u8	(* RxData)		(u8*, u16);
}tcpServerFops_t;

void TCP_ServerPortInit(u16	port);
void TCP_ServerRegisterInterface(void *pVoid);
u8 TCP_ServerInit();
u8 TCP_ServerTransmitData(u8 *pData, u16 length);

#endif	//	_ETHERNET_TCPSERVER_H