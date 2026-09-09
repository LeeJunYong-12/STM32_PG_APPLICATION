#ifndef	_APP_SCRIPT_H
#define	_APP_SCRIPT_H

#include	"E7602_PG_Application.h"


#define	SCRIPT_DELIMITER			"\r\n"

typedef	struct{
	u32	flag;
	u32	state;
	u32	slotNo;
	u32	scriptSize;
	u8	*pScriptData;
	u8	*pRemainData;
}scriptInfo_t;

typedef	struct{
	u32	flag;
	u32	loadMemory;
	u32	scriptSize;
	u8	*pScriptData;
	u8	scriptFileName[256];
}scriptData_t;

u8 Script_SetFlag(u8 position);
u32 Script_GetFlag();
u32  Script_ClearFlag(u8 position);
u8 Script_FileLoad(u16 fileNo);
#if 1 // yeom_20200803 [start]
u8 Script_Action(u32 srcAdrs);
void Script_Execute(u8 position);
#else
u8 Script_Action(u8 *pData);
u8 Script_Execute(u8 position);
#endif // yeom_20200803 [end]

#if 0// yeom_20200823
u8 Script_Init();
#endif// yeom_20200823
#if 0// yeom_20200803
u8 Script_Enable(u32 slotNo);
#endif// yeom_20200803
u8 Script_Process();

#endif	// _APP_SCRIPT_H
