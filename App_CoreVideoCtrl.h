#ifndef	_APP_COREVIDEOCTRL_H
#define	_APP_COREVIDEOCTRL_H

#include	"E7602_PG_Application.h"

enum{
	CORE_VIDEO_INPUT_SOURCE_DVI					= 0,
	CORE_VIDEO_INPUT_SOURCE_DP_RX,
	CORE_VIDEO_INPUT_SOURCE_INTERNAL_PATTERN,
	CORE_VIDEO_INPUT_SOURCE_MCU_BUS,
};

enum{
	CORE_VIDEO_CTRL_MODE_RGB888					= 0,
	CORE_VIDEO_CTRL_MODE_RGB101010,
	CORE_VIDEO_CTRL_MODE_ARGB8888,
	BUS_DATA_WRITE 								= 7,
};

#if 1 // yeom_20201117 [start]
enum{
	INPUT_SOURCE_DVI					= 0,
	INPUT_SOURCE_DP_RX,
	INPUT_SOURCE_INTERNAL_PATTERN,
	INPUT_SOURCE_MCU_BUS,
};

enum{
	INPUT_PORT_MAP_RGB101010_10BIT_ORDER				= 0,
	INPUT_PORT_MAP_RGB101010_8BIT_LSB_LSB_DUMMY_ORDER,
	INPUT_PORT_MAP_RGB101010_8BIT_MSB_LSB_DUMMY_ORDER,
	INPUT_PORT_MAP_RGB101010_8BIT_LSB_MSB_DUMMY_ORDER,
	INPUT_PORT_MAP_RGB101010_8BIT_MSB_MSB_DUMMY_ORDER,
	INPUT_PORT_MAP_RGB888_LSB_DUMMY,
	INPUT_PORT_MAP_RGB888_MSB_DUMMY,
	INPUT_PORT_MAP_BUS_DATA_WRITE,	
};

enum{
	SINGLE_PORT_ENABLE_SINGLE_PORT_DISABLE_DUAL				= 0,
	SINGLE_PORT_ENABLE_SINGLE_PORT_ENABLE,
};
#endif // yeom_20201117 [end]


enum{
	CORE_VIDEO_MODE_AUTO					= 0,
	CORE_VIDEO_MODE_SEMI_AUTO,
	CORE_VIDEO_MODE_MANUAL,
};

typedef	struct{
	u32	mode;
	u32	hActive;
	u32	vActive;
	u32	hfp;
	u32	hbp;
	u32	vfp;
	u32	vbp;
	u32	hSync;
	u32	vSync;
	u32	cFrameRate;		//	Current Framerate
	u32	mFrameRate;		//	Max Framerate
	u32	streamClock;
}coreVideoInfo_t;

u8 CoreVC_VideoStreamConfig(u8 position, void *pVoid);
u8 CoreVC_VideoStreamInputEnable(u8 position);
u8 CoreVC_VideoStreamInputDisable(u8 position);
u8 CoreVC_VideoStreamInputType(u8 position, u32 state);
u8 CoreVC_VideoStreamInputMode(u8 position, u32 state);
u8 CoreVC_FPGADATAInputMode(u8 position, u32 state);
u8 CoreVC_VideoFrameWriteSlot(u8 position, u32 slotNo);
u8 CoreVC_VideoFrameWriteStart(u8 position);
u8 CoreVC_VideoFrameWriteStop(u8 position);
u8 CoreVC_VideoFrameReadSlot(u32 slotNo);
u8 CoreVC_VideoStreamOutputEnable();
u8 CoreVC_VideoStreamOutputDisable();
u8 CoreVC_VideoStreamFormat(u8 position, u32 toggle, u32 mode);
u8 CoreVC_Init();
#if 1 // yeom_20201117 [start]
u8 CoreVC_VideoStreamInputDataTypeInputSource(u32 state);
u8 CoreVC_VideoStreamInputDataTypeInputPortMap(u32 state);
u8 CoreVC_VideoStreamInputDataTypeSinglePortEnable(u32 state);
#endif // yeom_20201117 [end]

#endif	// _APP_COREVIDEOCTRL_H
