#ifndef	_CMDSET_PROCESS_H
#define	_CMDSET_PROCESS_H

#include	"CommandSet.h"

u8 CMD_RegisterInterface(void *pVoid);
u8 CMD_PutReceiveData(u32 src, u8 *pData, u32 length);
u8 CMD_Process();
#endif	//	!_CMDSET_PROCESS_H