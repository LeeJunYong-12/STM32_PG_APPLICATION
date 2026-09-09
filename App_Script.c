#include	"App_Script.h"

static	scriptData_t	scriptData;

u8 Script_SetFlag(u8 position)
{
	u8	result = 0;

	if(position >= 32)		return result;

	scriptData.flag	|= (0x00000001 << position);

	result = 1;

	return result;
}

u32 Script_GetFlag()
{
	return scriptData.flag;
}

u32  Script_ClearFlag(u8 position)
{
	u8	result = 0;

	if(position >= 32)		return result;

	scriptData.flag	&= ~(0x00000001 << position);

	result = 1;

	return result;
}

u8 Script_Init()
{

	u8	result = 0;
#if 0 // yeom_20200803 [start]
	u32	size;

	size = 1 * 1024 * 1024;			//	1MB

	memset(&scriptData, NULL, sizeof(scriptData));

	scriptData.pScriptData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE_ADRS;

	memset(scriptData.pScriptData, NULL, size);

	result = 1;
#endif // yeom_20200803 [end]

	return result;
}

#if 0
u8 Script_Init()
{
	u8	result = 0;

	memset(&scriptInfo, NULL, sizeof(scriptInfo));

	result = 1;

	return result;
}
#endif

#if 0// yeom_20200803
u8 Script_Enable(u32 slotNo)
{
	u8	result = 0;

	if(scriptInfo.state == 0)
	{
		scriptInfo.slotNo	= slotNo;
		scriptInfo.flag		= 1;

		result = 1;
	}

	return result;
}
#endif// yeom_20200803

#if 1 // yeom_20200803 [start]
u8 Script_FileLoad(u16 fileNo)
{
	FIL keyFile;
	u8	*dstData;
	u8	*pFilePath;

	pFilePath = malloc(256);

	memset(pFilePath, NULL, 256);
	
	switch(fileNo)
	{
		case 0:
			FM_IndexRead(FM_FILE_KEY, 0, &fmIndexKey);
			if(fmIndexKey.fState == 0)		goto SCRIPT_FILELOAD_ERROR;
			strcpy((char*)pFilePath, (char*)fmIndexKey.fName);
			dstData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE__0_ADRS;
			break;

		case 1:
			FM_IndexRead(FM_FILE_KEY, 1, &fmIndexKey);
			if(fmIndexKey.fState == 0)		goto SCRIPT_FILELOAD_ERROR;
			strcpy((char*)pFilePath, (char*)fmIndexKey.fName);
			dstData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE__1_ADRS;
			break;

		case 2:
			FM_IndexRead(FM_FILE_KEY, 2, &fmIndexKey);
			if(fmIndexKey.fState == 0)		goto SCRIPT_FILELOAD_ERROR;
			strcpy((char*)pFilePath, (char*)fmIndexKey.fName);
			dstData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE__2_ADRS;
			break;

		case 3:
			FM_IndexRead(FM_FILE_KEY, 3, &fmIndexKey);
			if(fmIndexKey.fState == 0)		goto SCRIPT_FILELOAD_ERROR;
			strcpy((char*)pFilePath, (char*)fmIndexKey.fName);
			dstData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE__3_ADRS;
			break;

		case 4:
			FM_IndexRead(FM_FILE_KEY, 4, &fmIndexKey);
			if(fmIndexKey.fState == 0)		goto SCRIPT_FILELOAD_ERROR;
			strcpy((char*)pFilePath, (char*)fmIndexKey.fName);
			dstData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE__4_ADRS;
			break;

		case 5:
			FM_IndexRead(FM_FILE_KEY, 5, &fmIndexKey);
			if(fmIndexKey.fState == 0)		goto SCRIPT_FILELOAD_ERROR;
			strcpy((char*)pFilePath, (char*)fmIndexKey.fName);
			dstData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE__5_ADRS;
			break;

		case 6:
			FM_IndexRead(FM_FILE_KEY, 6, &fmIndexKey);
			if(fmIndexKey.fState == 0)		goto SCRIPT_FILELOAD_ERROR;
			strcpy((char*)pFilePath, (char*)fmIndexKey.fName);
			dstData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE__6_ADRS;
			break;

		case 7:
			FM_IndexRead(FM_FILE_KEY, 7, &fmIndexKey);
			if(fmIndexKey.fState == 0)		goto SCRIPT_FILELOAD_ERROR;
			strcpy((char*)pFilePath, (char*)fmIndexKey.fName);
			dstData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE__7_ADRS;
			break;

		default:
			SCRIPT_FILELOAD_ERROR:
			free(pFilePath);
			return 0;
			break;
	}
	
	memset(dstData, NULL, 131072);
	FatFS_FileOpen(&keyFile, pFilePath);
	FatFS_FilePtrWrite(&keyFile, 0);
	FatFS_FileReadAll(&keyFile, (u32)dstData);
	FatFS_FileClose(&keyFile);
	free(pFilePath);

	return 1;
}
#else	
static u8 Script_FileLoad(u8 slotNo)
{
	u8	result = 0;
	u8	filePath[256];
	u32	fileSize;
	FRESULT	fsRtn;
	FIL	loadFile;

	if(slotNo >= FM_INDEX_KEY_LIST_MAX_COUNT)				return result;

	if(pFmIndexKeyFile->key[slotNo].fState == 0x0000)		return result;

	scriptInfo.pScriptData = (u8*)MEMORY_MAP_SDRAM_SCRIPT_FILE_ADRS;

	memset(filePath, NULL, sizeof(filePath));
	memcpy(filePath, pFmIndexKeyFile->key[slotNo].fName, sizeof(filePath));

	fsRtn = FatFS_FileOpen(&loadFile, filePath);
	if(fsRtn != FR_OK)				return result;

	fileSize = FatFS_FileSize(&loadFile);
	if(fileSize > 128 * 1024)		return result;
	scriptInfo.scriptSize = fileSize;

	fsRtn = FatFS_FilePtrWrite(&loadFile, 0);
	if(fsRtn != FR_OK)				return result;

	fsRtn = FatFS_FileRead(&loadFile, (u32)scriptInfo.pScriptData, fileSize);
	if(fsRtn != FR_OK)				return result;

	fsRtn = FatFS_FileClose(&loadFile);
	if(fsRtn != FR_OK)				return result;

	memset(&scriptInfo.pScriptData[fileSize], NULL, 32);

	scriptInfo.pRemainData = scriptInfo.pScriptData;

	result = 1;

	return result;
}
#endif // yeom_20200803 [end]

#if 1 // yeom_20200803 [start]
u8 Script_Action(u32 srcAdrs)
{
	u8	result = 0, rtn;
	u32	errCnt = 0;	
	u8 *srcData;
	u8 *token, *tokenCpy;
	u16	length;

	srcData = (u8*)srcAdrs;
	token = NULL;	

	token = (u8*)strtok((char*)srcData, "\r\n");
	
	do{
		if(token != NULL)
		{
			length = (u32)strlen((char*)token);

			rtn = CMD_PutReceiveData(CMD_COMM_SRC_TYPE_SCRIPT, token, length);
			if(rtn == 0)		break;

			rtn = CMD_Process();
			if(rtn == 0)		errCnt += 1;

			tokenCpy = token + length + 1;

			token = (u8*)strtok((char*)tokenCpy, "\r\n");
		}		
	}while(token != NULL);

	result = 1;

	return result;	
}
#endif // yeom_20200803 [end]

#if 1 // yeom_20200803 [start]
void Script_Execute(u8 position)
{
	u8	rtn;
	u32  srcAdrs;

	if(position >= 8)		return;

	rtn = Script_FileLoad(position);

	if(rtn == 0)	return;	

	switch(position)
	{
		case 0:
			srcAdrs = MEMORY_MAP_SDRAM_SCRIPT_FILE__0_ADRS;
			break;

		case 1:
			srcAdrs = MEMORY_MAP_SDRAM_SCRIPT_FILE__1_ADRS;
			break;

		case 2:
			srcAdrs = MEMORY_MAP_SDRAM_SCRIPT_FILE__2_ADRS;
			break;

		case 3:
			srcAdrs = MEMORY_MAP_SDRAM_SCRIPT_FILE__3_ADRS;
			break;

		case 4:
			srcAdrs = MEMORY_MAP_SDRAM_SCRIPT_FILE__4_ADRS;
			break;

		case 5:
			srcAdrs = MEMORY_MAP_SDRAM_SCRIPT_FILE__5_ADRS;
			break;
			
		case 6:
			srcAdrs = MEMORY_MAP_SDRAM_SCRIPT_FILE__6_ADRS;
			break;

		case 7:
			srcAdrs = MEMORY_MAP_SDRAM_SCRIPT_FILE__7_ADRS;
			break;
	}
	
	Script_Action(srcAdrs);
}
#else
static u8 Script_Execute()
{
	u8	result = 0, rtn;
	u8	*pToken, *pNextData;
	u32	tokenSize = 0, executeCnt = 0;

	do{
		if(executeCnt >= 3)			break;

		pToken = (u8*)strtok_r((char*)scriptInfo.pRemainData, SCRIPT_DELIMITER, (char**)pNextData);
		if(pToken == NULL)
		{
			result = 1;
			break;
		}

		scriptInfo.pRemainData = pNextData;

		tokenSize = strlen((char*)pToken);

		rtn = CMD_PutReceiveData(CMD_COMM_SRC_TYPE_SCRIPT, pToken, tokenSize);
		if(rtn ==  0)				break;

		rtn = CMD_Process();
		if(rtn != 0)				break;

		executeCnt += 1;
	}while(1);

	return result;
}
#endif // yeom_20200803 [end]

#if 1 // yeom_20200803 [start]
u8 Script_Process()
{
	u8	result = 0;

	if(Script_GetFlag() == NULL)			return result;

	if(Switch_GetIoStatus() != 0x00)		return result;

	if((Script_GetFlag() & 0x00000001) != 0)
	{
		Script_Execute(0);

		Script_ClearFlag(0);
	}
	
	if((Script_GetFlag() & 0x00000002) != 0)
	{
		Script_Execute(1);

		Script_ClearFlag(1);
	}

	if((Script_GetFlag() & 0x00000004) != 0)
	{
		Script_Execute(2);

		Script_ClearFlag(2);
	}

	if((Script_GetFlag() & 0x00000008) != 0)
	{
		Script_Execute(3);

		Script_ClearFlag(3);
	}

	if((Script_GetFlag() & 0x00000010) != 0)
	{
		Script_Execute(4);

		Script_ClearFlag(4);
	}

	if((Script_GetFlag() & 0x00000020) != 0)
	{
		Script_Execute(5);

		Script_ClearFlag(5);
	}

	if((Script_GetFlag() & 0x00000040) != 0)
	{
		Script_Execute(6);

		Script_ClearFlag(6);
	}

	if((Script_GetFlag() & 0x00000080) != 0)
	{
		Script_Execute(7);

		Script_ClearFlag(7);
	}

	return result;
}
#else
u8 Script_Process()
{
	u8	result = 0, rtn;

	if(scriptInfo.state == 0)
	{
		if(ExtSW_GetFlag() != 0)
		{
			Script_Enable(ExtSW_GetFlag() - 1);
		}

		if(scriptInfo.flag == 0)		return result;

		if(scriptInfo.slotNo >= FM_INDEX_KEY_LIST_MAX_COUNT)
		{
			memset(&scriptInfo, NULL, sizeof(scriptInfo));

			ExtSW_ClearFlag();

			return result;
		}

		rtn = Script_FileLoad(scriptInfo.slotNo);
		if(rtn == 0)
		{
			memset(&scriptInfo, NULL, sizeof(scriptInfo));

			ExtSW_ClearFlag();

			return result;
		}

		rtn = Script_Execute();
		if(rtn == 0)					scriptInfo.state = 1;
		else
		{
			memset(&scriptInfo, NULL, sizeof(scriptInfo));

			ExtSW_ClearFlag();

			result = 1;
		}
	}
	else
	{
		rtn = Script_Execute();
		if(rtn != 0)
		{
			memset(&scriptInfo, NULL, sizeof(scriptInfo));

			ExtSW_ClearFlag();

			result = 1;
		}
	}

	return result;
}
#endif // yeom_20200803 [end]
