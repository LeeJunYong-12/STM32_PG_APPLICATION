#include	"Cmd_EquipmentNY.h"

static u8 Cmd_ENY_RawDataInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = ENY_BufferInit();

	return result;
}

static u8 Cmd_ENY_RawDataSave(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slotNo, status = 1;
	u8	*pMemoryBuffer;
	u32	size, rxSrc, rcvSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	slotNo	= CMD_StrToUL(pData->pArgv[0]);
	if(slotNo >= 8)				return result;

	size	= CMD_StrToUL(pData->pArgv[1]);
	if(size > 64 * 1024)		return result;

	rxSrc = pData->srcType;

	pMemoryBuffer = (u8*)ENY_GetBufferAdrs(slotNo);
	if(pMemoryBuffer == NULL)	return result;

	if(rxSrc == CMD_COMM_SRC_TYPE_USB)
	{
		rtn = UsbPkt_FileDownload(pMemoryBuffer, &rcvSize);
	}
	else if(rxSrc == CMD_COMM_SRC_TYPE_ETHERNET)
	{
		rtn = NetworkPKT_FileDownload(pMemoryBuffer, &rcvSize);
	}
	else						return result;

	if(rtn == 0)				return result;

	if(size != rcvSize)			return result;

	result = ENY_SetBufferInfo(slotNo, status, size);

	return result;
}

static u8 Cmd_ENY_RawDataLoad(void *pVoid)
{
	u8	result = 0;
	u8	slotNo;
	u32	txSrc;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	slotNo	= CMD_StrToUL(pData->pArgv[0]);
	if(slotNo >= 8)				return result;

	txSrc = pData->srcType;

	if(txSrc == CMD_COMM_SRC_TYPE_USB)
	{
		result = UsbPkt_FileUpload(slotNo);
	}
	else if(txSrc == CMD_COMM_SRC_TYPE_ETHERNET)
	{
		result = NetworkPkt_FileUpload(slotNo);
	}

	return result;
}

static u8 Cmd_ENY_CustomWriteMapInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = ENY_CustomWriteMapInit();

	return result;
}

static u8 Cmd_ENY_GetCustomMapData(void *pVoid)
{
	u8	result = 0, rtn;
	eNY_CustomWrite_t	mapData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	for(u8 cnt = 0; cnt < 128; cnt++)
	{
		rtn = ENY_GetCustomWriteMapData(cnt, &mapData);
		if(rtn)
		{
			if(mapData.status == 0x00)		break;
			else							CMD_Printf(";INDEX=%d,REG=0x%02X,MEMADRS=%d,SIZE=%d", cnt, mapData.reg, mapData.startAdrs, mapData.size);
		}
	}

	result = 1;

	return result;
}

static u8 Cmd_ENY_SetCustomMapData(void *pVoid)
{
	u8	result = 0;
	u8	cnt;
	eNY_CustomWrite_t	mapData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	cnt					= CMD_StrToUL(pData->pArgv[0]);
	mapData.reg			= CMD_StrToUL(pData->pArgv[1]);
	mapData.status		= 0x01;
	mapData.startAdrs	= CMD_StrToUL(pData->pArgv[2]);
	mapData.size		= CMD_StrToUL(pData->pArgv[3]);

	result = ENY_SetCustomWriteMapData(cnt, &mapData);

	return result;
}
#if 0	// later...
static u8 Cmd_ENY_CustomPacketWrite(void *pVoid)
{
	u8	result = 0;
	eNY_PacketWrite_t	wPacket;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	memset(&wPacket, NULL, sizeof(wPacket));

	wPacket.slotNo			= CMD_StrToUL(pData->pArgv[0]);
	wPacket.position		= pData->position + 1;	//board1: position=2, board2: position=3

	result = ENY_CustomPacketWrite(&wPacket);

	return result;
}
#endif
#if 0	// e7502_210225_lyh
static u8 Cmd_ENY_PacketWrite(void *pVoid)
{
	u8	result = 0;
	eNY_PacketWrite_t	wPacket;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 6)		return result;

	memset(&wPacket, NULL, sizeof(wPacket));

	wPacket.slotNo			= CMD_StrToUL(pData->pArgv[0]);
	wPacket.coreChannel		= pData->position;
	wPacket.dsiChannel		= pData->option;
	if(CMD_Compare(pData->pArgv[1], "LP"))			wPacket.pktTxMode = 0;
	else if(CMD_Compare(pData->pArgv[1], "HS"))		wPacket.pktTxMode = 1;
	wPacket.pktTxSize		= CMD_StrToUL(pData->pArgv[2]);
	wPacket.txDI			= CMD_StrToUL(pData->pArgv[3]);
	wPacket.txReg_First		= CMD_StrToUL(pData->pArgv[4]);
	wPacket.txReg_Continue	= CMD_StrToUL(pData->pArgv[5]);

	result = ENY_PacketWrite(&wPacket);

	return result;
}

#endif
const	commandList_t	cmdList_EquipmentNY[] =	{
//	{	Command,				Function,							Option,		Mode,		RootMessage					Note}
	{	"help",					Cmd_Help,							NULL,		0x01,		";HELP",					NULL},
	{	"RawData.Init",			Cmd_ENY_RawDataInit,				NULL,		0x01,		";RAWDATA.INIT",			NULL},	//ok
	{	"RawData.Save",			Cmd_ENY_RawDataSave,				NULL,		0x01,		";RAWDATA.SAVE",			NULL},	//ok
	{	"RawData.Load",			Cmd_ENY_RawDataLoad,				NULL,		0x01,		";RAWDATA.LOAD",			NULL},	//ok
#if 1	// e7502_210225_lyh
//	{	"RawData.Write_T1",		Cmd_ENY_PacketWrite,				NULL,		NULL,		";RAWDATA.WRITE_T1",		NULL},
//	{	"RawData.Write_T2",		Cmd_ENY_CustomPacketWrite,			NULL,		NULL,		";RAWDATA.WRITE_T2",		NULL},
	{	"Map.Init",				Cmd_ENY_CustomWriteMapInit,			NULL,		0x01,		";MAP.INIT",				NULL},
	{	"Map.Get",				Cmd_ENY_GetCustomMapData,			NULL,		0x01,		";MAP.GET",					NULL},
	{	"Map.Set",				Cmd_ENY_SetCustomMapData,			NULL,		0x01,		";MAP.SET",					NULL},
#endif
	{	NULL,					NULL,								NULL,		NULL,		NULL,						NULL},
};

