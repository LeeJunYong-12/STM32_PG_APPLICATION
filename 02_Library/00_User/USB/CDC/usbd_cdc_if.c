#include	"usbd_cdc_if.h"

static	USBD_HandleTypeDef	*pUsbCdcHandle = NULL;
static	u8	usbCdcRxBuffer[512];
static	usbCdcCallback_t	*pUsbCdcDataHandle = NULL;

static	USBD_CDC_LineCodingTypeDef	usbCdcLineCoding = {115200, 0, 0 ,8};

static int8_t USB_CDC_IF_Init();
static int8_t USB_CDC_IF_Deinit();
static int8_t USB_CDC_IF_Control(uint8_t cmd, uint8_t *pBuff, uint16_t length);
static int8_t USB_CDC_IF_Receive(uint8_t *pBuff, uint32_t *length);

USBD_CDC_ItfTypeDef	usbdCdcFops = {USB_CDC_IF_Init, USB_CDC_IF_Deinit, USB_CDC_IF_Control, USB_CDC_IF_Receive};

void USB_CDC_DataRegister(void *pHandle, void *pFops)
{
	pUsbCdcHandle		= (USBD_HandleTypeDef*)pHandle;
	pUsbCdcDataHandle	= (usbCdcCallback_t*)pFops;
}

static int8_t USB_CDC_IF_Init()
{
	if(pUsbCdcHandle != NULL)
	{
		USBD_CDC_SetRxBuffer(pUsbCdcHandle, usbCdcRxBuffer);
		USBD_CDC_SetTxBuffer(pUsbCdcHandle, usbCdcRxBuffer, 0);
	}

	return USBD_OK;
}

static int8_t USB_CDC_IF_Deinit()
{
	return USBD_OK;
}

static int8_t USB_CDC_IF_Control(uint8_t cmd, uint8_t *pBuff, uint16_t length)
{
	switch(cmd)
	{
		case CDC_SEND_ENCAPSULATED_COMMAND:
			break;

		case CDC_GET_ENCAPSULATED_RESPONSE:
			break;

		case CDC_SET_COMM_FEATURE:
			break;

		case CDC_GET_COMM_FEATURE:
			break;

		case CDC_CLEAR_COMM_FEATURE:
			break;

		case CDC_SET_LINE_CODING:
			usbCdcLineCoding.bitrate		|= pBuff[3];
			usbCdcLineCoding.bitrate		<<= 8;
			usbCdcLineCoding.bitrate		|= pBuff[2];
			usbCdcLineCoding.bitrate		<<= 8;
			usbCdcLineCoding.bitrate		|= pBuff[1];
			usbCdcLineCoding.bitrate		<<= 8;
			usbCdcLineCoding.bitrate		|= pBuff[0];

			usbCdcLineCoding.format			= pBuff[4];
			usbCdcLineCoding.paritytype		= pBuff[5];
			usbCdcLineCoding.datatype		= pBuff[6];
			break;

		case CDC_GET_LINE_CODING:
			pBuff[0]	= (uint8_t)(usbCdcLineCoding.bitrate >> 0);
			pBuff[1]	= (uint8_t)(usbCdcLineCoding.bitrate >> 8);
			pBuff[2]	= (uint8_t)(usbCdcLineCoding.bitrate >> 16);
			pBuff[3]	= (uint8_t)(usbCdcLineCoding.bitrate >> 24);

			pBuff[4]	= usbCdcLineCoding.format;
			pBuff[5]	= usbCdcLineCoding.paritytype;
			pBuff[6]	= usbCdcLineCoding.datatype;
			break;

		case CDC_SET_CONTROL_LINE_STATE:
			break;

		case CDC_SEND_BREAK:
			break;
	}

	return USBD_OK;
}

static int8_t USB_CDC_IF_Receive(uint8_t *pBuff, uint32_t *length)
{
	u32	size;

	size = *length;

	if(pUsbCdcDataHandle != NULL)
	{
		pUsbCdcDataHandle->RxCpltCallback(pBuff, size);
	}

	return USBD_OK;
}

