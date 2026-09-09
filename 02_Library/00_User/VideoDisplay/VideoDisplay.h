#ifndef	_VIDEODISPLAY_H
#define	_VIDEODISPLAY_H

#include	"UserDefine.h"

typedef struct{
	u32	vLine;
	u32	hPixel;
	u32	vbp;
	u32	hbp;
	u32	vfp;
	u32	hfp;
	u32	vSync;
	u32	hSync;
	u32	framerate;
}videoDisplayInfo_t;

#endif	//	_VIDEODISPLAY_H