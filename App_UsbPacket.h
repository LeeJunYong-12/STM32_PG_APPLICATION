#ifndef	_APP_USBPACKET_H
#define	_APP_USBPACKET_H

#include	"E7602_PG_Application.h"

#define	USB_PACKET_RX_ACK_TIMEOUT			1000
#define	USB_PACKET_DATA_DOWNLOAD_TIMEOUT	3000

enum{
	USB_PACKET_MODE_COMMAND	= 0,
	USB_PACKET_MODE_DATA,
};

enum{
	USB_PACKET_RCV_ERROR = 0,
	USB_PACKET_RCV_ING,
	USB_PACKET_RCV_COMPLETE
};

u8 UsbPkt_PacketChecker(u8 *pData, u16 size);
u8 UsbPkt_PacketSender(u8 nextData, u8 *pData, u8 length);
u8 UsbPkt_TransmitAck(u8 nextData, u8 state);
u8 UsbPkt_PacketReceiver();
u8 UsbPkt_PacketProcess();
u8 UsbPkt_FileDownload(u8 *pData,u32 *pRcvSize);
u8 UsbPkt_ReceiveAck();
u8 UsbPkt_TransmitPacket(u8 *pData, u32 length);
u8 UsbPkt_Init();

#endif	// _APP_USBPACKET_H