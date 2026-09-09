#ifndef	_APP_IR_H
#define	_APP_IR_H

#include	"E7602_PG_Application.h"

typedef	enum{
	IR_TYPE_NEC_DEFAULT		= 0,
	IR_TYPE_NEC_EXTENDED,
}irType_NEC_t;

typedef	enum{
	IR_STATE_NEC_GET_SOF_LEAD_HIGH	= 0,
	IR_STATE_NEC_GET_SOF_LEAD_LOW,
	IR_STATE_NEC_GET_SOF_SPACE_HIGH,
	IR_STATE_NEC_GET_SOF_SPACE_LOW,
	IR_STATE_NEC_GET_COMMAND_HIGH,
	IR_STATE_NEC_GET_COMMAND_LOW,
	IR_STATE_NEC_RCV_DATA_DECODE,
}irState_NEC_t;

typedef struct{
	irType_NEC_t	type;
	irState_NEC_t	state;

	u32	rcvCnt;

	u32	*pSOF;
	u32	*pRcvTime;

	u8	*pDecodeData;

	u32	sofLeadTime;					//	uS
	u32	sofLeadTimeMargin;				//	uS
	u32	sofSpaceTime;					//	uS
	u32	sofSpaceTimeMargin;				//	uS

	u32	dataPulseTime;					//	uS
	u32	dataPulseTimeMargin;			//	uS
	u32	dataSpaceTime;					//	uS
	u32	dataSpaceTimeMargin;			//	uS
}irData_NEC_t;

typedef	struct{
	u32	type;
	u32	cmd;
	u32	adrs;
	u32	repeat;
}irResult_NEC_t;

u8 IR_GpioInit();
u8 IR_IoRead();
u8 IR_Init();
u8 IR_ByteDecode(u32	*pTimeData, u8 *pDecodeData);
u8 IR_SignalDecode();
u8 IR_CmdDecode();
u8 IR_SignalReceive();
u8 IR_Process();

#endif	// _APP_IR_H
