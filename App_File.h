#ifndef	_APP_FILE_H
#define	_APP_FILE_H

#include	"E7602_PG_Application.h"

#define	FOLDER_MAX_COUNT		10
#define	FILE_MAX_COUNT			256

u8 FatFS_DriverInit();
void FatFS_SetScanCount(u32 folderCnt, u32 fileCnt);
FRESULT FatFS_Mount(FATFS *fs, u8 driveNo);
FRESULT FatFS_Unmount(u8 driveNo);
FRESULT FatFS_Format(u8 driveNo);
u8 FatFS_DriveMount(u8 data);
FRESULT FatFS_FileOpen(FIL *pFile, u8 *pFileName);
FRESULT FatFS_FileCheck(u8 *pFileName);
FRESULT FatFS_FileMake(FIL *pFile, u8 *pFileName);
FRESULT FatFS_FileClose(FIL *pFile);
FRESULT FatFS_FileExpand(FIL *pFile, u32 size);
FRESULT FatFS_FilePtrWrite(FIL *pFile, u32 adrs);
u32 FatFS_FilePtrRead(FIL *pFile);
u32 FatFS_FileSize(FIL *pFile);
FRESULT FatFS_FileTruncate(FIL *pFile);
FRESULT FatFS_FileRead(FIL *pFile, u32 dst, u32 size);
FRESULT FatFS_FileReadAll(FIL *pFile, u32 dst);
FRESULT FatFS_FileWrite(FIL *pFile, u8 *pData, u32 size);
FRESULT FatFS_Delete(u8 *pData);
FRESULT FatFS_DirOpen(DIR *pDir, u8 *pPath);
FRESULT FatFS_DirClose(DIR *pDir);
FRESULT FatFS_DirMake(u8 *pPath);
FRESULT FatFS_DirRead(DIR *pDir, FILINFO *pFInfo);
u8 FatFS_ScanPath(u8 *pPath, u16 *pFolderCnt, u8 **pFolderList, u16 *pFileCnt, u8 **pFileList);
u8 FatFS_GetFree(u8 driveNo, u64p pTotal, u64p pFree);
void FatFS_MsdChecker();
u8 FatFS_ProcessMicroSD();

#endif	// _APP_FILE_H
