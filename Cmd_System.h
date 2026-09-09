#ifndef	_CMD_SYSTEM_H
#define	_CMD_SYSTEM_H

#include	"E7602_PG_Application.h"

extern	const	commandList_t	cmdList_System[];

u8 Cmd_SystemVersion(void *pVoid);
u8 Cmd_SystemBuild(void *pVoid);
u8 Cmd_SystemType(void *pVoid);
u8 Cmd_SystemReset(void *pVoid);
u8 Cmd_SystemConfigClear(void *pVoid);
u8 Cmd_SystemConfigLoad(void *pVoid);
u8 Cmd_SystemConfigSave(void *pVoid);
u8 Cmd_SystemSetConfig(void *pVoid);
u8 Cmd_SystemGetConfig(void *pVoid);
u8 Cmd_SystemSetNetconfig(void *pVoid);
u8 Cmd_SystemGetNetconfig(void *pVoid);
u8 Cmd_SystemFanEnable(void *pVoid);
u8 Cmd_SystemFanDisable(void *pVoid);

u8 Cmd_SystemPGBoot(void *pVoid);
u8 Cmd_SystemPowerBoot(void *pVoid);
u8 Cmd_SystemPowerDown(void *pVoid);
u8 Cmd_SystemPowerBootStart(void *pVoid);
u8 Cmd_SystemPowerBootErase(void *pVoid);
u8 Cmd_SystemPowerBootCheck(void *pVoid);
u8 Cmd_SystemEEPROMClear(void *pVoid);
u8 Cmd_SystemFPGADown(void *pVoid);
u8 Cmd_SystemDDRDown(void *pVoid);
u8 Cmd_Systemflashcheck1(void *pVoid);
u8 Cmd_Systemflashcheck2(void *pVoid);
u8 Cmd_SystemPGflashcheck(void *pVoid);
#endif	// _CMD_SYSTEM_H