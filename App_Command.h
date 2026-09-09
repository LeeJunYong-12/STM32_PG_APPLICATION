#ifndef	_APP_COMMAND_H
#define	_APP_COMMAND_H

#include	"E7602_PG_Application.h"

enum{
	CMD_COMM_SRC_TYPE_USB		= 0,
	CMD_COMM_SRC_TYPE_ETHERNET,
	CMD_COMM_SRC_TYPE_SERIAL,
	CMD_COMM_SRC_TYPE_SCRIPT,
};

extern	commandList_t	*pCommandList;

u8 CMD_RcvCheck();
u8 CMD_ColorConverter(u8 *pData, u32 *pColor);
u8 CMD_SubCommandExecute(void *pvFuncData, void *pvCmdList);
u8 CMD_Init();

#endif	// _APP_COMMAND_H