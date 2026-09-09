#include	"App_DisplayPortCore.h"
u8 hpd_event[2] = {0, 0};

u8 DPC_Init()
{
	u8	result = 0, rtn, errCnt = 0;

//	FPGA_WriteSingle(FPGA_CMD_CORE_NRESET, 0);

//	HAL_Delay(10);

	rtn = ExtGPIO_WritePin(2, ON); // Core FPGA 1 POWER ON, pin = extGpio[2]
	if(rtn ==0)								errCnt += 1;
	rtn = ExtGPIO_WritePin(3, ON); // Core FPGA 2 POWER ON, pin = extGpio[3]
	if(rtn ==0)								errCnt += 1;

//	HAL_Delay(100);

//	FPGA_WriteSingle(FPGA_CMD_CORE_NRESET, 1);

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_DirectRegWrite(u8 position, u16 adrs, u32 *pData, u16 size)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	cmd;
	u16	regAdrs;

	if(position >= 4)						return result;

	if(size == 0)							return result;
	else if(size == 1)						cmd = DPC_SPI_REG_WRITE_SINGLE;
	else if(size > 255)						return result;
	else									cmd = DPC_SPI_REG_WRITE_BURST;

	rtn = CoreSPI_CsCtrl(position, LOW);
	if(rtn == 0)							errCnt += 1;

	rtn = CoreSPI_TransmitData(&cmd, 1);
	if(rtn == 0)							errCnt += 1;

	regAdrs = (adrs << 2) & ~0x0003;
	rtn = CoreSPI_TransmitData((u8*)&regAdrs, 2);
	if(rtn == 0)							errCnt += 1;

	rtn = CoreSPI_TransmitData((u8*)pData, (size * 4));
	if(rtn == 0)							errCnt += 1;

	rtn = CoreSPI_CsCtrl(position, HIGH);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_DirectRegRead(u8 position, u16 adrs, u32 *pData, u16 size)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	cmd;
	u16	regAdrs;

	if(position >= 4)						return result;

	if(size == 0)							return result;
	else if(size == 1)						cmd = DPC_SPI_REG_READ_SINGLE;
	else if(size > 255)						return result;
	else									cmd = DPC_SPI_REG_READ_BURST;

	rtn = CoreSPI_CsCtrl(position, LOW);
	if(rtn == 0)							errCnt += 1;

	rtn = CoreSPI_TransmitData(&cmd, 1);
	if(rtn == 0)							errCnt += 1;

	regAdrs = (adrs << 2) & ~0x0003;
	rtn = CoreSPI_TransmitData((u8*)&regAdrs, 2);
	if(rtn == 0)							errCnt += 1;

	if(cmd == DPC_SPI_REG_READ_BURST)
	{
		rtn = CoreSPI_TransmitData((u8*)&size, 1);
		if(rtn == 0)						errCnt += 1;
	}

	rtn = CoreSPI_ReceiveData((u8*)pData, (size * 4));
	if(rtn == 0)							errCnt += 1;

	rtn = CoreSPI_CsCtrl(position, HIGH);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_DirectRegWriteSingle(u8 position, u16 adrs, u32 data)
{
	return DPC_DirectRegWrite(position, adrs, &data, 1);
}

u8 DPC_DirectRegReadSingle(u8 position, u16 adrs, u32 *pData)
{
	return DPC_DirectRegRead(position, adrs, pData, 1);
}

u8 DPC_RegWrite(u8 position, u16 adrs, u32 *pData, u16 size)
{
	u8	result = 0, rtn;
	u8	chipSelect;
	u16	wAdrs, baseAdrs;

	rtn = DP_GetChannelInfo(position, &chipSelect, &baseAdrs);
	if(rtn == 0)					return result;

	wAdrs = (baseAdrs | 0x00FF) & adrs;

	result = DPC_DirectRegWrite(chipSelect, wAdrs, pData, size);

	return result;
}

u8 DPC_RegRead(u8 position, u16 adrs, u32 *pData, u16 size)
{
	u8	result = 0, rtn;
	u8	chipSelect;
	u16	wAdrs, baseAdrs;

	rtn = DP_GetChannelInfo(position, &chipSelect, &baseAdrs);
	if(rtn == 0)					return result;

	wAdrs = (baseAdrs | 0x00FF) & adrs;

	result = DPC_DirectRegRead(chipSelect, wAdrs, pData, size);

	return result;
}

u8 DPC_RegWriteSingle(u8 position, u16 adrs, u32 data)
{
	return DPC_RegWrite(position, adrs, &data, 1);
}

u8 DPC_RegReadSingle(u8 position, u16 adrs, u32 *pData)
{
	return DPC_RegRead(position, adrs, pData, 1);
}

u8 DPC_GetFpgaVersion(u8 position, u8 *pData)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	type, channel, major, minor;
	u16	adrs;
	u32	regData;

	if(position >= DPC_CORE_MAX_COUNT)		return result;

	adrs = CORE_REG_FPGA_VERSION;
	rtn = DPC_DirectRegReadSingle(position, adrs, &regData);
	if(rtn == 0)							errCnt += 1;
	else
	{
		type	= (regData >> 24) & 0xFF;
		channel	= (regData >> 16) & 0xFF;
		major	= (regData >> 8) & 0xFF;
		minor	= (regData >> 0) & 0xFF;

		sprintf((char*)pData, "%02X.%d.%02X.%02X\0", type, channel, major, minor);
	}

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_GetFpgaBuild(u8 position, u8 *pData)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	year, month, day;
	u16	adrs;
	u32	regData;

	if(position >= DPC_CORE_MAX_COUNT)		return result;

	adrs = CORE_REG_FPGA_BUILD_DATE;
	rtn = DPC_DirectRegReadSingle(position, adrs, &regData);
	if(rtn == 0)							errCnt += 1;
	else
	{
		year	= (regData >> 16) & 0xFFFF;
		month	= (regData >> 8) & 0xFF;
		day		= (regData >> 0) & 0xFF;

		sprintf((char*)pData, "%04X.%02X.%02X\0", year, month, day);
	}

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_GetNiosVersion(u8 position, u8 *pData)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	major, minor1, minor2, type;
	u16	adrs;
	u32	regData;

	if(position >= DPC_CORE_MAX_COUNT)		return result;

	adrs = CORE_REG_NIOS_VERSION;
	rtn = DPC_DirectRegReadSingle(position, adrs, &regData);
	if(rtn == 0)							errCnt += 1;
	else
	{
		major	= ((regData >> 24) & 0xFF);
		minor1	= ((regData >> 16) & 0xFF);
		minor2	= ((regData >> 8) & 0xFF);
		type	= (regData >> 0) & 0xFF;

		sprintf((char*)pData, "%d.%d.%d.%c\0", major, minor1, minor2, type);
	}

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_GetNiosBuild(u8 position, u8 *pData)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	year, month, day;
	u16	adrs;
	u32	regData;

	if(position >= DPC_CORE_MAX_COUNT)		return result;

	adrs = CORE_REG_NIOS_BUILD_DATE;
	rtn = DPC_DirectRegReadSingle(position, adrs, &regData);
	if(rtn == 0)							errCnt += 1;
	else
	{
		year	= (regData >> 16) & 0xFFFF;
		month	= (regData >> 8) & 0xFF;
		day		= (regData >> 0) & 0xFF;

		sprintf((char*)pData, "%04X.%02X.%02X\0", year, month, day);
	}

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_CoreSoftReset(u8 channel)
{
	u8 	result = 0, rtn, errCnt = 0;
	
	rtn = DPC_DirectRegWriteSingle(channel, CORE_REG_SOFT_RESET, 0x01);
	if(rtn == 0)							errCnt += 1;

	HAL_Delay(2);

	rtn = DPC_DirectRegWriteSingle(channel, CORE_REG_SOFT_RESET, 0x00);
	if(rtn == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_MainLinkConfig(u8 channel, u8 position, u8 *pLaneOrder, u8 pnSwap, u8 voltRange)
{
	u8	result = 0, rtn, errCnt = 0;
	u8	evenOdd;
	u16	adrs;
	u32	regData, wData = 0;

	if(channel >= DPC_CORE_MAX_COUNT)			return result;
	if((position >= 5) && (position != 0xFF))	return result;

	for(u8 cnt = 0; cnt < 4; cnt++)
	{
		if(cnt != 0)	if(pLaneOrder[cnt] == pLaneOrder[0])	return result;
		if(cnt != 1)	if(pLaneOrder[cnt] == pLaneOrder[1])	return result;
		if(cnt != 2)	if(pLaneOrder[cnt] == pLaneOrder[2])	return result;
		if(cnt != 3)	if(pLaneOrder[cnt] == pLaneOrder[3])	return result;
	}

	evenOdd = position % 2;

	wData = 0;
	wData |= voltRange & 0x07;
	wData <<= 4;
	wData |= pnSwap & 0x0F;
	wData <<= 8;
	wData |= ((pLaneOrder[3] & 0x03) << 6) | ((pLaneOrder[2] & 0x03) << 4) | ((pLaneOrder[1] & 0x03) << 2) | ((pLaneOrder[0] & 0x03) << 0);

	adrs = CORE_REG_DP_TX_LANE_CONFIG_0;

	if(position == 0xFF)
	{
		regData = (wData << 16) | (wData << 0);

		rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
		if(rtn == 0)								errCnt += 1;

		rtn = DPC_DirectRegWriteSingle(channel, adrs + 1, regData);
		if(rtn == 0)								errCnt += 1;

		rtn = DPC_DirectRegWriteSingle(channel, adrs + 2, regData);
		if(rtn == 0)								errCnt += 1;
	}
	else
	{
		if(position >= 2)							adrs += 1;
		if(position >= 4)							adrs += 1;

		rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
		if(rtn == 0)								errCnt += 1;

		if(evenOdd)			regData = (regData & 0x0000FFFF) | (wData << 16);
		else				regData = (regData & 0xFFFF0000) | (wData << 0);

		rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
		if(rtn == 0)								errCnt += 1;
	}

	if(errCnt == 0)									result = 1;

	return result;
}

u8 DPC_NiosWorkingPeriod(u8 channel, u32 *pPeriod)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;

	if(channel >= DPC_CORE_MAX_COUNT)		return result;

	adrs = CORE_REG_NIOS_WORKING_PERIOD;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)							errCnt += 1;
	else									*pPeriod = regData;

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_GetTemperature(u8 channel, float *pTemperature)
{
	u8	result = 0, rtn, errCnt = 0;
	u16	adrs;
	u32	regData;

	if(channel >= DPC_CORE_MAX_COUNT)		return result;

	adrs = CORE_REG_TEMPERATURE_FLOAT;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)							errCnt += 1;
	else									*pTemperature = SYS_HexToFloat((u8*)&regData);

	if(errCnt == 0)							result = 1;

	return result;
}

u8 DPC_UpdateCheck(u8 channel, u8 *data)
{
	u8	result = 0, rtn;
    u32 mask = 0x00001000;
	u32	regData;

	rtn = DPC_DirectRegReadSingle(channel, CORE_REG_FPGA_VERSION, &regData);
	if(rtn == 0)							return result;

	regData &= mask;
	*data = (u8)(regData >> 12);

	result = 1;

	return result;
}


u8 HPDEVENT_Enable(u8 channel, u8 enable)
{
	u8	result = 0, rtn;
	u8	errCnt = 0;
	u8	enable_check;
	u16	adrs;
	u32	regData;
	u32	mask = 0x00000002;

	if(enable == 0)	enable_check = 1;
	else			enable_check = 2;

	hpd_event[channel-2] = enable_check-1;

	adrs = CORE_REG_ADDR_CORE_IRQ_STATUS;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	regData &= ~mask;
	regData |= (enable & mask);

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 EXT_CLK_CNT_START(u8 channel, u32 time)
{
	u8	result = 0, rtn;
	u8	errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	mask = 0x0FFF0001;

	adrs = CORE_REG_ADDR_EX_CLK_CNT_START;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	regData &= ~mask;
	regData |= ((time << 16) | 0x00000001);

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 EXT_CLK_CNT_VALUE(u8 channel, float *cnt_value)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	regData, period, time;
#if 0
	adrs = CORE_REG_ADDR_EX_CLK_CNT_VALUE1;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			return result;

	*cnt_value = regData;
#endif
#if 1
	// read current period
	adrs = CORE_REG_ADDR_EX_CLK_CNT_VALUE3;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			return result;

	period = regData;

	// calculate pulse count
	adrs = CORE_REG_ADDR_EX_CLK_CNT_START;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			return result;

	time = (regData >> 16) & 0x0FFF;	// time(0.1sec)

	*cnt_value = (float)(time * 100 * 1000) / period;	// period(usec)
#endif
	result = 1;

	return result;
}

u8 EXT_CLK_PERIOD(u8 channel, u32 *period)
{
	u8	result = 0, rtn;
	u16	adrs;
	u32	regData;

	adrs = CORE_REG_ADDR_EX_CLK_CNT_VALUE3;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			return result;

	*period = regData;

	result = 1;

	return result;
}

u8 PPA_Enable(u8 channel)
{
	u8	result = 0, rtn;
	u8	errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	mask = 0x00010000;

	adrs = 0x0029;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	regData &= ~mask;
	regData |= 0x00010000;

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 PPA_Disable(u8 channel)
{
	u8	result = 0, rtn;
	u8	errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	mask = 0x00010000;

	adrs = 0x0029;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	regData &= ~mask;

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 PPA_Mode(u8 channel, u8 pnl_mode, u8 simple_en, u8 green, u8 avg_mode, u8 end_mode)
{
	u8	result = 0, rtn;
	u8	errCnt = 0;
	u16	adrs;
	u32	regData;
	u32	mask = 0x003E0000;

	adrs = 0x0029;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	regData &= ~mask;
	regData |= (pnl_mode << 17) | (simple_en << 18) | (green << 19) | (avg_mode << 20) | (end_mode << 21);

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 OSD_Enable(u8 channel, u8 enable)
{
	u8	result = 0, rtn;
	u8	errCnt = 0;
	u32	regData, mask = 0x0000003F;
	u16	adrs;

	adrs = 0x15;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	if(enable)
	{
		regData |= (enable << 0);
	}
	else
	{
		regData &= ~mask;
	}

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 OSD_Setting(u8 channel, u16 red, u16 green, u16 blue, u16 size, u16 thick)
{
	u8	result = 0, rtn;
	u8	errCnt = 0;
	u32	regData, mask;
	u16	adrs;

	// Color
	adrs = 0x16;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	mask = 0x00FFFFFF;
	regData &= ~mask;
	regData |= (red << 16) | (green << 8) | (blue << 0);

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	// Size & Thick
	adrs = 0x17;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	mask = 0x0000FFFF;
	regData &= ~mask;
	regData |= (thick << 12) | (size << 0);

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 OSD_Box(u8 channel, u8 box, u16 xpos, u16 ypos)
{
	u8	result = 0, rtn;
	u8	errCnt = 0;
	u32	regData;
	u16	adrs;

	adrs = 0x15;
	rtn = DPC_DirectRegReadSingle(channel, adrs, &regData);
	if(rtn == 0)			errCnt += 1;

	regData |= (1 << box + 1);

	rtn = DPC_DirectRegWriteSingle(channel, adrs, regData);
	if(rtn == 0)			errCnt += 1;

	adrs = 0x18;
	rtn = DPC_DirectRegReadSingle(channel, adrs + box, &regData);
	if(rtn == 0)			errCnt += 1;

	regData = (ypos << 16) | (xpos << 0);

	rtn = DPC_DirectRegWriteSingle(channel, adrs + box, regData);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}