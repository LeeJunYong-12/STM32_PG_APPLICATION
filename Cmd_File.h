#ifndef	_CMD_FILE_H
#define	_CMD_FILE_H

#include	"E7602_PG_Application.h"

extern	const	commandList_t	cmdList_FILE[];

u8 Cmd_FileCheck(void *pVoid);
u8 Cmd_FileCount(void *pVoid);
u8 Cmd_FileInfo(void *pVoid);
u8 Cmd_FileSave(void *pVoid);
u8 Cmd_FileSaveOther(void *pVoid);
u8 Cmd_FileSavePath(void *pVoid);
u8 Cmd_FileDelete(void *pVoid);
u8 Cmd_FileListOut(void *pVoid);
u8 Cmd_FileInsert(void *pVoid);
u8 Cmd_FileArrange(void *pVoid);
u8 Cmd_FileEnable(void *pVoid);
u8 Cmd_FileDisable(void *pVoid);
u8 Cmd_FileAdd(void *pVoid);
u8 Cmd_FileScan(void *pVoid);
u8 Cmd_FileScanDir(void *pVoid);
u8 Cmd_FileScanCount(void *pVoid);
u8 Cmd_FileGetFree(void *pVoid);
u8 Cmd_FileFormat(void *pVoid);
u8 Cmd_FileCopy(void *pVoid);

#endif	// _CMD_FILE_H