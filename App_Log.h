#ifndef	_APP_LOG_H
#define	_APP_LOG_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u32	state;
	u32	length;
	u8	*logData;
}logType_t;

//#define	LOG_DATA_MEM_ADRS		((u32)0xC7F00000)		// 127MB-128MB(MEMORY SIZE 128MB)

#define	LOG_DATA_MAX_SIZE		((u32)0x100000)			// 1MB
#define	LOG_DATA_MAX_LENGTH		((u32)0x000E0000)		// 896KB

#define LOG_FILE_EXTENTION		".log"

#define	LOG_USER_DATA_ADRS		MEMORY_MAP_SDRAM_LOG_USER_ADRS
#define	LOG_DEBUG_DATA_ADRS		MEMORY_MAP_SDRAM_LOG_DEBUG_ADRS

#define LOG_USER_FILE_NAME		"userLog%02d.log"
#define LOG_USER_FILE_PATH		"1:/log"

#define LOG_DEBUG_FILE_NAME		"debug%02d.log"
#define LOG_DEBUG_FILE_PATH		"1:/debug"

extern	u8	logUserEnable, logDebugEnable;

void LOG_Init();
u8 DBG_FileCheck();
void DBG_Printf(const char *pData, ...);
void DBG_TxData(u8 *pData, u32 length);
u8 DBG_Save();
u8 LOG_FileCheck();
void LOG_Printf(const char *pData, ...);
void LOG_TxData(u8 *pData, u32 length);
u8 LOG_Save();

#endif	// _APP_LOG_H