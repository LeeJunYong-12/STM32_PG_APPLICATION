#include	"App_ExtCtrl.h"

static	u32						extctrlFactorInfo[BOARD_CHANNEL_MAX_COUNT];
static	u32						extctrlChannel = 0;
static	configPacket_t			cPacket;
static	extctrlFactor_t			extctrlFactor[BOARD_CHANNEL_MAX_COUNT];

static u8 ExtCtrl_ChannelSelect(u8 channel)
{
	u8	result = 0;

	if(channel >= BOARD_CHANNEL_MAX_COUNT)		return result;

	extctrlChannel = channel;

	result = 1;

	return result;
}

static u8 ExtCtrl_EEPROMWrite(u8 *pData, u16 size)
{
	u8	result = 0, rtn;
	u8	devAdrs = 0xA0, mask = 0xF0;
	u16	adrs = 0, cnt = 0, pageSize = 16, remainSize = size;
	u32	delaySize = 5;

	do{
		if(remainSize > pageSize)
		{
			devAdrs &= mask;
			devAdrs |= ((adrs + cnt) >> 7) & 0x0E;

			rtn = LCM_I2CMemWrite(extctrlChannel, 1, devAdrs, (u8)(adrs + cnt), 0, &pData[cnt], pageSize);
			if(rtn == 0)		return result;

			cnt			+= pageSize;
			remainSize	-= pageSize;
		}
		else
		{
			devAdrs &= mask;
			devAdrs |= ((adrs + cnt) >> 7) & 0x0E;

			rtn = LCM_I2CMemWrite(extctrlChannel, 1, devAdrs, (u8)(adrs + cnt), 0, &pData[cnt], remainSize);
			if(rtn == 0)		return result;

			break;
		}

		HAL_Delay(delaySize);
	}while(1);

	result = 1;

	return result;
}

static u8 ExtCtrl_EEPROMRead(u8 *pData, u16 size)
{
	u8	result = 0, rtn;
	u8	devAdrs = 0xA0, mask = 0xF0;
	u16	adrs = 0, cnt = 0, pageSize = 128, remainSize = size;

	do{
		if(remainSize > pageSize)
		{
			devAdrs &= mask;
			devAdrs |= ((adrs + cnt) >> 7) & 0x0E;

			rtn = LCM_I2CMemRead(extctrlChannel, 1, devAdrs, (u8)(adrs + cnt), 0, &pData[cnt], pageSize);
			if(rtn == 0)		return result;

			cnt			+= pageSize;
			remainSize	-= pageSize;
		}
		else
		{
			devAdrs &= mask;
			devAdrs |= ((adrs + cnt) >> 7) & 0x0E;

			rtn = LCM_I2CMemRead(extctrlChannel, 1, devAdrs, (u8)(adrs + cnt), 0, &pData[cnt], remainSize);
			if(rtn == 0)		return result;

			break;
		}
	}while(1);

	result = 1;

	return result;
}

static u8 ExtCtrl_ConfigPacketInit(u8 channel)
{
	u8	result = 0, rtn;
	configPacketHandle_t	handle;

	rtn = ExtCtrl_ChannelSelect(channel);
	if(rtn == 0)		return result;

	memset(&handle, NULL, sizeof(configPacketHandle_t));

	handle.pktType			= 0x00020000;
	handle.pktVersion		= 0x00010001;
	handle.WritePacket		= ExtCtrl_EEPROMWrite;
	handle.ReadPacket		= ExtCtrl_EEPROMRead;
	handle.pPacket			= &cPacket;

	result = CP_RegisterInterface(&handle);

	return result;
}

static u8 ExtCtrl_ConfigPacketDeInit()
{
	u8	result = 0;

	result = Config_Init();

	return result;
}

static u8 ExtCtrl_CalculateGainOffset(float *pIdeal, float *pReal, float *pGain, float *pOffset)
{
	u8	result = 0;
	float	gain, offset;
	float	ideal, real;

	ideal	= pIdeal[1] - pIdeal[0];
	real	= pReal[1] - pReal[0];

	if(real == 0)			return result;

	gain	= ideal / real;
	offset	= gain * pReal[0] - pIdeal[0];

	*pGain		= gain;
	*pOffset	= offset;

	result = 1;

	return result;
}

u8 ExtCtrl_FactorSave(u8 channel)
{
	u8	result = 0, rtn;

	rtn = ExtCtrl_ConfigPacketInit(channel);
	if(rtn == 0)			return result;

	rtn = CP_WritePacket(extctrlFactor[channel].rawData, sizeof(extctrlFactor_t));
	if(rtn != 0)			result = 1;

	rtn = ExtCtrl_ConfigPacketDeInit();

	return result;
}

u8 ExtCtrl_FactorLoad(u8 channel)
{
	u8	result = 0, rtn;

	rtn = ExtCtrl_ConfigPacketInit(channel);
	if(rtn == 0)			return result;

	rtn = CP_ReadPacket(extctrlFactor[channel].rawData, sizeof(extctrlFactor_t));
	if(rtn)
	{
		extctrlFactorInfo[channel] = 1;

		result = 1;
	}

	rtn = ExtCtrl_ConfigPacketDeInit();

	return result;
}

u8 ExtCtrl_FactorInit(u8 channel)
{
	u8	result = 0;
	u8	cnt;

	memset(extctrlFactor[channel].rawData, NULL, sizeof(extctrlFactor_t));

	extctrlFactor[channel].type		= 0xFF00FF00;
	extctrlFactor[channel].version	= 0x00FF00FF;

	extctrlFactor[channel].vMag		= 0.5;
	extctrlFactor[channel].iMag		= (50000/1247) + 1;

	for(cnt = 0; cnt < 4; cnt++)
	{
		extctrlFactor[channel].vGain[cnt]		= 1;
		extctrlFactor[channel].vOffset[cnt]		= 0;

		extctrlFactor[channel].iShunt[cnt]		= 0.1;
		extctrlFactor[channel].iGain[cnt]		= 1;
		extctrlFactor[channel].iOffset[cnt]		= 0;
	}

	extctrlFactorInfo[channel] = 1;

	result = 1;

	return result;
}

u8 ExtCtrl_FactorValueChange(u8 channel, u8 position, float value)
{
	u8	result = 0;

	if(channel >= BOARD_CHANNEL_MAX_COUNT)		return result;

	switch(position)
	{
		case EXTCTRL_FACTOR_VALUE_MAGNIFICATION_VOLTAGE:
			extctrlFactor[channel].vMag		= value;
			break;

		case EXTCTRL_FACTOR_VALUE_MAGNIFICATION_CURRENT:
			extctrlFactor[channel].iMag		= value;
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_1:
			extctrlFactor[channel].iShunt[0]	= value;
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_2:
			extctrlFactor[channel].iShunt[1]	= value;
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_3:
			extctrlFactor[channel].iShunt[2]	= value;
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_4:
			extctrlFactor[channel].iShunt[3]	= value;
			break;

		default:
			return result;
			break;
	}

	result = 1;

	return result;
}

u8 ExtCtrl_FactorValueRead(u8 channel, u8 position, float *pData)
{
	u8	result = 0;

	if(channel >= BOARD_CHANNEL_MAX_COUNT)		return result;

	switch(position)
	{
		case EXTCTRL_FACTOR_VALUE_MAGNIFICATION_VOLTAGE:
			*pData = extctrlFactor[channel].vMag;
			break;

		case EXTCTRL_FACTOR_VALUE_MAGNIFICATION_CURRENT:
			*pData = extctrlFactor[channel].iMag;
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_1:
			*pData = extctrlFactor[channel].iShunt[0];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_2:
			*pData = extctrlFactor[channel].iShunt[1];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_3:
			*pData = extctrlFactor[channel].iShunt[2];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_SHUNT_4:
			*pData = extctrlFactor[channel].iShunt[3];
			break;

		case EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_1:
			*pData = extctrlFactor[channel].vGain[0];
			break;

		case EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_2:
			*pData = extctrlFactor[channel].vGain[1];
			break;

		case EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_3:
			*pData = extctrlFactor[channel].vGain[2];
			break;

		case EXTCTRL_FACTOR_VALUE_VOLTAGE_GAIN_4:
			*pData = extctrlFactor[channel].vGain[3];
			break;

		case EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_1:
			*pData = extctrlFactor[channel].vOffset[0];
			break;

		case EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_2:
			*pData = extctrlFactor[channel].vOffset[1];
			break;

		case EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_3:
			*pData = extctrlFactor[channel].vOffset[2];
			break;

		case EXTCTRL_FACTOR_VALUE_VOLTAGE_OFFSET_4:
			*pData = extctrlFactor[channel].vOffset[3];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_1:
			*pData = extctrlFactor[channel].iGain[0];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_2:
			*pData = extctrlFactor[channel].iGain[1];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_3:
			*pData = extctrlFactor[channel].iGain[2];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_GAIN_4:
			*pData = extctrlFactor[channel].iGain[3];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_1:
			*pData = extctrlFactor[channel].iOffset[0];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_2:
			*pData = extctrlFactor[channel].iOffset[1];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_3:
			*pData = extctrlFactor[channel].iOffset[2];
			break;

		case EXTCTRL_FACTOR_VALUE_CURRENT_OFFSET_4:
			*pData = extctrlFactor[channel].iOffset[3];
			break;

		default:
			return result;
			break;
	}

	result = 1;

	return result;
}

u8 ExtCtrl_SetAdcConfig(u8 bChannel, u8 channel)
{
	u8	result = 0, rtn;
	u8	wData;

	if(channel >= 4)		return result;
	channel += 3;

	wData	= (channel << 5) & 0xe0;			//	Channel
	wData	&= ~(0x10);							//	Gain
	wData	|= (0x03 << 2) & 0x0C;				//	Data rate
	wData	&= ~(0x02);							//	Conversion mode
	wData	|= 0x01;							//	Voltage reference selection

	rtn = LCM_I2CMemWrite(bChannel, 0, 0x80, 0x40, 0, &wData, 1);
	if(rtn)		result = 1;

	return result;
}

u8 ExtCtrl_StartADC(u8 bChannel)
{
	u8	result = 0, rtn;
	u8	wData;

	wData = 0x08;

	rtn = LCM_I2CTransmitData(bChannel, 0, 0x80, &wData, 1);
	if(rtn)		result = 1;

	return result;
}

u8 ExtCtrl_GetReady(u8 bChannel, u8 *pState)
{
	u8	result = 0, rtn;
	u8	rData;

	rtn = LCM_I2CMemRead(bChannel, 0, 0x80, 0x21, 0, &rData, 1);
	if(rtn)		result = 1;

	if((rData & 0x80) != 0)		*pState = 1;
	else						*pState = 0;

	return result;
}

u8 ExtCtrl_GetData(u8 bChannel, s16 *pBuffer)
{
	u8	result = 0, rtn;
	u8	rData[4];
	bitCtrl16_t	rValue;

	rtn = LCM_I2CMemRead(bChannel, 0, 0x80, 0x10, 0, rData, 2);
	if(rtn)
	{
		rValue.u8Data[0] = rData[1];
		rValue.u8Data[1] = rData[0];

		*pBuffer = rValue.s16Data;

		result = 1;
	}

	return result;
}

u8 ExtCtrl_GetAdcVoltage(u8 bChannel, u8 channel, float *pVoltage)
{
	u8	result = 0, rtn, getState = 0;//, state = 0;
	s16	buffer;
//	u32	sTime, rTime;
	float	getVoltage, resolution = (65536 / 2), maxVoltage = 4.096;

	rtn = ExtCtrl_SetAdcConfig(bChannel, channel);
	if(rtn == 0)		return result;

	rtn = ExtCtrl_StartADC(bChannel);
	if(rtn == 0)		return result;

	HAL_Delay(2);

	getState = 1;

/*
	sTime = HAL_GetTick();
	do{
		rtn = ExtCtrl_GetReady(bChannel, &state);
		if(rtn == 0)				return result;

		if(state)
		{
			getState = 1;
			break;
		}

		rTime = HAL_GetTick();
		if((rTime - sTime) > 100)	break;
	}while(1);
*/
	if(getState)
	{
		rtn = ExtCtrl_GetData(bChannel, &buffer);
		if(rtn == 0)				return result;
	}
	else							return result;

	getVoltage = (float)buffer;

	getVoltage	*= (maxVoltage / resolution);

	*pVoltage = getVoltage;

	result = 1;

	return result;
}

u8 ExtCtrl_MuxInit(u8 bChannel)
{
	u8	result = 0, rtn;

	rtn = LCM_SpiDeInit(bChannel);
	if(rtn)			result = 1;

	return result;
}

u8 ExtCtrl_MuxCtrl(u8 bChannel, u8 mode)
{
	u8	result = 0, rtn;
	bitCtrl8_t	bitData;

	bitData.u8Data = 0;

	if(mode & 0x01)		bitData.b2	= 1;
	else				bitData.b2	= 0;

	if(mode & 0x02)		bitData.b1	= 1;
	else				bitData.b1	= 0;

	rtn = LCM_SpiIoWrite(bChannel, bitData.u8Data);
	if(rtn)			result = 1;

	return result;
}

u8 ExtCtrl_CalibrationApply(u8 bChannel, u8 position, float *pIdeal, float *pReal)
{
	u8	result = 0, rtn;
	u8	factorID;
	float	gain, offset;
	float	*pGain, *pOffset;

	switch(position)
	{
		case EXTCTRL_MEASURE_VOLTAGE_1:
			factorID	= 0;
			pGain		= &extctrlFactor[bChannel].vGain[factorID];
			pOffset		= &extctrlFactor[bChannel].vOffset[factorID];
			break;

		case EXTCTRL_MEASURE_VOLTAGE_2:
			factorID	= 1;
			pGain		= &extctrlFactor[bChannel].vGain[factorID];
			pOffset		= &extctrlFactor[bChannel].vOffset[factorID];
			break;

		case EXTCTRL_MEASURE_VOLTAGE_3:
			factorID	= 2;
			pGain		= &extctrlFactor[bChannel].vGain[factorID];
			pOffset		= &extctrlFactor[bChannel].vOffset[factorID];
			break;

		case EXTCTRL_MEASURE_CURRENT_1:
			factorID	= 0;
			pGain		= &extctrlFactor[bChannel].iGain[factorID];
			pOffset		= &extctrlFactor[bChannel].iOffset[factorID];
			break;

		case EXTCTRL_MEASURE_CURRENT_2:
			factorID	= 1;
			pGain		= &extctrlFactor[bChannel].iGain[factorID];
			pOffset		= &extctrlFactor[bChannel].iOffset[factorID];
			break;

		case EXTCTRL_MEASURE_CURRENT_3:
			factorID	= 2;
			pGain		= &extctrlFactor[bChannel].iGain[factorID];
			pOffset		= &extctrlFactor[bChannel].iOffset[factorID];
			break;

		case EXTCTRL_MEASURE_CURRENT_4:
			factorID	= 3;
			pGain		= &extctrlFactor[bChannel].iGain[factorID];
			pOffset		= &extctrlFactor[bChannel].iOffset[factorID];
			break;

		default:
			return result;
			break;
	}

	rtn = ExtCtrl_CalculateGainOffset(pIdeal, pReal, &gain, &offset);
	if(rtn)
	{
		*pGain		= gain;
		*pOffset	= offset;

		result = 1;
	}

	return result;
}

u8 ExtCtrl_MeasureVoltage(u8 bChannel, u8 position, float *pVoltage)
{
	u8	result = 0, rtn;
	u8	adChannel, factorID;
	float	getVoltage, outVoltage;

	if(extctrlFactorInfo[bChannel] == 0)		return result;

	switch(position)
	{
		case EXTCTRL_MEASURE_VOLTAGE_1:
			factorID	= 0;
			adChannel	= 1;
			break;

		case EXTCTRL_MEASURE_VOLTAGE_2:
			factorID	= 1;
			adChannel	= 2;
			break;

		case EXTCTRL_MEASURE_VOLTAGE_3:
			factorID	= 2;
			adChannel	= 3;
			break;

		default:
			return result;
			break;
	}

	rtn = ExtCtrl_GetAdcVoltage(bChannel, adChannel, &getVoltage);
	if(rtn == 0)		return result;

	rtn = ExtCtrl_GetAdcVoltage(bChannel, adChannel, &getVoltage);
	if(rtn == 0)		return result;

	getVoltage /= extctrlFactor[bChannel].vMag;

	outVoltage = (getVoltage * extctrlFactor[bChannel].vGain[factorID]) - extctrlFactor[bChannel].vOffset[factorID];

	pVoltage[0] = getVoltage;
	pVoltage[1] = outVoltage;

	result = 1;

	return result;
}

u8 ExtCtrl_MeasureCurrent(u8 bChannel, u8 position, float *pCurrent)
{
	u8	result = 0, rtn;
	u8	adChannel, factorID, muxValue;
	float	getCurrent, outCurrent;

	if(extctrlFactorInfo[bChannel] == 0)		return result;

	switch(position)
	{
		case EXTCTRL_MEASURE_CURRENT_1:
			factorID	= 0;
			adChannel	= 0;
			muxValue	= 0;
			break;

		case EXTCTRL_MEASURE_CURRENT_2:
			factorID	= 1;
			adChannel	= 0;
			muxValue	= 1;
			break;

		case EXTCTRL_MEASURE_CURRENT_3:
			factorID	= 2;
			adChannel	= 0;
			muxValue	= 2;
			break;

		case EXTCTRL_MEASURE_CURRENT_4:
			factorID	= 3;
			adChannel	= 0;
			muxValue	= 3;
			break;

		default:
			return result;
			break;
	}

	rtn = ExtCtrl_MuxCtrl(bChannel, muxValue);
	if(rtn == 0)				return result;

	HAL_Delay(7);

	rtn = ExtCtrl_GetAdcVoltage(bChannel, adChannel, &getCurrent);
	if(rtn == 0)				return result;

	rtn = ExtCtrl_GetAdcVoltage(bChannel, adChannel, &getCurrent);
	if(rtn == 0)				return result;

	getCurrent *= 1000;
	getCurrent /= extctrlFactor[bChannel].iShunt[factorID];
	getCurrent /= extctrlFactor[bChannel].iMag;

	outCurrent = (getCurrent * extctrlFactor[bChannel].iGain[factorID]) - extctrlFactor[bChannel].iOffset[factorID];

	pCurrent[0] = getCurrent;
	pCurrent[1] = outCurrent;

	result = 1;

	return result;
}

