#ifndef	_CMD_MBTEST_H
#define	_CMD_MBTEST_H

#include	"E7602_PG_Application.h"

enum{
	VSSEL = 0,
	VGL,
	AVDDH,
	VDDEL,
	VSSEL_L,
};

extern 	u8  ngCnt[7];
extern	u8	indexCnt;
extern	u8	pModel[10];
extern	u8	testItem;
extern	u32	remove_io_ng;
extern	u8 	errDetector;

extern	const	commandList_t	cmdList_MBtest[];

extern	double	hVoltageMin[9], hVoltageMax[9];
extern 	double	vmVoltageMin[7], vmVoltageMax[7];
extern	double	vmCurrentMin[7][4], vmCurrentMax[7][4];

extern	double	pmicVoltageMin[4], pmicVoltageMax[4];
extern	double	pmicCurrentMin[5], pmicCurrentMax[5];	

extern	double	oscMin, oscMax;

extern	double	vol_adjavddhMin, vol_adjavddhMax;
extern	double	vol_adjvsselMin, vol_adjvsselMax;
extern	double	vol_adjvglMin, vol_adjvglMax;
extern	double	vol_adjvddelMin, vol_adjvddelMax;

extern	double	cur_1v2Min, cur_1v2Max; 
extern	double	cur_2v5Min, cur_2v5Max;
extern	double	vol_1v2Min, vol_1v2Max;
extern	double	vol_2v5Min, vol_2v5Max;

extern	double	vspVoltageMin, vspVoltageMax;
extern	double	vsnVoltageMin, vsnVoltageMax;

#define AVDDH_SHUNT		0.1
#define VGL_SHUNT		1.0
#define VSSEL_SHUNT		0.1
#define VSSEL_L_SHUNT	200
#define TOUCH_SHUNT		0.1

//0x42 REG
#define	GPIO_MTP_TO_TOUCH_RESET_L_CONN_042			0x00000001
#define	SPI0_MTP_TO_TOUCH_SCLK_CONN_042				0x00000002
#define	SPI0_MTP_FROM_TOUCH_MISO_CONN_042			0x00000004
#define	SPI0_MTP_TO_TOUCH_MOSI_CONN_042				0x00000008
#define	GPIO_MTP_FROM_TOUCH_INT0_L_CONN_042			0x00000010
#define	IO_KRAKEN_TO_TOUCH_AID_CONN_042				0x00000020
#define	SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN_042			0x00000040
#define	SPI0_MTP_TO_TOUCH_SSIN_CONN_042				0x00000080
#define	GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_042			0x00000100
#define	IO_TOUCH_TO_MANY_SCAN_ACTIVE_042			0x00000200
#define	I2C_SDA_TOUCH_EEPROM_1V8_042				0x00000400
#define	I2C_SCL_TOUCH_EEPROM_1V8_042				0x00000800
#define	GPIO_MTP_FROM_TOUCH_INT1_L_CONN_042			0x00001000
#define	GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN_042		0x00002000
#define	SWD_NUB_BI_TOUCH_SWDIO_CONN_042				0x00004000
#define	IO_AOP_TO_TOUCH_BSYNC0_CONN_1_042			0x00008000
#define	IO_AOP_TO_TOUCH_BSYNC1_CONN_1_042			0x00010000

//0x44 REG
#define	FRAME_GND_MEAS_OUT_044						0x00000001
#define	IO_TOUCH_TO_VDDH_EN_CONN_044				0x00000002
#define	IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN_044		0x00000004
#define	AB_DISP_SPI_MOSI_044						0x00000008
#define	AB_DISP_SPI_MISO_044						0x00000010
#define	IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN_044		0x00000020
#define	AP_TO_CHIANTI_PANICB_L_IO_044				0x00000040
#define	AB_DISP_SPI_CLK_044							0x00000080
#define	CLK_PMU_TO_TOUCH_32K_CONN_044				0x00000100
#define	UTIL_1V8_GPIO_3_044							0x00000200
#define	UTIL_1V8_GPIO_4_044							0x00000400
#define	UTIL_1V8_GPIO_0_044							0x00000800
#define	UTIL_1V8_GPIO_1_044							0x00001000
#define	AB_DISP_SPI_CS_044							0x00002000

u8 Cmd_MBTestCurrentLimit(void *pVoid);
u8 Cmd_MBTestVoltageLimit(void *pVoid);
u8 Cmd_MBTestTestItem(void *pVoid);

#endif	// _CMD_MBTEST_H