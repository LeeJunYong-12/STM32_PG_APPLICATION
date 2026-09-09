#ifndef	_APP_NETWORKPACKET_H
#define	_APP_NETWORKPACKET_H

#include	"E7602_PG_Application.h"

#define	NETWORK_PKT_DATA_DOWNLOAD_TIMEOUT	4000
#define	NETWORK_PKT_RCV_ACK_TIMEOUT			10000

enum{
	NETWORK_PKT_MODE_CMD = 0,
	NETWORK_PKT_MODE_DATA,
};

enum{
	NETWORK_PKT_RCV_ERROR = 0,
	NETWORK_PKT_RCV_ING,
	NETWORK_PKT_RCV_COMPLETE,
};

void NetworkPKT_FrameCheck();
void NetworkPKT_DataProcess();
void NetworkPKT_ServerConnect();
void NetworkPKT_ServerDisconnect();
void NetworkPKT_TransmitData(u8 *pData, u16 length);
void NetworkPKT_RxRecover();
u8 NetworkPKT_RxData(u8 *pData, u16 length);
u8 NetworkPKT_PacketChecker(u8 *pData, u16 size);
u8 NetworkPKT_PacketSender(u8 nextData, u16 waitTime, u8 *pData, u16 length);
u8 NetworkPKT_TransmitAck(u8 nextData, u8 state);
u8 NetworkPKT_WaitTimeSend(u16 waitTime);
u8 NetworkPKT_PacketReceiver();
u8 NetworkPKT_PacketProcess();
u8 NetworkPKT_FileDownload(u8 *pRcvData, u32 *pRcvSize);
u8 NetworkPKT_ReceiveAck(u8 *pState);
u8 NetworkPKT_TransmitPacket(u8 *pData, u32 length);

#endif	// _APP_NETWORKPACKET_H