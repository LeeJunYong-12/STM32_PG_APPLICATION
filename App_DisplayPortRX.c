#include	"App_DisplayPortRX.h"

static u8 DPRX_SF_RegisterWrite(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dprxSFunction_t	*pSFunction;

	pSFunction = (dprxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	//	MODE
	adrs	= CORE_REG_RX_S_FUNCTION_MODE;
	regData	= RP_DP_RX_SFUNCTION_MODE_DP_REG_WRITE;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	//	ADRS
	adrs	= CORE_REG_RX_S_FUNCTION_BUFFER_0;
	regData = pSFunction->buffer[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	//	DATA
	adrs	= CORE_REG_RX_S_FUNCTION_BUFFER_1;
	regData = pSFunction->buffer[1];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_RX_SFUNCTION;
		flag <<= bitFlag;

		delay = 100;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;
	}

	if(errCnt == 0)						result = 1;

	return result;
}

static u8 DPRX_SF_RegisterRead(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dprxSFunction_t	*pSFunction;

	pSFunction = (dprxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	//	MODE
	adrs	= CORE_REG_RX_S_FUNCTION_MODE;
	regData	= RP_DP_RX_SFUNCTION_MODE_DP_REG_READ;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	//	ADRS
	adrs	= CORE_REG_RX_S_FUNCTION_BUFFER_0;
	regData = pSFunction->buffer[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_RX_SFUNCTION;
		flag <<= bitFlag;

		delay = 100;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;

		if(errCnt == 0)
		{
			//	DATA
			adrs	= CORE_REG_RX_S_FUNCTION_BUFFER_1;
			rtn = DPC_RegReadSingle(channel, adrs, &regData);
			if(rtn == 0)			errCnt += 1;
			else					pSFunction->buffer[1] = regData;
		}
	}

	if(errCnt == 0)						result = 1;

	return result;
}

static	const	userFuncList_t	dprxSFunctionFL[] =	{
//														{	MODE,													Function,							Note	},
														{	RP_DP_RX_SFUNCTION_MODE_DP_REG_WRITE,					DPRX_SF_RegisterWrite,				NULL	},
														{	RP_DP_RX_SFUNCTION_MODE_DP_REG_READ,					DPRX_SF_RegisterRead,				NULL	},
														{	NULL,													NULL,								NULL	},
};

u8 DPRX_SpecialFunction(void *pVoid)
{
	u8	result = 0;
	u16	cnt = 0;
	dprxSFunction_t	*pSFunction;

	pSFunction = (dprxSFunction_t*)pVoid;

	do{
		if(dprxSFunctionFL[cnt].mode == NULL)						return result;
		else if(dprxSFunctionFL[cnt].mode == pSFunction->mode)		break;
		else														cnt += 1;
	}while(1);

	if(dprxSFunctionFL[cnt].pFunction == NULL)						return result;

	result = dprxSFunctionFL[cnt].pFunction(pVoid);

	return result;
}

u8 DPRX_RegWrite(u8 channel, u32 adrs, u32 data)
{
	u8	result = 0;
	dprxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_RX_SFUNCTION_MODE_DP_REG_WRITE;
	sFunction.buffer[0]	= adrs;
	sFunction.buffer[1]	= data;

	result = DPRX_SpecialFunction(&sFunction);

	return result;
}

u8 DPRX_RegRead(u8 channel, u32 adrs, u32 *pData)
{
	u8	result = 0;
	dprxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_RX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= adrs;

	result = DPRX_SpecialFunction(&sFunction);
	if(result)			*pData = sFunction.buffer[1];

	return result;
}
