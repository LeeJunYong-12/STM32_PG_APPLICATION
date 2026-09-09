#ifndef	_APP_DISPLAYPORTCORE_H
#define	_APP_DISPLAYPORTCORE_H

#include	"E7602_PG_Application.h"

#define	DPC_CORE_MAX_COUNT		4

enum{
	DPC_SPI_REG_NONE			= 0,
	DPC_SPI_REG_WRITE_SINGLE	= 0x02,
	DPC_SPI_REG_READ_SINGLE		= 0x06,
	DPC_SPI_REG_WRITE_BURST		= 0x0A,
	DPC_SPI_REG_READ_BURST		= 0x0E,
};

/*
	Bit[15]		: RX
	Bit[14:10]	: TX
	Bit[9:0]	: COMMAND
*/
extern u8 hpd_event[2];

u8 DPC_Init();
u8 DPC_DirectRegWrite(u8 position, u16 adrs, u32 *pData, u16 size);
u8 DPC_DirectRegRead(u8 position, u16 adrs, u32 *pData, u16 size);
u8 DPC_DirectRegWriteSingle(u8 position, u16 adrs, u32 data);
u8 DPC_DirectRegReadSingle(u8 position, u16 adrs, u32 *pData);
u8 DPC_RegWrite(u8 position, u16 adrs, u32 *pData, u16 size);
u8 DPC_RegRead(u8 position, u16 adrs, u32 *pData, u16 size);
u8 DPC_RegWriteSingle(u8 position, u16 adrs, u32 data);
u8 DPC_RegReadSingle(u8 position, u16 adrs, u32 *pData);
u8 DPC_GetFpgaVersion(u8 position, u8 *pData);
u8 DPC_GetFpgaBuild(u8 position, u8 *pData);
u8 DPC_GetNiosVersion(u8 position, u8 *pData);
u8 DPC_GetNiosBuild(u8 position, u8 *pData);
u8 DPC_CoreSoftReset(u8 channel);
u8 DPC_MainLinkConfig(u8 channel, u8 position, u8 *pLaneOrder, u8 pnSwap, u8 voltRange);
u8 DPC_NiosWorkingPeriod(u8 channel, u32 *pPeriod);
u8 DPC_GetTemperature(u8 channel, float *pTemperature);
u8 DPC_UpdateCheck(u8 channel, u8 *data);
u8 HPDEVENT_Enable(u8 channel, u8 enable);
u8 EXT_CLK_CNT_START(u8 channel, u32 time);
u8 EXT_CLK_CNT_VALUE(u8 channel, float *cnt_value);
u8 EXT_CLK_PERIOD(u8 channel, u32 *period);
u8 PPA_Enable(u8 channel);
u8 PPA_Disable(u8 channel);
u8 PPA_Mode(u8 channel, u8 pnl_mode, u8 simple_en, u8 green, u8 avg_mode, u8 end_mode);

u8 OSD_Enable(u8 channel, u8 enable);
u8 OSD_Setting(u8 channel, u16 red, u16 green, u16 blue, u16 size, u16 thick);
u8 OSD_Box(u8 channel, u8 box, u16 xpos, u16 ypos);

#endif	// _APP_DISPLAYPORTCORE_H
