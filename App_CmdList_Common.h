#ifndef	_APP_CMDLIST_COMMON_H
#define	_APP_CMDLIST_COMMON_H

#include	"E7602_PG_Application.h"

u8 Cmd_Board(void *pVoid);
u8 Cmd_File(void *pVoid);
u8 Cmd_FPGA(void *pVoid);
u8 Cmd_IMAGE(void *pVoid);
u8 Cmd_System(void *pVoid);
u8 Cmd_Test(void *pVoid);
u8 Cmd_Delay(void *pVoid);
#if 1 // yeom_20210118 [start]
u8 Cmd_Log(void *pVoid);
#endif // yeom_20210118 [end]

#endif	// _APP_CMDLIST_COMMON_H