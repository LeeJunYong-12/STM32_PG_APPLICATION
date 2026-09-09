#ifndef	_SYSTEM_FONT_H
#define	_SYSTEM_FONT_H

#include	"UserDefine.h"

typedef	struct{
	uc8 *fontData;
	u8	width;
	u8	height;
}font_t;

extern	font_t	fontVerdana_64;
extern	font_t	fontConsolas_32;

#endif	// _SYSTEM_FONT_H
