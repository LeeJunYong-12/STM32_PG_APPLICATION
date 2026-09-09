#include	"App_USB.h"

static	USBD_HandleTypeDef		hUSBD_FS;
static	USBD_CDC_HandleTypeDef	*phUSBD_CDC;
static	usbCdcCallback_t		hUsbDataHandle;

u8	usbRxBuffer[512], usbRxFlag;
u32	usbRxSize;

static void USB_RxCpltCallback(u8 *pData, u32 size)
{
	if(size > 512)	usbRxSize = 512;
	else			usbRxSize = size;

	memcpy(usbRxBuffer, pData, usbRxSize);

	usbRxFlag = 1;
}

void USB_RxRecover()
{
	usbRxSize = 0;
	usbRxFlag = 0;

	USBD_CDC_ReceivePacket(&hUSBD_FS);
}

u8 USB_Init()
{
	u8	result = 0;

	hUsbDataHandle.RxCpltCallback		= USB_RxCpltCallback;

	USB_CDC_DataRegister(&hUSBD_FS, &hUsbDataHandle);

	USBD_Init(&hUSBD_FS, &VCP_Desc, 0);

	USBD_RegisterClass(&hUSBD_FS, USBD_CDC_CLASS);

	USBD_CDC_RegisterInterface(&hUSBD_FS, &usbdCdcFops);

	USBD_Start(&hUSBD_FS);

	USB_RxRecover();

	phUSBD_CDC = (USBD_CDC_HandleTypeDef*)hUSBD_FS.pClassData;

	result = 1;

	return result;
}

u8 USB_TxData(u8 *pData, u16 size)
{
	u8	result = 0, rtn;
	u32	txRtn, sTime, rTime;

	rtn = USBD_CDC_SetTxBuffer(&hUSBD_FS, pData, size);
	if(rtn != USBD_OK)	return result;

	rtn = USBD_CDC_TransmitPacket(&hUSBD_FS);
	if(rtn != USBD_OK)	return result;

	sTime = HAL_GetTick();
	do{
		txRtn = phUSBD_CDC->TxState;

		rTime = HAL_GetTick();

		if((rTime - sTime) > 5)	break;
	}while(txRtn);

	result = 1;

	return result;
}

