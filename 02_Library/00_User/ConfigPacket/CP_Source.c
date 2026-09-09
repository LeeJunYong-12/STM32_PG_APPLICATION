#include	"CP_Source.h"

#define CP_CRC_GENCODE				(u16)0x8408

static	configPacketHandle_t	hConfigPacket;

u8 CP_RegisterInterface(void *pVoid)
{
	u8	result = 0;

	memcpy(&hConfigPacket, pVoid, sizeof(configPacketHandle_t));

	result = 1;

	return result;
}

static u16 CP_GetCrc16(u8 *pData, u16 size)
{
	u8	bitCnt, cData;
	u16 result = 0xffff, byteCnt;

	for(byteCnt = 0; byteCnt < size; byteCnt++)
	{
		cData = pData[byteCnt];

		for(bitCnt = 0; bitCnt < 8; bitCnt++)
		{
			if(((result & 0x0001) ^ ((0x0001 * cData) & 0x0001)) > 0)
			{
				result = ((result >> 1) & 0x7fff) ^ CP_CRC_GENCODE;
			}
			else
			{
				result = (result >> 1) & 0x7fff;
			}
			cData = (cData >> 1) & 0x7f;
		}
	}

	return result;
}

static u8 CP_PacketMaker(u8 *pData, u16 size)
{
	u8	result = 0;
	u16	crc16;

	if(size > CONFIG_PACKET_PROTOCOL_DATA_SIZE)		return result;

	hConfigPacket.pPacket->stx			= CONFIG_PACKET_PROTOCOL_STX;
	hConfigPacket.pPacket->pktType		= hConfigPacket.pktType;
	hConfigPacket.pPacket->pktVersion	= hConfigPacket.pktVersion;
	hConfigPacket.pPacket->length		= CONFIG_PACKET_PROTOCOL_DATA_SIZE;

	memset(hConfigPacket.pPacket->pData, NULL, CONFIG_PACKET_PROTOCOL_DATA_SIZE);
	memcpy(hConfigPacket.pPacket->pData, pData, size);

	crc16 = CP_GetCrc16(hConfigPacket.pPacket->rawData, (CONFIG_PACKET_PROTOCOL_TOTAL_SIZE - CONFIG_PACKET_PROTOCOL_FOOTER_SIZE));

	hConfigPacket.pPacket->crc16		= crc16;
	hConfigPacket.pPacket->etx			= CONFIG_PACKET_PROTOCOL_ETX;

	result = 1;

	return result;
}

static u8 CP_PacketParser()
{
	u8	result = 0, errCnt = 0;
	u16	crc16;

	if(hConfigPacket.pPacket->stx			!= CONFIG_PACKET_PROTOCOL_STX)			errCnt += 1;
	if(hConfigPacket.pPacket->pktType		!= hConfigPacket.pktType)				errCnt += 1;
	if(hConfigPacket.pPacket->pktVersion	!= hConfigPacket.pktVersion)			errCnt += 1;
	if(hConfigPacket.pPacket->length		!= CONFIG_PACKET_PROTOCOL_DATA_SIZE)	errCnt += 1;

	crc16 = CP_GetCrc16(hConfigPacket.pPacket->rawData, (CONFIG_PACKET_PROTOCOL_TOTAL_SIZE - CONFIG_PACKET_PROTOCOL_FOOTER_SIZE));

	if(hConfigPacket.pPacket->crc16			!= crc16)								errCnt += 1;
	if(hConfigPacket.pPacket->etx			!= CONFIG_PACKET_PROTOCOL_ETX)			errCnt += 1;

	if(errCnt == 0)																	result = 1;

	return result;
}

static u8 CP_PacketChecker(u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = CP_PacketParser();
	if(rtn != 0)
	{
		memcpy(pData, hConfigPacket.pPacket->pData, size);

		result = 1;
	}

	return result;
}

u16 CP_GetDataSize()
{
	return CONFIG_PACKET_PROTOCOL_DATA_SIZE;
}

u8 CP_WritePacket(u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	if(size > CONFIG_PACKET_PROTOCOL_TOTAL_SIZE)		return result;
	if(hConfigPacket.WritePacket == NULL)				return result;

	rtn = CP_PacketMaker(pData, size);
	if(rtn == 0)										return result;

	rtn = hConfigPacket.WritePacket(hConfigPacket.pPacket->rawData, CONFIG_PACKET_PROTOCOL_TOTAL_SIZE);
	if(rtn != 0)							result = 1;

	return result;
}

u8 CP_ReadPacket(u8 *pData, u16 size)
{
	u8 result = 0, rtn;

	if(size > CONFIG_PACKET_PROTOCOL_TOTAL_SIZE)		return result;
	if(hConfigPacket.ReadPacket == NULL)				return result;

	rtn = hConfigPacket.ReadPacket(hConfigPacket.pPacket->rawData, CONFIG_PACKET_PROTOCOL_TOTAL_SIZE);
	if(rtn == 0)										return result;

	rtn = CP_PacketChecker(pData, size);
	if(rtn != 0)										result = 1;

	return result;
}

u8 CP_VerifyPacket(u8 *pData, u16 size)
{
	u8	result = 0, rtn;
	u32	chkr;

	if(size > CONFIG_PACKET_PROTOCOL_TOTAL_SIZE)		return result;
	if(hConfigPacket.ReadPacket == NULL)				return result;

	rtn = hConfigPacket.ReadPacket(hConfigPacket.pPacket->rawData, CONFIG_PACKET_PROTOCOL_TOTAL_SIZE);
	if(rtn == 0)										return result;

	rtn = CP_PacketParser();
	if(rtn == 0)										return result;

	chkr = memcmp(hConfigPacket.pPacket->pData, pData, size);
	if(chkr == 0)										result = 1;

	return result;
}

