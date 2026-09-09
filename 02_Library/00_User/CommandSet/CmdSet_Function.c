#include	"CmdSet_Function.h"

static	u8	*pCmdArgv[CMDSET_MAX_COUNT];
static	u32	cmdArgc;

extern	cmdSetHandle_t	cmdSetHandle;

static u8 CMD_Parsing(u8 *pRcvData, u8 **pArgv, u32 *pArgc)
{
	u8	result = 0;
	u8	*pToken, *pData, *pLastData;
	u32	argc = 0, length, totalLength;

	totalLength = strlen((char*)pRcvData);

	pLastData = &pRcvData[totalLength];

	pToken = (u8*)strtok((char*)pRcvData, COMMAND_DELIMITER_1);

	do{
		if(pToken == NULL)						break;

		pArgv[argc] = pToken;
		argc += 1;

		length = strlen((char*)pToken);

		length += 1;

		do{
			if((pToken[length] == ' ') || (pToken[length] == '\t'))
			{
				pToken[length] = NULL;

				length += 1;
			}
			else
			{
				break;
			}

			if(&pToken[length] >= pLastData)	break;
		}while(1);

		pData = &pToken[length];

		if(pToken[length] == '"')
		{
			pToken = (u8*)strtok((char*)pData, COMMAND_DELIMITER_3);
		}
		else
		{
			pToken = (u8*)strtok((char*)pData, COMMAND_DELIMITER_2);
		}

		if(argc > cmdSetHandle.parseMaxCount)	break;
	}while(1);

	if(argc != 0)
	{
		*pArgc	= argc;
		result	= 1;
	}

	return result;
}

u8 CMD_Analysis()
{
	u8	result = 0, rtn;
	u8	*pComment;

	memset(pCmdArgv, NULL, sizeof(pCmdArgv));

	cmdArgc = 0;

	pComment = (u8*)strstr((char*)cmdSetHandle.pRxData, "#");
	if(pComment != NULL)
	{
		memset(pComment, NULL, 4);
	}

	rtn = CMD_Parsing(cmdSetHandle.pRxData, pCmdArgv, &cmdArgc);
	if(rtn != 0)			result = 1;
	
	return result;
}

u32 CMD_GetCommunicationType()
{
	return cmdSetHandle.rxSrcType;
}

u8 CMD_RunningState(u8 state)
{
	u8	result = 0;

	if(cmdSetHandle.RunningState == NULL)		return result;

	result = cmdSetHandle.RunningState(state);

	return result;
}

u8 CMD_ErrorState(u8 state)
{
	u8	result = 0;

	if(cmdSetHandle.ErrorState == NULL)			return result;

	result = cmdSetHandle.ErrorState(state);

	return result;
}

u8 CMD_DataSplit(u8 *pString, u8 *pDelimiter, u8 **pArgv, u32 argvMaxCount, u32 *pArgc)
{
	u8	result = 0;
	u8	*pToken;
	u32	argc = 0;

	pToken = (u8*)strtok((char*)pString, (char*)pDelimiter);

	do{
		if(pToken == NULL)			break;

		pArgv[argc] = pToken;

		argc += 1;

		if(argc > argvMaxCount)		break;

		pToken = (u8*)strtok(NULL, (char*)pDelimiter);
	}while(1);

	if(argc != 0)					result = 1;

	*pArgc = argc;

	return result;
}

u8 CMD_Compare(u8 *pS1, const char *pS2)
{
	u8	result = 0;

	if(strcasecmp((char*)pS1, pS2) == NULL)		result = 1;

	return result;
}

u8 CMD_StrLwr(u8 *pData)
{
	u8	result = 0;
	u32	cnt = 0;

	if(pData == NULL)			return result;

	do{
		if(pData[cnt] == NULL)	break;

		if(pData[cnt] < 'A')
		{
			cnt += 1;
			continue;
		}

		if(pData[cnt] > 'Z')
		{
			cnt += 1;
			continue;
		}

		pData[cnt] += 0x20;

		cnt += 1;
	}while(1);

	if(cnt != 0)				result = 1;

	return result;
}

u8 CMD_StrUpr(u8 *pData)
{
	u8	result = 0;
	u32	cnt = 0;

	if(pData == NULL)			return result;

	do{
		if(pData[cnt] == NULL)	break;

		if(pData[cnt] < 'a')
		{
			cnt += 1;
			continue;
		}

		if(pData[cnt] > 'z')
		{
			cnt += 1;
			continue;
		}

		pData[cnt] -= 0x20;

		cnt += 1;
	}while(1);

	if(cnt != 0)				result = 1;

	return result;
}

u32	CMD_StrToUL(u8 *pData)
{
	return strtoul((char*)pData, NULL, NULL);
}

double CMD_AToF(u8 *pData)
{
	return atof((char*)pData);
}

u8 CMD_IpDataParsing(u8 *pString, u8 *pIP)
{
	u8	result = 0, rtn;
	u8	ipString[64], ipDelimiter[4];
	u8	*pArgv[8];
	u32	size, argvCnt, argc;

	size = strlen((char*)pString);

	if(size > sizeof(ipString))		return result;

	memset(ipString, NULL, sizeof(ipString));

	memset(ipDelimiter, NULL, sizeof(ipDelimiter));

	memcpy(ipString, pString, size);

	ipDelimiter[0] = '.';

	argvCnt	= sizeof(pArgv);
	argvCnt	/= sizeof(u8*);

	rtn = CMD_DataSplit(ipString, ipDelimiter, pArgv, argvCnt, &argc);
	if(rtn == 0)					return result;

	if(argc != 4)					return result;

	pIP[0] = CMD_StrToUL(pArgv[0]);
	pIP[1] = CMD_StrToUL(pArgv[1]);
	pIP[2] = CMD_StrToUL(pArgv[2]);
	pIP[3] = CMD_StrToUL(pArgv[3]);

	result = 1;

	return result;
}

u8 CMD_TxData(u8 *pData, u32 length)
{
	u8	result = 0;
	u8	*pwData;

	if((cmdSetHandle.txDataCnt + length) > cmdSetHandle.txDataSize)
	{
		return result;
	}

	pwData = &cmdSetHandle.pTxData[cmdSetHandle.txDataCnt];

	memcpy(pwData, pData, length);

	cmdSetHandle.txDataCnt += length;

	result = 1;

	return result;
}

u8 CMD_Printf(const char *pData, ...)
{
	u8	result = 0;
	u8	*pwData;
	u32	length;
	va_list	ap;

	va_start(ap, pData);

	if((cmdSetHandle.txDataCnt + 32) > cmdSetHandle.txDataSize)
	{
		return result;
	}

	pwData = &cmdSetHandle.pTxData[cmdSetHandle.txDataCnt];

	length = vsprintf((char*)pwData, pData, ap);
	cmdSetHandle.txDataCnt += length;

	cmdSetHandle.pTxData[cmdSetHandle.txDataCnt] = 0;

	result = 1;

	return result;
}

u8 CMD_TransmitData()
{
	u8	result = 0, rtn;
	u32	position;

	position = cmdSetHandle.txSrcType;

	if(position >= CMDSET_COMM_SRC_COUNT)				return result;

	if(cmdSetHandle.TransmitData[position] == NULL)		return result;

	rtn = cmdSetHandle.TransmitData[position](cmdSetHandle.pTxData, cmdSetHandle.txDataCnt);
	if(rtn != 0)		result = 1;

	return result;
}

u8 CMD_Execute()
{
	u8	result = 0, rtn = 0, chkr = 0;
	u32	cmdCnt = 0;
	cmdFunctionData_t	functionData;

	if(cmdArgc == 0)	return result;

	do{
		if(cmdSetHandle.pCmdList[cmdCnt].pCmd == NULL)
		{
			CMD_Printf(">>NO_COMMAND;ERROR");

			CMD_Printf("\r\n");
			
			return result;
		}

		chkr = CMD_Compare(pCmdArgv[0], (char*)cmdSetHandle.pCmdList[cmdCnt].pCmd);
		if(chkr)
		{
			break;
		}

		cmdCnt += 1;
	}while(1);

	memset(&functionData, NULL, sizeof(functionData));

	functionData.srcType		= cmdSetHandle.txSrcType;
	functionData.pCmdList		= cmdSetHandle.pCmdList;
	functionData.pArgv			= &pCmdArgv[1];
	functionData.argc			= cmdArgc - 1;
	functionData.option			= cmdSetHandle.pCmdList[cmdCnt].option;
	functionData.retryCnt		= 0;
	
	if(cmdSetHandle.pCmdList[cmdCnt].option != NULL)
	{
		functionData.deactivate		= 1;
	}
	else
	{
		functionData.deactivate		= 0;
	}
	functionData.channel			= 0;
	functionData.position			= 0;

	if(cmdSetHandle.pCmdList[cmdCnt].pMessage != NULL)
	{
		CMD_Printf((char*)cmdSetHandle.pCmdList[cmdCnt].pMessage);
	}
	else
	{
		CMD_Printf(">>NO_MESSAGE");
	}

	if(cmdSetHandle.pCmdList[cmdCnt].pFunction != NULL)
	{
		rtn = cmdSetHandle.pCmdList[cmdCnt].pFunction(&functionData);
	}
	else
	{
		CMD_Printf(";NO_FUNCTION");

		rtn = 0;
	}

	if(rtn != 0)
	{
		CMD_Printf(";OK");

		result = 1;
	}
	else
	{
		CMD_Printf(";ERROR");
	}

	CMD_Printf("\r\n");

	return result;
}

u8 Cmd_Help(void *pVoid)
{
	u8	result = 0;
	u32	cnt = 0;
	cmdFunctionData_t	*pData;
	commandList_t		*pCmdList;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->pCmdList == NULL)		return result;

	pCmdList = pData->pCmdList;

	if(pData->argc == 0)
	{
		CMD_Printf(";CMD_LIST=");

		do{
			if(pCmdList[cnt].pFunction != NULL)
			{
				if(cnt == 0)	CMD_Printf("%s", pCmdList[cnt].pCmd);
				else			CMD_Printf(", %s", pCmdList[cnt].pCmd);
			}
			else				break;

			cnt += 1;
		}while(1);

		result = 1;
	}
	else if(pData->argc == 1)
	{
		do{
			if(pCmdList[cnt].pCmd == NULL)
			{
				CMD_Printf(";NO_COMMAND");

				break;
			}

			if(CMD_Compare(pData->pArgv[0], (char*)pCmdList[cnt].pCmd))
			{
				if(pCmdList[cnt].pFunction == NULL)
				{
					CMD_Printf(";NO_FUNCTION");

					break;
				}

				if(pCmdList[cnt].pNote != NULL)
				{
					CMD_Printf(";%s=%s", pCmdList[cnt].pCmd, pCmdList[cnt].pNote);
				}
				else
				{
					CMD_Printf(";%s=NO_NOTE", pCmdList[cnt].pCmd);
				}

				break;
			}

			cnt += 1;
		}while(1);

		result = 1;
	}

	return result;
}


