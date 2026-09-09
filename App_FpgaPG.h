#ifndef	_APP_FPGAPG_H
#define	_APP_FPGAPG_H

#include	"E7602_PG_Application.h"

enum{
	UPDATE_DONE_STATUS		= 0,
	UPDATE_STATUS,
	UPDATE_ERROR_STATUS,
};

typedef	struct{
	u32	forceInit;
	u32	txMode;
	u32	pgOnly;
	u32	forceActive;
	u32	hPixel;
	u32	hfp;
	u32 hbp;
	u32	hSync;
	u32	vLine;
	u32	vfp;
	u32	vbp;
	u32	vSync;
	u32	framerate;
}fpgaPgVideo_t;

u8 FpgaPG_DataInit();
u8 FpgaPG_GetStatus();
u8 FpgaPG_SetStatus(u8 status);
u8 FpgaPG_ClearSatatus();
u8 FpgaPG_ConfigPixelClock(u32 pixelClk);
u8 FpgaPG_ConfigVideo(void *pVoid);
u8 FpgaPG_Reset();
void FpgaCore_Reset();
u8 FpgaPG_RemoteUpdateControl(u8 data);
u8 FpgaPG_RemoteUpdateStatus(u8 mode);

#endif	// _APP_FPGAPG_H