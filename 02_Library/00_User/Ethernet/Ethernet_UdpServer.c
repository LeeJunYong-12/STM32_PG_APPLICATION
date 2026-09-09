#include	"Ethernet_UdpServer.h"
#include	"lwip/pbuf.h"
#include	"lwip/udp.h"
#include	"lwip/tcp.h"

static ip_addr_t	udpRcvAdrs;
static u16			udpRcvPort;
static u16			ethUdpPort;
static u8			udpConnect;
static struct udp_pcb	*pUdpPcb;
static udpServerFops_t	udpServerFops;

void UDP_ServerPortInit(u16 port)
{
	ethUdpPort = port;
}

void UDP_ServerRegisterInterface(void *pVoid)
{
	memcpy(&udpServerFops, pVoid, sizeof(udpServerFops));
}

static void UDP_ServerReceiveCallback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	memcpy(&udpRcvAdrs, addr, sizeof(udpRcvAdrs));
	udpRcvPort	= port;

	if(udpServerFops.RxData != NULL)
	{
		udpServerFops.RxData(p->payload, p->len);
	}

	pbuf_free(p);
}

void UDP_ServerConnectIP(ip_addr_t adrs, u16 port)
{
	if(udpConnect == 1)			return;

	udp_connect(pUdpPcb, &adrs, port);

	udpConnect = 1;
}

void UDP_ServerConnect()
{
	UDP_ServerConnectIP(udpRcvAdrs, udpRcvPort);
}

void UDP_ServerDisconnect()
{
	udp_disconnect(pUdpPcb);

	udpConnect = 0;
}

void UDP_TransmitData(u8 *pData, u16 size)
{
	struct pbuf *pBuff;

	if(udpConnect == 0)
	{
		UDP_ServerConnect();
	}

	pBuff = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_POOL);

	if(pBuff != NULL)
	{
		pbuf_take(pBuff, pData, size);

		udp_send(pUdpPcb, pBuff);
	}

	pbuf_free(pBuff);
}

void UDP_ServerInit()
{
	err_t	err;

	pUdpPcb = udp_new();

	udpConnect = 0;

	if(pUdpPcb != NULL)
	{
		err = udp_bind(pUdpPcb, IP_ADDR_ANY, ethUdpPort);

		if(err == ERR_OK)
		{
			udp_recv(pUdpPcb, UDP_ServerReceiveCallback, NULL);
		}
		else
		{
			udp_remove(pUdpPcb);
		}
	}
}
