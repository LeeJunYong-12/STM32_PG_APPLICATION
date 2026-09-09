#include	"Cmd_Board.h"

static u8 Cmd_BoardChannelInit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)
	{
		CMD_Printf("too many argument");
		return result;
	}

	result = Board_ChannelInit();

	return result;
}

static u8 Cmd_BoardChannelSelect(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	cnt, mode[BOARD_CHANNEL_MAX_COUNT];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)
	{
		CMD_Printf("not enough argument");
		return result;
	}
	if(pData->argc >= BOARD_CHANNEL_MAX_COUNT)
	{
		CMD_Printf("over BOARD_CHANNEL_MAX_COUNT");
		return result;
	}

	memset(mode, BOARD_USE_CHANNEL_FALSE, sizeof(mode));

	for(cnt = 0; cnt < pData->argc; cnt++)
	{
		if(CMD_Compare(pData->pArgv[cnt], "ch1"))
		{
			if(boardChannel[0].hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
			{
				CMD_Printf("hwConstruct is BOARD_HW_CONSTRUCT_FALSE");
				return result;
			}

			mode[0] = BOARD_USE_CHANNEL_TRUE;
		}
		else if(CMD_Compare(pData->pArgv[cnt], "ch2"))
		{
			if(boardChannel[1].hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
			{
				CMD_Printf("hwConstruct is BOARD_HW_CONSTRUCT_FALSE");
				return result;
			}

			mode[1] = BOARD_USE_CHANNEL_TRUE;
		}
		else if(CMD_Compare(pData->pArgv[cnt], "ch3"))
		{
			if(boardChannel[2].hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
			{
				CMD_Printf("hwConstruct is BOARD_HW_CONSTRUCT_FALSE");
				return result;
			}

			mode[2] = BOARD_USE_CHANNEL_TRUE;
		}
		else if(CMD_Compare(pData->pArgv[cnt], "ch4"))
		{
			if(boardChannel[3].hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
			{
				CMD_Printf("hwConstruct is BOARD_HW_CONSTRUCT_FALSE");
				return result;
			}

			mode[3] = BOARD_USE_CHANNEL_TRUE;
		}
		else
		{
			CMD_Printf("not found argument");
			return result;
		}
	}

	for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
	{
		if(boardChannel[cnt].hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;

		rtn = Board_ChannelSelect(cnt, mode[cnt]);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)			result = 1;

	return result;
}

static u8 Cmd_BoardChannelEnable(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)
	{
		CMD_Printf("not enough or too many argument");
		return result;
	}

	if(CMD_Compare(pData->pArgv[0], "ch1"))			channel = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	channel = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	channel = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	channel = 3;
	else
	{
		CMD_Printf("not found argument");
		return result;
	}

	result = Board_ChannelSelect(channel, BOARD_USE_CHANNEL_TRUE);

	return result;
}

static u8 Cmd_BoardChannelDisable(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)
	{
		CMD_Printf("not enough or too many argument");
		return result;
	}

	if(CMD_Compare(pData->pArgv[0], "ch1"))			channel = 0;
	else if(CMD_Compare(pData->pArgv[0], "ch2"))	channel = 1;
	else if(CMD_Compare(pData->pArgv[0], "ch3"))	channel = 2;
	else if(CMD_Compare(pData->pArgv[0], "ch4"))	channel = 3;
	else
	{
		CMD_Printf("not found argument");
		return result;
	}

	result = Board_ChannelSelect(channel, BOARD_USE_CHANNEL_FALSE);

	return result;
}

const	commandList_t	cmdList_Board[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note},
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},
												{	"channel.init",			Cmd_BoardChannelInit,		NULL,		NULL,		";CHANNEL.INIT",		NULL},
												{	"channel.select",		Cmd_BoardChannelSelect,		NULL,		NULL,		";CHANNEL.SELECT",		NULL},
												{	"channel.enable",		Cmd_BoardChannelEnable,		NULL,		NULL,		";CHANNEL.ENABLE",		NULL},
												{	"channel.disable",		Cmd_BoardChannelDisable,	NULL,		NULL,		";CHANNEL.DISABLE",		NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};

