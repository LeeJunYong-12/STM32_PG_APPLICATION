#ifndef	_APP_BOARD_H
#define	_APP_BOARD_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u32	status;
	u32	startTime;
	u32	timeOut;
}boardReset_t;

typedef struct{
	u32	type;
	u32	mipiHwType;
	u32	eDpHwType;
}boardPhyInfo_t;

enum{
	BOARD_PHY_TYPE_NONE		= 0,
	BOARD_PHY_TYPE_MIPI,
	BOARD_PHY_TYPE_EDP,
	BOARD_PHY_TYPE_END,
};

enum{
	BOARD_CONSTRUCT_SINGLE		= 0,
	BOARD_CONSTRUCT_MULTI,
	BOARD_CONSTRUCT_RELIABLITY,
	BOARD_CONSTRUCT_END,
};

enum{
	BOARD_TYPE_RESERVED_0			= 0,		//	0
	BOARD_TYPE_RESERVED_1,						//	1
	BOARD_TYPE_RESERVED_2,						//	2
	BOARD_TYPE_MIPI_MULTI_CAHNNEL_8_R,			//	3
	BOARD_TYPE_RESERVED_4,						//	4
	BOARD_TYPE_RESERVED_5,						//	5
	BOARD_TYPE_MIPI_MULTI_CHANNEL_2,			//	6
	BOARD_TYPE_RESERVED_7,						//	7
	BOARD_TYPE_RESERVED_8,						//	8
	BOARD_TYPE_EDP_MULTI_CHANNEL_4_R,			//	9
	BOARD_TYPE_RESERVED_10,						//	10
	BOARD_TYPE_RESERVED_11,						//	11
	BOARD_TYPE_RESERVED_12,						//	12
	BOARD_TYPE_RESERVED_13,						//	13
	BOARD_TYPE_EDP_MULTI_CHANNEL_2,				//	14
	BOARD_TYPE_RESERVED_15,						//	15
	BOARD_TYPE_END,
};

extern	boardChannel_t	boardChannel[BOARD_CHANNEL_MAX_COUNT];

u8 Board_DataInit();
u8 Board_GpioInit();
u8 Board_LoadHwType();
u8 Board_GetHwType();
u8 Board_TypeLoad();
u8 Board_LoadID();
u8 Board_GetID();
u8 Board_GetConstruct();
u8 Board_ChannelLoad();
u8 Board_GetPhyType(u32 *pPhyType);
u8 Board_GetPhyHwType(u32 phyType, u32 *pHwType);
u8 Board_ChannelInit();
u8 Board_ChannelSelect(u8 channel, u8 mode);
u8 Board_GetHwConstruct(u8 channel);
u8 Board_GetChannelStatus(u8 channel);
u8 Board_GetChannelPosition(u8 channel, u32 *pPosition);
u8 Board_GetVersion(u8 *pData);
u8 Board_GetBuildDate(u8 *pData);
u8 Board_GetFirmwareType(u8 *pData);
u8 Board_ResetEnable();
u8 Board_ResetProcess();
u8 Board_BootMode();

#endif	// _APP_BOARD_H
