#ifndef	_APP_DISPLAYPORTRX_H
#define	_APP_DISPLAYPORTRX_H

#include	"E7602_PG_Application.h"

enum{
	RP_DP_RX_SFUNCTION_MODE_NONE						= 0,

	RP_DP_RX_SFUNCTION_MODE_DP_REG_WRITE				= 0xFFFE,
	RP_DP_RX_SFUNCTION_MODE_DP_REG_READ					= 0xFFFF,
};

typedef	struct{
	u32	channel;
	u32	regAdrs;
	u32	dataAdrs;
	u32	dataSize;
	u8	*pBuffer;
}dprxRegData_t;

typedef	struct{
	u32	mode;

	u8	(*DataWrite)		(void*);
	u8	(*DataWritemd)		(void*);
	u8	(*DataWritemx)		(void*);
	u8	(*DataWritemxlong)	(void*);
	u8	(*DataRead)			(void*);
	u8	(*DataReadmd)		(void*);
	u8	(*DataReadmx)		(void*);
}dprxRegHandle_t;

typedef	struct{
	u32	channel;
	u32	mode;
	u32	ctrl[4];
	u32	buffer[16];
}dprxSFunction_t;

u8 DPRX_SpecialFunction(void *pVoid);
u8 DPRX_RegWrite(u8 channel, u32 adrs, u32 data);
u8 DPRX_RegRead(u8 channel, u32 adrs, u32 *pData);

#endif	// _APP_DISPLAYPORTRX_H
