#ifndef	_SB_PACKET_H
#define	_SB_PACKET_H

#include	"SubBoard.h"

#define SBP_CRC_GENCODE				(u16)0x8408

enum{
	SBP_TX_DATA = 0x01,
	SBP_RX_DATA = 0x10,
	SBP_TRX_DATA = 0x11
};

enum{
	SBP_RCV_STX_1 = 0,
	SBP_RCV_STX_2,
	SBP_RCV_HEADER,
	SBP_RCV_DATA,
	SBP_RCV_CRC_1,
	SBP_RCV_CRC_2,
	SBP_RCV_ETX_1,
	SBP_RCV_ETX_2,
};

void SBP_DataInit(u8 select);
u16 SBP_PacketCRC16(void *pVoid);
u8 SBP_DataMaker(void *pVoid, u8 target, u16 cmd, u8 *pData, u16 length);
u8 SBP_DataSender(void *pVoid);
u8 SBP_ErrorSender(u8 target, u16 cmd);
u8	SBP_DataChecker(void *pVoid, u8 *srcID, u16 *pCmd, u8 *pData);
u8 SBP_DataReceiver(u8 rcvData, void *pVoid);

#endif	//	_SB_PACKET_H