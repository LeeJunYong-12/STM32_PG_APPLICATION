#ifndef	_BOARDINFO_H
#define	_BOARDINFO_H

#include	"UserDefine.h"

#define	BOARD_CHANNEL_MAX_COUNT		4

enum{
	BOARD_HW_CONSTRUCT_FALSE		= 0,
	BOARD_HW_CONSTRUCT_TRUE,
};

enum{
	BOARD_USE_CHANNEL_FALSE			= 0,
	BOARD_USE_CHANNEL_TRUE,
};

enum{
	BOARD_POSITION_1				= 0,
	BOARD_POSITION_2,
	BOARD_POSITION_3,
	BOARD_POSITION_4,
	BOARD_POSITION_END,
};

enum{
	BOARD_OPTION_NONE				= 0,
	BOARD_OPTION_END,
};

typedef	struct{
	u32	hwConstruct;
	u32	useChannel;
	u32	position;
	u32	option;
}boardChannel_t;

typedef	struct{
	u16	mode;
	u8	mac[6];
	u8	ip[4];
	u8	nm[4];
	u8	gw[4];
	u16	tcpPort;
	u16	udpPort;
}networkInfo_t;

#endif	//	_BOARDINFO_H
