#ifndef _ETHERNET_APPLICATION_H
#define _ETHERNET_APPLICATION_H

#ifdef __cplusplus
 extern "C" {
#endif

#include	"lwip/opt.h"
#include	"lwip/init.h"
#include	"lwip/netif.h"
#include	"lwip/timeouts.h"
#include	"netif/etharp.h"
#include	"Ethernet_Interface.h"

#define DHCP_OFF                   (uint8_t) 0
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5

extern	struct netif gnetif;

void User_notification(struct netif *netif);
void DHCP_Process(struct netif *netif);
void DHCP_Periodic_Handle(struct netif *netif);
void Netif_Config();

#ifdef __cplusplus
}
#endif

#endif	//	_ETHERNET_APPLICATION_H