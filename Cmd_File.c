#include	"Cmd_File.h"

static	u8	folderPath[128], fileName[128], filePath[256];
static	u8p	folderList[10], fileList[256];
static	FIL	cmdLoadFile, cmdSaveFile;
static	DIR	cmdDir;
static	FILINFO	cmdFileInfo;

const	commandList_t	cmdList_FILE[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"check",				Cmd_FileCheck,				NULL,		NULL,		";CHECK",				NULL},
												{	"count",				Cmd_FileCount,				NULL,		NULL,		";COUNT",				NULL},
												{	"info",					Cmd_FileInfo,				NULL,		NULL,		";INFO",				NULL},
												{	"save",					Cmd_FileSave,				NULL,		NULL,		";SAVE",				NULL},
												{	"save.other",			Cmd_FileSaveOther,			NULL,		NULL,		";SAVE.OTHER",			NULL},
												{	"save.path",			Cmd_FileSavePath,			NULL,		NULL,		";SAVE.PATH",			NULL},
												{	"delete",				Cmd_FileDelete,				NULL,		NULL,		";SAVE.DELETE",			NULL},
												{	"listout",				Cmd_FileListOut,			NULL,		NULL,		";LISTOUT",				NULL},
												{	"insert",				Cmd_FileInsert,				NULL,		NULL,		";INSERT",				NULL},
												{	"arrange",				Cmd_FileArrange,			NULL,		NULL,		";ARRANGE",				NULL},
												{	"enable",				Cmd_FileEnable,				NULL,		NULL,		";ENABLE",				NULL},
												{	"disable",				Cmd_FileDisable,			NULL,		NULL,		";DISABLE",				NULL},
												{	"add",					Cmd_FileAdd,				NULL,		NULL,		";ADD",					NULL},
												{	"scan",					Cmd_FileScan,				NULL,		NULL,		";SCAN",				NULL},
												{	"scandir",				Cmd_FileScanDir,			NULL,		NULL,		";SCANDIR",				NULL},
												{	"scancount",			Cmd_FileScanCount,			NULL,		NULL,		";SCANCOUNT",			NULL},
												{	"getfree",				Cmd_FileGetFree,			NULL,		NULL,		";GETFREE",				NULL},
												{	"format",				Cmd_FileFormat,				NULL,		NULL,		";FORMAT",				NULL},
												{	"copy",					Cmd_FileCopy,				NULL,		NULL,		";COPY",				NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};


u8 Cmd_FileCheck(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)				return result;

	if(CMD_Compare(pData->pArgv[0], "nand"))
	{
		if(sysError.nandFlash == 1)		CMD_Printf(";NAND_ERROR");
		else							CMD_Printf(";NAND_OK");
	}
	else if(CMD_Compare(pData->pArgv[0], "sd"))
	{
		if(sysError.microSD == 1)		CMD_Printf(";SD_ERROR");
		else							CMD_Printf(";SD_OK");
	}
	else								return result;

	result = 1;

	return result;
}

u8 Cmd_FileCount(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "key"))
	{
		CMD_Printf(";KEY;%d", FM_INDEX_KEY_LIST_MAX_COUNT);
	}
	else if(CMD_Compare(pData->pArgv[0], "image"))
	{
		CMD_Printf(";IMAGE;%d", FM_INDEX_IMAGE_LIST_AVAILABLE_COUNT);
	}
	else		return result;

	result = 1;

	return result;
}

u8 Cmd_FileInfo(void *pVoid)
{
	u8	result = 0, rtn;
	u16	indexNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	indexNo = CMD_StrToUL(pData->pArgv[1]);
	indexNo -= 1;

	if(CMD_Compare(pData->pArgv[0], "key"))
	{
		CMD_Printf(";KEY");
			
		if(indexNo >= FM_INDEX_KEY_LIST_MAX_COUNT)		return result;

		rtn = FM_IndexRead(FM_FILE_KEY, indexNo, &fmIndexKey);
		if(rtn == 0)									return result;

		if(fmIndexKey.fState != 0x0000)
		{
			CMD_Printf(";F");
			CMD_Printf(";%d", fmIndexKey.fSize);
			CMD_Printf(";%s", fmIndexKey.fName);
		}
		else
		{
			CMD_Printf(";0;0;0");
		}
	}
	else if(CMD_Compare(pData->pArgv[0], "image"))
	{
		CMD_Printf(";IMAGE");

		if(indexNo >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;

		rtn = FM_IndexRead(FM_FILE_IMAGE, indexNo, &fmIndexImage);
		if(rtn == 0)									return result;

		if(fmIndexImage.fState != 0x0000)
		{
			if((fmIndexImage.fState & 0x8000) == 0x8000)
			{
				CMD_Printf(";F");
			}
			else
			{
				CMD_Printf(";T");
			}
			CMD_Printf(";%d", fmIndexImage.fSize);
			CMD_Printf(";%s", fmIndexImage.fName);
			CMD_Printf(";%d", fmIndexImage.vLine);
			CMD_Printf(";%d", fmIndexImage.hPixel);
		}
		else
		{
			CMD_Printf(";0;0;0;0;0");
		}
	}
	else		return result;

	result = 1;

	return result;
}

u8 Cmd_FileSave(void *pVoid)
{
	u8	result = 0, rtn;
	u8	*pFileData, *pFileName;
	u16	indexNo;
//	u16	waitTime;
	u32	rxSrc;
	u32	fileSize, rcvSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	rxSrc = pData->srcType;

	pFileData = (u8*)MEMORY_MAP_SDRAM_FILE_DUMP_ADRS;

	if(CMD_Compare(pData->pArgv[0], "key"))
	{
		if(pData->argc != 4)							return result;

		fileSize	= CMD_StrToUL(pData->pArgv[1]);
		indexNo		= CMD_StrToUL(pData->pArgv[2]);
		pFileName	= pData->pArgv[3];

		if(indexNo == 0)								return result;
		indexNo	-= 1;

		if(indexNo >= FM_INDEX_KEY_LIST_MAX_COUNT)		return result;

		if(rxSrc == CMD_COMM_SRC_TYPE_USB)
		{
			rtn = UsbPkt_FileDownload(pFileData, &rcvSize);
		}
		else if(rxSrc == CMD_COMM_SRC_TYPE_ETHERNET)
		{
			rtn = NetworkPKT_FileDownload(pFileData, &rcvSize);
		}
		else											return result;

		if(rtn == 0)									return result;

		if(fileSize != rcvSize)							return result;

		if(fmDrive == 0)
		{
			sprintf((char*)filePath, "0:/system/key/%s", pFileName);
		}
		else
		{
			sprintf((char*)filePath, "1:/system/key/%s", pFileName);
		}

		FM_IndexRead(FM_FILE_KEY, indexNo, &fmIndexKey);

		if(fmIndexKey.fState != 0)
		{
			u8	chkr = 0;

			for(u16 cnt = 0; cnt < FM_INDEX_KEY_LIST_MAX_COUNT; cnt++)
			{
				FM_IndexRead(FM_FILE_KEY, cnt, &fmIndexKeyCpy);

				if(fmIndexKeyCpy.fState == 0)			continue;

				if(memcmp(fmIndexKey.fName, fmIndexKeyCpy.fName, 256) == 0)
				{
					chkr = 1;
				}
			}

			if(chkr == 0)	rtn = FatFS_Delete(fmIndexKey.fName);
		}

		// FILE SAVE

		if(fmDrive == 0)
		{
			rtn = FatFS_DirMake("0:/system/key");
		}
		else
		{
			rtn = FatFS_DirMake("1:/system/key");
		}

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileMake(&cmdSaveFile, filePath);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileOpen(&cmdSaveFile, filePath);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FilePtrWrite(&cmdSaveFile, 0);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileTruncate(&cmdSaveFile);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileWrite(&cmdSaveFile, pFileData, rcvSize);
		
		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileClose(&cmdSaveFile);

		if(rtn != FR_OK)								return result;

		memset(&fmIndexKey, NULL, sizeof(fmIndexKey));

		fmIndexKey.fState = 0x01;
		fmIndexKey.fSize = rcvSize;
		memset(fmIndexKey.fName, NULL, sizeof(filePath));
		memcpy(fmIndexKey.fName, filePath, sizeof(filePath));

		FM_IndexWrite(FM_FILE_KEY, indexNo, &fmIndexKey);
		FM_IndexSave(FM_FILE_KEY);
	}
	else if(CMD_Compare(pData->pArgv[0], "image"))
	{
		u16 vLine, hPixel;

		if(pData->argc != 7)							return result;

		fileSize	= CMD_StrToUL(pData->pArgv[1]);
		indexNo		= CMD_StrToUL(pData->pArgv[2]);
		pFileName	= pData->pArgv[3];
		vLine		= CMD_StrToUL(pData->pArgv[4]);
		hPixel		= CMD_StrToUL(pData->pArgv[5]);

		memset(folderPath, NULL, sizeof(folderPath));

		if(CMD_Compare(pData->pArgv[6], "nand"))
		{
			sprintf((char*)folderPath, "0:/image");
		}
		else if(CMD_Compare(pData->pArgv[6], "sd"))
		{
			sprintf((char*)folderPath, "1:/image");
		}
		else											return result;

		if(indexNo == 0)								return result;
		indexNo	-= 1;

		if(indexNo >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;

//		waitTime = fileSize / 5000;

		if(rxSrc == CMD_COMM_SRC_TYPE_USB)
		{
			rtn = UsbPkt_FileDownload(pFileData, &rcvSize);
		}
		else if(rxSrc == CMD_COMM_SRC_TYPE_ETHERNET)
		{
			rtn = NetworkPKT_FileDownload(pFileData, &rcvSize);
		}
		else											return result;

		if(rtn == 0)									return result;

		if(fileSize != rcvSize)							return result;

//		CMD_TransmitWaitTime(waitTime);

		sprintf((char*)filePath, "%s/%s", folderPath, pFileName);

		FM_IndexRead(FM_FILE_IMAGE, indexNo, &fmIndexImage);

		if(fmIndexImage.fState != 0)
		{
			u8	chkr = 0;

			for(u16 cnt = 0; cnt < FM_INDEX_IMAGE_LIST_MAX_COUNT; cnt++)
			{
				FM_IndexRead(FM_FILE_IMAGE, cnt, &fmIndexImageCpy);

				if(fmIndexImageCpy.fState == 0)			continue;

				if(memcmp(fmIndexImage.fName, fmIndexImageCpy.fName, 256) == 0)
				{
					chkr = 1;
				}
			}

			if(chkr == 0)	rtn = FatFS_Delete(fmIndexImage.fName);
		}
		
		// FILE SAVE

		rtn = FatFS_DirMake(folderPath);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileMake(&cmdSaveFile, filePath);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileOpen(&cmdSaveFile, filePath);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FilePtrWrite(&cmdSaveFile, 0);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileTruncate(&cmdSaveFile);

		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileWrite(&cmdSaveFile, pFileData, rcvSize);
		
		if(rtn != FR_OK)								return result;

		rtn = FatFS_FileClose(&cmdSaveFile);

		if(rtn != FR_OK)								return result;

		memset(&fmIndexImage, NULL, sizeof(fmIndexImage));

		fmIndexImage.fState		= 0x8001;
		fmIndexImage.fSize		= rcvSize;
		memset(fmIndexImage.fName, NULL, sizeof(filePath));
		memcpy(fmIndexImage.fName, filePath, sizeof(filePath));
		fmIndexImage.hPixel		= hPixel;
		fmIndexImage.vLine		= vLine;

		FM_IndexWrite(FM_FILE_IMAGE, indexNo, &fmIndexImage);
		FM_IndexSave(FM_FILE_IMAGE);
	}
	else												return result;

	result = 1;

	return result;
}

u8 Cmd_FileSaveOther(void *pVoid)
{
	u8	result = 0, rtn;
	u8	*pFileData, *pFileName;
//	u16	waitTime;
	u32	rxSrc, fileSize, rcvSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	rxSrc = pData->srcType;

	memset(folderPath, NULL, sizeof(folderPath));

	if(CMD_Compare(pData->pArgv[0], "nand"))
	{
		sprintf((char*)folderPath, "0:/other");
	}
	else if(CMD_Compare(pData->pArgv[0], "sd"))
	{
		sprintf((char*)folderPath, "1:/other");
	}
	else						return result;

	fileSize = CMD_StrToUL(pData->pArgv[1]);
	pFileName = pData->pArgv[2];

	pFileData = (u8*)MEMORY_MAP_SDRAM_FILE_DUMP_ADRS;

//	waitTime = fileSize / 5000;

	if(rxSrc == CMD_COMM_SRC_TYPE_USB)
	{
		rtn = UsbPkt_FileDownload(pFileData, &rcvSize);
	}
	else if(rxSrc == CMD_COMM_SRC_TYPE_ETHERNET)
	{
		rtn = NetworkPKT_FileDownload(pFileData, &rcvSize);
	}
	else						return result;

	if(rtn == 0)				return result;

	if(fileSize != rcvSize)		return result;

//	CMD_TransmitWaitTime(waitTime);

	memset(filePath, NULL, sizeof(filePath));

	sprintf((char*)filePath, "%s/%s\0", folderPath, pFileName);

	rtn = FatFS_DirMake(folderPath);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileMake(&cmdSaveFile, filePath);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileOpen(&cmdSaveFile, filePath);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FilePtrWrite(&cmdSaveFile, 0);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileTruncate(&cmdSaveFile);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileWrite(&cmdSaveFile, pFileData, rcvSize);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileClose(&cmdSaveFile);

	if(rtn != FR_OK)			return result;

	result = 1;

	return result;
}

u8 Cmd_FileSavePath(void *pVoid)
{
	u8	result = 0, rtn;
	u8	*pFileData;
//	u16	waitTime;
	u32 rxSrc, fileSize = 0, rcvSize = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	memset(folderPath, NULL, sizeof(folderPath));
	memset(fileName, NULL, sizeof(fileName));
	memset(filePath, NULL, sizeof(filePath));

	fileSize = CMD_StrToUL(pData->pArgv[0]);
	strcpy((char*)folderPath, (char*)pData->pArgv[1]);
	strcpy((char*)fileName, (char*)pData->pArgv[2]);

//	waitTime = fileSize / 5000;

	rxSrc = pData->srcType;

	pFileData = (u8*)MEMORY_MAP_SDRAM_FILE_DUMP_ADRS;

	if(rxSrc == CMD_COMM_SRC_TYPE_USB)
	{
		rtn = UsbPkt_FileDownload(pFileData, &rcvSize);
	}
	else if(rxSrc == CMD_COMM_SRC_TYPE_ETHERNET)
	{
		rtn = NetworkPKT_FileDownload(pFileData, &rcvSize);
	}
	else						return result;

	if(rtn == 0)				return result;

	if(fileSize != rcvSize)		return result;

//	CMD_TransmitWaitTime(waitTime);

	sprintf((char*)filePath, "%s/%s\0", folderPath, fileName);

	rtn = FatFS_DirMake(folderPath);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileMake(&cmdSaveFile, filePath);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileOpen(&cmdSaveFile, filePath);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FilePtrWrite(&cmdSaveFile, 0);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileTruncate(&cmdSaveFile);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileWrite(&cmdSaveFile, pFileData, rcvSize);

	if(rtn != FR_OK)			return result;

	rtn = FatFS_FileClose(&cmdSaveFile);

	if(rtn != FR_OK)			return result;

	result = 1;

	return result;
}

u8 Cmd_FileDelete(void *pVoid)
{
	u8	result = 0;
	u16	indexNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	indexNo = CMD_StrToUL(pData->pArgv[1]);
	indexNo -= 1;

	if(CMD_Compare(pData->pArgv[0], "key"))
	{
		CMD_Printf(";KEY");
			
		if(indexNo >= FM_INDEX_KEY_LIST_MAX_COUNT)	return result;

		FM_IndexRead(FM_FILE_KEY, indexNo, &fmIndexKey);

		for(u16 cnt = 0; cnt < FM_INDEX_KEY_LIST_MAX_COUNT; cnt++)
		{
			if(cnt == indexNo)				continue;

			FM_IndexRead(FM_FILE_KEY, cnt, &fmIndexKeyCpy);

			if(fmIndexKey.fState == 0)		continue;

			if(memcmp(fmIndexKey.fName, fmIndexKeyCpy.fName, 256) == 0)
			{
				memset(&fmIndexKeyCpy, NULL, sizeof(fmIndexKeyCpy));

				FM_IndexWrite(FM_FILE_KEY, cnt, &fmIndexKeyCpy);
			}
		}

		FatFS_Delete(fmIndexKey.fName);

		memset(&fmIndexKey, NULL, sizeof(fmIndexKey));

		FM_IndexWrite(FM_FILE_KEY, indexNo, &fmIndexKey);

		FM_IndexSave(FM_FILE_KEY);
	}
	else if(CMD_Compare(pData->pArgv[0], "image"))
	{
		CMD_Printf(";IMAGE");
			
		if(indexNo >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;

		FM_IndexRead(FM_FILE_IMAGE, indexNo, &fmIndexImage);

		for(u16 cnt = 0; cnt < FM_INDEX_IMAGE_LIST_MAX_COUNT; cnt++)
		{
			if(cnt == indexNo)					continue;

			FM_IndexRead(FM_FILE_IMAGE, cnt, &fmIndexImageCpy);

			if(fmIndexImageCpy.fState == 0)		continue;

			if(memcmp(fmIndexImage.fName, fmIndexImageCpy.fName, 256) == 0)
			{
				memset(&fmIndexImageCpy, NULL, sizeof(fmIndexImageCpy));

				FM_IndexWrite(FM_FILE_IMAGE, cnt, &fmIndexImageCpy);
			}
		}

		FatFS_Delete(fmIndexImage.fName);

		memset(&fmIndexImage, NULL, sizeof(fmIndexImage));

		FM_IndexWrite(FM_FILE_IMAGE, indexNo, &fmIndexImage);

		FM_IndexSave(FM_FILE_IMAGE);
	}
	else						return result;

	result = 1;

	return result;
}

u8 Cmd_FileListOut(void *pVoid)
{
	u8	result = 0;
	u16	indexNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	indexNo = CMD_StrToUL(pData->pArgv[1]);
	indexNo -= 1;

	if(CMD_Compare(pData->pArgv[0], "key"))
	{
		if(indexNo >= FM_INDEX_KEY_LIST_MAX_COUNT)		return result;

		memset(&fmIndexKey, NULL, sizeof(fmIndexKey));

		FM_IndexWrite(FM_FILE_KEY, indexNo, &fmIndexKey);

		FM_IndexSave(FM_FILE_KEY);
	}
	else if(CMD_Compare(pData->pArgv[0], "image"))
	{
		if(indexNo >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;

		memset(&fmIndexImage, NULL, sizeof(fmIndexImage));

		FM_IndexWrite(FM_FILE_IMAGE, indexNo, &fmIndexImage);

		FM_IndexSave(FM_FILE_IMAGE);
	}
	else						return result;

	result = 1;

	return result;
}

u8 Cmd_FileInsert(void *pVoid)
{
	u8	result = 0;
	u16	indexNo0, indexNo1;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	indexNo0 = CMD_StrToUL(pData->pArgv[1]);
	indexNo0 -= 1;

	indexNo1 = CMD_StrToUL(pData->pArgv[2]);
	indexNo1 -= 1;

	if(CMD_Compare(pData->pArgv[0], "image"))
	{
		if(indexNo0 >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;
		if(indexNo1 >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;
		if(indexNo0 == indexNo1)						return result;

		FM_IndexRead(FM_FILE_IMAGE, indexNo0, &fmIndexImageCpy);
		FM_IndexRead(FM_FILE_IMAGE, indexNo1, &fmIndexImage);

		FM_IndexWrite(FM_FILE_IMAGE, indexNo0, &fmIndexImage);
		FM_IndexWrite(FM_FILE_IMAGE, indexNo1, &fmIndexImageCpy);

		FM_IndexSave(FM_FILE_IMAGE);
	}
	else						return result;

	result = 1;

	return result;
}

u8 Cmd_FileArrange(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "image"))
	{
		FM_ImageIndexArrange();

		FM_IndexSave(FM_FILE_IMAGE);
	}
	else						return result;

	result = 1;

	return result;
}

u8 Cmd_FileEnable(void *pVoid)
{
	u8	result = 0;
	u16	indexNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	indexNo = CMD_StrToUL(pData->pArgv[1]);
	indexNo -= 1;

	if(CMD_Compare(pData->pArgv[0], "image"))
	{
		if(indexNo >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;

		FM_IndexRead(FM_FILE_IMAGE, indexNo, &fmIndexImage);
		
		if(fmIndexImage.fState == 0)					return result;

		fmIndexImage.fState = 0x8001;

		FM_IndexWrite(FM_FILE_IMAGE, indexNo, &fmIndexImage);

		FM_IndexSave(FM_FILE_IMAGE);
	}
	else						return result;

	result = 1;

	return result;
}

u8 Cmd_FileDisable(void *pVoid)
{
	u8	result = 0;
	u16	indexNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	indexNo = CMD_StrToUL(pData->pArgv[1]);
	indexNo -= 1;

	if(CMD_Compare(pData->pArgv[0], "image"))
	{
		if(indexNo >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;

		FM_IndexRead(FM_FILE_IMAGE, indexNo, &fmIndexImage);
		
		if(fmIndexImage.fState == 0)					return result;

		fmIndexImage.fState = 0x0001;

		FM_IndexWrite(FM_FILE_IMAGE, indexNo, &fmIndexImage);

		FM_IndexSave(FM_FILE_IMAGE);
	}
	else						return result;

	result = 1;

	return result;
}

u8 Cmd_FileAdd(void *pVoid)
{
	u8	result = 0, rtn;
	u16	indexNo;
	u32	length;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 3)		return result;

	if(CMD_Compare(pData->pArgv[0], "key"))
	{
		CMD_Printf(";KEY");

		indexNo = CMD_StrToUL(pData->pArgv[1]);

		indexNo -= 1;

		if(indexNo >= FM_INDEX_KEY_LIST_MAX_COUNT)	return result;

		memset(filePath, NULL, sizeof(filePath));

		length = strlen((char*)pData->pArgv[2]);

		if(length > sizeof(filePath))				return result;

		memcpy(filePath, pData->pArgv[2], length);

		rtn = FatFS_FileCheck(filePath);

		if(rtn != FR_OK)							return result;

		memset(&fmIndexKey, NULL, sizeof(fmIndexKey));

		fmIndexKey.fState		= 0x0001;
		memcpy(fmIndexKey.fName, filePath, sizeof(filePath));
		fmIndexKey.fSize		= 0;

		FM_IndexWrite(FM_FILE_KEY, indexNo, &fmIndexKey);

		FM_IndexSave(FM_FILE_KEY);
	}
	else if(CMD_Compare(pData->pArgv[0], "image"))
	{
		CMD_Printf(";IMAGE");

		indexNo = CMD_StrToUL(pData->pArgv[1]);

		indexNo -= 1;

		if(indexNo >= FM_INDEX_IMAGE_LIST_MAX_COUNT)	return result;

		memset(filePath, NULL, sizeof(filePath));

		length = strlen((char*)pData->pArgv[2]);

		if(length > sizeof(filePath))					return result;

		memcpy(filePath, pData->pArgv[2], length);

		rtn = FatFS_FileCheck(filePath);

		if(rtn != FR_OK)								return result;

		memset(&fmIndexImage, NULL, sizeof(fmIndexImage));

		fmIndexImage.fState		= 0x8001;
		memcpy(fmIndexImage.fName, filePath, sizeof(filePath));
		fmIndexImage.fSize		= 0;
		fmIndexImage.hPixel		= 0;
		fmIndexImage.vLine		= 0;

		FM_IndexWrite(FM_FILE_IMAGE, indexNo, &fmIndexImage);

		FM_IndexSave(FM_FILE_IMAGE);
	}
	else						return result;

	result = 1;

	return result;
}

u8 Cmd_FileScan(void *pVoid)
{
	u8	result = 0;
	u32	fCnt;
	FRESULT	rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	memset(filePath, NULL, sizeof(filePath));

	if(CMD_Compare(pData->pArgv[0], "key"))
	{
		CMD_Printf(";KEY");
		
		sprintf((char*)filePath, "/system/key");
	}
	else if(CMD_Compare(pData->pArgv[0], "imagenand"))
	{
		CMD_Printf(";IMAGE");
		
		sprintf((char*)filePath, "0:/image");
	}
	else if(CMD_Compare(pData->pArgv[0], "imagesd"))
	{
		CMD_Printf(";IMAGE");
		
		sprintf((char*)filePath, "1:/image");
	}
	else				return result;

	rtn = FatFS_DirOpen(&cmdDir, filePath);

	if(rtn != FR_OK)	return result;

	fCnt = 0;

	do{
		rtn = FatFS_DirRead(&cmdDir, &cmdFileInfo);

		if((rtn != FR_OK)  || (cmdFileInfo.fname[0] == 0))
		{
			break;
		}

		if(cmdFileInfo.fattrib & AM_DIR)
		{
			continue;
		}

		fCnt++;

		CMD_Printf(";%s/%s", filePath, cmdFileInfo.fname);

		if(fCnt >= 300)	break;
	}while(1);

	if(fCnt == 0)		CMD_Printf(";NO_FILE");

	result = 1;

	return result;
}

u8 Cmd_FileScanDir(void *pVoid)
{
	u8	result = 0, rtn;
	u16	cnt, folderCnt, fileCnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	for(cnt = 0; cnt < FOLDER_MAX_COUNT; cnt++)
	{
		folderList[cnt] = (u8p)(MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS + (cnt * 256));
	}

	for(cnt = 0; cnt < FILE_MAX_COUNT; cnt++)
	{
		fileList[cnt] = (u8p)(MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS + (cnt * 256) + 0x1000);
	}

	rtn = FatFS_ScanPath(pData->pArgv[0], &folderCnt, folderList, &fileCnt, fileList);

	if(rtn == 0)	return result;

	for(cnt = 0; cnt < folderCnt; cnt++)
	{
		CMD_Printf(";%s", folderList[cnt]);
	}

	for(cnt = 0; cnt < fileCnt; cnt++)
	{
		CMD_Printf(";%s", fileList[cnt]);
	}

	result = 1;

	return result;
}

u8 Cmd_FileScanCount(void *pVoid)
{
	u8	result = 0;
	u32	folderCnt, fileCnt;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	folderCnt	= CMD_StrToUL(pData->pArgv[0]);
	fileCnt		= CMD_StrToUL(pData->pArgv[1]);

	if(folderCnt > FOLDER_MAX_COUNT)	return result;
	if(fileCnt > FILE_MAX_COUNT)		return result;
	
	FatFS_SetScanCount(folderCnt, fileCnt);

	result = 1;

	return result;
}

u8 Cmd_FileGetFree(void *pVoid)
{
	u8	result = 0, rtn;
	u8	driveNo;
	u64	totalSpace, freeSpace;
	double tSize, fSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	driveNo = CMD_StrToUL(pData->pArgv[0]);

	rtn = FatFS_GetFree(driveNo, &totalSpace, &freeSpace);

	if(rtn == 0)				return result;

	tSize = (double)totalSpace;		// Byte
	tSize /= 1024;					// KByte
	tSize /= 1024;					// MByte

	fSize = (double)freeSpace;		// Byte
	fSize /= 1024;					// KByte
	fSize /= 1024;					// MByte

	CMD_Printf(";Total %.2fMB;Free %.2fMB", tSize, fSize);

	result = 1;

	return result;
}

u8 Cmd_FileFormat(void *pVoid)
{
	u8	result = 0, rtn;
	u8	driveNo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	driveNo = CMD_StrToUL(pData->pArgv[0]);

	if(driveNo != 0)			return result;

	rtn = FatFS_Format(driveNo);

	if(rtn != FR_OK)			return result;

	result = 1;

	return result;
}

u8 Cmd_FileCopy(void *pVoid)
{
	u8	result = 0, rtn;
	u8	*pFilePath, *pFileName;
	u8 	*pFileData;
	u32	fileSize;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	pFileData = (u8*)MEMORY_MAP_SDRAM_FILE_DUMP_ADRS;

	pFilePath = pData->pArgv[0];
	pFileName = pData->pArgv[1];

//		FILE LOAD

	memset(filePath, NULL, sizeof(filePath));

	strcpy((char*)filePath, (char*)pFilePath);

	rtn = FatFS_FileOpen(&cmdLoadFile, filePath);

	if(rtn != FR_OK)	return result;

	fileSize = FatFS_FileSize(&cmdLoadFile);

	rtn = FatFS_FilePtrWrite(&cmdLoadFile, 0);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileReadAll(&cmdLoadFile, (u32)pFileData);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileClose(&cmdLoadFile);

	if(rtn != FR_OK)	return result;



//		FILE SAVE

	memset(filePath, NULL, sizeof(filePath));

	strcpy((char*)filePath, (char*)pFileName);

	rtn = FatFS_FileMake(&cmdSaveFile, pFileName);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileOpen(&cmdSaveFile, pFileName);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FilePtrWrite(&cmdSaveFile, 0);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileWrite(&cmdSaveFile, pFileData, fileSize);

	if(rtn != FR_OK)	return result;

	rtn = FatFS_FileClose(&cmdSaveFile);

	if(rtn != FR_OK)	return result;

	result = 1;

	return result;
}

