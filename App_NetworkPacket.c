#include	"App_NetworkPacket.h"

static	u8	networkPktMode = NETWORK_PKT_MODE_CMD;

static	u8	*pNetworkPktRxCmdBuffer = (u8*)MEMORY_MAP_SDRAM_ETHERNET_CMD_BUFFER_ADRS;
static	u32	networkPktRxCmdSize;

static	u8	*pNetworkPktRxFileBuffer;
static	u32	networkPktRxFileSize;

static	u8	networkPktRxFlag;
static	u8	*pNetworkPktRxBuffer;

static	u8	networkPktTxBuffer[1280];
static	u16	networkPktRxCount;

void NetworkPKT_FrameCheck()
{
	/*
	if(netif_is_link_up(&gnetif))
	{
		ethernetif_input(&gnetif);

		sys_check_timeouts();
	}
	*/

	ethernetif_input(&gnetif);
}

void NetworkPKT_DataProcess()
{
	if(networkInfo.mode == NETWORK_MODE_NONE)	return;
	if(networkInfo.mode >= NETWORK_MODE_END)	return;

	NetworkPKT_FrameCheck();
}

void NetworkPKT_ServerConnect()
{
	switch(networkInfo.mode)
	{
		case NETWORK_MODE_UDP:
			UDP_ServerConnect();
			break;
	}
}

void NetworkPKT_ServerDisconnect()
{
	switch(networkInfo.mode)
	{
		case NETWORK_MODE_UDP:
			UDP_ServerDisconnect();
			break;
	}
}

void NetworkPKT_TransmitData(u8 *pData, u16 length)
{
	switch(networkInfo.mode)
	{
		case NETWORK_MODE_TCP:
			TCP_ServerTransmitData(pData, length);
			break;

		case NETWORK_MODE_UDP:
			UDP_TransmitData(pData, length);
			break;
	}
}

void NetworkPKT_RxRecover()
{
	networkPktRxCmdSize = 0;
}

u8 NetworkPKT_RxData(u8 *pData, u16 length)
{
	u8	result = 0;

	if(length != 0)
	{
		networkPktRxCount = length;

		pNetworkPktRxBuffer = pData;

		networkPktRxFlag = 1;

		result = 1;
	}

	return result;
}

u8 NetworkPKT_PacketChecker(u8 *pData, u16 size)
{
	u8	result = 0;
	u8	stx, etx, nextData;
	u16	length, waitTime;

	// Check STX
	stx = pData[0];
	if(stx != 'T')			return result;

	// Check Length
	length = SYS_HexToHWord(&pData[1]);
	if(length > 1024)		return result;
	if(size < (length + 7))	return result;

	// Check NextData
	nextData = pData[3];
	if(nextData > 1)		return result;

	// Check WaitTime
	waitTime = SYS_HexToHWord(&pData[4]);
	if(waitTime > 0)		return result;

	// Check ETX
	etx = pData[length + 6];
	if(etx != 'E')			return result;

	result = 1;

	return result;
}

u8 NetworkPKT_PacketSender(u8 nextData, u16 waitTime, u8 *pData, u16 length)
{
	u8	result = 0;
	u16	totalLength;

	nextData &= 0x01;
	if(waitTime > 65000)		waitTime = 65000;
	if(length > 1024)			return result;

	networkPktTxBuffer[0] = 'T';										//	STX
	SYS_HWordToHex(length, &networkPktTxBuffer[1]);						//	LENGTH
	networkPktTxBuffer[3] = nextData;									//	NEXTDATA
	SYS_HWordToHex(waitTime, &networkPktTxBuffer[4]);					//	WAITTIME
	memcpy(&networkPktTxBuffer[6], pData, length);						//	DATA
	networkPktTxBuffer[length + 6] = 'E';								//	ETX

	totalLength = length + 7;

	// Transmit Data
	NetworkPKT_TransmitData(networkPktTxBuffer, totalLength);

	result = 1;

	return result;
}

u8 NetworkPKT_TransmitAck(u8 nextData, u8 state)
{
	u8	result = 0, rtn;
	u8	data[4];

	data[0] = state;

	rtn = NetworkPKT_PacketSender(nextData, 0, data, 1);

	if(rtn != 0)	result = 1;

	return result;
}

u8 NetworkPKT_WaitTimeSend(u16 waitTime)
{
	u8	result = 0, rtn;

	if(waitTime == 0)	waitTime = 1;

	rtn = NetworkPKT_PacketSender(1, waitTime, 0, 0);

	if(rtn != 0)	result = 1;

	return result;
}

u8 NetworkPKT_PacketReceiver()
{
	u8	result = NETWORK_PKT_RCV_ERROR, rtn = 0;
	u8p	pData;
	u8	nextData;
	u16	length;
	
	rtn = NetworkPKT_PacketChecker(pNetworkPktRxBuffer, networkPktRxCount);

	networkPktRxFlag = 0;

	if(rtn)
	{
		pData		= &pNetworkPktRxBuffer[6];
		length		= SYS_HexToHWord(&pNetworkPktRxBuffer[1]);
		nextData	= pNetworkPktRxBuffer[3];

		if(networkPktMode == NETWORK_PKT_MODE_CMD)
		{
			memcpy(&pNetworkPktRxCmdBuffer[networkPktRxCmdSize], pData, length);

			networkPktRxCmdSize += length;
		}
		else if(networkPktMode == NETWORK_PKT_MODE_DATA)
		{
			memcpy(&pNetworkPktRxFileBuffer[networkPktRxFileSize], pData, length);

			networkPktRxFileSize += length;
		}

		if(nextData)
		{
			NetworkPKT_TransmitAck(nextData, 1);

			result = NETWORK_PKT_RCV_ING;
		}
		else
		{
			result = NETWORK_PKT_RCV_COMPLETE;
		}
	}
	else
	{
		NetworkPKT_TransmitAck(0, 0);
	}

	return result;
}

u8 NetworkPKT_PacketProcess()
{
	u8	result = 0, rtn;

	if(Ethernet_GetIoFlag())
	{
		ethernetif_set_link(&gnetif);

		Ethernet_ClearIoFlag();
	}

	NetworkPKT_DataProcess();

	if(networkPktRxFlag == 0)	return result;

	rtn = NetworkPKT_PacketReceiver();
	if(rtn == NETWORK_PKT_RCV_COMPLETE)
	{
		result = CMD_PutReceiveData(CMD_COMM_SRC_TYPE_ETHERNET, pNetworkPktRxCmdBuffer, networkPktRxCmdSize);

		NetworkPKT_RxRecover();
	}

	return result;
}

u8 NetworkPKT_FileDownload(u8 *pRcvData, u32 *pRcvSize)
{
	u8	result = NETWORK_PKT_RCV_ERROR, rtn;
	u32	sTime, rTime;

	pNetworkPktRxFileBuffer	= pRcvData;
	networkPktRxFileSize	= 0;

	NetworkPKT_PacketSender(0, 0, "Ready\r\n", 7);

	networkPktMode = NETWORK_PKT_MODE_DATA;

	sTime = HAL_GetTick();
	
	do{
		NetworkPKT_FrameCheck();
		
		if(networkPktRxFlag)
		{
			rtn = NetworkPKT_PacketReceiver();

			if(rtn == NETWORK_PKT_RCV_ING)
			{
				sTime = HAL_GetTick();
			}

			if(rtn == NETWORK_PKT_RCV_COMPLETE)
			{
				result = 1;
				break;
			}
		}

		rTime = HAL_GetTick();

		if((rTime - sTime) > NETWORK_PKT_DATA_DOWNLOAD_TIMEOUT)
		{
			break;
		}
	}while(1);

	networkPktMode = NETWORK_PKT_MODE_CMD;

	*pRcvSize = networkPktRxFileSize;

	return result;
}

u8 NetworkPKT_ReceiveAck(u8 *pState)
{
	u8	result = 0, rtn;
	u32	sTime, rTime;
	u8p	pData;
	u16	length;

	sTime = HAL_GetTick();
	
	do{
		NetworkPKT_FrameCheck();

		if(networkPktRxFlag)
		{
			rtn = NetworkPKT_PacketChecker(pNetworkPktRxBuffer, networkPktRxCount);

			networkPktRxFlag = 0;

			if(rtn)
			{
				pData		= &pNetworkPktRxBuffer[6];
				length		= SYS_HexToHWord(&pNetworkPktRxBuffer[1]);

				if(length == 1)
				{
					*pState = pData[0];
					result = 1;
					break;
				}
				else
				{
					NetworkPKT_TransmitAck(0, 0);
				}
			}
			else
			{
				NetworkPKT_TransmitAck(0, 0);
			}
		}

		rTime = HAL_GetTick();

		if((rTime - sTime) > NETWORK_PKT_RCV_ACK_TIMEOUT)
		{
			break;
		}
	}while(1);

	return result;
}

u8 NetworkPKT_TransmitPacket(u8 *pData, u32 length)
{
	u8	result = 0, rtn;
	u8	state = 0;
	u32	cnt = 0, remainSize = length;

	NetworkPKT_ServerConnect();

	do{
		// TRANSMIT DATA
		if(remainSize > 1024)
		{
			NetworkPKT_PacketSender(1, 0, &pData[cnt], 1024);

			rtn = NetworkPKT_ReceiveAck(&state);
			if(rtn == 0)	break;

			remainSize	-= 1024;
			cnt			+= 1024;
		}
		else
		{
			NetworkPKT_PacketSender(0, 0, &pData[cnt], remainSize);

			result = 1;

			break;
		}
	}while(1);

	NetworkPKT_ServerDisconnect();

	return result;
}

