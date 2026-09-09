#include	"App_Extension.h"

static	extensionFactor_t	extensionFactor[BOARD_CHANNEL_MAX_COUNT];

u8 Extension_Init(u8 position, u8	channel)
{
	u8	result = 0, rtn;

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;
	if(channel >= 2)								return result;

	extensionFactor[position].i2cChannel			= channel;

	rtn = LCM_I2CInit(position, extensionFactor[position].i2cChannel);
	if(rtn != 0)									result = 1;

	if(result == 1)									extensionFactor[position].initialized = 1;

	return result;
}

u8 Extension_Deinit(u8 position)
{
	u8	result = 0, rtn;

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].i2cChannel >= 2)
	{
		result = 1;

		return result;
	}

	rtn = LCM_I2CDeInit(position, extensionFactor[position].i2cChannel);
	if(rtn != 0)									result = 1;

	extensionFactor[position].initialized = 0;

	return result;
}

u8 Extension_TCA6424A_RegWrite(u8 position, u8 cmd, u8 data)
{
	u8	result = 0;
	u8	devAdrs = 0x44;
	u8	wData[4];

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].initialized == 0)	return result;

	memset(wData, NULL, sizeof(wData));

	wData[0] = cmd;
	wData[1] = data;

	result = LCM_I2CTransmitData(position, extensionFactor[position].i2cChannel, devAdrs, wData, 2);

	return result;
}

u8 Extension_TCA6424A_RegRead(u8 position, u8 cmd, u8 *pData)
{
	u8	result = 0;
	u8	devAdrs = 0x44;

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].initialized == 0)	return result;

	result = LCM_I2CMemRead(position, extensionFactor[position].i2cChannel, devAdrs, cmd, 0, pData, 1);

	return result;
}

u8 Extension_TCA6424A_Direction(u8 position, u8 port, u8 data)
{
	u8	result = 0;
	u8	cmd, regData;

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].initialized == 0)	return result;

	switch(port)
	{
		case 0:
			cmd = EXTENSION_TCA6424A_CMD_CONFIG_0;
			break;

		case 1:
			cmd = EXTENSION_TCA6424A_CMD_CONFIG_1;
			break;

		case 2:
			cmd = EXTENSION_TCA6424A_CMD_CONFIG_2;
			break;

		default:
			return result;
			break;
	}

	regData = ~data;

	result = Extension_TCA6424A_RegWrite(position, cmd, regData);

	return result;
}

u8 Extension_TCA6424A_OutputWrite(u8 position, u8 port, u8 data)
{
	u8	result = 0;
	u8	cmd, regData;

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].initialized == 0)	return result;

	switch(port)
	{
		case 0:
			cmd = EXTENSION_TCA6424A_CMD_OUTPUT_PORT_0;
			break;

		case 1:
			cmd = EXTENSION_TCA6424A_CMD_OUTPUT_PORT_1;
			break;

		case 2:
			cmd = EXTENSION_TCA6424A_CMD_OUTPUT_PORT_2;
			break;

		default:
			return result;
			break;
	}

	regData = data;

	result = Extension_TCA6424A_RegWrite(position, cmd, regData);

	return result;
}

u8 Extension_TCA6424A_OutputRead(u8 position, u8 port, u8 *pData)
{
	u8	result = 0;
	u8	cmd;

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].initialized == 0)	return result;

	switch(port)
	{
		case 0:
			cmd = EXTENSION_TCA6424A_CMD_OUTPUT_PORT_0;
			break;

		case 1:
			cmd = EXTENSION_TCA6424A_CMD_OUTPUT_PORT_1;
			break;

		case 2:
			cmd = EXTENSION_TCA6424A_CMD_OUTPUT_PORT_2;
			break;

		default:
			return result;
			break;
	}

	result = Extension_TCA6424A_RegRead(position, cmd, pData);

	return result;
}

u8 Extension_TCA6424A_InputRead(u8 position, u8 port, u8 *pData)
{
	u8	result = 0;
	u8	cmd;

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].initialized == 0)	return result;

	switch(port)
	{
		case 0:
			cmd = EXTENSION_TCA6424A_CMD_INPUT_PORT_0;
			break;

		case 1:
			cmd = EXTENSION_TCA6424A_CMD_INPUT_PORT_1;
			break;

		case 2:
			cmd = EXTENSION_TCA6424A_CMD_INPUT_PORT_2;
			break;

		default:
			return result;
			break;
	}

	result = Extension_TCA6424A_RegRead(position, cmd, pData);

	return result;
}

u8 Extension_LTC2633_RegWrite(u8 position, u8 cmd, u16 data)
{
	u8	result = 0;
	u8	devAdrs = 0x20;
	u8	wData[4];

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].initialized == 0)	return result;

	memset(wData, NULL, sizeof(wData));

	wData[0] = cmd;
	wData[1] = (u8)(data >> 8);
	wData[2] = (u8)(data >> 0);

	result = LCM_I2CTransmitData(position, extensionFactor[position].i2cChannel, devAdrs, wData, 3);

	return result;
}

u8 Extension_LTC2633_RefCtrl(u8 position, u8 data)
{
	u8	result = 0;
	u8	cmd;
	u16	regData;

	if(position >= BOARD_CHANNEL_MAX_COUNT)			return result;

	if(extensionFactor[position].initialized == 0)	return result;

	if(data == 0)									cmd = EXTENSION_LTC2633_CMD_SELECT_INTERNAL_REF;
	else if(data == 1)								cmd = EXTENSION_LTC2633_CMD_SELECT_EXTERNAL_REF;
	else											return result;

	cmd <<= 4;

	cmd |= EXTENSION_LTC2633_ADRS_DAC_ALL;

	regData = 0;

	result = Extension_LTC2633_RegWrite(position, cmd, regData);

	return result;
}

u8 Extension_LTC2633_PowerDown(u8 position, u8 channel)
{
	u8	result = 0;
	u8	cmd;
	u16	regData;

	if(position >= BOARD_CHANNEL_MAX_COUNT)				return result;

	if(extensionFactor[position].initialized == 0)		return result;

	cmd = EXTENSION_LTC2633_CMD_POWER_DOWN;
	cmd <<= 4;

	if(channel == EXTENSION_LTC2633_ADRS_DAC_A)			cmd |= EXTENSION_LTC2633_ADRS_DAC_A;
	else if(channel == EXTENSION_LTC2633_ADRS_DAC_B)	cmd |= EXTENSION_LTC2633_ADRS_DAC_B;
	else if(channel == EXTENSION_LTC2633_ADRS_DAC_ALL)	cmd |= EXTENSION_LTC2633_ADRS_DAC_ALL;
	else 												return result;

	regData = 0;

	result = Extension_LTC2633_RegWrite(position, cmd, regData);

	return result;
}

u8 Extension_LTC2633_VoltCtrl(u8 position, u8 channel, double volt)
{
	u8	result = 0;
	u8	cmd;
	u16	regData;
	double	dacValue, refVolt, resolution;

	if(position >= BOARD_CHANNEL_MAX_COUNT)				return result;

	if(extensionFactor[position].initialized == 0)		return result;

	cmd = EXTENSION_LTC2633_CMD_WRITE_AND_UPDATE;
	cmd <<= 4;

	if(channel == EXTENSION_LTC2633_ADRS_DAC_A)			cmd |= EXTENSION_LTC2633_ADRS_DAC_A;
	else if(channel == EXTENSION_LTC2633_ADRS_DAC_B)	cmd |= EXTENSION_LTC2633_ADRS_DAC_B;
	else if(channel == EXTENSION_LTC2633_ADRS_DAC_ALL)	cmd |= EXTENSION_LTC2633_ADRS_DAC_ALL;
	else												return result;

	if(volt > 2.5)		return result;
	if(volt < 0)		return result;

	refVolt		= 2.5;
	resolution	= 65535;

	dacValue	= volt / (refVolt / resolution);

	regData		= (u16)dacValue;

	result = Extension_LTC2633_RegWrite(position, cmd, regData);

	return result;
}

u8 Extension_E5ETBOARD_VoltOutput(u8 position, u8 channel, double volt)
{
	u8	result = 0;
	double	vfb, r1, r2, r3;
	double	vMax, vConstant;
	double	dacVolt;

	if(position >= BOARD_CHANNEL_MAX_COUNT)				return result;

	if(extensionFactor[position].initialized == 0)		return result;

	vfb	= 0.6;
	r1	= 1000000;
	r2	= 180000;
	r3	= 665000;

	vMax		= vfb * (1 + (r1 + r2));
	vConstant	= r1 / r3;

	dacVolt		= vMax - volt;
	dacVolt		/= vConstant;
	dacVolt		+= vfb;

	result = Extension_LTC2633_VoltCtrl(position, channel, dacVolt);

	return result;
}


