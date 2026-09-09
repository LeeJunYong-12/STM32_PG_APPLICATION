#include	"App_Power.h"

u8 PWR_M_GetVersion(u8 position, u8 *pData)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.waitTime		= 100;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_VERSION;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize == 0)							return result;

	memcpy(pData, sbParsingData.pRxData, sbParsingData.rxSize);

	result = 1;

	return result;
}

u8 PWR_M_GetBuildDate(u8 position, u8 *pData)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.waitTime		= 100;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_BUILD_DATE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize == 0)							return result;

	memcpy(pData, sbParsingData.pRxData, sbParsingData.rxSize);

	result = 1;

	return result;
}

u8 PWR_FirmwareType(u8 position, u8 *pData, u16 *pSize)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_FW_TYPE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize == 0)							return result;

	*pSize = sbParsingData.rxSize;

	memcpy(pData, sbParsingData.pRxData, sbParsingData.rxSize);

	result = 1;

	return result;
}

u8 PWR_EthernetStatus(u8 position, u8 status)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_ETHERNET_STATUS;

	sbParsingData.pTxData[sbParsingData.txSize] = status;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_EthernetConfig(u8 position, u16 mode, u8 *pMAC, u8 *pIP, u8 *pNM, u8 *pGW, u16 port)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_ETHERNET_CONFIG;

	SYS_HWordToHex(mode, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize += 2;

	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pMAC, 6);
	sbParsingData.txSize += 6;

	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pIP, 6);
	sbParsingData.txSize+= 4;

	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pNM, 6);
	sbParsingData.txSize += 4;

	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pGW, 6);
	sbParsingData.txSize += 4;

	SYS_HWordToHex(port, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize += 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_Reboot(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_BOARD_REBOOT;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_OutputInit(u8 position, u8 type)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	if(type == PWR_INIT_TYPE_ALL)
	{
		sbParsingData.channel		= position;
		sbParsingData.cmd			= SB_CMD_PWR_M_BOARD_INIT;
	}
	else
	{
		sbParsingData.channel		= position;
		sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_INIT;

		sbParsingData.pTxData[sbParsingData.txSize]	= type;
		sbParsingData.txSize		+= 1;
	}

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_OutputVoltage(u8 position, u8 pwrSrc, double voltage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_SET_VOLTAGE;

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

u8 PWR_OutputCurrent(u8 position, u8 pwrSrc, double current)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_SET_CURRENT;

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

u8 PWR_MeasureRange(u8 position, u8 pwrSrc, u8 range)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_SET_RANGE;

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

u8 PWR_OutputEnable(u8 position, u8 pwrSrc, u8 enable)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_SET_ENABLE;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= enable;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_MesureVI(u8 position, u8 pwrSrc, double *pVoltage, double *pCurrent)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_MEASUREMENT;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 8)							return result;

	*pVoltage = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	*pCurrent = (double)SYS_HexToFloat(&sbParsingData.pRxData[4]);

	result = 1;

	return result;
}

u8 PWR_MesureAutoVI(u8 position, u8 pwrSrc, double *pVoltage, double *pCurrent)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_MEASUREMENT_AUTO;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 8)							return result;

	*pVoltage = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	*pCurrent = (double)SYS_HexToFloat(&sbParsingData.pRxData[4]);

	result = 1;

	return result;
}

u8 PWR_ExtVoltageMeasure(u8 position, u8 mode, double *pVoltage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_EXT_VOLTAGE_MEASUREMENT;

	sbParsingData.pTxData[sbParsingData.txSize]	= mode;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*pVoltage = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_ReadAlarm(u8 position, u8 *valarm, u8 *ialarm)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_READ_ALARM;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 8)							return result;

	*valarm = (u8)SYS_HexToWord(&sbParsingData.pRxData[0]);

	*ialarm = (u8)SYS_HexToWord(&sbParsingData.pRxData[4]);

	result = 1;

	return result;
}

u8 PWR_VfbState(u8 position, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_VFB_ENABLE;

	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_CalState(u8 position, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_STATUS;

	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_CalOutputVoltage(u8 position, u8 pwrSrc, double *pIdeal, double *pReal)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_OUTPUT_VOLTAGE;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	SYS_FloatToHex(pIdeal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pReal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pIdeal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pReal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_CalPMICCurrent(u8 position, u8 pwrSrc, double *pIdeal, double *pReal)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_PMIC_CURRENT;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	SYS_FloatToHex(pIdeal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pReal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pIdeal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pReal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}


u8 PWR_CalPMICVoltage(u8 position, u8 pwrSrc, double *pIdeal, double *pReal)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_PMIC_VOLTAGE;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	SYS_FloatToHex(pIdeal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pReal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pIdeal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pReal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}


u8 PWR_CalMeasureVoltage(u8 position, u8 pwrSrc, double *pIdeal, double *pReal)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_MEASURE_VOLTAGE;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	SYS_FloatToHex(pIdeal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pReal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pIdeal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex(pReal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_CalMeasureCurrent(u8 position, u8 pwrSrc, u8 src, double *pIdeal, double *pReal)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_MEASURE_CURRENT;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	sbParsingData.pTxData[sbParsingData.txSize]	= src;
	sbParsingData.txSize		+= 1;

	SYS_FloatToHex((float)pIdeal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex((float)pReal[0], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex((float)pIdeal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex((float)pReal[1], &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_CalSetData(u8 position, u8 pwrSrc, u8 src, double gain, double offset)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_SET_DATA;

	sbParsingData.pTxData[sbParsingData.txSize] = pwrSrc;
	sbParsingData.txSize		+= 1;

	sbParsingData.pTxData[sbParsingData.txSize] = src;
	sbParsingData.txSize		+= 1;

	SYS_FloatToHex((float)gain, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	SYS_FloatToHex((float)offset, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_CalGetData(u8 position, u8 pwrSrc, u8 src, double *pGain, double *pOffset)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_GET_DATA;

	sbParsingData.pTxData[sbParsingData.txSize] = pwrSrc;
	sbParsingData.txSize		+= 1;

	sbParsingData.pTxData[sbParsingData.txSize] = src;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 8)							return result;

	*pGain		= (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);
	*pOffset	= (double)SYS_HexToFloat(&sbParsingData.pRxData[4]);

	result = 1;

	return result;
}

u8 PWR_ExtRange(u8 position, double range)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_CAL_EXT_VALUE;

	SYS_FloatToHex((float)range, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_Vssel(u8 position, double range, double *current)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_VSSEL_CURRENT;

	SYS_FloatToHex((float)range, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*current = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_Vssel_Voltage(u8 position, double *voltage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_VSSEL_VOLTAGE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*voltage = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_Vgl(u8 position, double range, double *current)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_VGL_CURRENT;

	SYS_FloatToHex((float)range, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*current = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_Vgl_Voltage(u8 position, double *voltage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_VGL_VOLTAGE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*voltage = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_avddh_current(u8 position, double range, double *current)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_AVDDH_CURRENT;

	SYS_FloatToHex((float)range, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*current = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_avddh_voltage(u8 position, double *voltage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_AVDDH_VOLTAGE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*voltage = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_vddel_voltage(u8 position, double *voltage)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_CAL_VDDEL_VOLTAGE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*voltage = (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_FactorSetData(u8 position, u8 pwrSrc, u8	src, double factor)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_FACTOR_SET_DATA;

	sbParsingData.pTxData[sbParsingData.txSize]	= pwrSrc;
	sbParsingData.txSize		+= 1;

	sbParsingData.pTxData[sbParsingData.txSize]	= src;
	sbParsingData.txSize		+= 1;

	SYS_FloatToHex((float)factor, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize		+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_FactorGetData(u8 position, u8 pwrSrc, u8 src, double *pFactor)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_FACTOR_GET_DATA;

	sbParsingData.pTxData[sbParsingData.txSize] = pwrSrc;
	sbParsingData.txSize		+= 1;

	sbParsingData.pTxData[sbParsingData.txSize] = src;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 4)							return result;

	*pFactor		= (double)SYS_HexToFloat(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_DataClear(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_DATA_CLEAR;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_DataLoad(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_DATA_LOAD;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_DataSave(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_DATA_SAVE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_BootEnter(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)					return result;

	sbParsingData.waitTime			= 3000;

	sbParsingData.channel			= position;
	sbParsingData.cmd				= SB_CMD_PWR_BOOT;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)					return result;

	if(sbParsingData.rxSize != 0)	return result;

	result = 1;

	return result;
}

u8 PWR_BootErase(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)					return result;

	sbParsingData.waitTime			= 6000; // EXISTING 6000

	sbParsingData.channel			= position;
	sbParsingData.cmd				= SB_CMD_PWR_ERASE;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)					return result;

	if(sbParsingData.rxSize != 0)	return result;

	result = 1;

	return result;
}

u8 PWR_BOOTCHECK(u8 position, u32 *status)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)					return result;

	sbParsingData.waitTime			= 600; // EXISTING 6000

	sbParsingData.channel			= position;
	sbParsingData.cmd				= SB_CMD_PWR_FILE_CHECK;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)					return result;

	if(sbParsingData.rxSize != 4)	return result;

	*status = SYS_HexToWord(&sbParsingData.pRxData[0]);

	result = 1;

	return result;
}

u8 PWR_EEPROMSET(u8 position, u32 status)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)					return result;

	sbParsingData.waitTime			= 600; // EXISTING 6000

	sbParsingData.channel			= position;
	sbParsingData.cmd				= SB_CMD_PWR_EEPROM_SET;

	SYS_WordToHex(status, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize 							+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)					return result;

	if(sbParsingData.rxSize != 0)	return result;

	result = 1;

	return result;
}

u8 CheckAlarmFlag(u8 position, u8 *valarm, u8 *ialarm, u8 *error)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_CHECK_ALARM_FLAG;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 12)							return result;

	*valarm = (u8)SYS_HexToWord(&sbParsingData.pRxData[0]);

	*ialarm = (u8)SYS_HexToWord(&sbParsingData.pRxData[4]);

	*error = (u8)SYS_HexToWord(&sbParsingData.pRxData[8]);

	result = 1;

	return result;
}

u8 PWR_SetError(u8 position, u8 error)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_CHECK_ALARM_CNT;

	sbParsingData.pTxData[sbParsingData.txSize]	= error;
	sbParsingData.txSize		+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}

u8 PWR_ADCcount(u8 position, u8 maxcount, u8 delcount)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)											return result;

	sbParsingData.channel		= position;
	sbParsingData.cmd			= SB_CMD_PWR_M_MODULE_ADC_COUNT;

	sbParsingData.pTxData[sbParsingData.txSize]	= maxcount;
	sbParsingData.txSize		+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= delcount;
	sbParsingData.txSize		+= 1;	

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)											return result;

	if(sbParsingData.rxSize != 0)							return result;

	result = 1;

	return result;
}
