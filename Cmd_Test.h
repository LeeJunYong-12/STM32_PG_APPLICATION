#ifndef	_CMD_TEST_H
#define	_CMD_TEST_H

#include	"E7602_PG_Application.h"

extern	const	commandList_t	cmdList_Test[];

u8 CRC_Test_Function();
u8 Cmd_Uart1_Tx(void *pVoid);
u8 Cmd_Uart1_Rx(void *pVoid);

#endif	// _CMD_TEST_H