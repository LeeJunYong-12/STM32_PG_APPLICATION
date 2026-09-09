#include	"SB_Packet.h"

extern	sbHandle_t 	sbHandle;

void SBP_DataInit(u8 select)
{
	u8	rtn, temp;

	if(select & SBP_TX_DATA)
	{
		memset(sbHandle.txPkt.rawData, NULL, sizeof(sbProtocol_t));
		sbHandle.txPkt.pData = sbHandle.pTempBuffer;
	}

	if(select & SBP_RX_DATA)
	{
		memset(sbHandle.rxPkt.rawData, NULL, sizeof(sbProtocol_t));
		sbHandle.rxPkt.pData = sbHandle.pTempBuffer;

		do{
			rtn = sbHandle.GetChar(&temp);

			if(rtn == 0)		break;
		}while(1);
	}
}

u16 SBP_PacketCRC16(void *pVoid)
{
	u8	bitCnt, cData;
	u16 result = 0xffff, byteCnt;
	sbProtocol_t *pPacket;

	pPacket = (sbProtocol_t*)pVoid;

	for(byteCnt = 0; byteCnt < SBP_PACKET_HEADER_SIZE; byteCnt++)
	{
		cData = pPacket->rawData[byteCnt];

		for(bitCnt = 0; bitCnt < 8; bitCnt++)
		{
			if(((result & 0x0001) ^ ((0x0001 * cData) & 0x0001)) > 0)
			{
				result = ((result >> 1) & 0x7fff) ^ SBP_CRC_GENCODE;
			}
			else
			{
				result = (result >> 1) & 0x7fff;
			}
			cData = (cData >> 1) & 0x7f;
		}
	}

	for(byteCnt = 0; byteCnt < pPacket->length; byteCnt++)
	{
		cData = pPacket->pData[byteCnt];

		for(bitCnt = 0; bitCnt < 8; bitCnt++)
		{
			if(((result & 0x0001) ^ ((0x0001 * cData) & 0x0001)) > 0)
			{
				result = ((result >> 1) & 0x7fff) ^ SBP_CRC_GENCODE;
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

u8 SBP_DataMaker(void *pVoid, u8 target, u16 cmd, u8 *pData, u16 length)
{
	u8				result = 0;
	sbProtocol_t	*pPacket;

	if(length > SBP_PACKET_DATA_MAX_SIZE)	return result;

	pPacket				= (sbProtocol_t*)pVoid;

	pPacket->stx[0]		= SBP_STX_1;
	pPacket->stx[1]		= SBP_STX_2;

	pPacket->dstID		= target;
	pPacket->srcID		= (u8)sbHandle.myID;

	pPacket->mode		= sbHandle.mode;
	pPacket->type		= sbHandle.type;

	pPacket->cmd		= cmd;

	pPacket->length		= length;

	memcpy(pPacket->pData, pData, length);

	pPacket->crc16		= SBP_PacketCRC16(pVoid);
	pPacket->etx		= SBP_ETX;

	result = 1;

	return result;
}

u8 SBP_DataSender(void *pVoid)
{
	u8	result = 0, rtn;
	u16	length = 0;
	sbProtocol_t *pPacket;

	pPacket = (sbProtocol_t*)pVoid;

	memcpy(&sbHandle.pTransmitBuffer[length], pPacket->rawData, SBP_PACKET_HEADER_SIZE);
	length += SBP_PACKET_HEADER_SIZE;
	memcpy(&sbHandle.pTransmitBuffer[length], pPacket->pData, pPacket->length);
	length += pPacket->length;
	memcpy(&sbHandle.pTransmitBuffer[length], &pPacket->crc16, 4);
	length += 4;

	rtn = sbHandle.TransmitData(sbHandle.pTransmitBuffer, length);
	if(rtn != 0)	result = 1;

	return result;
}

u8 SBP_ErrorSender(u8 target, u16 cmd)
{
	u8	result = 0, rtn;
	sbProtocol_t txPacket;
	sbCmd_t	txCmd;

	txCmd.u16Data = cmd;
	txCmd.response = 1;
	txCmd.cmd = 0;

	rtn = SBP_DataMaker(&txPacket, target, txCmd.u16Data, NULL, NULL);
	if(rtn == 0)		return result;

	rtn = SBP_DataSender(&txPacket);
	if(rtn != 0)		result = 1;

	return result;
}

u8	SBP_DataChecker(void *pVoid, u8 *srcID, u16 *pCmd, u8 *pData)
{
	u8 result = 0, errCnt = 0;
	u16 crc16;
	sbProtocol_t *pPacket;

	pPacket = (sbProtocol_t*)pVoid;

	if(pPacket->stx[0]	!= SBP_STX_1)			errCnt += 1;
	if(pPacket->stx[1]	!= SBP_STX_2)			errCnt += 1;

	if(pPacket->mode	!= sbHandle.mode)		errCnt += 1;
	if(pPacket->type	!= sbHandle.type)		errCnt += 1;

	crc16 = SBP_PacketCRC16(pPacket);

	if(pPacket->crc16	!= crc16)				errCnt += 1;
	if(pPacket->etx		!= SBP_ETX)				errCnt += 1;

	if(errCnt == 0)
	{
		*srcID = pPacket->srcID;

		*pCmd = pPacket->cmd;

		memcpy(pData, pPacket->pData, pPacket->length);

		result = 1;
	}

	return result;
}

u8 SBP_DataReceiver(u8 rcvData, void *pVoid)
{
	u8 result = 0;
	static u16 state = 0, rcvCnt;
	sbProtocol_t *pPacket;

	pPacket = (sbProtocol_t*)pVoid;

	switch(state)
	{
		case SBP_RCV_STX_1:
			if(rcvData == SBP_STX_1)
			{
				pPacket->stx[0] = rcvData;
				state++;
			}
			break;

		case SBP_RCV_STX_2:
			if(rcvData == SBP_STX_2)
			{
				pPacket->stx[1] = rcvData;
				rcvCnt = 2;
				state++;
			}
			else
			{
				state = 0;
			}
			break;

		case SBP_RCV_HEADER:
			pPacket->rawData[rcvCnt++] = rcvData;

			if(rcvCnt >= SBP_PACKET_HEADER_SIZE)
			{
				rcvCnt = 0;
				state++;
				if(pPacket->length == 0)
				{
					state++;
				}
			}
			break;

		case SBP_RCV_DATA:
			pPacket->pData[rcvCnt++] = rcvData;

			if(rcvCnt >= pPacket->length)
			{
				state++;
			}
			break;

		case SBP_RCV_CRC_1:
			pPacket->rawData[16] = rcvData;

			state++;
			break;

		case SBP_RCV_CRC_2:
			pPacket->rawData[17] = rcvData;

			state++;
			break;

		case SBP_RCV_ETX_1:
			pPacket->rawData[18] = rcvData;

			state++;
			break;

		case SBP_RCV_ETX_2:
			pPacket->rawData[19] = rcvData;

			result = 1;
		default:
			state = 0;
			break;
	}

	return result;
}


