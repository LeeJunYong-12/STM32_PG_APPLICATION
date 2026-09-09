#ifndef	_APP_EXTENSION_H
#define	_APP_EXTENSION_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u8	initialized;
	u8	i2cChannel;
}extensionFactor_t;

enum{
	EXTENSION_TCA6424A_CMD_INPUT_PORT_0		= 0x00,
	EXTENSION_TCA6424A_CMD_INPUT_PORT_1		= 0x01,
	EXTENSION_TCA6424A_CMD_INPUT_PORT_2		= 0x02,
	EXTENSION_TCA6424A_CMD_OUTPUT_PORT_0	= 0x04,
	EXTENSION_TCA6424A_CMD_OUTPUT_PORT_1	= 0x05,
	EXTENSION_TCA6424A_CMD_OUTPUT_PORT_2	= 0x06,
	EXTENSION_TCA6424A_CMD_POLARITY_0		= 0x08,
	EXTENSION_TCA6424A_CMD_POLARITY_1		= 0x09,
	EXTENSION_TCA6424A_CMD_POLARITY_2		= 0x0a,
	EXTENSION_TCA6424A_CMD_CONFIG_0			= 0x0c,
	EXTENSION_TCA6424A_CMD_CONFIG_1			= 0x0d,
	EXTENSION_TCA6424A_CMD_CONFIG_2			= 0x0e,
};

enum{
	EXTENSION_LTC2633_CMD_WRITE_INPUT_REG		= 0x00,
	EXTENSION_LTC2633_CMD_UPDATE				= 0x01,
	EXTENSION_LTC2633_CMD_WRITE_AND_UPDATE_ALL	= 0x02,
	EXTENSION_LTC2633_CMD_WRITE_AND_UPDATE		= 0x03,
	EXTENSION_LTC2633_CMD_POWER_DOWN			= 0x04,
	EXTENSION_LTC2633_CMD_POWER_DOWN_ALL		= 0x05,
	EXTENSION_LTC2633_CMD_SELECT_INTERNAL_REF	= 0x06,
	EXTENSION_LTC2633_CMD_SELECT_EXTERNAL_REF	= 0x07,
};

enum{
	EXTENSION_LTC2633_ADRS_DAC_A				= 0x00,
	EXTENSION_LTC2633_ADRS_DAC_B				= 0x01,
	EXTENSION_LTC2633_ADRS_DAC_ALL				= 0x0f,
};

u8 Extension_Init(u8 position, u8	channel);
u8 Extension_Deinit(u8 position);
u8 Extension_TCA6424A_RegWrite(u8 position, u8 cmd, u8 data);
u8 Extension_TCA6424A_RegRead(u8 position, u8 cmd, u8 *pData);
u8 Extension_TCA6424A_Direction(u8 position, u8 port, u8 data);
u8 Extension_TCA6424A_OutputWrite(u8 position, u8 port, u8 data);
u8 Extension_TCA6424A_OutputRead(u8 position, u8 port, u8 *pData);
u8 Extension_TCA6424A_InputRead(u8 position, u8 port, u8 *pData);
u8 Extension_LTC2633_RegWrite(u8 position, u8 cmd, u16 data);
u8 Extension_LTC2633_RefCtrl(u8 position, u8 data);
u8 Extension_LTC2633_PowerDown(u8 position, u8 channel);
u8 Extension_LTC2633_VoltCtrl(u8 position, u8 channel, double volt);
u8 Extension_E5ETBOARD_VoltOutput(u8 position, u8 channel, double volt);

#endif	// _APP_EXTENSION_H