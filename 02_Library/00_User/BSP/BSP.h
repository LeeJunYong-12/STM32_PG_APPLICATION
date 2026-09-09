#ifndef	_BSP_H
#define	_BSP_H

#include	"UserDefine.h"

#include	"stm32f7xx_hal.h"

#ifndef	BSP_USE_RTOS
#define	BSP_USE_RTOS						0
#endif	//	BSP_USE_RTOS

#if	(BSP_USE_RTOS)
#include	"FreeRTOS.h"
#include	"task.h"
#include	"timers.h"
#include	"queue.h"
#include	"semphr.h"
#include	"event_groups.h"

#include	"cmsis_os.h"
#endif	//	BSP_USE_RTOS

#include	"BSP_Clock.h"
#include	"BSP_DMA.h"
#include	"BSP_Error.h"
#include	"BSP_Ethernet.h"
#include	"BSP_Flash.h"
#include	"BSP_FMC.h"
#include	"BSP_GPIO.h"
#include	"BSP_I2C.h"
#include	"BSP_JPEG.h"
#include	"BSP_Log.h"
#include	"BSP_QSPI.h"
#include	"BSP_SD.h"
#include	"BSP_SPI.h"
#include	"BSP_SysTick.h"
#include	"BSP_Task.h"
#include	"BSP_Timer.h"
#include	"BSP_UART.h"
#include	"BSP_USB.h"

#define	BSP_RUNNING_MODE_POLLING			0
#define	BSP_RUNNING_MODE_INTERRUPT			1
#define BSP_RUNNING_MODE_DMA				2

void DelayCycles(u32 cnt);
u8 BSP_DataInit();

#endif	//	_BSP_H