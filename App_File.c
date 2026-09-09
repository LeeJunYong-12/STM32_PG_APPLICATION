#include	"App_File.h"

FATFS fatFsNand, fatFsMicroSD;

u8	fileDriveCnt;
u32	fileScanMaxCnt;

u8	fatfsMsdPinStat;
u16	fatfsMsdTcnt;

static	FIL	fatFsFile;
static	DIR	fatFsDir;
static	FILINFO	fatFsFileInfo;

u8 FatFS_DriverInit()
{
	fileDriveCnt = 2;
	fileScanMaxCnt = 0x0005000f;

	memset(uNandPath, 0, sizeof(uNandPath));
	FATFS_LinkDriver(&NAND_Driver, uNandPath);

	memset(uSdPath, 0, sizeof(uSdPath));
	FATFS_LinkDriver(&SD_Driver, uSdPath);

	return 1;
}

void FatFS_SetScanCount(u32 folderCnt, u32 fileCnt)
{
	if(folderCnt > 10)	folderCnt	= FOLDER_MAX_COUNT;
	if(fileCnt > 256)	fileCnt		= FILE_MAX_COUNT;

	fileScanMaxCnt = folderCnt;
	fileScanMaxCnt <<= 16;
	fileScanMaxCnt += fileCnt;
}

FRESULT FatFS_Mount(FATFS *fs, u8 driveNo)
{
	char path[16];

	if(driveNo >= fileDriveCnt)	return FR_DISK_ERR;

	memset(path, NULL, sizeof(path));

	sprintf(path, "%d:/\0", driveNo);

	return f_mount(fs, path, 1);
}

FRESULT FatFS_Unmount(u8 driveNo)
{
	char path[16];

	if(driveNo >= fileDriveCnt)	return FR_DISK_ERR;

	memset(path, NULL, sizeof(path));

	sprintf(path, "%d:/\0", driveNo);
	
	return f_mount(NULL, path, 0);
}

extern	u32 MEMCTRL_NAND_GetSectorSize();
extern	u32 MEMCTRL_NAND_GetBlockSize();

FRESULT FatFS_Format(u8 driveNo)
{
	FRESULT	rtn;
	u8p pBuffer;
	char path[16];

	pBuffer = (u8p)MEMORY_MAP_SDRAM_FLASH_LOAD_ADRS;

	if(driveNo >= fileDriveCnt)	return FR_DISK_ERR;

	memset(path, NULL, sizeof(path));

	sprintf(path, "%d:/\0", driveNo);

	memset(pBuffer, NULL, _MAX_SS);

	if(driveNo == 0)
	{
		u32	au;

		au = MEMCTRL_NAND_GetSectorSize() * MEMCTRL_NAND_GetBlockSize();
		
		rtn = f_mkfs((char*)path, FM_FAT, au, pBuffer, _MAX_SS);
	}
	else
	{
		rtn = f_mkfs((char*)path, FM_FAT32, 0, pBuffer, _MAX_SS);
	}

	return rtn;
}

u8 FatFS_DriveMount(u8 data)
{
	u8	result = 0;
	FRESULT nandRtn, uSdRtn;

	if((data & 0x01) == 0x01)
	{
		sysError.nandFlash = 0;
		
		nandRtn = FatFS_Mount(&fatFsNand, 0);

		if(nandRtn == FR_NO_FILESYSTEM)
		{
			nandRtn = FatFS_Format(0);

			if(nandRtn != FR_OK)
			{
				sysError.nandFlash = 1;
			}

			nandRtn = FatFS_Mount(&fatFsNand, 0);
		}

		if(nandRtn != FR_OK)
		{
			sysError.nandFlash = 1;
		}
	}

	if((data & 0x02) == 0x02)
	{
		sysError.microSD = 0;
		
		if(MSD_Detect() != 0)
		{
			uSdRtn = FatFS_Mount(&fatFsMicroSD, 1);

			if(uSdRtn != FR_OK)
			{
				sysError.microSD = 1;
			}
		}
		else
		{
			sysError.microSD = 1;
		}
	}

	result = 1;

	return result;
}

FRESULT FatFS_FileOpen(FIL *pFile, u8 *pFileName)
{
	BYTE mode;

	mode = FA_READ | FA_WRITE;

	memset((u8*)pFile, NULL, sizeof(FIL));
	
	return f_open(pFile, (TCHAR*)pFileName, mode);
}

FRESULT FatFS_FileCheck(u8 *pFileName)
{
	FRESULT rtn;
	BYTE mode;

	mode = FA_READ;
	
	rtn = f_open(&fatFsFile, (TCHAR*)pFileName, mode);

	if(rtn == FR_OK)
	{
		f_close(&fatFsFile);
	}

	return rtn;
}

FRESULT FatFS_FileMake(FIL *pFile, u8 *pFileName)
{
	FRESULT rtn;
	BYTE mode;

	mode = FA_READ;

	memset((u8*)pFile, NULL, sizeof(FIL));

	rtn = f_open(pFile, (TCHAR*)pFileName, mode);

	if(rtn == FR_OK)
	{
		f_close(pFile);

		return rtn;
	}
	else if((rtn != FR_NO_FILE) && (rtn == FR_NO_PATH))
	{
		f_close(pFile);
		
		return rtn;
	}

	memset((u8*)pFile, NULL, sizeof(FIL));

	mode = FA_READ | FA_WRITE | FA_CREATE_NEW | FA_CREATE_ALWAYS;
	
	rtn =  f_open(pFile, (TCHAR*)pFileName, mode);

	f_close(pFile);

	return rtn;
}

FRESULT FatFS_FileClose(FIL *pFile)
{
	return f_close(pFile);
}

FRESULT FatFS_FileExpand(FIL *pFile, u32 size)
{
	return f_expand(pFile, (FSIZE_t)size, 1);
}

FRESULT FatFS_FilePtrWrite(FIL *pFile, u32 adrs)
{
	return f_lseek(pFile, (FSIZE_t)adrs);
}

u32 FatFS_FilePtrRead(FIL *pFile)
{
	return f_tell(pFile);
}

u32 FatFS_FileSize(FIL *pFile)
{
	return f_size(pFile);
}

FRESULT FatFS_FileTruncate(FIL *pFile)
{
	return f_truncate(pFile);
}

FRESULT FatFS_FileRead(FIL *pFile, u32 dst, u32 size)
{
	u8p	pData;
	UINT br = 0;

	pData = (u8p)dst;
	
	return f_read(pFile, pData, size, &br);
}

FRESULT FatFS_FileReadAll(FIL *pFile, u32 dst)
{
	FRESULT	rtn;
	u8p	pData;
	UINT size = 0, br = 0;

	pData = (u8p)dst;

	rtn = f_lseek(pFile, 0);

	if(rtn != FR_OK)		return rtn;

	size = f_size(pFile);
	
	return f_read(pFile, pData, size, &br);
}


FRESULT FatFS_FileWrite(FIL *pFile, u8 *pData, u32 size)
{
	u32	bw;

	return f_write(pFile, pData, size, (UINT*)&bw);
}

FRESULT FatFS_Delete(u8 *pData)
{
	return f_unlink((TCHAR*)pData);
}

FRESULT FatFS_DirOpen(DIR *pDir, u8 *pPath)
{
	memset((u8*)pDir, NULL, sizeof(DIR));

	return f_opendir(pDir, (TCHAR*)pPath);
}

FRESULT FatFS_DirClose(DIR *pDir)
{
	return f_closedir(pDir);
}

FRESULT FatFS_DirMake(u8 *pPath)
{
	FRESULT	rtn;

	memset((u8*)&fatFsDir, NULL, sizeof(DIR));

	rtn = f_opendir(&fatFsDir, (TCHAR*)pPath);

	if(rtn == FR_OK)
	{
		f_closedir(&fatFsDir);
		return rtn;
	}
	else if(rtn != FR_NO_PATH)
	{
		f_closedir(&fatFsDir);
		return rtn;
	}

	return f_mkdir((TCHAR*)pPath);
}

FRESULT FatFS_DirRead(DIR *pDir, FILINFO *pFInfo)
{
	return f_readdir(pDir, pFInfo);
}

u8 FatFS_ScanPath(u8 *pPath, u16 *pFolderCnt, u8 **pFolderList, u16 *pFileCnt, u8 **pFileList)
{
	u8	result = 0;
	FRESULT rtn;
	u16 folderMaxCnt, fileMaxCnt;

	folderMaxCnt	= (u16)(fileScanMaxCnt >> 16);
	fileMaxCnt		= (u16)fileScanMaxCnt;

	*pFileCnt	= 0;
	*pFolderCnt	= 0;

	rtn = FatFS_DirOpen(&fatFsDir, pPath);

	if(rtn != FR_OK)	return result;

	do{
		rtn = f_readdir(&fatFsDir, &fatFsFileInfo);

		if(rtn != FR_OK)												break;

		result = 1;

		if(fatFsFileInfo.fname[0] == 0)									break;

		if(fatFsFileInfo.fattrib & AM_DIR)
		{
			if(*pFolderCnt < folderMaxCnt)
			{
				strcpy((char*)pFolderList[*pFolderCnt], fatFsFileInfo.fname);
				(*pFolderCnt)++;
			}
		}
		else
		{
			if(*pFileCnt < fileMaxCnt)
			{
				strcpy((char*)pFileList[*pFileCnt], fatFsFileInfo.fname);
				(*pFileCnt)++;
			}
		}

		if((*pFolderCnt >= folderMaxCnt) && (*pFileCnt >= fileMaxCnt))	break;
	}while(1);

	FatFS_DirClose(&fatFsDir);

	return result;
}

u8 FatFS_GetFree(u8 driveNo, u64p pTotal, u64p pFree)
{
	u8	result = 0, rtn;
	u8	path[16];
	u32	freeCluster;
	u64	totalSpace, freeSpace;
	FATFS *fs;

	if(driveNo >= fileDriveCnt)	return result;

	memset(path, NULL, sizeof(path));

	sprintf((char*)path, "%d:/\0", driveNo);

	rtn = f_getfree((TCHAR*)path, &freeCluster, &fs);

	if(rtn != FR_OK)			return result;

	totalSpace = (u64)(fs->n_fatent - 2) * (u64)fs->csize * (u64)fs->ssize;

	freeSpace = (u64)freeCluster * (u64)fs->csize * (u64)fs->ssize;

	*pTotal = totalSpace;

	*pFree = freeSpace;

	result = 1;

	return result;
}

u8 FatFS_DetectMicroSD()
{
	u8	result = 0;

	result = MSD_Detect();

	return result;
}

u8 FatFS_DisableMicroSD()
{
	u8	result = 0;
#if 1 // yeom_20210120 [start]
	result = BSP_SD_DeInit();
#else
	result = MSD_DeInit();
#endif // yeom_20210120 [end]	

	return result;
}

void FatFS_MsdChecker()
{
	static u8 pinStat = 0xff;

	fatfsMsdPinStat = FatFS_DetectMicroSD();

	if(fatfsMsdTcnt < 0x8000)	fatfsMsdTcnt++;

	if(pinStat != fatfsMsdPinStat)
	{
		fatfsMsdTcnt = 0;
	}

	pinStat = fatfsMsdPinStat;
}

u8 FatFS_ProcessMicroSD()
{
	static	u8	msdStat = 0xff;
	u8	result = 0;
	FRESULT	rtn;

	if(fatfsMsdTcnt < 5)		return result;

	if(fatfsMsdTcnt >= 0x8000)	return result;

	if(fatfsMsdPinStat == 1)
	{
		if(msdStat == 1)		return result;

		rtn = FatFS_Mount(&fatFsMicroSD, 1);

		if(rtn == FR_OK)
		{
			result = 1;
			msdStat = 1;
			sysError.microSD = 0;
		}
	}
	else
	{
		if(msdStat == 0)		return result;

		FatFS_Unmount(1);

		FatFS_DisableMicroSD();

		FATFS_LinkDriverInit(1);

		memset(&fatFsMicroSD, NULL, sizeof(fatFsMicroSD));

		result = 1;
		msdStat = 0;
		sysError.microSD = 1;
	}

	fatfsMsdTcnt = 0x8000;

	return result;
}

