#ifndef	_APP_VIDEOINPUT_H
#define	_APP_VIDEOINPUT_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u32	memAdrs;
	u32	enable;
	u32	type;
	u32	hPixel;
	u32	vLines;
}videoInput_t;

enum{
	VIDEOINPUT_DVI_SINGLE = 0,
	VIDEOINPUT_DVI_DUAL,
	VIDEOINPUT_DP,
	VIDEOINPUT_HDMI
};

enum{
	VIDEOINPUT_LVDS_2PORT = 0,
	VIDEOINPUT_LVDS_4PORT = 1,
	VIDEOINPUT_LVDS_4PORT_EOSWAP = 2
};

enum{
	VIDEOINPUT_ACTIVE_LOW = 0,
	VIDEOINPUT_ACTIVE_HIGH = 1,
};

u8 VI_ResetCtrl(u32 data);
u8 VI_Init();
u8 VI_ParameterSet(u8 mode, u16 hPixel, u16 vLines);
u8 VI_LvdsRxDataMap(u32 data);
u8 VI_LvdsInputSyncPolarity(u32 vPol, u32 hPol);
u8 VI_StreamInputSkip(u32 data);
u8 VI_ModuleReset();
u8 VI_Enable();
u8 VI_Disable();
u8 VI_Display();

#endif	// _APP_VIDEOINPUT_H