#include	"App_Log.h"

u8	logUserEnable, logDebugEnable;
u8	logUserCnt, logDebugCnt;

logType_t	logUser, logDebug;

DIR	logDir;
FIL	logFile;

void LOG_memcpy(u8 *pDst, u8 *pSrc, u32 length)
{
	u32	cnt;

	for(cnt = 0; cnt < length; cnt++)
	{
		pDst[cnt] = pSrc[cnt];
	}
}

void LOG_Init()
{
	u8 *logAdrs;

	logAdrs = (u8*)MEMORY_MAP_SDRAM_LOG_USER_ADRS;

	memset(logAdrs, NULL, LOG_DATA_MAX_SIZE);
	
	logUserEnable		= (u8)Config_LogStatus();
	logUserCnt			= 0;
	logUser.state		= 0;
	logUser.length		= 0;
	logUser.logData		= logAdrs;

	logAdrs = (u8*)MEMORY_MAP_SDRAM_LOG_DEBUG_ADRS;

	memset(logAdrs, NULL, LOG_DATA_MAX_SIZE);
	
	logDebugEnable		= (u8)Config_DebugStatus();
	logDebugCnt			= 0;
	logDebug.state		= 0;
	logDebug.length		= 0;
	logDebug.logData	= logAdrs;
}

u8 DBG_FileCheck()
{
	u8	result = 0, rtn;
	u8	cnt;
	u8	*pFilePath;
	
	if(sysError.microSD == 1)	return result;

	if(logDebugEnable == 0)
	{
		result = 1;
		logDebugCnt = 0;
		return result;
	}

	rtn = FatFS_DirMake(LOG_DEBUG_FILE_PATH);

	if(rtn != FR_OK)			return result;

	pFilePath = malloc(256);

	if(pFilePath == NULL)
	{
		free(pFilePath);
		return result;
	}

	for(cnt = 0; cnt < 100; cnt++)
	{
		memset(pFilePath, NULL, 256);
		
		sprintf((char*)pFilePath, "%s/debug%02d.log", LOG_DEBUG_FILE_PATH, cnt);

		rtn = FatFS_FileOpen(&logFile, pFilePath);

		if(rtn == FR_OK)
		{
			FatFS_FileClose(&logFile);

			continue;
		}
		else if(rtn == FR_NO_FILE)
		{
			FatFS_FileClose(&logFile);
			
			break;
		}
	}

	free(pFilePath);
	
	logDebugCnt = cnt;

	result = 1;

	return result;
}

void DBG_Printf(const char *pData, ...)
{
	u32	length;

	va_list	ap;
	
	u8 *pStartData = &(logDebug.logData[logDebug.length]);

	if(logDebugEnable == 0)	return;

	if(logDebug.length > LOG_DATA_MAX_LENGTH)	return;

	va_start(ap, pData);

	length = vsprintf((char*)pStartData, pData, ap);

	logDebug.length += length;		//strlen((char*)pStartData);

	va_end(ap);

	logDebug.state	= 1;
}

void DBG_TxData(u8 *pData, u32 length)
{
	u32 cnt;
	u8 *pStartData = &(logDebug.logData[logDebug.length]);

	if(logDebugEnable == 0)	return;

	if(logDebug.length > LOG_DATA_MAX_LENGTH)	return;

	//memcpy(pStartData, pData, length);

	for(cnt = 0; cnt < length; cnt++)
	{
		pStartData[cnt] = pData[cnt];
	}

	logDebug.length += length;

	logDebug.state	= 1;
}

u8 DBG_Save()
{
	u8	result = 0, rtn;
	u8	logFileName[20];
	u8	logFilePath[50];
	u32	fileSize;

	if(sysError.microSD == 1)							return result;
	if((logDebugEnable == 0) || (logDebug.state == 0))	return result;

	memset(logFileName, NULL, sizeof(logFileName));
	memset(logFilePath, NULL, sizeof(logFilePath));

	sprintf((char*)logFileName, LOG_DEBUG_FILE_NAME, logDebugCnt);
	sprintf((char*)logFilePath, "%s/%s", LOG_DEBUG_FILE_PATH, logFileName);

	rtn = FatFS_DirMake(LOG_DEBUG_FILE_PATH);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileClose(&logFile);

	rtn = FatFS_FileMake(&logFile, logFilePath);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileOpen(&logFile, logFilePath);

	if(rtn != FR_OK)	return result;

	fileSize = FatFS_FileSize(&logFile);

	rtn = FatFS_FilePtrWrite(&logFile, fileSize);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileWrite(&logFile, logDebug.logData, logDebug.length);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileClose(&logFile);

	if(rtn != FR_OK)	return result;

	logDebug.state = 0;

	memset(logDebug.logData, NULL, logDebug.length);

	logDebug.length = 0;

	result = 1;

	return result;
}

u8 LOG_FileCheck()
{
	u8	result = 0, rtn;
	u8	cnt;
	u8	*pFilePath;
	
	if(sysError.microSD == 1)	return result;

	if(logUserEnable == 0)
	{
		result = 1;
		logUserCnt = 0;
		return result;
	}

	rtn = FatFS_DirMake(LOG_USER_FILE_PATH);

	if(rtn != FR_OK)			return result;

	pFilePath = malloc(256);

	if(pFilePath == NULL)
	{
		free(pFilePath);
		return result;
	}

	for(cnt = 0; cnt < 100; cnt++)
	{
		memset(pFilePath, NULL, 256);
		
		sprintf((char*)pFilePath, "%s/userLog%02d.log", LOG_USER_FILE_PATH, cnt);

		rtn = FatFS_FileOpen(&logFile, pFilePath);

		if(rtn == FR_OK)
		{
			FatFS_FileClose(&logFile);

			continue;
		}
		else if(rtn == FR_NO_FILE)
		{
			FatFS_FileClose(&logFile);
			
			break;
		}
	}

	free(pFilePath);
	
	logUserCnt = cnt;

	result = 1;

	return result;
}

void LOG_Printf(const char *pData, ...)
{
	u32	length;

	va_list	ap;
	
	u8 *pStartData = &(logUser.logData[logUser.length]);

	if(logUserEnable == 0)	return;

	if(logUser.length > LOG_DATA_MAX_LENGTH)	return;

	va_start(ap, pData);

	length = vsprintf((char*)pStartData, pData, ap);

	logUser.length += length;		//strlen((char*)pStartData);

	va_end(ap);

	logUser.state	= 1;
}

void LOG_TxData(u8 *pData, u32 length)
{
	u32 cnt;
	u8 *pStartData = &(logUser.logData[logUser.length]);

	if(logUserEnable == 0)	return;

	if(logUser.length > LOG_DATA_MAX_LENGTH)	return;

	//memcpy(pStartData, pData, length);

	for(cnt = 0; cnt < length; cnt++)
	{
		pStartData[cnt] = pData[cnt];
	}

	logUser.length += length;

	logUser.state	= 1;
}

u8 LOG_Save()
{
	u8	result = 0, rtn;
	u8	logFileName[20];
	u8	logFilePath[50];
	u32	fileSize;

	if(sysError.microSD == 1)							return result;
	if((logUserEnable == 0) || (logUser.state == 0))	return result;

	memset(logFileName, NULL, sizeof(logFileName));
	memset(logFilePath, NULL, sizeof(logFilePath));

	sprintf((char*)logFileName, LOG_USER_FILE_NAME, logUserCnt);
	sprintf((char*)logFilePath, "%s/%s", LOG_USER_FILE_PATH, logFileName);

	rtn = FatFS_DirMake(LOG_USER_FILE_PATH);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileClose(&logFile);

	rtn = FatFS_FileMake(&logFile, logFilePath);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileOpen(&logFile, logFilePath);

	if(rtn != FR_OK)	return result;

	fileSize = FatFS_FileSize(&logFile);

	rtn = FatFS_FilePtrWrite(&logFile, fileSize);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileWrite(&logFile, logUser.logData, logUser.length);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileClose(&logFile);

	if(rtn != FR_OK)	return result;

	logUser.state = 0;

	memset(logUser.logData, NULL, logUser.length);

	logUser.length = 0;

	result = 1;

	return result;
}

