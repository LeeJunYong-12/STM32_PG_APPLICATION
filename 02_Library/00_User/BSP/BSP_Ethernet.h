#ifndef	_BSP_ETHERNET_H
#define	_BSP_ETHERNET_H

#include	"BSP.h"

typedef	struct{
	void	(* MspInit)					(void);
	void	(* MspDeInit)				(void);
	void	(* TxCpltCallback)			(void);
	void	(* RxCpltCallback)			(void);
	void	(* ErrorCallback)			(void);
}bspEthFops_t;

extern	ETH_HandleTypeDef	hEthernet;
extern	u8	ethMac[6], ethIP[4], ethNM[4], ethGW[4];

u32 BSP_Ethernet_GetHandle();
u8 BSP_Ethernet_RegisterInterface(void *pVoid);
u8 BSP_Ethernet_RegisterMacAddress(u8 *pBuffer);
u8 BSP_Ethernet_RegisterNetworkAdrdress(u8 *pIP, u8 *pNM, u8 *pGW);

#endif	//	_BSP_ETHERNET_H
