#include	"App_Initialize.h"

static u8 Init_Data()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = Board_DataInit();
	if(rtn == 0)			errChkr |= (1 << 0);

	if(errChkr == 0)		result = 1;

	return result;
}

static u8 Init_LED()
{
	u8	result = 0, rtn;

	rtn = HSP_LedInit();
	if(rtn != 0)			result = 1;

	return result;
}

static u8 Init_BoardIO()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = Board_GpioInit();
	if(rtn == 0)			errChkr |= (1 << 0);

	rtn = MSD_GpioInit();
	if(rtn == 0)			errChkr |= (1 << 1);

	rtn = FPGA_GpioInit();
	if(rtn == 0)			errChkr |= (1 << 2);

	rtn = CoreSPI_IoInit();
	if(rtn == 0)			errChkr |= (1 << 3);

	rtn = SBC_IoInit();
	if(rtn == 0)			errChkr |= (1 << 4);

	rtn = Switch_GpioInit();
	if(rtn == 0)			errChkr |= (1 << 5);

	rtn = ExtGPIO_Init();
	if(rtn == 0)			errChkr |= (1 << 6);

	rtn = ExtI2C_IoInit();
	if(rtn == 0)			errChkr |= (1 << 7);

	if(errChkr == 0)		result = 1;

	return result;
}

static u8 Init_DisplayPort()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = DP_Init();
	if(rtn == 0)				errChkr |= (1 << 0);

	rtn = DPC_Init();
	if(rtn == 0)				errChkr |= (1 << 1);

	rtn = CoreVC_Init();
	if(rtn == 0)				errChkr |= (1 << 2);

	rtn = 1;
	if(rtn == 0)				errChkr |= (1 << 3);

	if(errChkr == 0)			result = 1;

	return result;
}

static u8 Init_Peripheral()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = MEMCTRL_Init();
	if(rtn == 0)			errChkr |= (1 << 0);

	rtn = FPGA_Init();
	if(rtn == 0)			errChkr |= (1 << 1);

	rtn = JPEG_Init();
	if(rtn == 0)			errChkr |= (1 << 2);

	rtn = I2C_Init();
	if(rtn == 0)			errChkr |= (1 << 3);

	rtn = CoreSPI_Init();
	if(rtn == 0)			errChkr |= (1 << 4);

	rtn = ExtI2C_I2C0Init();
	if(rtn == 0)			errChkr |= (1 << 5);

	rtn = ExtI2C_I2C1Init();
	if(rtn == 0)			errChkr |= (1 << 6);

	rtn = SBC_UartInit();
	if(rtn == 0)			errChkr |= (1 << 7);

	rtn = Serial_Init();
	if(rtn == 0)			errChkr |= (1 << 8);

	rtn = Switch_Init();
	if(rtn == 0)			errChkr |= (1 << 9);

	rtn = Init_DisplayPort();
	if(rtn == 0)			errChkr |= (1 << 10);

	rtn = Polling_HPD();
	if(rtn == 0)			errChkr |= (1 << 11);

	if(errChkr == 0)		result = 1;

	return result;
}

static u8 Init_Board()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = Config_Init();
	if(rtn == 0)			errChkr |= (1 << 0);

	rtn = Config_BoardData();
	if(rtn == 0)			errChkr |= (1 << 1);

	rtn = Board_LoadHwType();
	if(rtn == 0)			errChkr |= (1 << 2);

	rtn = Board_LoadID();
	if(rtn == 0)			errChkr |= (1 << 3);

	rtn = Board_TypeLoad();
	if(rtn == 0)			errChkr |= (1 << 4);

	rtn = Board_ChannelLoad();
	if(rtn == 0)			errChkr |= (1 << 5);

	if(errChkr == 0)		result = 1;

	return result;
}

static u8 Init_FileSystem()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = FatFS_DriverInit();
	if(rtn == 0)		errChkr |= (1 << 0);

	rtn = FatFS_DriveMount(0x03);
	if(rtn == 0)		errChkr |= (1 << 1);

	if(errChkr == 0)	result = 1;

	return result;
}

static u8 Init_FileManager()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = FM_IndexInit();
	if(rtn == 0)		errChkr |= (1 << 0);

	if(errChkr == 0)	result = 1;

	return result;
}

static u8 Init_Command()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = CMD_Init();
	if(rtn == 0)		errChkr |= (1 << 0);

	if(errChkr == 0)	result = 1;

	return result;
}

static u8 Init_SubBoard()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = SB_Init();
	if(rtn == 0)		errChkr |= (1 << 0);

	if(errChkr == 0)	result = 1;

	return result;
}

static u8 Init_Communication()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = Network_Init();
	if(rtn == 0)		errChkr |= (1 << 0);

	rtn = UsbPkt_Init();
	if(rtn == 0)		errChkr |= (1 << 1);

	rtn = USB_Init();
	if(rtn == 0)		errChkr |= (1 << 2);


#if 0// yeom_20200823
	rtn = Script_Init();
	if(rtn == 0)		errChkr |= (1 << 3);
#endif// yeom_20200823

	if(errChkr == 0)	result = 1;

	return result;
}

static u8 Init_Complete()
{
	u8	result = 0, rtn, errCnt = 0;

	rtn = HSP_LedCtrl(1, LOW);
	if(rtn == 0)		errCnt += 1;

	rtn = HSP_LedCtrl(2, LOW);
	if(rtn == 0)		errCnt += 1;

	if(errCnt == 0)		result = 1;

	return result;
}

static u8 Init_SystemLog()
{
	u8	result = 0, rtn, errCnt = 0;

	LOG_Init();

	rtn = DBG_FileCheck();
	if(rtn == 0)		errCnt += 1;

	rtn = LOG_FileCheck();
	if(rtn == 0)		errCnt += 1;

	if(errCnt == 0)		result = 1;

	return result;
}

u8 Init_System()
{
	u8	result = 0, rtn;
	u32	errChkr = 0;

	rtn = BSP_Clock_Init();
	if(rtn == 0)		errChkr |= (1 << 0);

    HAL_NVIC_DisableIRQ(OTG_FS_IRQn); // for the initial waiting

	rtn = Init_Data();
	if(rtn == 0)		errChkr |= (1 << 1);

	rtn = Init_LED();
	if(rtn == 0)		errChkr |= (1 << 2);

	rtn = Init_BoardIO();
	if(rtn == 0)		errChkr |= (1 << 3);

	rtn = Init_Peripheral();
	if(rtn == 0)		errChkr |= (1 << 4);

	rtn = Init_Board();
	if(rtn == 0)		errChkr |= (1 << 5);

	rtn = Init_FileSystem();
	if(rtn == 0)		errChkr |= (1 << 6);

	rtn = Init_FileManager();
	if(rtn == 0)		errChkr |= (1 << 7);

	rtn = Init_Command();
	if(rtn == 0)		errChkr |= (1 << 8);

	rtn = Init_SubBoard();
	if(rtn == 0)		errChkr |= (1 << 9);

	rtn = Init_Communication();
	if(rtn == 0)		errChkr |= (1 << 10);

	rtn = Init_SystemLog();
	if(rtn == 0)		errChkr |= (1 << 11);

	rtn = Init_Complete();
	if(rtn == 0)		errChkr |= (1 << 16);

	HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

	if(errChkr == 0)	result = 1;

	return result;
}
