#include	"App_DisplayPortTX.h"

static	dptxRegHandle_t	dptxRegHandle = {DP_TX_REG_DATA_MODE_TYPE_1, DPTX_RegDataWrite_TYPE_1, DPTX_RegDataWrite_TYPE_1_md,DPTX_RegDataWrite_TYPE_1_mx, DPTX_RegDataWrite_TYPE_1_mx_long,DPTX_RegDataRead_TYPE_1, DPTX_RegDataRead_TYPE_1_md, DPTX_RegDataRead_TYPE_1_mx};
static u8 dptxSdpContain[2][5];

static u8 DPTX_DS_HpdEventConfig(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 1000;
	u32	regData, bitFlag, flag;

	dptxDefaultSetup_t	*pDefaultSetup;

	pDefaultSetup = (dptxDefaultSetup_t*)pVoid;

	channel = (u8)pDefaultSetup->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)					return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)					return result;

	adrs	= CORE_REG_TX_HPD_EVENT_CONFIG;
	regData	= (pDefaultSetup->buffer[0] << 31);
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_DEFAULT;
		flag <<= bitFlag;

		delay = 100;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

static	const	userFuncList_t	dptxDefaultSetupFL[] =	{
//															{	MODE,													Function,							Note	},
															{	RP_DP_TX_DEFAULT_SETUP_MODE_HPD_EVENT_CONFIG,			DPTX_DS_HpdEventConfig,				NULL	},
															{	NULL,													NULL,								NULL	},
};

u8 DPTX_DefaultSetup(void *pVoid)
{
	u8	result = 0;
	u16	cnt = 0;
	dptxDefaultSetup_t	*pDefaultSetup;

	pDefaultSetup = (dptxDefaultSetup_t*)pVoid;

	do{
		if(dptxDefaultSetupFL[cnt].mode == pDefaultSetup->mode)		break;
		else if(dptxDefaultSetupFL[cnt].mode == NULL)				return result;
		else														cnt += 1;
	}while(1);

	if(dptxDefaultSetupFL[cnt].pFunction == NULL)					return result;

	result = dptxDefaultSetupFL[cnt].pFunction(pVoid);

	return result;
}

u8 DPTX_LinkTraining(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 1000;
	u32	regData, bitFlag, flag, mask = 0xFFFF0000;
	dptxLinkTraining_t	*pData;

	pData = (dptxLinkTraining_t*)pVoid;

	channel = (u8)pData->channel;

	if(pData->mode >= RP_DP_TX_LINK_TRAINING_MODE_END)		return result;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)											return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)											return result;

	adrs = CORE_REG_TX_LINKTRAINING_MODE;
	regData = pData->mode;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)											errCnt += 1;

	adrs = CORE_REG_DP_TX_LANE_CONFIG;
	rtn = DPC_RegReadSingle(channel, adrs, &regData);
	if(rtn == 0)											errCnt += 1;
	regData &= ~mask;
	regData |= (pData->linkRate << 16);
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)											errCnt += 1;

	adrs = CORE_REG_TX_LINKTRAINING_CTRL_1;
	regData = ((pData->linkRate & 0x00FF) << 16) | ((pData->laneCount & 0x00FF) << 0);
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)											errCnt += 1;

	adrs = CORE_REG_TX_LINKTRAINING_CTRL_2;
	regData = ((pData->statusCheck & 0x01) << 10) | ((pData->capabilityCheck & 0x01) << 9) | ((pData->dpcdCheck & 0x01) << 8) | ((pData->preEmphasis & 0x03) << 4) | ((pData->voltSwing & 0x03) << 0);
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)											errCnt += 1;

	adrs = CORE_REG_TX_LINKTRAINING_CTRL_3;
	regData = ((pData->fTPattern & 0x0F) << 28) | ((pData->fTDelay & 0x0FFF) << 16) | ((pData->sTPattern & 0x0F) << 12) | ((pData->sTDelay & 0x0FFF) << 0);
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)											errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_LINKTRAINING;
		flag <<= bitFlag;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)										errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)										errCnt += 1;
	}

	if(errCnt == 0)											result = 1;

	return result;
}

static u8 DPTX_SF_VideoOutputCtrl(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData	= RP_DP_TX_SFUNCTION_MODE_VIDEO_OUTPUT_CTRL;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_S_FUNCTION_CTRL_1;
	regData = pSFunction->ctrl[0] & 0x00000001;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
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

static u8 DPTX_SF_ColorSpaceReInit(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData = RP_DP_TX_SFUNCTION_MODE_COLOR_SPACE_REINIT;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
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

static u8 DPTX_SF_AssrCtrl(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData = RP_DP_TX_SFUNCTION_MODE_ASSR_CTRL;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_S_FUNCTION_CTRL_1;
	regData = pSFunction->ctrl[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
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

static u8 DPTX_SF_LtdoneAutoVideoOutput(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData = RP_DP_TX_SFUNCTION_MODE_LTDONE_AUTO_VIDEO_OUTPUT;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_S_FUNCTION_CTRL_1;
	regData = pSFunction->ctrl[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
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

static u8 DPTX_SF_DummyVideo(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData = RP_DP_TX_SFUNCTION_MODE_ALPDP_DUMMY_VIDEO;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_S_FUNCTION_CTRL_1;
	regData = pSFunction->ctrl[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
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

static u8 DPTX_SF_FixedMVID(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData = RP_DP_TX_SFUNCTION_MODE_FIXED_MVID_CTRL;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_S_FUNCTION_CTRL_1;
	regData = pSFunction->ctrl[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
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

static u8 DPTX_SF_AlpmCtrl(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData	= RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)						errCnt += 1;

	adrs	= CORE_REG_TX_S_FUNCTION_CTRL_1;
	regData	= pSFunction->ctrl[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)						errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
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

static u8 DPTX_SF_RegisterWrite(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	//	MODE
	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData	= RP_DP_TX_SFUNCTION_MODE_DP_REG_WRITE;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	//	ADRS
	adrs	= CORE_REG_TX_S_FUNCTION_BUFFER_0;
	regData = pSFunction->buffer[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	//	DATA
	adrs	= CORE_REG_TX_S_FUNCTION_BUFFER_1;
	regData = pSFunction->buffer[1];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
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

static u8 DPTX_SF_RegisterRead(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	channel = (u8)pSFunction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)						return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)						return result;

	//	MODE
	adrs	= CORE_REG_TX_S_FUNCTION_MODE;
	regData	= RP_DP_TX_SFUNCTION_MODE_DP_REG_READ;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	//	ADRS
	adrs	= CORE_REG_TX_S_FUNCTION_BUFFER_0;
	regData = pSFunction->buffer[0];
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_SFUNCTION;
		flag <<= bitFlag;

		delay = 100;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;

		if(errCnt == 0)
		{
			//	DATA
			adrs	= CORE_REG_TX_S_FUNCTION_BUFFER_1;
			rtn = DPC_RegReadSingle(channel, adrs, &regData);
			if(rtn == 0)			errCnt += 1;
			else					pSFunction->buffer[1] = regData;
		}
	}

	if(errCnt == 0)						result = 1;

	return result;
}

static	const	userFuncList_t	dptxSFunctionFL[] =	{
//														{	MODE,													Function,							Note	},
														{	RP_DP_TX_SFUNCTION_MODE_VIDEO_OUTPUT_CTRL,				DPTX_SF_VideoOutputCtrl,			NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_COLOR_SPACE_REINIT,				DPTX_SF_ColorSpaceReInit,			NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_ASSR_CTRL,						DPTX_SF_AssrCtrl,					NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_LTDONE_AUTO_VIDEO_OUTPUT,		DPTX_SF_LtdoneAutoVideoOutput,		NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_ALPDP_DUMMY_VIDEO,				DPTX_SF_DummyVideo,					NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_FIXED_MVID_CTRL,				DPTX_SF_FixedMVID,					NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL,						DPTX_SF_AlpmCtrl,					NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_STATUS_READ,					NULL,								NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_DP_REG_WRITE,					DPTX_SF_RegisterWrite,				NULL	},
														{	RP_DP_TX_SFUNCTION_MODE_DP_REG_READ,					DPTX_SF_RegisterRead,				NULL	},
														{	NULL,													NULL,								NULL	},
};

u8 DPTX_SpecialFunction(void *pVoid)
{
	u8	result = 0;
	u16	cnt = 0;
	dptxSFunction_t	*pSFunction;

	pSFunction = (dptxSFunction_t*)pVoid;

	do{
		if(dptxSFunctionFL[cnt].mode == NULL)						return result;
		else if(dptxSFunctionFL[cnt].mode == pSFunction->mode)		break;
		else														cnt += 1;
	}while(1);

	if(dptxSFunctionFL[cnt].pFunction == NULL)						return result;

	result = dptxSFunctionFL[cnt].pFunction(pVoid);

	return result;
}

static u8 DPTX_TR_AuxWrite(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxTransaction_t	*pTransaction;

	pTransaction = (dptxTransaction_t*)pVoid;

	channel = (u8)pTransaction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)					return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)					return result;

	if(pTransaction->size == 0)		return result;
	if(pTransaction->size > 256)	return result;

	adrs	= CORE_REG_TX_TRANSACTION_CTRL;
	regData	= RP_DP_TX_TRANSACTION_MODE_AUX_WRITE;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_ADRS;
	regData	= pTransaction->adrs & 0x000FFFFF;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_SIZE;
	regData	= pTransaction->size;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_BUFFER_0;
	rtn = DPC_RegWrite(channel, adrs, (u32*)pTransaction->pData, (pTransaction->size / 4) + 1);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_TRANSACTION;
		flag <<= bitFlag;

		delay = 100;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;

		adrs	= CORE_REG_TX_TRANSACTION_CTRL;
		rtn = DPC_RegReadSingle(channel, adrs, &regData);
		if(rtn == 0)				errCnt += 1;

		pTransaction->status = (regData >> 16) & 0x0000FFFF;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

static u8 DPTX_TR_AuxRead(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxTransaction_t	*pTransaction;

	pTransaction = (dptxTransaction_t*)pVoid;

	channel = (u8)pTransaction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)					return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)					return result;

	if(pTransaction->size == 0)		return result;
	if(pTransaction->size > 256)	return result;

	adrs	= CORE_REG_TX_TRANSACTION_CTRL;
	regData	= RP_DP_TX_TRANSACTION_MODE_AUX_READ;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_ADRS;
	regData	= pTransaction->adrs & 0x000FFFFF;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_SIZE;
	regData	= pTransaction->size;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_TRANSACTION;
		flag <<= bitFlag;

		delay = 3000;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;

		adrs	= CORE_REG_TX_TRANSACTION_BUFFER_0;
		rtn = DPC_RegRead(channel, adrs, (u32*)pTransaction->pData, (pTransaction->size / 4) + 1);
		if(rtn == 0)				errCnt += 1;

		adrs	= CORE_REG_TX_TRANSACTION_CTRL;
		rtn = DPC_RegReadSingle(channel, adrs, &regData);
		if(rtn == 0)				errCnt += 1;

		pTransaction->status = (regData >> 16) & 0x0000FFFF;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

static u8 DPTX_TR_EdidRead(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxTransaction_t	*pTransaction;

	pTransaction = (dptxTransaction_t*)pVoid;

	channel = (u8)pTransaction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)					return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)					return result;

	adrs	= CORE_REG_TX_TRANSACTION_CTRL;
	regData	= RP_DP_TX_TRANSACTION_MODE_EDID_READ;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_ADRS;
	regData	= 0;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_SIZE;
	regData	= 0;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_TRANSACTION;
		flag <<= bitFlag;

		delay = 3000;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;

		adrs	= CORE_REG_TX_TRANSACTION_CTRL;
		rtn = DPC_RegReadSingle(channel, adrs, &regData);
		if(rtn == 0)				errCnt += 1;

		pTransaction->status = (regData >> 16) & 0x0000FFFF;

		if(pTransaction->status > 512)		pTransaction->size = 512;
		else								pTransaction->size = pTransaction->status;

		adrs	= CORE_REG_TX_TRANSACTION_BUFFER_0;
		rtn = DPC_RegRead(channel, adrs, (u32*)pTransaction->pData, (pTransaction->size / 4) + 1);
		if(rtn == 0)				errCnt += 1;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

static u8 DPTX_TR_I2cWrite(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxTransaction_t	*pTransaction;

	pTransaction = (dptxTransaction_t*)pVoid;

	channel = (u8)pTransaction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)					return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)					return result;

	if(pTransaction->size == 0)		return result;
	if(pTransaction->size > 256)	return result;

	adrs	= CORE_REG_TX_TRANSACTION_CTRL;
	regData	= RP_DP_TX_TRANSACTION_MODE_I2C_WRITE;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_ADRS;
	regData	= pTransaction->adrs & 0x000000FF;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_SIZE;
	regData	= pTransaction->size;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_BUFFER_0;
	rtn = DPC_RegWrite(channel, adrs, (u32*)pTransaction->pData, (pTransaction->size / 4) + 1);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_TRANSACTION;
		flag <<= bitFlag;

		delay = 100;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;

		adrs	= CORE_REG_TX_TRANSACTION_CTRL;
		rtn = DPC_RegReadSingle(channel, adrs, &regData);
		if(rtn == 0)				errCnt += 1;

		pTransaction->status = (regData >> 16) & 0x0000FFFF;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

static u8 DPTX_TR_I2cRead(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	channel, chipSelect;
	u16	adrs;
	u32	delay = 100;
	u32	regData, bitFlag, flag;
	dptxTransaction_t	*pTransaction;

	pTransaction = (dptxTransaction_t*)pVoid;

	channel = (u8)pTransaction->channel;

	rtn = DP_GetChipSelect(channel, &chipSelect);
	if(rtn == 0)					return result;

	rtn = DP_GetFlagPosition(channel, &bitFlag);
	if(rtn == 0)					return result;

	if(pTransaction->size == 0)		return result;
	if(pTransaction->size > 256)	return result;

	adrs	= CORE_REG_TX_TRANSACTION_CTRL;
	regData	= RP_DP_TX_TRANSACTION_MODE_I2C_READ;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_ADRS;
	regData	= pTransaction->adrs & 0x000000FF;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	adrs	= CORE_REG_TX_TRANSACTION_SIZE;
	regData	= pTransaction->size;
	rtn = DPC_RegWriteSingle(channel, adrs, regData);
	if(rtn == 0)					errCnt += 1;

	if(errCnt == 0)
	{
		flag = CORE_REG_BIT_REGION_DP_TX_TRANSACTION;
		flag <<= bitFlag;

		delay = 3000;

		rtn = DP_ProcessStart(chipSelect, flag);
		if(rtn == 0)				errCnt += 1;

		rtn = DP_ProcessCheck(chipSelect, delay);
		if(rtn == 0)				errCnt += 1;

		adrs	= CORE_REG_TX_TRANSACTION_BUFFER_0;
		rtn = DPC_RegRead(channel, adrs, (u32*)pTransaction->pData, (pTransaction->size / 4) + 1);
		if(rtn == 0)				errCnt += 1;

		adrs	= CORE_REG_TX_TRANSACTION_CTRL;
		rtn = DPC_RegReadSingle(channel, adrs, &regData);
		if(rtn == 0)				errCnt += 1;

		pTransaction->status = (regData >> 16) & 0x0000FFFF;
	}

	if(errCnt == 0)					result = 1;

	return result;
}

static	const	userFuncList_t	dptxTransactionFL[] =	{
//															{	MODE,													Function,							Note	},
															{	RP_DP_TX_TRANSACTION_MODE_AUX_WRITE,					DPTX_TR_AuxWrite,					NULL	},
															{	RP_DP_TX_TRANSACTION_MODE_AUX_READ,						DPTX_TR_AuxRead,					NULL	},
															{	RP_DP_TX_TRANSACTION_MODE_EDID_READ,					DPTX_TR_EdidRead,					NULL	},
															{	RP_DP_TX_TRANSACTION_MODE_I2C_WRITE,					DPTX_TR_I2cWrite,					NULL	},
															{	RP_DP_TX_TRANSACTION_MODE_I2C_READ,						DPTX_TR_I2cRead,					NULL	},
															{	NULL,													NULL,								NULL	},
};

u8 DPTX_Transaction(void *pVoid)
{
	u8	result = 0;
	u16	cnt = 0;
	dptxTransaction_t	*pTransaction;

	pTransaction = (dptxTransaction_t*)pVoid;

	do{
		if(dptxTransactionFL[cnt].mode == pTransaction->mode)		break;
		else if(dptxTransactionFL[cnt].mode == NULL)				return result;
		else														cnt += 1;
	}while(1);

	if(dptxTransactionFL[cnt].pFunction == NULL)					return result;

	result = dptxTransactionFL[cnt].pFunction(pVoid);

	return result;
}

u8 DPTX_SecondaryDataPacketContainClear(u8 channel)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	chipSelect, txChannel;
	u16	baseAdrs;
	u32	regData;

	result = DP_GetChannelInfo(channel, &chipSelect, &baseAdrs);
	if(result != 0)
	{
		if(baseAdrs == 0x0100)			txChannel = 0;
		else if(baseAdrs == 0x0200)		txChannel = 1;
		else if(baseAdrs == 0x0400)		txChannel = 2;
		else if(baseAdrs == 0x0800)		txChannel = 3;
		else if(baseAdrs == 0x1000)		txChannel = 4;

		dptxSdpContain[chipSelect][txChannel] = 0;

		regData = (1 << 8);
		regData <<= txChannel;

		rtn = DPC_DirectRegWriteSingle(chipSelect, CORE_REG_DP_TX_SDP_CTRL, regData);
		if(rtn == 0)					errCnt += 1;

		regData = 0;

		rtn = DPC_DirectRegWriteSingle(chipSelect, CORE_REG_DP_TX_SDP_CTRL, regData);
		if(rtn == 0)					errCnt += 1;
	}

	if(errCnt == 0)						result = 1;

	return result;
}

u8 DPTX_SecondaryDataPacketContain(u8 chipSelect, u16 adrs, u8 *pData, u16 size)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	*pwData, wData;

	if(size != 36)				return result;

	for(u32 cnt = 0; cnt < 4; cnt++)
	{
		wData = (u32)pData[cnt];
		rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, wData);
		if(rtn == 0)			errCnt += 1;
	}

	pwData = (u32*)&pData[4];

	for(u32 cnt = 0; cnt < 8; cnt++)
	{
		rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, pwData[cnt]);
		if(rtn == 0)			errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

u8 DPTX_SecondaryDataPacketWrite(u8 channel, u8 *pData, u16 size)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	chipSelect, txChannel;
	u16	baseAdrs, adrs;
	u32	regData, flagMask;

	if(size == 0)									return result;
	if(size > 36)									return result;

	rtn = DP_GetChannelInfo(channel, &chipSelect, &baseAdrs);
	if(rtn == 0)									return result;

	switch(baseAdrs)
	{
		case 0x0100:
			adrs = CORE_REG_DP_TX0_SDP_BUFFER;
			txChannel = 0;
			break;

		case 0x0200:
			adrs = CORE_REG_DP_TX1_SDP_BUFFER;
			txChannel = 1;
			break;

		case 0x0400:
			adrs = CORE_REG_DP_TX2_SDP_BUFFER;
			txChannel = 2;
			break;

		case 0x0800:
			adrs = CORE_REG_DP_TX3_SDP_BUFFER;
			txChannel = 3;
			break;

		case 0x1000:
			adrs = CORE_REG_DP_TX4_SDP_BUFFER;
			txChannel = 4;
			break;

		default:
			return result;
			break;
	}

	rtn = DPTX_SecondaryDataPacketContain(chipSelect, adrs, pData, size);
	if(rtn == 0)									return result;

	if(adrs == CORE_REG_DP_TX0_SDP_BUFFER)			flagMask = 0x00000001;
	else if(adrs == CORE_REG_DP_TX1_SDP_BUFFER)		flagMask = 0x00000002;
	else if(adrs == CORE_REG_DP_TX2_SDP_BUFFER)		flagMask = 0x00000004;
	else if(adrs == CORE_REG_DP_TX3_SDP_BUFFER)		flagMask = 0x00000008;
	else if(adrs == CORE_REG_DP_TX4_SDP_BUFFER)		flagMask = 0x00000010;
	else											return result;

	adrs = CORE_REG_DP_TX_SDP_CTRL;

	rtn = DPC_DirectRegReadSingle(chipSelect, adrs, &regData);
	if(rtn == 0)									errCnt += 1;

	regData |= flagMask;

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)									errCnt += 1;

	if(errCnt == 0)									result = 1;

	if(result)
	{
		dptxSdpContain[chipSelect][txChannel] = 0;
	}

	return result;
}

u8 DPTX_SecondaryDataPacketMultiContain(u8 channel, u8 *pData, u16 size)
{
	u8	result = 0, rtn;
	u8	chipSelect, txChannel;
	u16	baseAdrs, adrs;

	if(size != 36)									return result;

	rtn = DP_GetChannelInfo(channel, &chipSelect, &baseAdrs);
	if(rtn == 0)									return result;

	switch(baseAdrs)
	{
		case 0x0100:
			adrs = CORE_REG_DP_TX0_SDP_BUFFER;
			txChannel = 0;
			break;

		case 0x0200:
			adrs = CORE_REG_DP_TX1_SDP_BUFFER;
			txChannel = 1;
			break;

		case 0x0400:
			adrs = CORE_REG_DP_TX2_SDP_BUFFER;
			txChannel = 2;
			break;

		case 0x0800:
			adrs = CORE_REG_DP_TX3_SDP_BUFFER;
			txChannel = 3;
			break;

		case 0x1000:
			adrs = CORE_REG_DP_TX4_SDP_BUFFER;
			txChannel = 4;
			break;

		default:
			return result;
			break;
	}

	if(dptxSdpContain[chipSelect][txChannel] >= 5)	return result;

	rtn = DPTX_SecondaryDataPacketContain(chipSelect, adrs, pData, size);
	if(rtn != 0)									result = 1;

	if(result)
	{
		dptxSdpContain[chipSelect][txChannel] += 1;
	}

	return result;
}

u8 DPTX_SecondaryDataPacketMultiWrite(u8 channel)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	chipSelect, txChannel;
	u16	baseAdrs, adrs;
	u32	regData, flagMask;

	rtn = DP_GetChannelInfo(channel, &chipSelect, &baseAdrs);
	if(rtn == 0)									return result;

	switch(baseAdrs)
	{
		case 0x0100:
			flagMask = 0x00000001;
			txChannel = 0;
			break;

		case 0x0200:
			flagMask = 0x00000002;
			txChannel = 1;
			break;

		case 0x0400:
			flagMask = 0x00000004;
			txChannel = 2;
			break;

		case 0x0800:
			flagMask = 0x00000008;
			txChannel = 3;
			break;

		case 0x1000:
			flagMask = 0x00000010;
			txChannel = 4;
			break;

		default:
			return result;
			break;
	}

	if(dptxSdpContain[chipSelect][txChannel] == 0)	return result;

	adrs = CORE_REG_DP_TX_SDP_CTRL;

	rtn = DPC_DirectRegReadSingle(chipSelect, adrs, &regData);
	if(rtn == 0)									errCnt += 1;

	regData |= flagMask;

	rtn = DPC_DirectRegWriteSingle(chipSelect, adrs, regData);
	if(rtn == 0)									errCnt += 1;

	if(errCnt == 0)									result = 1;

	if(result)
	{
		dptxSdpContain[chipSelect][txChannel] = 0;
	}

	return result;
}

u8 DPTX_AuxWrite(u8 channel, u32 adrs, u8 *pData, u16 size)
{
	u8	result = 0;
	dptxTransaction_t	dptxTransaction;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= channel;
	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_WRITE;
	dptxTransaction.adrs	= adrs;
	dptxTransaction.pData	= pData;
	dptxTransaction.size	= size;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
#if 0
		if(dptxTransaction.status != 0)		result = 0;
#endif
#if 1	// status check _ 210730
		if(dptxTransaction.status != 0)
		{
			CMD_Printf(";AUX_WRITE, DPCD=%05X, DATA=%02X, STATUS=%d", adrs, pData[0], dptxTransaction.status);
			result = 0;
		}
#endif
	}

	return result;
}

u8 DPTX_AuxRead(u8 channel, u32 adrs, u8 *pData, u16 size)
{
	u8	result = 0;
	dptxTransaction_t	dptxTransaction;

	memset(&dptxTransaction, NULL, sizeof(dptxTransaction));

	dptxTransaction.channel	= channel;
	dptxTransaction.mode	= RP_DP_TX_TRANSACTION_MODE_AUX_READ;
	dptxTransaction.adrs	= adrs;
	dptxTransaction.pData	= pData;
	dptxTransaction.size	= size;

	result = DPTX_Transaction(&dptxTransaction);
	if(result)
	{
#if 0
		if(dptxTransaction.status != 0)		result = 0;
#endif
#if 1	// status check _ 210730
		if(dptxTransaction.status != 0)
		{
			CMD_Printf(";AUX_READ, DPCD=%05X, STATUS=%d", adrs, dptxTransaction.status);
			result = 0;
		}
#endif
	}

	return result;
}

u8 DPTX_AlpmCtrl(u8 channel, void *pVoid)
{
	u8	result = 0;
	dptxALPM_t	*pData;
	dptxSFunction_t	sFunction;

	pData = (dptxALPM_t*)pVoid;

	memset(&sFunction, NULL, sizeof(dptxSFunction_t));

	sFunction.mode = RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL;

	sFunction.ctrl[0] = ((pData->enable & 0x01) << 0) | ((pData->alpdpEnable & 0x01) << 8) | ((pData->tryOnce & 0x01) << 16);
	sFunction.ctrl[1] = ((pData->pdwnSignal & 0x01) << 0) | ((pData->pdwnSeqPeriod & 0x03) << 8) | ((pData->pdwnSeqNumber & 0x0F) << 16) | ((pData->wakeCount & 0x0F) << 24);
	sFunction.ctrl[2] = ((pData->t1Count & 0xFF) << 0) | ((pData->t2Count & 0xFF) << 16);

	sFunction.buffer[0] = ((pData->selectTPS & 0x01) << 0) | ((pData->t12timing & 0xFF) << 16);
	sFunction.buffer[1] = ((pData->clkRecoveryCount & 0xFFFF) << 0) | ((pData->symbolLockCount & 0xFFFF) << 16);
	sFunction.buffer[2] = ((pData->linkRateValue & 0xFF) << 0) | ((pData->linkRateChange & 0x01) << 16);

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_RegDataWrite_TYPE_1(void *pVoid)
{
	u8	result = 0, rtn;
	u8	wBuffer[16];
	u16	writeSize;
	u32	dpcdAdrs, remainSize, txSize;
	dptxRegData_t	*pData;

	pData = (dptxRegData_t*)pVoid;

	if(pData->regAdrs > 0xFF)						return result;
	if(pData->dataAdrs > 0xFF)						return result;
	if(pData->dataSize > 256)						return result;

	remainSize	= pData->dataSize;
	txSize		= 0;

	memset(&wBuffer, NULL, sizeof(wBuffer));

	dpcdAdrs		= 0x004E0;
	wBuffer[0]		= (u8)pData->regAdrs;
	wBuffer[1]		= (u8)pData->dataAdrs;
	writeSize		= 2;
	rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
	if(rtn == 0)				return result;

	do{
		HAL_Delay(5);
		dpcdAdrs	= 0x004F0;
		if(remainSize > 16)
		{
			rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, &pData->pBuffer[txSize], 16);
			if(rtn == 0)		return result;
		}
		else
		{
			rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, &pData->pBuffer[txSize], remainSize);
			if(rtn == 0)		return result;

			break;
		}
		HAL_Delay(5);

		txSize += 16;
		remainSize -= 16;

		dpcdAdrs	= 0x004E1;
		wBuffer[0]	= (u8)(pData->dataAdrs + txSize);
		writeSize	= 1;
		rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
		if(rtn == 0)			return result;
	}while(1);

	result = 1;

	return result;
}

u8 DPTX_RegDataWrite_TYPE_1_md(void *pVoid)
{
	u8	result = 0, rtn;
	u8	wBuffer[16];
	u16	writeSize;
	u32	dpcdAdrs, remainSize, txSize;
	dptxRegData_t	*pData;

	pData = (dptxRegData_t*)pVoid;

	if(pData->regAdrs > 0xFF)						return result;
	if(pData->dataAdrs > 0xFF)						return result;
	if(pData->dataSize > 256)						return result;

	remainSize	= pData->dataSize;
	txSize		= 0;

	memset(&wBuffer, NULL, sizeof(wBuffer));

	dpcdAdrs		= 0x004E0;
	wBuffer[0]		= (u8)pData->regAdrs;
	wBuffer[1]		= (u8)pData->dataAdrs;
	writeSize		= 2;
	rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
	if(rtn == 0)				return result;

	HAL_Delay(5);

	dpcdAdrs	= 0xFFF00;
	rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, &pData->pBuffer[txSize], remainSize);
	if(rtn == 0)				return result;

	result = 1;

	return result;
}

u8 DPTX_RegDataWrite_TYPE_1_mx(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	wBuffer[16];
	u16	writeSize;
	u8 regadrs_part[2];
	u32	dpcdAdrs, remainSize, txSize;
	dptxRegData_t	*pData;

	pData = (dptxRegData_t*)pVoid;

	if(pData->regAdrs > 0xFFFF)						return result;
	if(pData->dataAdrs > 0xFF)						return result;

	remainSize	= pData->dataSize;
	txSize		= 0;

	memset(&wBuffer, NULL, sizeof(wBuffer));

	regadrs_part[0] = (u8)((pData->regAdrs&0x0000FF00)>>8);
	regadrs_part[1] = (u8)((pData->regAdrs&0x000000FF));

	dpcdAdrs		= 0x004E0;
	wBuffer[0]		= regadrs_part[0];
	wBuffer[1]		= regadrs_part[1];
	writeSize		= 2;
	rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
	if(rtn == 0)				return result;

	HAL_Delay(5);

	dpcdAdrs	= 0x004F0+(u32)(pData->dataAdrs);
	rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, &pData->pBuffer[txSize], remainSize);
	if(rtn == 0)		errCnt += 1;

	HAL_Delay(5);

	if(errCnt == 0)				result = 1;

	return result;
}

u8 DPTX_RegDataWrite_TYPE_1_mx_long(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	wBuffer[16];
	u16	writeSize;
	u8 cnt=0;
	u8 regadrs_part[2];
	u32	dpcdAdrs, remainSize, txSize;
	dptxRegData_t	*pData;

	pData = (dptxRegData_t*)pVoid;

	if(pData->regAdrs > 0xFFFF)						return result;
	if(pData->dataAdrs > 0xFF)						return result;

	remainSize	= pData->dataSize;
	txSize		= 0;

	memset(&wBuffer, NULL, sizeof(wBuffer));

	regadrs_part[0] = (u8)((pData->regAdrs&0x0000FF00)>>8);
	regadrs_part[1] = (u8)((pData->regAdrs&0x000000FF));

	do{
		dpcdAdrs		= 0x004E0;
		wBuffer[0]		= regadrs_part[0];
		wBuffer[1]		= regadrs_part[1]+cnt;
		writeSize		= 2;
		rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
		if(rtn == 0)				return result;
		HAL_Delay(5);

		dpcdAdrs	= 0x004F0+(u32)(pData->dataAdrs);
		if(remainSize > 16)
		{
			rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, &pData->pBuffer[txSize], 16);
			if(rtn == 0)		errCnt += 1;
		}
		else
		{
			rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, &pData->pBuffer[txSize], remainSize);
			if(rtn == 0)		errCnt += 1;

			break;
		}
		HAL_Delay(5);

		txSize += 16;
		remainSize -= 16;
		cnt++;

		if(rtn == 0)			errCnt += 1;
	}while(1);

	if(errCnt == 0)				result = 1;

	return result;
}


u8 DPTX_RegDataRead_TYPE_1(void *pVoid)
{
	u8	result = 0, rtn;
	u8	wBuffer[16];
	u16	writeSize;
	u32	dpcdAdrs, remainSize, rxSize;
	dptxRegData_t	*pData;

	pData = (dptxRegData_t*)pVoid;

	if(pData->regAdrs > 0xFF)						return result;
	if(pData->dataAdrs > 0xFF)						return result;
	if( pData->dataSize > 256)						return result;

	remainSize	= pData->dataSize;
	rxSize		= 0;

	memset(&wBuffer, NULL, sizeof(wBuffer));

	dpcdAdrs		= 0x004E0;
	wBuffer[0]		= (u8)pData->regAdrs;
	wBuffer[1]		= (u8)pData->dataAdrs;
	writeSize		= 2;
	rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
	if(rtn == 0)				return result;

	do{
		dpcdAdrs	= 0x004F0;
		if(remainSize > 16)
		{
			rtn = DPTX_AuxRead(pData->channel, dpcdAdrs, &pData->pBuffer[rxSize], 16);
			if(rtn == 0)		return result;
		}
		else
		{
			rtn = DPTX_AuxRead(pData->channel, dpcdAdrs, &pData->pBuffer[rxSize], remainSize);
			if(rtn == 0)		return result;

			break;
		}

		rxSize += 16;
		remainSize -= 16;

		dpcdAdrs	= 0x004E1;
		wBuffer[0]	= (u8)(pData->dataAdrs + rxSize);
		writeSize	= 1;
		rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
		if(rtn == 0)			return result;
	}while(1);

	result = 1;

	return result;
}

u8 DPTX_RegDataRead_TYPE_1_md(void *pVoid)
{
	u8	result = 0, rtn;
	u8	wBuffer[16];
	u16	writeSize;
	u32	dpcdAdrs, remainSize, rxSize;
	dptxRegData_t	*pData;

	pData = (dptxRegData_t*)pVoid;

	if(pData->regAdrs > 0xFF)						return result;
	if(pData->dataAdrs > 0xFF)						return result;
	if(pData->dataSize > 256)	return result;

	remainSize	= pData->dataSize;
	rxSize		= 0;

	memset(&wBuffer, NULL, sizeof(wBuffer));

	dpcdAdrs		= 0x004E0;
	wBuffer[0]		= (u8)pData->regAdrs;
	wBuffer[1]		= (u8)pData->dataAdrs;
	writeSize		= 2;
	rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
	if(rtn == 0)				return result;

	dpcdAdrs	= 0xFFF00;
	rtn = DPTX_AuxRead(pData->channel, dpcdAdrs, &pData->pBuffer[rxSize], remainSize);
	if(rtn == 0)				return result;
	
	result = 1;

	return result;
}

u8 DPTX_RegDataRead_TYPE_1_mx(void *pVoid)
{
	u8	result = 0, rtn;
	u8	wBuffer[16];
	u16	writeSize;
	u8 regadrs_part[2];
	u32	dpcdAdrs, remainSize, rxSize;
	dptxRegData_t	*pData;

	pData = (dptxRegData_t*)pVoid;

	if(pData->regAdrs > 0xFFFF)						return result;
	if(pData->dataAdrs > 0xFF)						return result;

	remainSize	= pData->dataSize;
	rxSize		= 0;

	memset(&wBuffer, NULL, sizeof(wBuffer));

	regadrs_part[0] = (u8)((pData->regAdrs&0x0000FF00)>>8);
	regadrs_part[1] = (u8)((pData->regAdrs&0x000000FF));

	dpcdAdrs		= 0x004E0;
	wBuffer[0]		= regadrs_part[0];
	wBuffer[1]		= regadrs_part[1];
	writeSize		= 2;
	rtn = DPTX_AuxWrite(pData->channel, dpcdAdrs, wBuffer, writeSize);
	if(rtn == 0)				return result;

	HAL_Delay(5);

	dpcdAdrs	= 0x004F0+(u32)(pData->dataAdrs);
	rtn = DPTX_AuxRead(pData->channel, dpcdAdrs, &pData->pBuffer[rxSize], remainSize);
	if(rtn == 0)				return result;
	
	result = 1;

	return result;
}


u8 DPTX_SetDataModeDIC(u32 mode)
{
	u8	result = 1;

	switch(mode)
	{
		case DP_TX_REG_DATA_MODE_TYPE_1:
			dptxRegHandle.mode			= DP_TX_REG_DATA_MODE_TYPE_1;
			dptxRegHandle.DataWrite		= DPTX_RegDataWrite_TYPE_1;
			dptxRegHandle.DataWritemd	= DPTX_RegDataWrite_TYPE_1_md;
			dptxRegHandle.DataWritemx	= DPTX_RegDataWrite_TYPE_1_mx;
			dptxRegHandle.DataWritemxlong	= DPTX_RegDataWrite_TYPE_1_mx_long;
			dptxRegHandle.DataRead		= DPTX_RegDataRead_TYPE_1;
			dptxRegHandle.DataReadmd	= DPTX_RegDataRead_TYPE_1_md;
			dptxRegHandle.DataReadmx	= DPTX_RegDataRead_TYPE_1_mx;
			break;

		default:
			dptxRegHandle.mode			= DP_TX_REG_DATA_MODE_NONE;
			dptxRegHandle.DataWrite		= NULL;
			dptxRegHandle.DataWritemd	= NULL;
			dptxRegHandle.DataRead		= NULL;
			dptxRegHandle.DataReadmd	= NULL;
			result = 0;
			break;
	}

	return result;
}

u32 DPTX_GetDataModeDIC()
{
	return dptxRegHandle.mode;
}

u8 DPTX_RegDataWriteDIC(void *pVoid)
{
	u8	result = 0;

	if(dptxRegHandle.mode == DP_TX_REG_DATA_MODE_NONE)		return result;

	if(dptxRegHandle.DataWrite == NULL)						return result;
	result = dptxRegHandle.DataWrite(pVoid);

	return result;
}

u8 DPTX_RegDataWriteDIC_md(void *pVoid)
{
	u8	result = 0;

	if(dptxRegHandle.mode == DP_TX_REG_DATA_MODE_NONE)		return result;

	if(dptxRegHandle.DataWrite == NULL)						return result;
	result = dptxRegHandle.DataWritemd(pVoid);

	return result;
}

u8 DPTX_RegDataWriteDIC_mx(void *pVoid)
{
	u8	result = 0;

	if(dptxRegHandle.mode == DP_TX_REG_DATA_MODE_NONE)		return result;

	if(dptxRegHandle.DataWrite == NULL)						return result;
	result = dptxRegHandle.DataWritemx(pVoid);

	return result;
}


u8 DPTX_RegDataWriteDIC_mx_long(void *pVoid)
{
	u8	result = 0;

	if(dptxRegHandle.mode == DP_TX_REG_DATA_MODE_NONE)		return result;

	if(dptxRegHandle.DataWrite == NULL)						return result;
	result = dptxRegHandle.DataWritemxlong(pVoid);

	return result;
}


u8 DPTX_RegDataReadDIC(void *pVoid)
{
	u8	result = 0;

	if(dptxRegHandle.mode == DP_TX_REG_DATA_MODE_NONE)		return result;

	if(dptxRegHandle.DataRead == NULL)						return result;
	result = dptxRegHandle.DataRead(pVoid);

	return result;
}

u8 DPTX_RegDataReadDIC_md(void *pVoid)
{
	u8	result = 0;

	if(dptxRegHandle.mode == DP_TX_REG_DATA_MODE_NONE)		return result;

	if(dptxRegHandle.DataRead == NULL)						return result;
	result = dptxRegHandle.DataReadmd(pVoid);

	return result;
}

u8 DPTX_RegDataReadDIC_mx(void *pVoid)
{
	u8	result = 0;

	if(dptxRegHandle.mode == DP_TX_REG_DATA_MODE_NONE)		return result;

	if(dptxRegHandle.DataRead == NULL)						return result;
	result = dptxRegHandle.DataReadmx(pVoid);

	return result;
}

u8 DPTX_RegWrite(u8 channel, u32 adrs, u32 data)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_WRITE;
	sFunction.buffer[0]	= adrs;
	sFunction.buffer[1]	= data;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_RegRead(u8 channel, u32 adrs, u32 *pData)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= adrs;

	result = DPTX_SpecialFunction(&sFunction);
	if(result)			*pData = sFunction.buffer[1];

	return result;
}

u8 DPTX_LaneConfig(u8 channel, u32 state)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	adrs, regData, mask = (0x1f << 5);

	if(state > 4)			return result;

	adrs = 0x00;
	rtn = DPTX_RegRead(channel, adrs, &regData);
	if(rtn == 0)			return result;

	regData &= ~mask;

	regData |= ((state & mask) << 5);

	rtn = DPTX_RegWrite(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 DPTX_AssrCtrl(u8 channel, u8 state)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_ASSR_CTRL;
	sFunction.ctrl[0]	= state;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_DummyVideoCtrl(u8 channel, u8 state)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_ALPDP_DUMMY_VIDEO;
	sFunction.ctrl[0]	= state;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_FixedMVID(u8 channel, u8 state)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_FIXED_MVID_CTRL;
	sFunction.ctrl[0]	= state;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_AlpmEnter(u8 channel)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL;
	sFunction.ctrl[0]	= DPTX_ALPM_MODE_ENTER;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_AlpmExit(u8 channel)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL;
	sFunction.ctrl[0]	= DPTX_ALPM_MODE_EXIT;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_AlpmEnable(u8 channel, u8 mode)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	if(mode > 1)		return result;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL;
	sFunction.ctrl[0]	= DPTX_ALPM_MODE_ENABLE;
	if(mode)			sFunction.ctrl[0] |= 0x10000;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_AlpmDisable(u8 channel)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL;
	sFunction.ctrl[0]	= DPTX_ALPM_MODE_DISABLE;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_AlpmLinkRateChange(u8 channel, u32 lrc_enable, u32 lrc_mode, u32 lrc_frame_period)
{
	u8	result = 0, rtn;
	u32	regData;
	dptxSFunction_t	sFunction;
	u8	chipSelect = 0;

	regData = ((lrc_frame_period << 8) & 0x0000FF00) | ((lrc_mode & 0X01) << 1) | ((lrc_enable & 0X01) << 0);

	chipSelect = channel - 1;

	rtn = DPC_DirectRegWriteSingle(chipSelect, CORE_REG_DP_LRC, regData);
	if(rtn == 0)							return result;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL;
	sFunction.ctrl[0]	= DPTX_ALPM_MODE_LINKRATE_CHANGE;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DPTX_LtAutoVideoOutput(u8 channel, u8 autoVideoOutput)
{
	u8	result = 0;
	dptxSFunction_t	sFunction;

	sFunction.channel	= channel;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_LTDONE_AUTO_VIDEO_OUTPUT;
	sFunction.ctrl[0]	= autoVideoOutput;

	result = DPTX_SpecialFunction(&sFunction);

	return result;
}

u8 DP_VVB(u8 chipSelect, u32 common, u32 hz)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	regData;
	u32	delay = 100;
	
	regData = hz;

	rtn = DPC_DirectRegWriteSingle(common, CORE_REG_DP_TX_VVB, regData);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessStart(common, CORE_REG_REGION_COMMON_INITIALIZE);
	if(rtn == 0)							errCnt += 1;

	rtn = DP_ProcessCheck(common, delay);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}
