#ifndef	_ETHERNET_UDPSERVER_H
#define	_ETHERNET_UDPSERVER_H

#include	"BSP.h"

typedef	struct{
	u8	(* RxData)		(u8*, u16);
}udpServerFops_t;

void UDP_ServerPortInit(u16 port);
void UDP_ServerRegisterInterface(void *pVoid);
void UDP_ServerConnect();
void UDP_ServerDisconnect();
void UDP_TransmitData(u8 *pData, u16 size);
void UDP_ServerInit();

#endif	//	_ETHERNET_UDPSERVER_H