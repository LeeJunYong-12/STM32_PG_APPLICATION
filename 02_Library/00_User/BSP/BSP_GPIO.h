#ifndef	_BSP_GPIO_H
#define	_BSP_GPIO_H

#include	"BSP.h"

typedef	struct{
	void (* ExtiFunc)			(void);
}bspGpioExtiFops_t;

typedef	struct{
	GPIO_TypeDef	*Port;
	u32				Pin;
	u32				Mode;
	u16				wData;
	u16				rData;
}bspGpio_t;

enum{
	BSP_GPIO_MODE_INPUT = 0,
	BSP_GPIO_MODE_OUTPUT_OD,
	BSP_GPIO_MODE_OUTPUT_PU,
	BSP_GPIO_MODE_OUTPUT_NP,
	BSP_GPIO_MODE_OUTPUT_PD,
	BSP_GPIO_MODE_END,
};

u8 BSP_GPIO_Init(void *pVoid);
u8 BSP_GPIO_Deinit(void *pVoid);
u8 BSP_GPIO_ReadPin(void *pVoid);
u8 BSP_GPIO_WritePin(void *pVoid, u8 state);
u8 BSP_GPIO_Toggle(void *pVoid);
u8 BSP_GPIO_RegisterInterface(u16 gpioPin, void *pVoid);

#endif	//	_BSP_GPIO_H
