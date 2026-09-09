#include	"App_Config.h"

#define	CONFIG_PACKET_TYPE				0x00010001
#define	CONFIG_PACKET_VERSION			0x00010000
#define	CONFIG_MEMORY_ADRS				0

static	configPacket_t	configPacket;
#if 1 // yeom_20210118 [start]
configE7602PG_t	configE7602PG;
#else
static	configE7602PG_t	configE7602PG;
#endif // yeom_20210118 [end]
static u8 Config_WritePacket(u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = EEPROM_Write(CONFIG_MEMORY_ADRS, pData, size);
	if(rtn != 0)			result = 1;

	return result;
}

static u8 Config_ReadPacket(u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = EEPROM_Read(CONFIG_MEMORY_ADRS, pData, size);
	if(rtn != 0)			result = 1;

	return result;
}

static u8 Config_DataInit()
{
	u8	result = 0;

	memset(configE7602PG.rawData, NULL, sizeof(configE7602PG));

	sprintf((char*)configE7602PG.modelType, "E7602_R2-0000000");
	sprintf((char*)configE7602PG.serialNo, "00000000");
	sprintf((char*)configE7602PG.buildDate, "2021XXXX");

	configE7602PG.networkInfo.mode	= NETWORK_MODE_NONE;

	configE7602PG.networkInfo.ip[0] = 192;
	configE7602PG.networkInfo.ip[1] = 168;
	configE7602PG.networkInfo.ip[2] = 0;
	configE7602PG.networkInfo.ip[3] = 10;

	configE7602PG.networkInfo.nm[0] = 255;
	configE7602PG.networkInfo.nm[1] = 255;
	configE7602PG.networkInfo.nm[2] = 255;
	configE7602PG.networkInfo.nm[3] = 0;

	configE7602PG.networkInfo.gw[0] = 192;
	configE7602PG.networkInfo.gw[1] = 168;
	configE7602PG.networkInfo.gw[2] = 0;
	configE7602PG.networkInfo.gw[3] = 0;

	configE7602PG.networkInfo.tcpPort	= 8999;
	configE7602PG.networkInfo.udpPort	= 7999;

	result = 1;

	return result;
}

u8 Config_Init()
{
	u8	result = 0;
	configPacketHandle_t	handle;

	memset(&handle, NULL, sizeof(handle));

	handle.pktType			= CONFIG_PACKET_TYPE;
	handle.pktVersion		= CONFIG_PACKET_VERSION;
	handle.WritePacket		= Config_WritePacket;
	handle.ReadPacket		= Config_ReadPacket;

	handle.pPacket			= &configPacket;

	result = CP_RegisterInterface(&handle);

	return result;
}

u8 Config_GetMacAdrs(u8 *pData)
{
	u8	result = 0;

	pData[0] = 0x0E;
	pData[1] = 0x76;
	pData[2] = 0x02;
	pData[3] = 'M';
	pData[4] = (((configE7602PG.serialNo[4] - '0') << 4) | ((configE7602PG.serialNo[5] - '0') << 0));
	pData[5] = (((configE7602PG.serialNo[6] - '0') << 4) | ((configE7602PG.serialNo[7] - '0') << 0));

	result = 1;

	return result;
}

u8 Config_SetData(u8 src, void *pVoid, u8 size)
{
	u8	result = 1;

	switch(src)
	{
		case CONFIG_DATA_MODELTYPE:
			{
				u8	*pData;
				if(size > CONFIG_MODELTYPE_SIZE)	return result;

				pData = (u8*)pVoid;

				memset(configE7602PG.modelType, NULL, CONFIG_MODELTYPE_SIZE);

				sprintf((char*)configE7602PG.modelType, "%s", pData);
			}
			break;

		case CONFIG_DATA_SERIALNO:
			{
				u8	*pData;
				if(size != CONFIG_SERIALNO_SIZE)	return result;

				pData = (u8*)pVoid;

				memset(configE7602PG.serialNo, NULL, CONFIG_SERIALNO_SIZE);

				sprintf((char*)configE7602PG.serialNo, "%s", pData);
			}
			break;

		case CONFIG_DATA_BUILDDATE:
			{
				u8	*pData;
				if(size != CONFIG_BUILDDATE_SIZE)	return result;

				pData = (u8*)pVoid;

				memset(configE7602PG.buildDate, NULL, CONFIG_BUILDDATE_SIZE);

				sprintf((char*)configE7602PG.buildDate, "%s", pData);
			}
			break;

		case CONFIG_DATA_NETWORK_MODE:
			{
				u16	*pMode;
				if(size != 2)						return result;

				pMode = (u16*)pVoid;

				configE7602PG.networkInfo.mode = *pMode;
			}
			break;

		case CONFIG_DATA_NETWORK_IP:
			{
				u8	*pIP;
				if(size != 4)						return result;

				pIP = (u8*)pVoid;

				memcpy(configE7602PG.networkInfo.ip, pIP, 4);
			}
			break;

		case CONFIG_DATA_NETWORK_NETMASK:
			{
				u8	*pNM;
				if(size != 4)						return result;

				pNM = (u8*)pVoid;

				memcpy(configE7602PG.networkInfo.nm, pNM, 4);
			}
			break;

		case CONFIG_DATA_NETWORK_GATEWAY:
			{
				u8	*pGW;
				if(size != 4)						return result;

				pGW = (u8*)pVoid;

				memcpy(configE7602PG.networkInfo.gw, pGW, 4);
			}
			break;

		case CONFIG_DATA_NETWORK_TCPPORT:
			{
				u16	*pPort;
				if(size != 2)						return result;

				pPort = (u16*)pVoid;

				configE7602PG.networkInfo.tcpPort = *pPort;
			}
			break;

		case CONFIG_DATA_NETWORK_UDPPORT:
			{
				u16	*pPort;
				if(size != 2)						return result;

				pPort = (u16*)pVoid;

				configE7602PG.networkInfo.udpPort = *pPort;
			}
			break;


		case CONFIG_DATA_LOG_STATUS:
			{
				u32	*pData;
				if(size != 4)						return result;

				pData = (u32*)pVoid;

				configE7602PG.logStatus = *pData;
			}
			break;

		case CONFIG_DATA_DEBUG_STATUS:
			{
				u32	*pData;
				if(size != 4)						return result;

				pData = (u32*)pVoid;

				configE7602PG.debugStatus = *pData;
			}
			break;

		default:
			result = 0;
			break;
	}

	return result;
}

u8 Config_GetData(u8 src, void *pVoid, u8 *pSize)
{
	u8	result = 1;

	switch(src)
	{
		case CONFIG_DATA_MODELTYPE:
			{
				u8	*pData;

				pData = (u8*)pVoid;

				memcpy(pData, configE7602PG.modelType, CONFIG_MODELTYPE_SIZE);

				*pSize = CONFIG_MODELTYPE_SIZE;
			}
			break;

		case CONFIG_DATA_SERIALNO:
			{
				u8	*pData;

				pData = (u8*)pVoid;

				memcpy(pData, configE7602PG.serialNo, CONFIG_SERIALNO_SIZE);

				*pSize = CONFIG_SERIALNO_SIZE;
			}
			break;

		case CONFIG_DATA_BUILDDATE:
			{
				u8	*pData;

				pData = (u8*)pVoid;

				memcpy(pData, configE7602PG.buildDate, CONFIG_BUILDDATE_SIZE);

				*pSize = CONFIG_BUILDDATE_SIZE;
			}
			break;

		case CONFIG_DATA_NETWORK_MODE:
			{
				u16	*pMode;

				pMode = (u16*)pVoid;

				*pMode = configE7602PG.networkInfo.mode;

				*pSize = 2;
			}
			break;

		case CONFIG_DATA_NETWORK_IP:
			{
				u8	*pIP;

				pIP = (u8*)pVoid;

				memcpy(pIP, configE7602PG.networkInfo.ip, 4);

				*pSize = 4;
			}
			break;

		case CONFIG_DATA_NETWORK_NETMASK:
			{
				u8	*pNM;

				pNM = (u8*)pVoid;

				memcpy(pNM, configE7602PG.networkInfo.nm, 4);

				*pSize = 4;
			}
			break;

		case CONFIG_DATA_NETWORK_GATEWAY:
			{
				u8	*pGW;

				pGW = (u8*)pVoid;

				memcpy(pGW, configE7602PG.networkInfo.gw, 4);

				*pSize = 4;
			}
			break;

		case CONFIG_DATA_NETWORK_TCPPORT:
			{
				u16	*pPort;

				pPort = (u16*)pVoid;

				*pPort = configE7602PG.networkInfo.tcpPort;

				*pSize = 2;
			}
			break;

		case CONFIG_DATA_NETWORK_UDPPORT:
			{
				u16	*pPort;

				pPort = (u16*)pVoid;

				*pPort = configE7602PG.networkInfo.udpPort;

				*pSize = 2;
			}
			break;

		case CONFIG_DATA_LOG_STATUS:
			{
				u32	*pData;

				pData = (u32*)pVoid;

				*pData = configE7602PG.logStatus;

				*pSize = 4;
			}
			break;

		case CONFIG_DATA_DEBUG_STATUS:
			{
				u32	*pData;

				pData = (u32*)pVoid;

				*pData = configE7602PG.debugStatus;

				*pSize = 4;
			}
			break;

		default:
			result = 0;
			break;
	}

	return result;
}

u8 Config_DataClear()
{
	return Config_DataInit();
}

u8 Config_DataLoad()
{
	return CP_ReadPacket(configE7602PG.rawData, sizeof(configE7602PG));
}

u8 Config_DataSave()
{
	return CP_WritePacket(configE7602PG.rawData, sizeof(configE7602PG));
}

u8 Config_BoardData()
{
	u8	result = 0, rtn;

	rtn = Config_DataLoad();
	if(rtn == 0)		Config_DataClear();

	rtn = Network_InfoLoad(&configE7602PG.networkInfo);
	if(rtn != 0)		result = 1;

	return result;
}

u32 Config_LogStatus()
{
	return configE7602PG.logStatus;
}

u32 Config_DebugStatus()
{
	return configE7602PG.debugStatus;
}

