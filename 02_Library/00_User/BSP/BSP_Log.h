#ifndef	_BSP_LOG_H
#define	_BSP_LOG_H

#include	"BSP.h"

typedef	struct{
	u32	enable;
	u8	*pBuffer;
	u32	bufferSize;
	u32	writeCnt;
}bspLogData_t;

#endif	//	_BSP_LOG_H_