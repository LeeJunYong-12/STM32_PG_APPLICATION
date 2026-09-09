#ifndef	_APP_EQUIPMENTNY_H
#define	_APP_EQUIPMENTNY_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u32	slotNo;
	u32	status;
	u32	size;
	u8	*pBuffer;
}eNY_RawDataInfo_t;

#if 0	//e7502_delete???_210225_lyh
typedef	struct{
	u32	slotNo;
	u32	coreChannel;
	u32	dsiChannel;
	u32	pktTxMode;
	u32	pktTxSize;
	u32	txDI;
	u32	txReg_First;
	u32	txReg_Continue;
}eNY_PacketWrite_t;

typedef	struct{
	u32	slotNo;
	u32	coreChannel;
	u32	dsiChannel;
	u32	pktTxMode;
	u32	txDI;
}eNY_CustomPacketWrite_t;
#endif
typedef	struct{
	u32	reg;
	u32	status;
	u32	startAdrs;
	u32	size;
}eNY_CustomWrite_t;

#if 1	// add_e7602_210225_lyh
typedef	struct{
	u32	slotNo;
	u32	pktTxSize;
	u32	adr;
    u32 reg_adr;
    u32 position;
}eNY_PacketWrite_t;

typedef	struct{
	u32	slotNo;
	u32	pktRxSize;
	u32	adr;
    u32 reg_adr;
    u32 position;
	u32 cnt;
}eNY_PacketRead_t;
#endif

u8 ENY_BufferInit();												// e7502(cmd_eny), e7602(cmd_system) ok
u32 ENY_GetBufferAdrs(u8 slotNo);									// e7502(cmd_eny), e7602(cmd_system) ok
u8 ENY_SetBufferInfo(u8 slotNo, u8 status, u32 size);				// e7502(cmd_eny), e7602(cmd_system) ok 
u8 ENY_PacketWrite(void *pVoid);									// e7502, e7602 다름 *********
u8 ENY_CustomWriteMapInit();										// e7502
u8 ENY_GetCustomWriteMapData(u8 cnt, void *pVoid);					// e7502
u8 ENY_SetCustomWriteMapData(u8 cnt, void *pVoid);					// e7502
u8 ENY_CustomPacketWrite(void *pVoid);								// e7502

u32 Getsize(u8 slot);												// e7602 add ok
u8 ENY_PacketWrite_DIC(void *pVoid);								// e7602 add ok
u8 ENY_PacketRead(void *pVoid);										// e7602 add ok
u8 ENY_PacketRead_DIC(void *pVoid);									// e7602 add ok
u8 UsbPkt_FileUpload(u8 slot);										// e7602 add ok
u8 NetworkPkt_FileUpload(u8 slot);									// e7602 add ok

#endif	// _APP_EQUIPMENTNY_H
