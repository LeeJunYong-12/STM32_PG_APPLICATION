#ifndef	_CMDSET_FUNCTION_H
#define	_CMDSET_FUNCTION_H

#include	"CommandSet.h"

#define	COMMAND_DELIMITER_1		" .\t\r\n"
#define	COMMAND_DELIMITER_2		" \t\r\n"
#define	COMMAND_DELIMITER_3		"\""

u32 CMD_GetCommunicationType();
u8 CMD_RunningState(u8 state);
u8 CMD_ErrorState(u8 state);
u8 CMD_DataSplit(u8 *pString, u8 *pDelimiter, u8 **pArgv, u32 argvMaxCount, u32 *pArgc);
u8 CMD_Compare(u8 *pS1, const char *pS2);
u8 CMD_StrLwr(u8 *pData);
u8 CMD_StrUpr(u8 *pData);
u32	CMD_StrToUL(u8 *pData);
double CMD_AToF(u8 *pData);
u8 CMD_IpDataParsing(u8 *pString, u8 *pIP);
u8 CMD_TxData(u8 *pData, u32 length);
u8 CMD_Printf(const char *pData, ...);
u8 CMD_TransmitData();
u8 CMD_Execute();
u8 Cmd_Help(void *pVoid);

#endif	//	!_CMDSET_FUNCTION_H