#ifndef	_SUBBOARD_H
#define	_SUBBOARD_H

#include	"UserDefine.h"

#include	"SB_Interface.h"
#include	"SB_Packet.h"
#include	"SB_Process.h"

#define	SBP_PACKET_DATA_MAX_SIZE	4096

#define	SBP_PACKET_HEADER_SIZE		12

#define SBP_STX_1					'T'
#define SBP_STX_2					'E'
#define	SBP_ETX						0x4321

#define	SUBBOARD_XFER_TYP_TIMEOUT	3000
#define	SUBBOARD_XFER_MAX_TIMEOUT	5000

typedef	struct{
	u32	position;
	u32	channel;
	u32	cmd;
	u32	waitTime;
	u8	*pTxData;
	u8	*pRxData;
	u32	txSize;
	u32	rxSize;
}sbParsingData_t;

typedef	struct{
	u32	cmd;
	u8	(*pFunction)	(void*);
	u8	*pNote;
}sbCmdList_t;

typedef	union{
	u8	rawData[2];
	u16	u16Data;

	struct{
		u16	cmd			: 15;
		u16	response	:1;
	};
}sbCmd_t;

typedef	union{
		u8	rawData[20];

		struct{
			u8	stx[2];
			u8	dstID;
			u8	srcID;
			u16	mode;
			u16	type;
			u16	cmd;
			u16	length;
			u8	*pData;
			u16	crc16;
			u16	etx;
		};
}sbProtocol_t;

typedef	struct{
	u16	myID;
	u16	mode;
	u16	type;
	u16	spare;

	u8	*pRxBuffer;
	u32	rxBufferSize;
	u8	*pTxBuffer;
	u32	txBufferSize;
	u8	*pTempBuffer;
	u32	tempBufferSize;
	u8	*pTransmitBuffer;
	u32	transmitBufferSize;

	sbCmdList_t		*pCmdList;						//	SLAVE ONLY

	u8	(*ChannelSelect)	(u16);					//	MASTER ONLY

	u8	(*GetChar)			(u8*);
	u8	(*TransmitData)		(u8*, u16);

	u8	(*RunningState)		(u8);					//	SLAVE ONLY
	u8	(*ErrorState)		(u8);					//	SLAVE ONLY

	sbProtocol_t	rxPkt;
	sbProtocol_t	txPkt;
}sbHandle_t;

#endif	//	_SUBBOARD_H
