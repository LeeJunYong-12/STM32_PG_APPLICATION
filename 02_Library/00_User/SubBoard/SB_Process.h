#ifndef	_SB_PROCESS_H
#define	_SB_PROCESS_H

#include	"SubBoard.h"

u8 SB_SubBoardXfer(void *pVoid);
u8 SB_CommandExecute(void *pVoid);
u8 SB_CommandProcess();
u8 SB_SubBoardXfer_boot(void *pVoid);
u8 SB_SubBoardXfer_boot_check(void *pVoid);

#endif	//	_SB_PROCESS_H