#ifndef	_APP_IOEXPANDER_H
#define	_APP_IOEXPANDER_H

#include	"E7602_PG_Application.h"

#define	OUT_XSYNC	0xaa

enum{
	I2C_NG = 0,		
	IO_NG,		
	POWER_NG, 	
	VENTURA_NG, 	
	DP_NG, 		
	OSC_NG, 		
	PMIC_NG, 	
	TOTAL_OK, 	
	TOTAL_NG, 	
	TESTING,
};

enum{
	WRITE = 0x00,
	READ = 0x01,
};

enum {
	 I2C = 0,
	 OPENSHORT,
	 POWER,
	 DP,
	 OSC,
	 VID,
	 PMIC,
};

enum{
	MT_OUT_EXPANDER_1 = 0,
	MT_OUT_EXPANDER_2,	
	MT_IN_EXPANDER_1,
	MT_IN_EXPANDER_2,
	MT_IN_EXPANDER_3,
	MB_EXPANDER_1,
	MB_EXPANDER_2,
	BT_EXPANDER_1,
	BT_EXPANDER_2,
	ET_TEST_EXPANDER_1,
	HT_ETC_EXPANDER,
	HT_VER_EXPANDER,
	HT_OUT_EXPANDER_1,
	HT_OUT_EXPANDER_2,
	HT_IN_EXPANDER_1,
	HT_IN_EXPANDER_2,
	HT_INOUT_EXPANDER,
};

enum{
	PORTA = 0,
	PORTB = 1,
};

enum{
	PORT_INPUT = 0xff,
	PORT_OUTPUT = 0x00,
};

enum{
	P0 = 0x01,
	P1 = 0x02,
	P2 = 0x04,
	P3 = 0x08,
	P4 = 0x10,
	P5 = 0x20,
	P6 = 0x40,
	P7 = 0x80,
	PORT_ALL = 0xFF,
};

enum{
	P10 = 0x01,
	P11 = 0x02,
	P12 = 0x04,
	P13 = 0x08,
	P14 = 0x10,
	P15 = 0x20,
	P16 = 0x40,
	P17 = 0x80,
};

enum {
	 AMUX_S1 = 0x00,
	 AMUX_S2 = 0x01,
	 AMUX_S3 = 0x02,
	 AMUX_S4 = 0x03,
	 AMUX_S5 = 0x04,
	 AMUX_S6 = 0x05,
	 AMUX_S7 = 0x06,
	 AMUX_S8 = 0x07,
};

u8 IoExp_PortDir(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 direction);
u8 IoExp_PortInputRead(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 *pBuffer);
u8 IoExp_PortWrite(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData);
u8 IoExp_PortSet(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData);
u8 IoExp_PortClear(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData);
u8 IoExp_ChannelSelect(u8 expander, u8 status);
u8 IoExp_I2CTransmit(void *pVoid, u8 i2cNo, u8 devAdrs, u8 *pBuffer, u16 size);
u8 IoExp_I2CMEMTransmit(void *pVoid, u8 i2cNo, u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pBuffer, u16 size);
u8 IoExp_I2CMEMReceive(void *pVoid, u8 i2cNo, u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pBuffer, u16 size);
u8 IoExp_LED_Ctrl(void *pVoid, u8 status);
u8 IoExp_ShortCheck_D854Hinge(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData);
u8 IoExp_ShortCheck_D852Hinge(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData);
u8 IoExp_ShortCheck_Prev(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData);
u8 IoExp_LED_I2C_NG(void *pVoid, u8 i2cNo);
u8 IoExp_LED_IO_NG(void *pVoid, u8 i2cNo);
u8 IoExp_LED_POWER_NG(void *pVoid, u8 i2cNo);
u8 IoExp_LED_DP_NG(void *pVoid, u8 i2cNo);
u8 IoExp_LED_VENTURA_NG(void *pVoid, u8 i2cNo);
u8 IoExp_LED_PMIC_NG(void *pVoid, u8 i2cNo);
u8 IoExp_LED_OSC_NG(void *pVoid, u8 i2cNo);
u8 IoExp_LED_TEST_RUN(void *pVoid, u8 i2cNo);
u8 IoExp_LED_TOTAL_OK(void *pVoid, u8 i2cNo);
u8 IoExp_LED_TOTAL_NG(void *pVoid, u8 i2cNo);



#endif	//	_APP_IOEXPANDER_H
