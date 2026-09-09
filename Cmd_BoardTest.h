#ifndef	_CMD_BOARDTEST_H
#define	_CMD_BOARDTEST_H

#include	"E7602_PG_Application.h"

#define		GPIO_NUM_MAX		8
#define		BIT32_FULL_CNT		32


enum{
	BIT0 = 0,
	BIT1,			
	BIT2,		 
	BIT3,			
	BIT4,			
	BIT5,			
	BIT6,			
	BIT7,			
	BIT_NUM_END		
};

enum{
	CS = 0,
	SCK,			
	MOSI,		 
	MISO,			
};

enum{
	UART_RX = 0,
	UART_TX,						
};

extern	bitCtrl32_t	ngNum[3];

void BoardTest_NgCheck(u8	ngName);

extern	const	commandList_t	cmdList_BoardTest[];

#endif	// _CMD_BOARDTEST_H
