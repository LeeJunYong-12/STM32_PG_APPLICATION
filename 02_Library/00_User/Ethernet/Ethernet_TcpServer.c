#include	"Ethernet_TcpServer.h"
#include	"lwip/stats.h"
#include	"lwip/tcp.h"

extern	void LED_Control(u8 pos, u8 data);

enum	tcp_server_states{
	ES_NONE = 0,
	ES_ACCEPTED,
	ES_RECEIVED,
	ES_CLOSING
};

struct tcp_server_struct{
	u8	state;
	u8	retries;
	struct tcp_pcb	*pcb;
	struct pbuf		*p;
};

static struct tcp_pcb	*pEthTcpRxPcb, *pEthTcpTxPcb;
static struct tcp_server_struct *pEthTcpServer;
static u16	ethTcpPort;
static tcpServerFops_t	tcpServerFops;

static err_t TCP_ServerAccept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t TCP_ServerReceive(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void TCP_ServerError(void *arg, err_t err);
static err_t TCP_ServerPoll(void *arg, struct tcp_pcb *tpcb);
static err_t TCP_ServerSent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void TCP_ServerSend(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
static void TCP_ServerConnectionClose(struct tcp_pcb *tpcb, struct tcp_server_struct *es);

void TCP_ServerPortInit(u16	port)
{
	ethTcpPort = port;
}

void TCP_ServerRegisterInterface(void *pVoid)
{
	memcpy(&tcpServerFops, pVoid, sizeof(tcpServerFops));
}

u8 TCP_ServerInit()
{
	u8	result = 0;

	pEthTcpRxPcb = tcp_new();

	pEthTcpTxPcb = pEthTcpRxPcb;

	if(pEthTcpRxPcb != NULL)
	{
		err_t	err;

		err = tcp_bind(pEthTcpRxPcb, IP_ADDR_ANY, ethTcpPort);

		if(err == ERR_OK)
		{
			pEthTcpRxPcb = tcp_listen(pEthTcpRxPcb);

			tcp_accept(pEthTcpRxPcb, TCP_ServerAccept);
			
			result = 1;
		}
		else
		{
			memp_free(MEMP_TCP_PCB, pEthTcpRxPcb);
		}
	}
	
	return result;
}

static err_t TCP_ServerAccept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	err_t	result;
	struct	tcp_server_struct	*es;

	tcp_setprio(newpcb, TCP_PRIO_MIN);

	es = (struct tcp_server_struct*)mem_malloc(sizeof(struct tcp_server_struct));

	if(es != NULL)
	{
		es->state	= ES_ACCEPTED;
		es->pcb		= newpcb;
		es->retries	= 0;
		es->p		= NULL;

		tcp_arg(newpcb, es);
		tcp_recv(newpcb, TCP_ServerReceive);
		tcp_err(newpcb, TCP_ServerError);
		tcp_poll(newpcb, TCP_ServerPoll, 0);
		tcp_sent(newpcb, TCP_ServerSent);

		result = ERR_OK;
	}
	else
	{
		TCP_ServerConnectionClose(newpcb, es);

		result = ERR_MEM;
	}

	pEthTcpServer = es;

	return result;
}

static err_t TCP_ServerReceive(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	err_t	result;
	struct tcp_server_struct	*es;

	es = (struct tcp_server_struct*)arg;

	if(p == NULL)
	{
		es->state = ES_CLOSING;
		if(es->p == NULL)
		{
			TCP_ServerConnectionClose(tpcb, es);
		}
		else
		{
			tcp_sent(tpcb, TCP_ServerSent);

			TCP_ServerSend(tpcb, es);
		}
		result = ERR_OK;
	}
	else if(err != ERR_OK)
	{
		if(p != NULL)
		{
			es->p = NULL;

			pbuf_free(p);
		}

		result = err;
	}
	else if(es->state == ES_ACCEPTED)
	{
		es->state = ES_RECEIVED;

		if(tcpServerFops.RxData != NULL)
		{
			tcpServerFops.RxData((u8*)p->payload, (u16)p->len);
		}

		tcp_recved(tpcb, p->tot_len);
		
		es->p = NULL;

		pbuf_free(p);

		result = ERR_OK;
	}
	else if(es->state == ES_RECEIVED)
	{
		if(es->p == NULL)
		{
			if(tcpServerFops.RxData != NULL)
			{
				tcpServerFops.RxData((u8*)p->payload, (u16)p->len);
			}

			tcp_recved(tpcb, p->tot_len);
			
			es->p = NULL;

			pbuf_free(p);
		}
		else
		{
			struct pbuf *ptr;

			ptr = es->p;
			pbuf_chain(ptr, p);
		}

		result = ERR_OK;
	}
	else if(es->state == ES_CLOSING)
	{
		tcp_recved(tpcb, p->tot_len);
		
		es->p = NULL;

		pbuf_free(p);

		result = ERR_OK;
	}
	else
	{
		tcp_recved(tpcb, p->tot_len);
		
		es->p = NULL;

		pbuf_free(p);

		result = ERR_OK;
	}

	return result;
}

static void TCP_ServerError(void *arg, err_t err)
{
	struct tcp_server_struct *es;

	es = (struct tcp_server_struct*)arg;

	if(es != NULL)
	{
		mem_free(es);
	}

#if 1 // yeom_20201119 [start]
	NVIC_SystemReset();
#endif // yeom_20201119 [end]
}

static err_t TCP_ServerPoll(void *arg, struct tcp_pcb *tpcb)
{
	err_t	result;
	struct tcp_server_struct *es;

	es = (struct tcp_server_struct*)arg;

	if(es != NULL)
	{
		if(es->p != NULL)
		{
			tcp_sent(tpcb, TCP_ServerSent);

			TCP_ServerSend(tpcb, es);
		}
		else
		{
			if(es->state == ES_CLOSING)
			{
				TCP_ServerConnectionClose(tpcb, es);
			}
		}

		result = ERR_OK;
	}
	else
	{
		tcp_abort(tpcb);

		result = ERR_ABRT;
	}

	return result;
}

static err_t TCP_ServerSent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_server_struct *es;

	es = (struct tcp_server_struct *)arg;
	es->retries = 0;

	if(es->p != NULL)
	{
		tcp_sent(tpcb, TCP_ServerSent);
		TCP_ServerSend(tpcb, es);
	}
	else
	{
		if(es->state == ES_CLOSING)
		TCP_ServerConnectionClose(tpcb, es);
	}
	
	return ERR_OK;
}

static void TCP_ServerSend(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	err_t	wrError = ERR_OK;
	struct pbuf	*ptr;

	while((wrError == ERR_OK) && (es->p != NULL) && (es->p->len <= tcp_sndbuf(tpcb)))
	{
		ptr	= es->p;

		wrError	= tcp_write(tpcb, ptr->payload, ptr->len, 1);
		tcp_output(tpcb);

		if(wrError == ERR_OK)
		{
			u8	freed;
			u16	pLen;

			pLen = ptr->len;

			es->p = ptr->next;

			if(es->p != NULL)
			{
				pbuf_ref(es->p);
			}

			do{
				freed = pbuf_free(ptr);
			}while(freed == 0);

			tcp_recved(tpcb, pLen);
		}
		else if(wrError == ERR_MEM)
		{
			es->p = ptr;
		}
		else
		{
		}
	}
}

static void TCP_ServerConnectionClose(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	tcp_arg(tpcb, NULL);
	tcp_sent(tpcb, NULL);
	tcp_recv(tpcb, NULL);
	tcp_err(tpcb, NULL);
	tcp_poll(tpcb, NULL, 0);

	if(es != NULL)
	{
		mem_free(es);
	}

	tcp_close(tpcb);
}

u8 TCP_ServerTransmitData(u8 *pData, u16 length)
{
	u8	result = 1;
	struct pbuf	*pBuff;
	
	pBuff = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_POOL);

	if(pBuff != NULL)
	{
		pbuf_take(pBuff, pData, length);

		pEthTcpServer->p	= pBuff;

		TCP_ServerSend(pEthTcpTxPcb, pEthTcpServer);
	}
	else
	{
		pbuf_free(pBuff);

		result = 0;
	}

	return	result;
}

