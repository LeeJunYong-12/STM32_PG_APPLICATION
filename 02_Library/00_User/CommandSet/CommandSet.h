#ifndef	_COMMAND_SET_H
#define	_COMMAND_SET_H

#include	"UserDefine.h"

#include	"CmdSet_Function.h"
#include	"CmdSet_Process.h"

#define	CMDSET_COMM_SRC_COUNT		5

#define	CMDSET_MAX_COUNT			8192

enum{
	CMD_MODE_NONE				= 0x00000000,
	CMD_MODE_ONCE				= 0x00000001,
	CMD_MODE_DEACTIVATE			= 0x00000002,
	CMD_MODE_RESULT_OUT			= 0x00000004,
};

typedef	struct{
	u8	*pCmd;
	u8	(*pFunction)				(void*);
	u32	option;
	u32	mode;
	u8	*pMessage;
	u8	*pNote;
}commandList_t;

typedef	u8 (*pfTransmitData)		(u8*, u32);

typedef	struct{
	u32				srcType;
	commandList_t	*pCmdList;
	u8				**pArgv;
	u32				argc;
	u32				option;
	u32				retryCnt;
	u32				deactivate;
	u32				channel;
	u32				position;
}cmdFunctionData_t;

typedef	struct{
	u32	parseMaxCount;

	u32	receiveFlag;

	u32	rxSrcType;
	u32	rxDataSize;
	u8	*pRxData;
	u32	rxDataCnt;

	u32	txSrcType;
	u32	txDataSize;
	u8 *pTxData;
	u32	txDataCnt;

	commandList_t	*pCmdList;

	u8	(*RunningState)		(u8);
	u8	(*ErrorState)		(u8);

	pfTransmitData			TransmitData[CMDSET_COMM_SRC_COUNT];
}cmdSetHandle_t;;

#endif	//	!_COMMAND_SET_H