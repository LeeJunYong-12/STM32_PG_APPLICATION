#include "BSP.h"
#include "lwip/dhcp.h"
#include "Ethernet_Application.h"

#define	ETHERNET_DHCP_RETRY_COUNT		4
u8	etherDhcpState	= DHCP_OFF;
u32	etherDhcpTimer	= 0;

struct netif gnetif;

void User_notification(struct netif *netif) 
{
	if(netif_is_up(netif))
	{
		etherDhcpState = DHCP_START;
	}
	else
	{
		etherDhcpState = DHCP_LINK_DOWN;
	}
}

void ethernetif_notify_conn_changed(struct netif *netif)
{
	ip_addr_t	ip, nm, gw;

	if(netif_is_link_up(netif))
	{
		
		if(0)
		{
			etherDhcpState = DHCP_START;
		}
		else
		{
			IP_ADDR4(&ip, ethIP[0], ethIP[1], ethIP[2], ethIP[3]);
			IP_ADDR4(&nm, ethNM[0], ethNM[1], ethNM[2], ethNM[3]);
			IP_ADDR4(&gw, ethGW[0], ethGW[1], ethGW[2], ethGW[3]);

			netif_set_addr(netif, &ip, &nm, &gw);
		}

		netif_set_up(netif);
	}
	else
	{
		etherDhcpState = DHCP_LINK_DOWN;
		
		netif_set_down(netif);
	}
}
#if	LWIP_DHCP
void DHCP_Process(struct netif *netif)
{
	ip_addr_t	ip, nm, gw;
	struct	dhcp	*dhcp;

	switch(etherDhcpState)
	{
		case DHCP_START:
			ip_addr_set_zero_ip4(&netif->ip_addr);
			ip_addr_set_zero_ip4(&netif->netmask);
			ip_addr_set_zero_ip4(&netif->gw);

			etherDhcpState = DHCP_WAIT_ADDRESS;

			dhcp_start(netif);
			break;

		case DHCP_WAIT_ADDRESS:
			if(dhcp_supplied_address(netif))
			{
				etherDhcpState	= DHCP_ADDRESS_ASSIGNED;
			}
			else
			{
				dhcp = (struct dhcp*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

				if(dhcp->tries	> ETHERNET_DHCP_RETRY_COUNT)
				{
					etherDhcpState	= DHCP_TIMEOUT;

					dhcp_stop(netif);

					IP_ADDR4(&ip, ethIP[0], ethIP[1], ethIP[2], ethIP[3]);
					IP_ADDR4(&nm, ethNM[0], ethNM[1], ethNM[2], ethNM[3]);
					IP_ADDR4(&gw, ethGW[0], ethGW[1], ethGW[2], ethGW[3]);

					netif_set_addr(netif, &ip, &nm, &gw);
				}
			}
			break;

		case DHCP_LINK_DOWN:
			dhcp_stop(netif);

			etherDhcpState	= DHCP_OFF;
			break;

		default:
			break;
	}
}

void DHCP_Periodic_Handle(struct netif *netif)
{
	if((HAL_GetTick() - etherDhcpTimer) >= DHCP_FINE_TIMER_MSECS)
	{
		etherDhcpTimer = HAL_GetTick();

		DHCP_Process(netif);
	}
}
#endif	//	LWIP_DHCP

void Netif_Config()
{
	ip_addr_t	ip, nm, gw;

	if(0)	// USE_DHCP
	{
	}
	else
	{
		IP_ADDR4(&ip, ethIP[0], ethIP[1], ethIP[2], ethIP[3]);
		IP_ADDR4(&nm, ethNM[0], ethNM[1], ethNM[2], ethNM[3]);
		IP_ADDR4(&gw, ethGW[0], ethGW[1], ethGW[2], ethGW[3]);
	}

	netif_add(&gnetif, &ip, &nm, &gw, NULL, ethernetif_init, ethernet_input);

	netif_set_default(&gnetif);

	if(netif_is_link_up(&gnetif))
	{
		netif_set_up(&gnetif);
	}
	else
	{
		netif_set_down(&gnetif);
	}

	netif_set_link_callback(&gnetif, ethernetif_update_config);
}

