#include	"App_Network.h"

static	u8	networkStatusTrigger = 0, networkInfoSetFlag = 0;

networkInfo_t	networkInfo;

u8 Network_InfoInit()
{
	u8	result = 0;

	networkInfo.mode	= NETWORK_MODE_TCP;

	networkInfo.mac[0]	= 0x12;//18
	networkInfo.mac[1]	= 0x34;//52
	networkInfo.mac[2]	= 0x56;//86
	networkInfo.mac[3]	= 0x78;//120
	networkInfo.mac[4]	= 0x9A;//154
	networkInfo.mac[5]	= 0xBC;//188

	networkInfo.ip[0]	= 192;
	networkInfo.ip[1]	= 168;
	networkInfo.ip[2]	= 37;
	networkInfo.ip[3]	= 101;

	networkInfo.nm[0]	= 255;
	networkInfo.nm[1]	= 255;
	networkInfo.nm[2]	= 255;
	networkInfo.nm[3]	= 0;

	networkInfo.gw[0]	= 192;
	networkInfo.gw[1]	= 168;
	networkInfo.gw[2]	= 37;
	networkInfo.gw[3]	= 254;

	networkInfo.tcpPort	= 8999;
	networkInfo.udpPort	= 7999;

	networkInfoSetFlag = 1;

	result = 1;

	return result;
}

u8 Network_GetInfoFlag()
{
	return networkInfoSetFlag;
}

u8 Network_RegisterInfoData(void *pVoid)
{
	u8	result = 0;

	memcpy(&networkInfo, pVoid, sizeof(networkInfo));

	networkInfoSetFlag = 1;

	result = 1;

	return result;
}

u8 Network_InfoLoad(void *pVoid)
{
	u8	result = 0;

	memcpy(&networkInfo, pVoid, sizeof(networkInfo));

	if(networkInfo.mode >= NETWORK_MODE_END)	networkInfo.mode = NETWORK_MODE_NONE;

	result = Config_GetMacAdrs(networkInfo.mac);

	if(result)									networkInfoSetFlag = 1;

	return result;
}

u8 Network_GetInfoData(u8 position, void *pVoid)
{
	u8	result = 1;

	switch(position)
	{
		case NETWORK_INFO_MODE:
			memcpy(pVoid, &networkInfo.mode, 2);
			break;

		case NETWORK_INFO_MAC:
			memcpy(pVoid, &networkInfo.mac, 6);
			break;

		case NETWORK_INFO_IP:
			memcpy(pVoid, &networkInfo.ip, 4);
			break;

		case NETWORK_INFO_NM:
			memcpy(pVoid, &networkInfo.nm, 4);
			break;

		case NETWORK_INFO_GW:
			memcpy(pVoid, &networkInfo.gw, 4);
			break;

		case NETWORK_INFO_TCPPORT:
			memcpy(pVoid, &networkInfo.tcpPort, 2);
			break;

		case NETWORK_INFO_UDPPORT:
			memcpy(pVoid, &networkInfo.udpPort, 2);
			break;

		default:
			result = 0;
			break;
	}

	return result;
}

u8 Network_Init()
{
	u8	result = 0, rtn;

	if(Network_GetInfoFlag() == 0)					return result;

	if(networkInfo.mode == NETWORK_MODE_NONE)		return result;
	if(networkInfo.mode	>= NETWORK_MODE_END)		return result;

	rtn = BSP_Ethernet_RegisterMacAddress(networkInfo.mac);
	if(rtn == 0)									return result;

	rtn = BSP_Ethernet_RegisterNetworkAdrdress(networkInfo.ip, networkInfo.nm, networkInfo.gw);
	if(rtn == 0)									return result;

	lwip_init();

	Netif_Config();

	if(networkInfo.mode == NETWORK_MODE_TCP)
	{
		tcpServerFops_t	fops;

		fops.RxData	= NetworkPKT_RxData;

		TCP_ServerPortInit(networkInfo.tcpPort);

		TCP_ServerRegisterInterface(&fops);

		TCP_ServerInit();
	}
	else if(networkInfo.mode == NETWORK_MODE_UDP)
	{
		udpServerFops_t	fops;

		fops.RxData = NetworkPKT_RxData;

		UDP_ServerPortInit(networkInfo.udpPort);

		UDP_ServerRegisterInterface(&fops);

		UDP_ServerInit();
	}

	User_notification(&gnetif);

	result = 1;

	return result;
}

u8 Network_SetStatus()
{
	u8	result = 0;

	networkStatusTrigger |= 0x01;

	result = 1;

	return result;
}

u8 Network_StatusProcess()
{
	u8	result = 0, rtn;

	if((networkStatusTrigger & 0x01) == 0)			return result;

	networkStatusTrigger &= ~0x01;

	if((networkStatusTrigger & 0x10) != 0)			return result;

	rtn = Network_Init();
	if(rtn != 0)
	{
		networkStatusTrigger |= 0x10;

		result = 1;
	}

	return result;
}

