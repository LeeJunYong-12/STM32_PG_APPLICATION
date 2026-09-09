#include	"App_EquipmentNY.h"

static	eNY_RawDataInfo_t	eNY_RawDataInfo[8];
static	u8	eNY_PacketWriteBuffer[4096 + 128];
#if 1	// e7502_210225_lyh
static	eNY_CustomWrite_t	eNY_CustomWrite[128 + 2] = {
	{	0x64,	0x01,		0,			98},
	{	0x65,	0x01,		99,			65},
	{	0x69,	0x01,		165,		63},
	{	0x6B,	0x01,		229,		63},
	{	0x6C,	0x01,		293,		98},
	{	0x6D,	0x01,		392,		98},
	{	0x6F,	0x01,		491,		98},
	{	0x70,	0x01,		590,		98},
	{	0x71,	0x01,		689,		98},
	{	0x72,	0x01,		788,		98},
	{	0x73,	0x01,		887,		98},
	{	0x74,	0x01,		986,		98},
	{	0x75,	0x01,		1085,		98},
	{	0x76,	0x01,		1184,		98},
	{	0x77,	0x01,		1283,		98},
	{	0x78,	0x01,		1382,		98},
	{	0x79,	0x01,		1481,		98},
	{	0x7E,	0x01,		1580,		98},
	{	0x7F,	0x01,		1679,		98},
	{	0x80,	0x01,		1778,		98},
	{	0x84,	0x01,		1877,		98},
	{	0x8E,	0x01,		1976,		98},
	{	0x91,	0x01,		2075,		98},
	{	0x92,	0x01,		2174,		98},
	{	0xA8,	0x01,		2273,		98},
	{	0xDE,	0x01,		2372,		98},
	{	0xDF,	0x01,		2471,		98},
	{	0xE0,	0x01,		2570,		98},
	{	0xE9,	0x01,		2669,		98},
	{	0xEA,	0x01,		2768,		98},
	{	0xFA,	0x01,		2867,		98},
	{	0xFB,	0x01,		2966,		98},
	{	0xBB,	0x01,		3065,		63},
	{	0xBC,	0x01,		3129,		63},
	{	0xBD,	0x01,		3193,		63},
	{	0xBF,	0x01,		3257,		63},
	{	0xE5,	0x01,		3321,		63},
	{	0xE6,	0x01,		3385,		63},
	{	0xE7,	0x01,		3449,		63},
	{	0xE8,	0x01,		3513,		63},
	{	0xFD,	0x01,		3577,		56},
	{	0x00,	0x00,		0,			0},
};
#endif
u8 ENY_BufferInit()
{
	u8	result = 0;
	u32	memSize = 64 * 1024;

	memset(&eNY_RawDataInfo, NULL, sizeof(eNY_RawDataInfo));

	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		eNY_RawDataInfo[cnt].slotNo		= cnt;
		eNY_RawDataInfo[cnt].pBuffer	= (u8*)(MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_4_ADRS + (memSize * cnt));

		memset(eNY_RawDataInfo[cnt].pBuffer, NULL, memSize);
	}

	result = 1;

	return result;
}

u32 Getsize(u8 slot)
{
	return eNY_RawDataInfo[slot].size;
}

u32 ENY_GetBufferAdrs(u8 slotNo)
{
	u32	result = 0;

	if(slotNo >= 8)			return result;

	result = (u32)eNY_RawDataInfo[slotNo].pBuffer;

	return result;
}

u8 ENY_SetBufferInfo(u8 slotNo, u8 status, u32 size)
{
	u8	result = 0;

	if(slotNo >= 8)			return result;
	if(status > 1)			return result;
	if(size > (64 * 1024))	return result;

	eNY_RawDataInfo[slotNo].status	= status;
	eNY_RawDataInfo[slotNo].size	= size;

	result = 1;

	return result;
}
#if 0	// e7502_210225_lyh
u8 ENY_PacketWrite(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	firstTime = 1;
	u32	txCnt = 0, totalSize;
	mipiPacketWrite_t	wPacket;
	eNY_PacketWrite_t	*pData;

	pData = (eNY_PacketWrite_t*)pVoid;

	if(pData->slotNo >= 8)		return result;

	if(eNY_RawDataInfo[pData->slotNo].status == 0)
	{
		return result;
	}

	if(pData->pktTxSize > 4096)
	{
		return result;
	}

	memset(&wPacket, NULL, sizeof(wPacket));
	memset(eNY_PacketWriteBuffer, NULL, sizeof(eNY_PacketWriteBuffer));

	wPacket.channel	= pData->dsiChannel;
	wPacket.lpHs	= pData->pktTxMode;

	wPacket.di		= pData->txDI;
	wPacket.pPacket	= eNY_PacketWriteBuffer;

	totalSize = eNY_RawDataInfo[pData->slotNo].size;

	do{
		if(firstTime)	wPacket.pPacket[0]	= (u8)pData->txReg_First;
		else			wPacket.pPacket[0]	= (u8)pData->txReg_Continue;

		firstTime = 0;

		wPacket.wordCnt = ((totalSize - txCnt) > pData->pktTxSize) ? (pData->pktTxSize) : (totalSize - txCnt);
		wPacket.wordCnt += 1;

		memcpy(&wPacket.pPacket[1], &eNY_RawDataInfo[pData->slotNo].pBuffer[txCnt], (wPacket.wordCnt - 1));

		rtn = MIPI_PacketWrite(pData->coreChannel, &wPacket);
		if(rtn == 0)				errCnt += 1;

		txCnt += wPacket.wordCnt - 1;

		if(txCnt >= totalSize)		break;
	}while(1);

	if(errCnt == 0)					result = 1;

	return result;
}
#endif

u8 ENY_PacketWrite(void *pVoid)
{
	u8	result = 0, rtn = 0, errCnt = 0;
	u32	txCnt = 0, totalSize;
	dptxTransaction_t	wPacket;
	eNY_PacketWrite_t	*pData;

	pData = (eNY_PacketWrite_t*)pVoid;

	if(pData->slotNo >= 8)		return result;

	if(eNY_RawDataInfo[pData->slotNo].status == 0)
	{
		return result;
	}

	if(pData->pktTxSize > 256)
	{
		return result;
	}

	memset(&wPacket, NULL, sizeof(wPacket));
	memset(eNY_PacketWriteBuffer, NULL, sizeof(eNY_PacketWriteBuffer));

	wPacket.channel	= pData->position;
	wPacket.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_WRITE;
	wPacket.adrs	= pData->adr;
    wPacket.pData   = eNY_PacketWriteBuffer;

	totalSize = eNY_RawDataInfo[pData->slotNo].size;
	//totalSize = 65536;

	do{

		wPacket.size = ((totalSize - txCnt) > pData->pktTxSize) ? (pData->pktTxSize) : (totalSize - txCnt);

		memcpy(&wPacket.pData[0], &eNY_RawDataInfo[pData->slotNo].pBuffer[txCnt], (wPacket.size));

		rtn = DPTX_Transaction(&wPacket);
      		if(rtn == 0)    errCnt++;

		txCnt += wPacket.size;

		HAL_Delay(1);

		if(txCnt >= totalSize)
		{
		  break;
		}

	}while(1);

	if(errCnt == 0)					result = 1;

	return result;
}

u8 ENY_PacketWrite_DIC(void *pVoid)
{
	u8	result = 0, rtn = 0, errCnt = 0;
	u32	txCnt = 0, totalSize;
	dptxRegData_t		wPacket;
	eNY_PacketWrite_t	*pData;

	pData = (eNY_PacketWrite_t*)pVoid;

	if(pData->slotNo >= 8)		return result;

	if(eNY_RawDataInfo[pData->slotNo].status == 0)
	{
		return result;
	}

	if(pData->pktTxSize > 1024)
	{
		return result;
	}

	memset(&wPacket, NULL, sizeof(wPacket));
	memset(eNY_PacketWriteBuffer, NULL, sizeof(eNY_PacketWriteBuffer));

	wPacket.channel	    = pData->position;
    wPacket.dataAdrs    = pData->adr;
	wPacket.regAdrs     = pData->reg_adr;
    wPacket.pBuffer     = eNY_PacketWriteBuffer;

	totalSize = eNY_RawDataInfo[pData->slotNo].size;
	//totalSize = 65536;

	do{

		wPacket.dataSize = ((totalSize - txCnt) > pData->pktTxSize) ? (pData->pktTxSize) : (totalSize - txCnt);

		memcpy(&wPacket.pBuffer[0], &eNY_RawDataInfo[pData->slotNo].pBuffer[txCnt], (wPacket.dataSize));

		rtn = DPTX_RegDataWriteDIC(&wPacket);
      	if(rtn == 0)    errCnt++;

		txCnt += wPacket.dataSize;

		HAL_Delay(1);

		if(txCnt >= totalSize)
		{
		  break;
		}

	}while(1);

	if(errCnt == 0)					result = 1;

	return result;
}

u8 ENY_PacketRead(void *pVoid)
{
	u8	result = 0, rtn = 0, errCnt = 0;
	u32 cycle_cnt, slot;
	dptxTransaction_t	wPacket;
	eNY_PacketRead_t	*pData;

	pData = (eNY_PacketRead_t*)pVoid;

	if(pData->slotNo >= 8)		return result;

	if(pData->pktRxSize > 256)
	{
		return result;
	}

	cycle_cnt = pData->cnt;
	slot 	  = pData->slotNo;

	memset(&wPacket, NULL, sizeof(wPacket));
	memset(eNY_PacketWriteBuffer, NULL, sizeof(eNY_PacketWriteBuffer));

	wPacket.channel	= pData->position;
	wPacket.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_READ;
	wPacket.adrs	= pData->adr;
    wPacket.pData   = eNY_PacketWriteBuffer;
	wPacket.size 	= pData->pktRxSize;

	for(u32 i=0; i<cycle_cnt; i++)
	{
		rtn = DPTX_Transaction(&wPacket);
		if(rtn == 0)    errCnt++;

		HAL_Delay(1);

		memcpy(&eNY_RawDataInfo[slot].pBuffer[i*wPacket.size], &wPacket.pData[0], wPacket.size);
	}

	eNY_RawDataInfo[slot].size = cycle_cnt*wPacket.size;

	if(errCnt == 0)					result = 1;

	return result;
}

u8 ENY_PacketRead_DIC(void *pVoid)
{
	u8	result = 0, rtn = 0, errCnt = 0;
	u32 cycle_cnt, slot;
	dptxRegData_t		wPacket;
	eNY_PacketRead_t	*pData;

	pData = (eNY_PacketRead_t*)pVoid;

	if(pData->slotNo >= 8)		return result;

	if(pData->pktRxSize > 1024)
	{
		return result;
	}

	cycle_cnt = pData->cnt;
	slot 	  = pData->slotNo;

	memset(&wPacket, NULL, sizeof(wPacket));
	memset(eNY_PacketWriteBuffer, NULL, sizeof(eNY_PacketWriteBuffer));

	wPacket.channel	 = pData->position;
	wPacket.dataAdrs = pData->adr;
	wPacket.regAdrs  = pData->reg_adr;
    wPacket.pBuffer  = eNY_PacketWriteBuffer;
	wPacket.dataSize = pData->pktRxSize;

	for(u32 i=0; i<cycle_cnt; i++)
	{
		rtn = DPTX_RegDataReadDIC(&wPacket);
		if(rtn == 0)    errCnt++;

		HAL_Delay(1);

		memcpy(&eNY_RawDataInfo[slot].pBuffer[i*wPacket.dataSize], &wPacket.pBuffer[0], wPacket.dataSize);
	}

	eNY_RawDataInfo[slot].size = cycle_cnt*wPacket.dataSize;

	if(errCnt == 0)					result = 1;

	return result;
}

u8 UsbPkt_FileUpload(u8 slot)
{
	u8	result = 0, rtn, errcnt = 0;
	u32 filesize;
	u32 txcnt=0;

	filesize = Getsize(slot);

	if(filesize <= 200)
	{
		rtn = UsbPkt_PacketSender(0, eNY_RawDataInfo[slot].pBuffer, filesize);
		if(rtn==0)
		{
			errcnt++;
		}
	}
	else
	{
		do{
			rtn = UsbPkt_PacketSender(1, &eNY_RawDataInfo[slot].pBuffer[txcnt], 200);
			if(rtn==0)
			{
				errcnt++;
				break;
			}

			HAL_Delay(100);

			txcnt = txcnt +200;
			filesize=filesize-200;

			if(filesize<=200)
			{
				rtn = UsbPkt_PacketSender(0, &eNY_RawDataInfo[slot].pBuffer[txcnt], filesize);
				break;
			}


		}while(1);
	}

	if(errcnt ==0)	result =1;

	return result;
}

u8 NetworkPkt_FileUpload(u8 slot)
{
	u8	result = 0, rtn, errcnt = 0;
	u32 filesize;
	u32 txcnt=0;

	filesize = Getsize(slot);

	if(filesize <= 500)
	{
		rtn = NetworkPKT_PacketSender(0, 0, eNY_RawDataInfo[slot].pBuffer, filesize);
		if(rtn==0)
		{
			errcnt++;
		}
	}
	else
	{
		do{
			rtn = NetworkPKT_PacketSender(1,0, &eNY_RawDataInfo[slot].pBuffer[txcnt], 500);
			if(rtn==0)
			{
				errcnt++;
				break;
			}

			txcnt = txcnt +500;
			filesize=filesize-500;

			if(filesize<=500)
			{
				rtn = NetworkPKT_PacketSender(0, 0, &eNY_RawDataInfo[slot].pBuffer[txcnt], filesize);
				break;
			}

			HAL_Delay(1);

		}while(1);
	}

	if(errcnt ==0)	result =1;

	return result;
}

u8 ENY_CustomWriteMapInit()
{
	u8	result = 0;

	memset(&eNY_CustomWrite, NULL, sizeof(eNY_CustomWrite));

	result = 1;

	return result;
}

u8 ENY_GetCustomWriteMapData(u8 cnt, void *pVoid)
{
	u8	result = 0;
	eNY_CustomWrite_t *pData;

	pData = (eNY_CustomWrite_t*)pVoid;

	if(cnt >= 128)			return result;

	memcpy(&pData, &eNY_CustomWrite[cnt], sizeof(eNY_CustomWrite_t));

	result = 1;

	return result;
}

u8 ENY_SetCustomWriteMapData(u8 cnt, void *pVoid)
{
	u8	result = 0;
	eNY_CustomWrite_t *pData;

	pData = (eNY_CustomWrite_t*)pVoid;

	if(cnt >= 128)			return result;

	memcpy(&eNY_CustomWrite[cnt], &pData, sizeof(eNY_CustomWrite_t));

	result = 1;

	return result;
}
#if 0	// e7502_210225_lyh
u8 ENY_CustomPacketWrite(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	txCnt = 0;
	dptxTransaction_t	wPacket;
	eNY_PacketWrite_t	*pData;

	pData = (eNY_PacketWrite_t*)pVoid;

	if(pData->slotNo >= 8)		return result;

	if(eNY_RawDataInfo[pData->slotNo].status == 0)
	{
		return result;
	}

	memset(&wPacket, NULL, sizeof(wPacket));
	memset(eNY_PacketWriteBuffer, NULL, sizeof(eNY_PacketWriteBuffer));

	wPacket.channel	= pData->position;
//	wPacket.lpHs	= pData->pktTxMode;

	wPacket.di		= pData->txDI;
	wPacket.pPacket	= eNY_PacketWriteBuffer;

	do{
		if(txCnt >= 128)							break;
		if(eNY_CustomWrite[txCnt].status == 0)		break;

		wPacket.pPacket[0] = (u8)eNY_CustomWrite[txCnt].reg;

		wPacket.wordCnt = eNY_CustomWrite[txCnt].size;
		wPacket.wordCnt += 1;

		memcpy(&wPacket.pPacket[1], &eNY_RawDataInfo[pData->slotNo].pBuffer[eNY_CustomWrite[txCnt].startAdrs], eNY_CustomWrite[txCnt].size);

		rtn = MIPI_PacketWrite(pData->coreChannel, &wPacket);
		if(rtn == 0)				errCnt += 1;

		txCnt += 1;
	}while(1);

	if(errCnt == 0)					result = 1;

	return result;
}
#endif

