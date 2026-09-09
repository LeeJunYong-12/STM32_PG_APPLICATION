#include	"Cmd_VideoStream.h"

u8 Cmd_VideoStreamReset(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = VideoStream_Reset();

	return result;
}

u8 Cmd_VideoStreamResolution(void *pVoid)
{
	u8	result = 0;
	u32	hPixel, vLine;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	vLine	= CMD_StrToUL(pData->pArgv[0]);
	hPixel	= CMD_StrToUL(pData->pArgv[1]);

	result = IMG_PreloadListSetCurrentResolution(hPixel, vLine);

	return result;
}

u8 Cmd_VideoStreamOutput(void *pVoid)
{
	u8	result = 0;
	videoStreamInfo_t	videoInfo;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc < 2)		return result;

	memset(&videoInfo, NULL, sizeof(videoInfo));
	
	videoInfo.vbp = 2;
	videoInfo.vfp = 2;
	videoInfo.hbp = 4;
	videoInfo.hfp = 4;
	videoInfo.vSync = 2;
	videoInfo.hSync = 4;
	videoInfo.framerate = 0;	

	videoInfo.vLine									= CMD_StrToUL(pData->pArgv[0]);
	videoInfo.hPixel								= CMD_StrToUL(pData->pArgv[1]);
	if(pData->argc > 2)		videoInfo.framerate		= CMD_StrToUL(pData->pArgv[2]);
	if(pData->argc > 3)		videoInfo.vbp			= CMD_StrToUL(pData->pArgv[3]);
	if(pData->argc > 4)		videoInfo.vfp			= CMD_StrToUL(pData->pArgv[4]);
	if(pData->argc > 5)		videoInfo.hbp			= CMD_StrToUL(pData->pArgv[5]);
	if(pData->argc > 6)		videoInfo.hfp			= CMD_StrToUL(pData->pArgv[6]);
	if(pData->argc > 7)		videoInfo.vSync			= CMD_StrToUL(pData->pArgv[7]);
	if(pData->argc > 8)		videoInfo.hSync			= CMD_StrToUL(pData->pArgv[8]);
	if(pData->argc > 9)		return result;

	result = VideoStream_Output(&videoInfo);

	return result;
}

u8 Cmd_VideoStreamInput(void *pVoid)
{
	u8	result = 0, state, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "disable"))				state = 0;
	else if(CMD_Compare(pData->pArgv[0], "enable"))			state = 1;
	else													return result;

	if(state)
	{
		if(pData->option == NULL)
		{
			rtn = CoreVC_VideoStreamInputEnable(2);
			if(rtn == 0)					errCnt += 1;
			rtn = CoreVC_VideoStreamInputEnable(3);
			if(rtn == 0)					errCnt += 1;
		}
		else if(pData->option == 0x01)
		{
			rtn = CoreVC_VideoStreamInputEnable(2);
			if(rtn == 0)					errCnt += 1;
		}
		else if(pData->option == 0x02)
		{
			rtn = CoreVC_VideoStreamInputEnable(3);
			if(rtn == 0)					errCnt += 1;
		}
	}
	else
	{
		if(pData->option == NULL)
		{
			rtn = CoreVC_VideoStreamInputDisable(2);
			if(rtn == 0)					errCnt += 1;
			rtn = CoreVC_VideoStreamInputDisable(3);
			if(rtn == 0)					errCnt += 1;
		}
		else if(pData->option == 0x01)
		{
			rtn = CoreVC_VideoStreamInputDisable(2);
			if(rtn == 0)					errCnt += 1;
		}
		else if(pData->option == 0x02)
		{
			rtn = CoreVC_VideoStreamInputDisable(3);
			if(rtn == 0)					errCnt += 1;			
		}
	}

	if(errCnt == 0)						result = 1;

	return result;
}

const	commandList_t	cmdList_VideoStream[] =	{
//	{	Command,					Function,						Option,			Mode,			RootMessage					Note}
	{	"help",						Cmd_Help,						NULL,			0x01,			";HELP",					"video help [command]"},
	{	"Stream.Reset",				Cmd_VideoStreamReset,			NULL,			NULL,			";STREAM.RESET",			NULL},
	{	"Stream.Resolution",		Cmd_VideoStreamResolution,		NULL,			NULL,			";STREAM.RESOLUTION",		"[vLine] [hPixel]"},
	{	"Stream.Output",			Cmd_VideoStreamOutput,			NULL,			NULL,			";STREAM.OUTPUT",			"[vLine] [hPixel] [framerate] [vbp] [vfp] [hbp] [hfp] [vSync] [hSync]"},
	{	"Stream.input",				Cmd_VideoStreamInput,			NULL,			NULL,			";STREAM.INPUT",			NULL},
	{	"Stream.input1",			Cmd_VideoStreamInput,			0x01,			NULL,			";STREAM.INPUT1",			NULL},
	{	"Stream.input2",			Cmd_VideoStreamInput,			0x02,			NULL,			";STREAM.INPUT2",			NULL},
	{	NULL,						NULL,							NULL,			NULL,			NULL,						NULL},
};

