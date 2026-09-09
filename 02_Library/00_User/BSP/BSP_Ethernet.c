#include	"BSP_Ethernet.h"

static	bspEthFops_t	bspEthFops = NULL;

ETH_HandleTypeDef	hEthernet;

u8	ethMac[6], ethIP[4], ethNM[4], ethGW[4];

u8 BSP_Ethernet_DataInit()
{
	u8	result = 0;

	memset(&bspEthFops, NULL, sizeof(bspEthFops));
	memset(&hEthernet, NULL, sizeof(hEthernet));

	memset(ethMac, NULL, sizeof(ethMac));
	memset(ethIP, NULL, sizeof(ethIP));
	memset(ethNM, NULL, sizeof(ethNM));
	memset(ethGW, NULL, sizeof(ethGW));

	result = 1;

	return result;
}

u32 BSP_Ethernet_GetHandle()
{
	u32	result;

	result = (u32)&hEthernet;
	hEthernet.Instance	= ETH;

	return result;
}

u8 BSP_Ethernet_RegisterInterface(void *pVoid)
{
	u8	result = 1;

	memcpy(&bspEthFops, pVoid, sizeof(bspEthFops_t));

	return result;
}

u8 BSP_Ethernet_RegisterMacAddress(u8 *pBuffer)
{
	u8	result = 1;

	memcpy(ethMac, pBuffer, sizeof(ethMac));

	return result;
}

u8 BSP_Ethernet_RegisterNetworkAdrdress(u8 *pIP, u8 *pNM, u8 *pGW)
{
	u8	result = 1;

	memcpy(ethIP, pIP, sizeof(ethIP));
	memcpy(ethNM, pNM, sizeof(ethNM));
	memcpy(ethGW, pGW, sizeof(ethGW));

	return result;
}

void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
	if(heth->Instance != ETH)		return;

	if(bspEthFops.MspInit != NULL)
	{
		bspEthFops.MspInit();
	}
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
	if(heth->Instance != ETH)		return;

	if(bspEthFops.MspDeInit != NULL)
	{
		bspEthFops.MspDeInit();
	}
}

void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
	if(heth->Instance != ETH)		return;

	if(bspEthFops.TxCpltCallback != NULL)
	{
		bspEthFops.TxCpltCallback();
	}
}

void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{	
	if(heth->Instance != ETH)		return;

	if(bspEthFops.RxCpltCallback != NULL)
	{
		bspEthFops.RxCpltCallback();
	}
}

void HAL_ETH_ErrorCallback(ETH_HandleTypeDef *heth)
{	
	if(heth->Instance != ETH)		return;

	if(bspEthFops.ErrorCallback != NULL)
	{
		bspEthFops.ErrorCallback();
	}
}

