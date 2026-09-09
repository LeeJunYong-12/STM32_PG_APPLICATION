#include	"usbd_core.h"

#include	"BSP.h"

static	PCD_HandleTypeDef	*phUsbFS;

static void UsbFS_MspInit()
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_USB_OTG_FS_CLK_ENABLE();

	gpio.Pin		= GPIO_PIN_11 | GPIO_PIN_12;
	gpio.Mode		= GPIO_MODE_AF_PP;
	gpio.Pull		= GPIO_NOPULL;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Alternate	= GPIO_AF10_OTG_FS;

	HAL_GPIO_Init(GPIOA, &gpio);

	HAL_NVIC_SetPriority(OTG_FS_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

static void UsbFS_MspDeInit()
{
	HAL_NVIC_DisableIRQ(OTG_FS_IRQn);

	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

	__HAL_RCC_USB_OTG_FS_CLK_DISABLE();
}

static void UsbFS_DataOutStageCallback(u8 epnum)
{
	USBD_LL_DataOutStage(phUsbFS->pData, epnum, phUsbFS->OUT_ep[epnum].xfer_buff);
}

static void UsbFS_DataInStageCallback(u8 epnum)
{
	USBD_LL_DataInStage(phUsbFS->pData, epnum, phUsbFS->IN_ep[epnum].xfer_buff);
}

static void UsbFS_SetupStageCallback()
{
	USBD_LL_SetupStage(phUsbFS->pData, (uint8_t*)phUsbFS->Setup);
}

static void UsbFS_SOFCallback()
{
	USBD_LL_SOF(phUsbFS->pData);
}

static void UsbFS_ResetCallback()
{
	USBD_SpeedTypeDef	speed	= USBD_SPEED_FULL;

	switch(phUsbFS->Init.speed)
	{
		case PCD_SPEED_HIGH:
			speed = USBD_SPEED_HIGH;
			break;

		case PCD_SPEED_FULL:
			speed = USBD_SPEED_FULL;
			break;

		default:
			speed = USBD_SPEED_FULL;
			break;
	}

	USBD_LL_Reset(phUsbFS->pData);

	USBD_LL_SetSpeed(phUsbFS->pData, speed);
}

static void UsbFS_SuspendCallback()
{
	USBD_LL_Suspend(phUsbFS->pData);
}

static void UsbFS_ResumeCallback()
{
	USBD_LL_Resume(phUsbFS->pData);
}

static void UsbFS_ISOOUTIncompleteCallback(u8 epnum)
{
	USBD_LL_IsoOUTIncomplete(phUsbFS->pData, epnum);
}

static void UsbFS_ISOINIncompleteCallback(u8 epnum)
{
	USBD_LL_IsoINIncomplete(phUsbFS->pData, epnum);
}

static void UsbFS_ConnectCallback()
{
	USBD_LL_DevConnected(phUsbFS->pData);
}

static void UsbFS_DisconnectCallback()
{
	USBD_LL_DevDisconnected(phUsbFS->pData);
}

USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev)
{
	u8	rtn;
	bspUsbFops_t	fops;

	phUsbFS = (PCD_HandleTypeDef*)BSP_USB_GetHandle(BSP_USB_FS);

	fops.MspInit					= UsbFS_MspInit;
	fops.MspDeInit					= UsbFS_MspDeInit;
	fops.DataOutStageCallback		= UsbFS_DataOutStageCallback;
	fops.DataInStageCallback		= UsbFS_DataInStageCallback;
	fops.SetupStageCallback			= UsbFS_SetupStageCallback;
	fops.SOFCallback				= UsbFS_SOFCallback;
	fops.ResetCallback				= UsbFS_ResetCallback;
	fops.SuspendCallback			= UsbFS_SuspendCallback;
	fops.ResumeCallback				= UsbFS_ResumeCallback;
	fops.ISOOUTIncompleteCallback	= UsbFS_ISOOUTIncompleteCallback;
	fops.ISOINIncompleteCallback	= UsbFS_ISOINIncompleteCallback;
	fops.ConnectCallback			= UsbFS_ConnectCallback;
	fops.DisconnectCallback			= UsbFS_DisconnectCallback;

	rtn = BSP_USB_RegisterInterface(phUsbFS, &fops);
	if(rtn == 0)	return USBD_FAIL;

	phUsbFS->Init.dev_endpoints = 4;
	phUsbFS->Init.use_dedicated_ep1 = 0;
	phUsbFS->Init.ep0_mps = 0x40;
	phUsbFS->Init.dma_enable = 0;
	phUsbFS->Init.low_power_enable = 0;
	phUsbFS->Init.phy_itface = PCD_PHY_EMBEDDED;
	phUsbFS->Init.Sof_enable = 0;
	phUsbFS->Init.speed = PCD_SPEED_FULL;
	phUsbFS->Init.vbus_sensing_enable = 0;
	phUsbFS->Init.lpm_enable = 0;

	phUsbFS->pData = pdev;
	pdev->pData = phUsbFS;

	HAL_PCD_Init(phUsbFS);

	HAL_PCDEx_SetRxFiFo(phUsbFS, 0x80);
	HAL_PCDEx_SetTxFiFo(phUsbFS, 0, 0x40);
	HAL_PCDEx_SetTxFiFo(phUsbFS, 1, 0x80);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
{
	HAL_PCD_DeInit(pdev->pData);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
	HAL_PCD_Start(pdev->pData);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
	HAL_PCD_Stop(pdev->pData);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_mps)
{
	HAL_PCD_EP_Open(pdev->pData, ep_addr, ep_mps, ep_type);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_Close(pdev->pData, ep_addr);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_Flush(pdev->pData, ep_addr);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_SetStall(pdev->pData, ep_addr);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);

	return USBD_OK;
}

uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	PCD_HandleTypeDef *hpcd = pdev->pData;

	if((ep_addr & 0x80) == 0x80)
	{
		return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
	}
	else
	{
		return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
	}
}

USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)
{
	HAL_PCD_SetAddress(pdev->pData, dev_addr);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint16_t size)
{
	pdev->ep_in[ep_addr & 0x7F].total_length = size;

	HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint16_t size)
{
	HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);
	return USBD_OK;
}

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	return HAL_PCD_EP_GetRxCount(pdev->pData, ep_addr);
}

void USBD_LL_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}

