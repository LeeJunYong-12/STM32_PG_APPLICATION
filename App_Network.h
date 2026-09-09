#ifndef	_APP_NETWORK_H
#define	_APP_NETWORK_H

#include	"E7602_PG_Application.h"

enum{
	NETWORK_MODE_NONE			= 0,
	NETWORK_MODE_TCP,
	NETWORK_MODE_UDP,
	NETWORK_MODE_END,
};

enum{
	NETWORK_INFO_MODE			= 0,
	NETWORK_INFO_MAC,
	NETWORK_INFO_IP,
	NETWORK_INFO_NM,
	NETWORK_INFO_GW,
	NETWORK_INFO_TCPPORT,
	NETWORK_INFO_UDPPORT,
	NETWORK_INFO_END,
};

extern	networkInfo_t	networkInfo;

u8 Network_InfoInit();
u8 Network_GetInfoFlag();
u8 Network_RegisterInfoData(void *pVoid);
u8 Network_InfoLoad(void *pVoid);
u8 Network_GetInfoData(u8 position, void *pVoid);
u8 Network_Init();
u8 Network_SetStatus();
u8 Network_StatusProcess();

#endif	// _APP_NETWORK_H
