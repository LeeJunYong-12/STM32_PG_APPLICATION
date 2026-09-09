#ifndef	_APP_VIDEOSTREAM_H
#define	_APP_VIDEOSTREAM_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u32	framerate;
	u32	pixelClk;
	u32	hPixel;
	u32	vLine;
	u32	hbp;
	u32	hfp;
	u32	vbp;
	u32	vfp;
	u32	hSync;
	u32	vSync;
}videoStreamInfo_t;

typedef	struct{
	u32					initialized;
	u32					status;
	videoStreamInfo_t	info;
}videoStreamHandle_t;

u8 VideoStream_Reset();
u8 VideoStream_GetStatus();
u8 VideoStream_CheckResolution(u32 hPixel, u32 vLine);
u8 VideoStream_ClockConfig(u32 clk);
u8 VideoStream_Output(void *pVoid);

#endif	// _APP_VIDEOSTREAM_H
