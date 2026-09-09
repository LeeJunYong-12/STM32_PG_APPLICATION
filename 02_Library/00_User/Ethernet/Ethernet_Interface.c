#include	"lwip/timeouts.h"
#include	"netif/etharp.h"
#include	"Ethernet_Interface.h"
#include	<string.h>
#include	"BSP.h"

#define IFNAME0 's'
#define IFNAME1 't'

#ifdef	__ICCARM__
	#pragma location=0x2001C000
	ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB];/* Ethernet Rx DMA Descriptors */

	#pragma location=0x2001C080
	ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB];/* Ethernet Tx DMA Descriptors */

	#pragma location=0x2001C100
	uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE]; /* Ethernet Receive Buffers */

	#pragma location=0x2001D8D0
	uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE]; /* Ethernet Transmit Buffers */
#elif
	#error	Please, Check Your Development Environment. Use Only IAR Compiler.
#endif

static	ETH_HandleTypeDef	*phEthernet;
static	bspEthFops_t		ethernetFops;

static	u8	ethernetIoFlag;

u8 Ethernet_GetIoFlag()
{
	return ethernetIoFlag;
}

void Ethernet_ClearIoFlag()
{
	ethernetIoFlag = 0;
}

static void Ethernet_IoInterruptCallback()
{
	ethernetIoFlag = 1;
}

static void Ethernet_MspInit()
{
	GPIO_InitTypeDef	gpio;
	bspGpioExtiFops_t	fops;

//	PA0		: MII_CRS
//	PA1		: MII_RX_CLK
//	PA2		: MII_MDIO
//	PA3		: MII_COL
//	PA7		: MII_RX_DV
//	PB10	: MII_RX_ER
//	PC1		: MII_MDC
//	PC2		: MII_TXD2
//	PC3		: MII_TX_CLK
//	PC4		: MII_RXD0
//	PC5		: MII_RXD1
//	PE2		: MII_TXD3
//	PG11	: MII_TX_EN
//	PG13	: MII_TXD0
//	PG14	: MII_TXD1
//	PH6		: MII_RXD2
//	PH7		: MII_RXD3

//	PB1		: MII_INT

	__HAL_RCC_ETH_CLK_ENABLE();

	memset(&gpio, NULL, sizeof(gpio));

	gpio.Pin		= GPIO_PIN_1;
	gpio.Mode		= GPIO_MODE_IT_FALLING;
	gpio.Pull		= GPIO_PULLUP;
	gpio.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOB, &gpio);

	fops.ExtiFunc = Ethernet_IoInterruptCallback;

	BSP_GPIO_RegisterInterface(GPIO_PIN_1, &fops);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	memset(&gpio, NULL, sizeof(gpio));

	gpio.Mode		= GPIO_MODE_AF_PP;
	gpio.Pull		= GPIO_NOPULL;
	gpio.Speed		= GPIO_SPEED_HIGH;
	gpio.Alternate	= GPIO_AF11_ETH;

//	GPIOA
	gpio.Pin		= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &gpio);

//	GPIOB
	gpio.Pin		= GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &gpio);

//	GPIOC
	gpio.Pin		= GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	HAL_GPIO_Init(GPIOC, &gpio);

//	GPIOE
	gpio.Pin		= GPIO_PIN_2;
	HAL_GPIO_Init(GPIOE, &gpio);

//	GPIOG
	gpio.Pin		= GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
	HAL_GPIO_Init(GPIOG, &gpio);

//	GPIOH
	gpio.Pin		= GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOH, &gpio);

	HAL_NVIC_SetPriority(ETH_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(ETH_IRQn);
}

static void Ethernet_MspDeinit()
{
}

static void Ethernet_TxCpltCallback()
{
}

static void Ethernet_RxCpltCallback()
{
}

static void Ethernet_ErrorCallback()
{
}

static void low_level_init(struct netif *netif)
{
	HAL_StatusTypeDef	rtn;
	u32	regValue = 0;

	phEthernet = (ETH_HandleTypeDef*)BSP_Ethernet_GetHandle();

	ethernetFops.MspInit		= Ethernet_MspInit;
	ethernetFops.MspDeInit		= Ethernet_MspDeinit;
	ethernetFops.TxCpltCallback	= Ethernet_TxCpltCallback;
	ethernetFops.RxCpltCallback	= Ethernet_RxCpltCallback;
	ethernetFops.ErrorCallback	= Ethernet_ErrorCallback;

	BSP_Ethernet_RegisterInterface(&ethernetFops);

	phEthernet->Init.AutoNegotiation	= ETH_AUTONEGOTIATION_ENABLE;
	phEthernet->Init.Speed				= ETH_SPEED_100M;
	phEthernet->Init.DuplexMode			= ETH_MODE_FULLDUPLEX;
	phEthernet->Init.PhyAddress			= DP83848_PHY_ADDRESS;
	phEthernet->Init.MACAddr			= (uint8_t*)ethMac;
	phEthernet->Init.RxMode				= ETH_RXPOLLING_MODE;
	phEthernet->Init.ChecksumMode		= ETH_CHECKSUM_BY_HARDWARE;
	phEthernet->Init.MediaInterface		= ETH_MEDIA_INTERFACE_MII;

	rtn = HAL_ETH_Init(phEthernet);

	if(rtn == HAL_OK)
	{
		netif->flags	|= NETIF_FLAG_LINK_UP;
	}

	HAL_ETH_DMATxDescListInit(phEthernet, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

	HAL_ETH_DMARxDescListInit(phEthernet, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

	netif->hwaddr_len = ETH_HWADDR_LEN;

	netif->hwaddr[0] =  ethMac[0];
	netif->hwaddr[1] =  ethMac[1];
	netif->hwaddr[2] =  ethMac[2];
	netif->hwaddr[3] =  ethMac[3];
	netif->hwaddr[4] =  ethMac[4];
	netif->hwaddr[5] =  ethMac[5];

	netif->mtu = 1500;

	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

	HAL_ETH_Start(phEthernet);

	HAL_ETH_ReadPHYRegister(phEthernet, PHY_MICR, (uint32_t*)&regValue);

	regValue |= (PHY_MICR_INT_EN | PHY_MICR_INT_OE);

	HAL_ETH_WritePHYRegister(phEthernet, PHY_MICR, regValue );

	HAL_ETH_ReadPHYRegister(phEthernet, PHY_MISR, (uint32_t*)&regValue);

	regValue |= PHY_MISR_LINK_INT_EN;

	HAL_ETH_WritePHYRegister(phEthernet, PHY_MISR, regValue);
}

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	err_t	errValue;
	struct	pbuf	*q;
	volatile ETH_DMADescTypeDef	*pDmaTxDesc;
	u8	*pBuffer = (u8*)(phEthernet->TxDesc->Buffer1Addr);
	u32	frameLength = 0, bufferOffset = 0, bytesLeftToCopy = 0, payloadOffset = 0;
	
	pDmaTxDesc = phEthernet->TxDesc;
	bufferOffset = 0;

	for(q = p; q != NULL; q = q->next)
	{
		if((pDmaTxDesc->Status & ETH_DMATXDESC_OWN) != 0)
		{
			errValue = ERR_USE;

			goto LOW_LEVEL_OUTPUT_ERROR;
		}

		bytesLeftToCopy = q->len;
		payloadOffset	= 0;

		while((bytesLeftToCopy + bufferOffset) > ETH_TX_BUF_SIZE)
		{
			memcpy((u8*)((u8*)pBuffer + bufferOffset), (u8*)((u8*)q->payload + payloadOffset), (ETH_TX_BUF_SIZE - bufferOffset));

			pDmaTxDesc = (ETH_DMADescTypeDef*)(pDmaTxDesc->Buffer2NextDescAddr);

			if((pDmaTxDesc->Status & ETH_DMATXDESC_OWN) != 0)
			{
				errValue = ERR_USE;

				goto LOW_LEVEL_OUTPUT_ERROR;
			}

			pBuffer = (u8*)(pDmaTxDesc->Buffer1Addr);

			bytesLeftToCopy		= bytesLeftToCopy - (ETH_TX_BUF_SIZE - bufferOffset);
			payloadOffset		= payloadOffset + (ETH_TX_BUF_SIZE - bufferOffset);
			frameLength			= frameLength + (ETH_TX_BUF_SIZE - bufferOffset);
			bufferOffset		= 0;
		}

		memcpy((u8*)((u8*)pBuffer + bufferOffset), (u8*)((u8*)q->payload + payloadOffset), bytesLeftToCopy);
		bufferOffset	= bufferOffset + bytesLeftToCopy;
		frameLength		= frameLength + bytesLeftToCopy;
	}

	HAL_ETH_TransmitFrame(phEthernet, frameLength);

	errValue = ERR_OK;

LOW_LEVEL_OUTPUT_ERROR:

	if((phEthernet->Instance->DMASR & ETH_DMASR_TUS) != 0)
	{
		phEthernet->Instance->DMASR = ETH_DMASR_TUS;
	
		phEthernet->Instance->DMATPDR = 0;
	}
	
	return errValue;
}

static struct pbuf * low_level_input(struct netif *netif)
{
	u8	*pBuffer;
	u16	length;
	u32	bufferOffset = 0, payloadOffset = 0, bytesLeftToCopy = 0, cnt = 0;
	volatile ETH_DMADescTypeDef	*pDmaRxDesc;
	struct pbuf *p = NULL, *q = NULL;

	if(HAL_ETH_GetReceivedFrame(phEthernet) != HAL_OK)
	{
		return NULL;
	}

	length	= phEthernet->RxFrameInfos.length;
	pBuffer	= (u8*)phEthernet->RxFrameInfos.buffer;

	if(length > 0)
	{
		p = pbuf_alloc(PBUF_RAW, length, PBUF_POOL);
	}

	if(p != NULL)
	{
		pDmaRxDesc		= phEthernet->RxFrameInfos.FSRxDesc;
		bufferOffset	= 0;

		for(q = p; q != NULL; q = q->next)
		{
			bytesLeftToCopy	= q->len;
			payloadOffset	= 0;

			while((bytesLeftToCopy + bufferOffset) > ETH_RX_BUF_SIZE)
			{
				memcpy((u8*)((u8*)q->payload + payloadOffset), (u8*)((u8*)pBuffer + bufferOffset), (ETH_RX_BUF_SIZE - bufferOffset));

				pDmaRxDesc	= (ETH_DMADescTypeDef*)(pDmaRxDesc->Buffer2NextDescAddr);
				pBuffer		= (u8*)(pDmaRxDesc->Buffer1Addr);

				bytesLeftToCopy	= bytesLeftToCopy - (ETH_RX_BUF_SIZE - bufferOffset);
				payloadOffset	= payloadOffset + (ETH_RX_BUF_SIZE - bufferOffset);
				bufferOffset	= 0;
			}

			memcpy((u8*)((u8*)q->payload + payloadOffset), (u8*)((u8*)pBuffer + bufferOffset), bytesLeftToCopy);
			bufferOffset = bufferOffset + bytesLeftToCopy;
		}
	}

	pDmaRxDesc	= phEthernet->RxFrameInfos.FSRxDesc;

	for(cnt = 0; cnt < phEthernet->RxFrameInfos.SegCount; cnt++)
	{
		pDmaRxDesc->Status	|= ETH_DMARXDESC_OWN;
		pDmaRxDesc = (ETH_DMADescTypeDef*)(pDmaRxDesc->Buffer2NextDescAddr);
	}

	phEthernet->RxFrameInfos.SegCount = 0;

	if((phEthernet->Instance->DMASR & ETH_DMASR_RBUS) != 0)
	{
		phEthernet->Instance->DMASR		= ETH_DMASR_RBUS;

		phEthernet->Instance->DMARPDR	= 0;
	}

	return p;
}

void ethernetif_input(struct netif *netif)
{
	err_t	err;
	struct	pbuf	*p;

	p = low_level_input(netif);

	if(p == NULL)	return;

	err = netif->input(p, netif);

	if(err != ERR_OK)
	{
		pbuf_free(p);
		p = NULL;
	}
}

err_t ethernetif_init(struct netif *netif)
{
#if	LWIP_NETIF_HOSTNAME
	netif->hostname	= "lwip";
#endif	//	LWIP_NETIF_HOSTNAME

	netif->name[0]		= IFNAME0;
	netif->name[0]		= IFNAME0;

	netif->output		= etharp_output;
	netif->linkoutput	= low_level_output;

	low_level_init(netif);

	return ERR_OK;
}

u32_t sys_now(void)
{
	return HAL_GetTick();
}

void ethernetif_set_link(struct netif *netif)
{
	u32	regValue = 0;

	HAL_ETH_ReadPHYRegister(phEthernet, PHY_MISR, (uint32_t*)&regValue);

	if((regValue & PHY_LINK_INTERRUPT) != RESET)
	{
		HAL_ETH_ReadPHYRegister(phEthernet, PHY_SR, (uint32_t*)&regValue);
		
		if((regValue & PHY_LINK_STATUS) != RESET)
		{
			netif_set_link_up(netif);
		}
		else//disconnect
		{
			netif_set_link_down(netif);
			
#if 0 // ethernet access error reset			
			HAL_NVIC_DisableIRQ(EXTI1_IRQn);
			HAL_Delay(100);
			NVIC_SystemReset();// yeom_20201119
#endif // lyh_210802
		}
	}
}

void ethernetif_update_config(struct netif *netif)
{
	volatile u32	tickStart = 0;
	u32	regValue = 0;

	if(netif_is_link_up(netif))
	{
		if(phEthernet->Init.AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE)
		{
			HAL_ETH_WritePHYRegister(phEthernet, PHY_BCR, PHY_AUTONEGOTIATION);

			tickStart = HAL_GetTick();

			do{
				HAL_ETH_ReadPHYRegister(phEthernet, PHY_BSR, (uint32_t*)&regValue);

				if((HAL_GetTick() - tickStart) > 1000)
				{
					goto ETHERNETIF_UPDATE_CONFIG_ERROR;
				}
			}while((regValue & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE);

			HAL_ETH_ReadPHYRegister(phEthernet, PHY_SR, (uint32_t*)&regValue);

			if((regValue & PHY_DUPLEX_STATUS) != 0)
			{
				phEthernet->Init.DuplexMode = ETH_MODE_FULLDUPLEX;
			}
			else
			{
				phEthernet->Init.DuplexMode = ETH_MODE_HALFDUPLEX;
			}

			if(regValue & PHY_SPEED_STATUS)
			{
				phEthernet->Init.Speed = ETH_SPEED_10M;
			}
			else
			{
				phEthernet->Init.Speed = ETH_SPEED_100M;
			}
		}
		else
		{
ETHERNETIF_UPDATE_CONFIG_ERROR:

			regValue = (u16)(phEthernet->Init.DuplexMode >> 3);
			regValue |= (u16)(phEthernet->Init.Speed >> 1);

			HAL_ETH_WritePHYRegister(phEthernet, PHY_BCR, regValue);
		}

		HAL_ETH_ConfigMAC(phEthernet, NULL);

		HAL_ETH_Start(phEthernet);
	}
	else
	{
		HAL_ETH_Stop(phEthernet);
	}

	ethernetif_notify_conn_changed(netif);
}

__weak void ethernetif_notify_conn_changed(struct netif *netif)
{
}

