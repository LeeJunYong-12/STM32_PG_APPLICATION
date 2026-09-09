#ifndef	_APP_CONFIG_H
#define	_APP_CONFIG_H

#include	"E7602_PG_Application.h"

#define	CONFIG_MODELTYPE_SIZE			16
#define	CONFIG_SERIALNO_SIZE			8
#define	CONFIG_BUILDDATE_SIZE			8

enum{
	CONFIG_DATA_MODELTYPE		= 0,
	CONFIG_DATA_SERIALNO,
	CONFIG_DATA_BUILDDATE,
	CONFIG_DATA_NETWORK_MODE,
	CONFIG_DATA_NETWORK_IP,
	CONFIG_DATA_NETWORK_NETMASK,
	CONFIG_DATA_NETWORK_GATEWAY,
	CONFIG_DATA_NETWORK_TCPPORT,
	CONFIG_DATA_NETWORK_UDPPORT,
	CONFIG_DATA_LOG_STATUS,
	CONFIG_DATA_DEBUG_STATUS,
	CONFIG_DATA_END,
};

typedef	union{
	u8	rawData[CONFIG_PACKET_PROTOCOL_DATA_SIZE];

	struct{
		u8	modelType[CONFIG_MODELTYPE_SIZE];
		u8	serialNo[CONFIG_SERIALNO_SIZE];
		u8	buildDate[CONFIG_BUILDDATE_SIZE];

		networkInfo_t	networkInfo;

		u32	logStatus;
		u32	debugStatus;
	};
}configE7602PG_t;

#if 1 // yeom_20210118 [start]
extern configE7602PG_t	configE7602PG;
#endif // yeom_20210118 [end]

u8 Config_Init();
u8 Config_GetMacAdrs(u8 *pData);
u8 Config_SetData(u8 src, void *pVoid, u8 size);
u8 Config_GetData(u8 src, void *pVoid, u8 *pSize);
u8 Config_DataClear();
u8 Config_DataLoad();
u8 Config_DataSave();
u8 Config_BoardData();
u32 Config_LogStatus();
u32 Config_DebugStatus();

#endif	// _APP_CONFIG_H