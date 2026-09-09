#ifndef	_CONFIG_PACKET_H
#define	_CONFIG_PACKET_H

#include	"UserDefine.h"

#include	"CP_Source.h"

#define	CONFIG_PACKET_PROTOCOL_STX			0x12345678
#define	CONFIG_PACKET_PROTOCOL_ETX			0x4321

#define	CONFIG_PACKET_PROTOCOL_TOTAL_SIZE	1024

#define	CONFIG_PACKET_PROTOCOL_HEADER_SIZE	16
#define	CONFIG_PACKET_PROTOCOL_DATA_SIZE	1004
#define	CONFIG_PACKET_PROTOCOL_FOOTER_SIZE	4

#if(CONFIG_PACKET_PROTOCOL_TOTAL_SIZE != (CONFIG_PACKET_PROTOCOL_HEADER_SIZE + CONFIG_PACKET_PROTOCOL_DATA_SIZE + CONFIG_PACKET_PROTOCOL_FOOTER_SIZE))
	#error Please, Check (Config Packet) Protocol Size.
#endif

typedef	union{
	u8	rawData[CONFIG_PACKET_PROTOCOL_TOTAL_SIZE];

	struct{
		u32	stx;										//	4
		u32	pktType;									//	4
		u32	pktVersion;									//	4
		u32	length;										//	4
		u8	pData[CONFIG_PACKET_PROTOCOL_DATA_SIZE];	//	4
		u16	crc16;										//	2
		u16	etx;										//	2
	};
}configPacket_t;

typedef	struct{
	u32	pktType;
	u32	pktVersion;
	u8	(*WritePacket)		(u8*, u16);
	u8	(*ReadPacket)		(u8*, u16);
	configPacket_t	*pPacket;
}configPacketHandle_t;

#endif	//	_CONFIG_PACKET_H