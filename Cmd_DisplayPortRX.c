#include	"Cmd_DisplayPortRX.h"

static uc8	cmdNote_dpRxRegWrite[] = "E7602 DP RX Internal Register Write | A-Cnt : 2 | A-Value : Address, Data\0";
static u8 Cmd_DPRX_RegWrite(void *pVoid)
{
	u8	result = 0;
	u32	adrs, regData;
	dprxSFunction_t	sFunction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)				return result;

	adrs	= CMD_StrToUL(pData->pArgv[0]);
	regData	= CMD_StrToUL(pData->pArgv[1]);

	memset(&sFunction, NULL, sizeof(sFunction));

	sFunction.channel	= pData->position;

	sFunction.mode		= RP_DP_RX_SFUNCTION_MODE_DP_REG_WRITE;
	sFunction.buffer[0]	= adrs;
	sFunction.buffer[1]	= regData;

	result = DPRX_SpecialFunction(&sFunction);

	return result;
}

static uc8	cmdNote_dpRxRegRead[] = "E7602 DP RX Internal Register Read | A-Cnt : 1 | A-Value : Address\0";
static u8 Cmd_DPRX_RegRead(void *pVoid)
{
	u8	result = 0;
	u32	adrs, regData;
	dprxSFunction_t	sFunction;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)				return result;

	adrs	= CMD_StrToUL(pData->pArgv[0]);

	memset(&sFunction, NULL, sizeof(sFunction));

	sFunction.channel	= pData->position;

	sFunction.mode		= RP_DP_RX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= adrs;

	result = DPRX_SpecialFunction(&sFunction);
	if(result)
	{
		regData = sFunction.buffer[1];

		CMD_Printf(";DATA=0x%08X", regData);
	}

	return result;
}


const	commandList_t	cmdList_DisplayPortRX[] = {
//		{	Command,				Function,						Option,		Mode,					RootMessage				Note},
		{	"help",					Cmd_Help,						NULL,		CMD_MODE_NONE,			";HELP",				NULL									},
		{	"Reg.Write",			Cmd_DPRX_RegWrite,				NULL,		CMD_MODE_NONE,			";REG.WRITE",			(u8*)cmdNote_dpRxRegWrite				},
		{	"Reg.Read",				Cmd_DPRX_RegRead,				NULL,		CMD_MODE_NONE,			";REG.READ",			(u8*)cmdNote_dpRxRegRead				},

		{	NULL,					NULL,							NULL,		NULL,					NULL,					NULL									},
};

