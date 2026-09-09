#ifndef	_E7602_PG_APPLICATION_H
#define	_E7602_PG_APPLICATION_H

#include	"UserDefine.h"

#include	"MemoryMap.h"

#include	"BoardInfo.h"

#include	"System_Converter.h"

#include	"BSP.h"

#include	"HSP.h"

#include	"CommandSet.h"

#include	"ConfigPacket.h"

#include	"Ethernet_Application.h"
#include	"Ethernet_Interface.h"
#include	"Ethernet_TcpServer.h"
#include	"Ethernet_UdpServer.h"

#include	"FatFS_GenDrv.h"
#include	"MicroSD_DiskIO.h"
#include	"NANDFlash_DiskIO.h"

#include	"System_Font.h"

#include	"jpeg_utils.h"

#include	"SubBoard.h"

#include	"usbd_conf.h"
#include	"usbd_desc.h"
#include	"usbd_cdc_if.h"

#include	"VideoDisplay.h"

#include	"File_Image.h"

#include	"App_BASE_M.h"
#include	"App_BASE_R.h"
#include	"App_BASE_S.h"
#include	"App_Board.h"
#include	"App_CmdList_Common.h"
#include	"App_CmdList_eDPr.h"
#include	"App_CmdList_eDPm.h"
#include	"App_Command.h"
#include	"App_Config.h"
#include	"App_Core.h"
#include	"App_CorePG.h"
#include	"App_CoreReg.h"
#include	"App_CoreRegister.h"
#include	"App_CoreSPI.h"
#include	"App_CoreVideoCtrl.h"
#include	"App_DisplayPort.h"
#include	"App_DisplayPortCore.h"
#include	"App_DisplayPortRx.h"
#include	"App_DisplayPortTx.h"
#include	"App_EDID.h"
#include	"App_eDP.h"
#include	"App_eDPMultiCore.h"
#include	"App_EEPROM.h"
#include	"App_EquipmentNY.h"
#include	"App_ExtCtrl.h"
#include	"App_Extension.h"
#include	"App_ExtGPIO.h"
#include	"App_ExtI2C.h"
#include	"App_ExtSW.h"
#include	"App_FanCtrl.h"
#include	"App_File.h"
#include	"App_FileManager.h"
#include	"App_FPGA.h"
#include	"App_FpgaPG.h"
#include	"App_GPIO.h"
#include	"App_GPIO_R.h"
#include	"App_I2C.h"
#include	"App_Image.h"
#include	"App_Initialize.h"
#include	"App_IR.h"
#include	"App_JpegDecode.h"
#include	"App_LCM.h"
#include	"App_LCM_R.h"
#include	"App_Log.h"
#include	"App_Network.h"
#include	"App_NetworkPacket.h"
#include	"App_Power.h"
#include	"App_Power_R.h"
#include	"App_SBC.h"
#include	"App_Script.h"
#include	"App_Serial.h"
#include	"App_SubBoard.h"
#include	"App_Switch.h"
#include	"App_System.h"
#include	"App_Timer.h"
#include	"App_USB.h"
#include	"App_UsbPacket.h"
#include	"App_VideoInput.h"
#include	"App_VideoStream.h"
#include	"App_IoExpander.h"

#include	"Cmd_Base_R.h"
#include	"Cmd_Board.h"
#include	"Cmd_DisplayPortCore.h"
#include	"Cmd_DisplayPortRx.h"
#include	"Cmd_DisplayPortTx.h"
#include	"Cmd_eDP_R.h"
#include	"Cmd_EquipmentNY.h"
#include	"Cmd_ExtCtrl.h"
#include	"Cmd_Extension.h"
#include	"Cmd_File.h"
#include	"Cmd_FPGA.h"
#include	"Cmd_GPIO.h"
#include	"Cmd_GPIO_R.h"
#include	"Cmd_Image.h"
#include	"Cmd_LCM.h"
#include	"Cmd_LCM_R.h"
#include	"Cmd_Power.h"
#include	"Cmd_Power_R.h"
#include	"Cmd_RXDPCore.h"
#include	"Cmd_System.h"
#include	"Cmd_Test.h"
#include	"Cmd_VideoStream.h"
#include	"Cmd_Log.h"
#include	"Cmd_MBTest.h"
#include	"Cmd_MBTest_prev.h"
#include	"Cmd_BoardTest.h"
#include	"Cmd_EtTest.h"
#include	"Cmd_HingeTest.h"

#include	"Application_MAIN.h"

enum{
	APPLICATION_TYPE_PG		= 0x76024D4D,	
	APPLICATION_TYPE_PWR_E	= 0x76025045,
	APPLICATION_TYPE_PWR_R	= 0x76025052,
};

/*
	0 : 0x30
	1 : 0x31
	2 : 0x32
	3 : 0x33
	4 : 0x34
	5 : 0x35
	6 : 0x36
	7 : 0x37
	8 : 0x38
	9 : 0x39
	
	A : 0x41
	B : 0x42
	C : 0x43
	D : 0x44
	E : 0x45
	F : 0x46
	R : 0x52
*/

#define	APPLICATION_VERSION		0x01010352 // 1.1.3.R
#define	APPLICATION_BUILD		0x20220523
#define	APPLICATION_TYPE		APPLICATION_TYPE_PG

#endif	//	_E7602_PG_APPLICATION_H