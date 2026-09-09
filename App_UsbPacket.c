#include	"App_UsbPacket.h"

static	u8	usbPktMode;
static	u8	usbPktTxBuffer[512];

static	u8	*pUsbCmdBuffer = NULL;
static	u32	usbCmdBufferCnt;

static	u8	*pUsbDataBuffer = NULL;
static	u32	usbDataBufferCnt;

u8 UsbPkt_PacketChecker(u8 *pData, u16 size)
{
	u8	result = 0;
	u8	stx, length, nextData, etx;

	stx = pData[0];
	if(stx != 'T')				return result;

	length = pData[1];
	if(length > 60)				return result;
	if(size < (length + 4))		return result;

	nextData = pData[2];
	if(nextData > 1)			return result;

	etx = pData[length + 3];
	if(etx != 'E')				return result;

	result = 1;
	return result;
}

u8 UsbPkt_PacketSender(u8 nextData, u8 *pData, u8 length)
{
	u8	result = 0;
	u16	totalSize;

	usbPktTxBuffer[0] = 'T';										// STX
	usbPktTxBuffer[1] = length;										// LENGTH
	usbPktTxBuffer[2] = nextData & 0x01;							// NEXTDATA
	memcpy(&usbPktTxBuffer[3], pData, length);						// DATA
	usbPktTxBuffer[length + 3] = 'E';								// ETX

	totalSize = length + 4;

	if((totalSize % 64) == 0)
	{
		usbPktTxBuffer[totalSize] = 0;
		totalSize += 1;
	}

	result = USB_TxData(usbPktTxBuffer, totalSize);

	return result;
}

u8 UsbPkt_TransmitAck(u8 nextData, u8 state)
{
	return UsbPkt_PacketSender(nextData, &state, 1);
}

u8 UsbPkt_PacketReceiver()
{
	u8	result = USB_PACKET_RCV_ERROR, rtn;
	u8	*pData, length, nextData;

	rtn = UsbPkt_PacketChecker(usbRxBuffer, usbRxSize);

	if(rtn)
	{
		length		= usbRxBuffer[1];
		nextData	= usbRxBuffer[2];
		pData		= (u8*)&usbRxBuffer[3];

		if(usbPktMode == USB_PACKET_MODE_COMMAND)
		{
			memcpy(&pUsbCmdBuffer[usbCmdBufferCnt], pData, length);

			usbCmdBufferCnt += length;
		}
		else if(usbPktMode == USB_PACKET_MODE_DATA)
		{
			memcpy(&pUsbDataBuffer[usbDataBufferCnt], pData, length);

			usbDataBufferCnt += length;
		}

		USB_RxRecover();

		if(nextData)
		{
			UsbPkt_TransmitAck(nextData, 1);
			
			result = USB_PACKET_RCV_ING;
		}
		else
		{
			result = USB_PACKET_RCV_COMPLETE;
		}
	}
	else
	{
		USB_RxRecover();
		UsbPkt_TransmitAck(0, 0);
	}

	return result;
}

u8 UsbPkt_PacketProcess()
{
	u8	result = 0, rtn;

	if(usbRxFlag == 0)		return result;

	rtn = UsbPkt_PacketReceiver();
	if(rtn == USB_PACKET_RCV_COMPLETE)
	{
		result = CMD_PutReceiveData(CMD_COMM_SRC_TYPE_USB, pUsbCmdBuffer, usbCmdBufferCnt);

		usbCmdBufferCnt = 0;
	}

	return result;
}

u8 UsbPkt_FileDownload(u8 *pData,u32 *pRcvSize)
{
	u8	result = 0, rtn;
	u32	sTime, rTime;

	pUsbDataBuffer		= pData;
	usbDataBufferCnt	= 0;

	usbPktMode = USB_PACKET_MODE_DATA;

	UsbPkt_PacketSender(0, "Ready\r\n", 7);

	sTime = HAL_GetTick();

	do{
		if(usbRxFlag)
		{
			rtn = UsbPkt_PacketReceiver();

			if(rtn == USB_PACKET_RCV_ING)
			{
				sTime = HAL_GetTick();
			}
			else if(rtn == USB_PACKET_RCV_COMPLETE)
			{
				result = 1;
				
				break;
			}
		}

		rTime = HAL_GetTick();

		if((rTime - sTime) > USB_PACKET_DATA_DOWNLOAD_TIMEOUT)
		{
			break;
		}
	}while(1);

	usbPktMode = USB_PACKET_MODE_COMMAND;

	*pRcvSize = usbDataBufferCnt;

	return result;
}

u8 UsbPkt_ReceiveAck()
{
	u8	result = 0, rtn;
	u8	length;
	u32	sTime, rTime;

	sTime = HAL_GetTick();

	do{
		if(usbRxFlag)
		{
			rtn = UsbPkt_PacketChecker(usbRxBuffer, usbRxSize);

			if(rtn)
			{
				length		= usbRxBuffer[1];

				USB_RxRecover();

				if(length == 1)
				{
					result = 1;
				}
				else
				{
					UsbPkt_TransmitAck(0, 0);
				}
			}
			else
			{
				USB_RxRecover();
				UsbPkt_TransmitAck(0, 0);
			}

			break;
		}

		rTime = HAL_GetTick();

		if((rTime - sTime) > USB_PACKET_RX_ACK_TIMEOUT)
		{
			break;
		}
	}while(1);

	return result;
}

u8 UsbPkt_TransmitPacket(u8 *pData, u32 length)
{
	u8	result = 0, rtn;
	u8	*pPacket;
	u32	size;

	size = length;

	pPacket = pData;

	do{
		if(size > 255)
		{
			UsbPkt_PacketSender(1, pPacket, 255);

			rtn = UsbPkt_ReceiveAck();

			if(rtn == 0)	break;

			pPacket = &pPacket[255];

			size -= 255;
		}
		else
		{
			UsbPkt_PacketSender(0, pPacket, (u8)size);

			size -= size;

			result = 1;
		}
	}while(size > 0);

	return result;
}

u8 UsbPkt_Init()
{
	u8	result = 0;

	usbPktMode		= USB_PACKET_MODE_COMMAND;
	pUsbCmdBuffer	= (u8*)MEMORY_MAP_SDRAM_USB_CMD_BUFFER_ADRS;
	usbCmdBufferCnt	= 0;

	result = 1;

	return result;
}

