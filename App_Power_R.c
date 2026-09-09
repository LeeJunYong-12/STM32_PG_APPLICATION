#include	"App_Power_R.h"

u8 PWR_R_Version(u8 *pData)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.waitTime		= 100;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_VERSION;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize == 0)							return result;

	memcpy(pData, sbParsingData.pRxData, sbParsingData.rxSize);

	result = 1;

	return result;
}

u8 PWR_R_BuildDate(u8 *pData)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;\

	sbParsingData.waitTime		= 100;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_BUILD_DATE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize == 0)							return result;

	memcpy(pData, sbParsingData.pRxData, sbParsingData.rxSize);

	result = 1;

	return result;
}

u8 PWR_R_FirmwareType(u8 *pData)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_FW_TYPE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize == 0)							return result;

	memcpy(pData, sbParsingData.pRxData, sbParsingData.rxSize);

	result = 1;

	return result;
}

u8 PWR_R_Reboot()
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_BOARD_REBOOT;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_R_OutputInit(u8 type)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	if(type == PWR_INIT_TYPE_ALL)
	{
		sbParsingData.channel		= 0;
		sbParsingData.cmd			= SB_CMD_PWR_R_BOARD_INIT;
	}
	else
	{
		sbParsingData.channel		= 0;
		sbParsingData.cmd			= SB_CMD_PWR_R_MODULE_INIT;

		sbParsingData.pTxData[sbParsingData.txSize]	= type;
		sbParsingData.txSize		+= 1;
	}

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_R_OutputVoltage(u8 pwrSrc, double voltage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_MODULE_SET_VOLTAGE;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;
	SYS_FloatToHex((float)voltage, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_R_OutputCurrent(u8 pwrSrc, double current)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_MODULE_SET_CURRENT;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;
	SYS_FloatToHex((float)current, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_R_MeasureRange(u8 pwrSrc, u8 range)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_MODULE_SET_RANGE;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= range;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_R_OutputEnable(u8 position, u8 pwrSrc, u8 enable)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_MODULE_SET_ENABLE;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= position;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= enable;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_R_MesureVI(u8 pwrSrc, double *pVoltage, double *pCurrent)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_MODULE_MEASUREMENT;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 32)							return result;

	for(u8 cnt = 0; cnt < 4; cnt++)
	{
		pVoltage[cnt] = (double)SYS_HexToFloat(&sbParsingData.pRxData[(cnt * 8) + 0]);
		pCurrent[cnt] = (double)SYS_HexToFloat(&sbParsingData.pRxData[(cnt * 8) + 4]);
	}

	result = 1;

	return result;
}

u8 PWR_R_ExtVoltageMeasure(u8 mode, double *pVoltage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= 0;
	sbParsingData.cmd			= SB_CMD_PWR_R_EXT_VOLTAGE_MEASUREMENT;

	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*pVoltage = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}


