#ifndef	_APP_CORE_H
#define	_APP_CORE_H

#include	"E7602_PG_Application.h"

u8 Core_Init();
u8 Core_PowerCtrl(u8 channel, u8 status);
u8 Core_Reset(u8 position);
u8 Core_ConfigClock(u8 position, u32 clk);
u8 Core_PixelClockConfig(u8 position, u32 clk);
u8 Core_DsiDatarateConfig(u8 position, u32 datarate, double *pPhase);
u8 Core_GetCoreType(u8 position, u8 *pData);
u8 Core_VideoInputConfig(u8 position, u32 mode);

u8 Core_PTGEnable(u8 position);
u8 Core_PTGDisable(u8 position);
u8 Core_RgbInterfaceTest(u8 position, void *pVoid);
u8 Core_DebugLedCtrl(u8 position, u8 data);
u8 Core_FrameBufferBankSelect_Input(u8 position, u16 slot);
u8 Core_FrameBufferBankSelect_Output(u8 position, u16 slot);

u8 Core_VideoReflesh(u8 position);
u8 Core_GetVersion(u8 position, u8 *pData);
u8 Core_GetBuildDate(u8 position, u8 *pData);

void Core_WriteBurst(u32 *pData, u32 size);

#endif	// _APP_CORE_H