#include	"HSP_MemoryControl.h"

static	SDRAM_HandleTypeDef			hMemCtrlSDRAM;
static	NAND_HandleTypeDef			hMemCtrlNAND;
static	SRAM_HandleTypeDef			hMemCtrlFPGA;

static	u32	memCtrlNandStatus, memCtrlNandBusDepth;

static u8 MEMCTRL_IoInit()
{
	u8	result = 0;
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_FMC_CLK_ENABLE();

	gpio.Mode		=	GPIO_MODE_AF_PP;
	gpio.Pull		=	GPIO_PULLUP;
	gpio.Speed		=	GPIO_SPEED_HIGH;
	gpio.Alternate	=	GPIO_AF12_FMC;

	// GPIOD	:	FMC_DATA2, FMC_DATA3, FMC_NOE, FMC_NWE, FMC_NWAIT, FMC_NE1, FMC_DATA13
	//				FMC_DATA14, FMC_DATA15, FMC_CLE, FMC_ALE, FMC_DATA0, FMC_DATA1

	gpio.Pin		=	GPIO_PIN_0;		// FMC_DATA2		: PD0
	gpio.Pin		|=	GPIO_PIN_1;		// FMC_DATA3		: PD1
	gpio.Pin		|=	GPIO_PIN_4;		// FMC_NOE			: PD4
	gpio.Pin		|=	GPIO_PIN_5;		// FMC_NWE			: PD5
	gpio.Pin		|=	GPIO_PIN_6;		// FMC_NWAIT		: PD6
	gpio.Pin		|=	GPIO_PIN_7;		// FMC_NE1			: PD7
	gpio.Pin		|=	GPIO_PIN_8;		// FMC_DATA13		: PD8
	gpio.Pin		|=	GPIO_PIN_9;		// FMC_DATA14		: PD9
	gpio.Pin		|=	GPIO_PIN_10;	// FMC_DATA15		: PD10
	gpio.Pin		|=	GPIO_PIN_11;	// FMC_CLE			: PD11
	gpio.Pin		|=	GPIO_PIN_12;	// FMC_ALE			: PD12
	gpio.Pin		|=	GPIO_PIN_14;	// FMC_DATA0		: PD14
	gpio.Pin		|=	GPIO_PIN_15;	// FMC_DATA1		: PD15

	HAL_GPIO_Init(GPIOD, &gpio);

	// GPIOE	:	FMC_NBL0, FMC_NBL1, FMC_DATA4, FMC_DATA5, FMC_DATA6, FMC_DATA7
	//				FMC_DATA8, FMC_DATA9, FMC_DATA10, FMC_DATA11, FMC_DATA12

	gpio.Pin		=	GPIO_PIN_0;		// FMC_NBL0			: PE0
	gpio.Pin		|=	GPIO_PIN_1;		// FMC_NBL1			: PE1
	gpio.Pin		|=	GPIO_PIN_7;		// FMC_DATA4		: PE7
	gpio.Pin		|=	GPIO_PIN_8;		// FMC_DATA5		: PE8
	gpio.Pin		|=	GPIO_PIN_9;		// FMC_DATA6		: PE9
	gpio.Pin		|=	GPIO_PIN_10;	// FMC_DATA7		: PE10
	gpio.Pin		|=	GPIO_PIN_11;	// FMC_DATA8		: PE11
	gpio.Pin		|=	GPIO_PIN_12;	// FMC_DATA9		: PE12
	gpio.Pin		|=	GPIO_PIN_13;	// FMC_DATA10		: PE13
	gpio.Pin		|=	GPIO_PIN_14;	// FMC_DATA11		: PE14
	gpio.Pin		|=	GPIO_PIN_15;	// FMC_DATA12		: PE15

	HAL_GPIO_Init(GPIOE, &gpio);

	// GPIOF	:	FMC_ADDR0, FMC_ADDR1, FMC_ADDR2, FMC_ADDR3, FMC_ADDR4, FMC_ADDR5
	//				FMC_SDNRAS, FMC_ADDR6, FMC_ADDR7, FMC_ADDR8, FMC_ADDR9

	gpio.Pin		=	GPIO_PIN_0;		// FMC_ADDR0		: PF0
	gpio.Pin		|=	GPIO_PIN_1;		// FMC_ADDR1		: PF1
	gpio.Pin		|=	GPIO_PIN_2;		// FMC_ADDR2		: PF2
	gpio.Pin		|=	GPIO_PIN_3;		// FMC_ADDR3		: PF3
	gpio.Pin		|=	GPIO_PIN_4;		// FMC_ADDR4		: PF4
	gpio.Pin		|=	GPIO_PIN_5;		// FMC_ADDR5		: PF5
	gpio.Pin		|=	GPIO_PIN_11;	// FMC_SDNRAS		: PF11
	gpio.Pin		|=	GPIO_PIN_12;	// FMC_ADDR6		: PF12
	gpio.Pin		|=	GPIO_PIN_13;	// FMC_ADDR7		: PF13
	gpio.Pin		|=	GPIO_PIN_14;	// FMC_ADDR8		: PF14
	gpio.Pin		|=	GPIO_PIN_15;	// FMC_ADDR9		: PF15

	HAL_GPIO_Init(GPIOF, &gpio);

	// GPIOG	:	FMC_ADDR10, FMC_ADDR11, FMC_ADDR12, FMC_BA0, FMC_BA1, FMC_SDCLK
	//				FMC_NCE3, FMC_SDNCAS

	gpio.Pin		=	GPIO_PIN_0;		// FMC_ADDR10		: PG0
	gpio.Pin		|=	GPIO_PIN_1;		// FMC_ADDR11		: PG1
	gpio.Pin		|=	GPIO_PIN_2;		// FMC_ADDR12		: PG2
	gpio.Pin		|=	GPIO_PIN_4;		// FMC_BA0			: PG4
	gpio.Pin		|=	GPIO_PIN_5;		// FMC_BA1			: PG5
	gpio.Pin		|=	GPIO_PIN_8;		// FMC_SDCLK		: PG8
	gpio.Pin		|=	GPIO_PIN_9;		// FMC_NCE3			: PG9
	gpio.Pin		|=	GPIO_PIN_15;	// FMC_SDNCAS		: PG15

	HAL_GPIO_Init(GPIOG, &gpio);

	// GPIOH	:	FMC_SDCKE0, FMC_SDNE0, FMC_SDNWE, FMC_DATA16, FMC_DATA17, FMC_DATA18
	//				FMC_DATA19, FMC_DATA20, FMC_DATA21, FMC_DATA22, FMC_DATA23

	gpio.Pin		=	GPIO_PIN_2;		// FMC_SDCKE0		: PH2
	gpio.Pin		|=	GPIO_PIN_3;		// FMC_SDNE0		: PH3
	gpio.Pin		|=	GPIO_PIN_5;		// FMC_SDNWE		: PH5
	gpio.Pin		|=	GPIO_PIN_8;		// FMC_DATA16		: PH8
	gpio.Pin		|=	GPIO_PIN_9;		// FMC_DATA17		: PH9
	gpio.Pin		|=	GPIO_PIN_10;	// FMC_DATA18		: PH10
	gpio.Pin		|=	GPIO_PIN_11;	// FMC_DATA19		: PH11
	gpio.Pin		|=	GPIO_PIN_12;	// FMC_DATA20		: PH12
	gpio.Pin		|=	GPIO_PIN_13;	// FMC_DATA21		: PH13
	gpio.Pin		|=	GPIO_PIN_14;	// FMC_DATA22		: PH14
	gpio.Pin		|=	GPIO_PIN_15;	// FMC_DATA23		: PH15

	HAL_GPIO_Init(GPIOH, &gpio);

	// GPIOI	:	FMC_DATA24, FMC_DATA25, FMC_DATA26, FMC_DATA27, FMC_NBL2, FMC_NBL3
	//				FMC_DATA28, FMC_DATA29, FMC_DATA30, FMC_DATA31

	gpio.Pin		=	GPIO_PIN_0;		// FMC_DATA24		: PI0
	gpio.Pin		|=	GPIO_PIN_1;		// FMC_DATA25		: PI1
	gpio.Pin		|=	GPIO_PIN_2;		// FMC_DATA26		: PI2
	gpio.Pin		|=	GPIO_PIN_3;		// FMC_DATA27		: PI3
	gpio.Pin		|=	GPIO_PIN_4;		// FMC_NBL2			: PI4
	gpio.Pin		|=	GPIO_PIN_5;		// FMC_NBL3			: PI5
	gpio.Pin		|=	GPIO_PIN_6;		// FMC_DATA28		: PI6
	gpio.Pin		|=	GPIO_PIN_7;		// FMC_DATA29		: PI7
	gpio.Pin		|=	GPIO_PIN_9;		// FMC_DATA30		: PI9
	gpio.Pin		|=	GPIO_PIN_10;	// FMC_DATA31		: PI10

	HAL_GPIO_Init(GPIOI, &gpio);

	result = 1;

	return result;
}

static u8 MEMCTRL_SDRAM_InitializationSequence(u32 refreshCount)
{
	u8	result = 0, errCnt = 0;
	u32	temp = 0;
	HAL_StatusTypeDef	rtn;
	FMC_SDRAM_CommandTypeDef	command;

	command.CommandMode					= FMC_SDRAM_CMD_CLK_ENABLE;	
	command.CommandTarget 				= FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber 			= 1;
	command.ModeRegisterDefinition 		= 0;

	rtn = HAL_SDRAM_SendCommand(&hMemCtrlSDRAM, &command, MEMCTRL_SDRAM_TIMEOUT);
	if(rtn != HAL_OK)					errCnt += 1;

	HAL_Delay(1);

	command.CommandMode					= FMC_SDRAM_CMD_PALL;	
	command.CommandTarget 				= FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber 			= 1;
	command.ModeRegisterDefinition 		= 0;

	rtn = HAL_SDRAM_SendCommand(&hMemCtrlSDRAM, &command, MEMCTRL_SDRAM_TIMEOUT);
	if(rtn != HAL_OK)					errCnt += 1;

	command.CommandMode					= FMC_SDRAM_CMD_AUTOREFRESH_MODE;	
	command.CommandTarget 				= FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber 			= 8;
	command.ModeRegisterDefinition 		= 0;

	rtn = HAL_SDRAM_SendCommand(&hMemCtrlSDRAM, &command, MEMCTRL_SDRAM_TIMEOUT);
	if(rtn != HAL_OK)					errCnt += 1;

	temp								|= MEMCTRL_SDRAM_MODEREG_BURST_LENGTH_1;
	temp								|= MEMCTRL_SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL;
	temp								|= MEMCTRL_SDRAM_MODEREG_CAS_LATENCY_3;
	temp								|= MEMCTRL_SDRAM_MODEREG_OPERATING_MODE_STANDARD;
	temp								|= MEMCTRL_SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	command.CommandMode					= FMC_SDRAM_CMD_LOAD_MODE;	
	command.CommandTarget 				= FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber 			= 1;
	command.ModeRegisterDefinition 		= temp;

	rtn = HAL_SDRAM_SendCommand(&hMemCtrlSDRAM, &command, MEMCTRL_SDRAM_TIMEOUT);
	if(rtn != HAL_OK)					errCnt += 1;

	rtn = HAL_SDRAM_ProgramRefreshRate(&hMemCtrlSDRAM, refreshCount);
	if(rtn != HAL_OK)					errCnt += 1;

	if(rtn == 0)						result = 1;

	return result;
}

static u8 MEMCTRL_SDRAM_Init()
{
	u8	result = 0, chkr, errCnt = 0;
	HAL_StatusTypeDef	rtn;
	FMC_SDRAM_TimingTypeDef	timing;

	memset(&hMemCtrlSDRAM, NULL, sizeof(hMemCtrlSDRAM));

	hMemCtrlSDRAM.Instance						= FMC_SDRAM_DEVICE;

	hMemCtrlSDRAM.Init.SDBank					= FMC_SDRAM_BANK1;
	hMemCtrlSDRAM.Init.ColumnBitsNumber			= FMC_SDRAM_COLUMN_BITS_NUM_10;
	hMemCtrlSDRAM.Init.RowBitsNumber			= FMC_SDRAM_ROW_BITS_NUM_13;
	hMemCtrlSDRAM.Init.MemoryDataWidth			= FMC_SDRAM_MEM_BUS_WIDTH_32;
	hMemCtrlSDRAM.Init.InternalBankNumber		= FMC_SDRAM_INTERN_BANKS_NUM_4;
	hMemCtrlSDRAM.Init.CASLatency				= FMC_SDRAM_CAS_LATENCY_3;
	hMemCtrlSDRAM.Init.WriteProtection			= FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	hMemCtrlSDRAM.Init.SDClockPeriod			= FMC_SDRAM_CLOCK_PERIOD_3;
	hMemCtrlSDRAM.Init.ReadBurst				= FMC_SDRAM_RBURST_ENABLE;
	hMemCtrlSDRAM.Init.ReadPipeDelay			= FMC_SDRAM_RPIPE_DELAY_0;

	timing.LoadToActiveDelay					= 2;
	timing.ExitSelfRefreshDelay					= 7;
	timing.SelfRefreshTime						= 4;
	timing.RowCycleDelay						= 6;
	timing.WriteRecoveryTime					= 2;
	timing.RPDelay								= 2;
	timing.RCDDelay								= 2;

	rtn = HAL_SDRAM_Init(&hMemCtrlSDRAM, &timing);
	if(rtn != HAL_OK)						errCnt += 1;

	chkr = MEMCTRL_SDRAM_InitializationSequence(MEMCTRL_REFRESH_COUNT);
	if(chkr == 0)							errCnt += 1;

	if(errCnt == 0)							result = 1;

	return result;
}

static u8 MEMCTRL_NAND_WriteProtectCtrl(u8 data)
{
	u8	result = 0;

	if(data > 1)		return result;

	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_14, (GPIO_PinState)data);

	result = 1;

	return result;
}

static u8 MEMCTRL_NAND_IoInit()
{
	u8	result = 0;
	GPIO_InitTypeDef	gpio;

	gpio.Pin		= GPIO_PIN_14;
	gpio.Mode		= GPIO_MODE_OUTPUT_PP;
	gpio.Pull		= GPIO_PULLUP;
	gpio.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOI, &gpio);

	MEMCTRL_NAND_WriteProtectCtrl(LOW);

	result = 1;

	return result;
}

static u8 MEMCTRL_NAND_Init()
{
	u8	result = 0, errCnt = 0;
	HAL_StatusTypeDef	rtn;
	FMC_NAND_PCC_TimingTypeDef	timing;

	memset(&hMemCtrlNAND, NULL, sizeof(hMemCtrlNAND));

	hMemCtrlNAND.Instance						= FMC_NAND_DEVICE;
	hMemCtrlNAND.Init.NandBank					= FMC_NAND_BANK3;
	hMemCtrlNAND.Init.Waitfeature				= FMC_NAND_WAIT_FEATURE_ENABLE;
	hMemCtrlNAND.Init.MemoryDataWidth			= FMC_NAND_MEM_BUS_WIDTH_8;
	hMemCtrlNAND.Init.EccComputation			= FMC_NAND_ECC_DISABLE;
	hMemCtrlNAND.Init.ECCPageSize				= FMC_NAND_ECC_PAGE_SIZE_4096BYTE;
	hMemCtrlNAND.Init.TCLRSetupTime				= 10;
	hMemCtrlNAND.Init.TARSetupTime				= 10;

	hMemCtrlNAND.Config.PageSize				= 0;
	hMemCtrlNAND.Config.SpareAreaSize			= 0;
	hMemCtrlNAND.Config.BlockSize				= 0;
	hMemCtrlNAND.Config.BlockNbr				= 0;
	hMemCtrlNAND.Config.PlaneNbr				= 0;
	hMemCtrlNAND.Config.PlaneSize				= 0;
	hMemCtrlNAND.Config.ExtraCommandEnable		= DISABLE;

	timing.SetupTime							= 8;
	timing.WaitSetupTime						= 20;
	timing.HoldSetupTime						= 30;
	timing.HiZSetupTime							= 16;

	MEMCTRL_NAND_IoInit();

	rtn = HAL_NAND_Init(&hMemCtrlNAND, &timing, &timing);
	if(rtn != HAL_OK)						errCnt += 1;

	rtn = HAL_NAND_Reset(&hMemCtrlNAND);
	if(rtn != HAL_OK)						errCnt += 1;

	HAL_Delay(5);

	if(errCnt == 0)							result = 1;

	return result;
}

static u8 MEMCTRL_NAND_ModelCheck()
{
	u8	result = 0, errCnt = 0;
	HAL_StatusTypeDef	rtn;
	NAND_IDTypeDef		id;
	FMC_NAND_PCC_TimingTypeDef	timing;

	memCtrlNandStatus	= 0;
	memCtrlNandBusDepth	= 0;

	rtn = HAL_NAND_Read_ID(&hMemCtrlNAND, &id);
	if(rtn != HAL_OK)			return result;

	if(id.Maker_Id != 0x2c)		return result;

	switch(id.Device_Id)
	{
		case 0xcc:		//	16Bit 4GBit
			hMemCtrlNAND.Init.MemoryDataWidth			= FMC_NAND_MEM_BUS_WIDTH_16;

			hMemCtrlNAND.Config.PageSize				= 1024;		// 2048;
			hMemCtrlNAND.Config.SpareAreaSize			= 32;		// 64;
			hMemCtrlNAND.Config.BlockSize				= 64;
			hMemCtrlNAND.Config.BlockNbr				= 4096;
			hMemCtrlNAND.Config.PlaneNbr				= 2;
			hMemCtrlNAND.Config.PlaneSize				= 2048;
			hMemCtrlNAND.Config.ExtraCommandEnable		= ENABLE;

			timing.SetupTime							= 8;		// tcs - twp - 1
			timing.WaitSetupTime						= 20;		// (twp or trp) - 1
			timing.HoldSetupTime						= 30;		// tch
			timing.HiZSetupTime							= 16;		// tcs - tds - 1

			memCtrlNandBusDepth = 16;
			break;

		case 0xc3:		//	16Bit 8GBit //MT29F8G16ABACAWP
			hMemCtrlNAND.Init.MemoryDataWidth			= FMC_NAND_MEM_BUS_WIDTH_16;

			hMemCtrlNAND.Config.PageSize				= 2048;		// 4096;
			hMemCtrlNAND.Config.SpareAreaSize			= 112;		// 224;
			hMemCtrlNAND.Config.BlockSize				= 64;
			hMemCtrlNAND.Config.BlockNbr				= 4096;
			hMemCtrlNAND.Config.PlaneNbr				= 2;
			hMemCtrlNAND.Config.PlaneSize				= 2048;
			hMemCtrlNAND.Config.ExtraCommandEnable		= ENABLE;

			timing.SetupTime							= 4;		// tcs - twp - 1
			timing.WaitSetupTime						= 9;		// (twp or trp) - 1
			timing.HoldSetupTime						= 5;		// tch
			timing.HiZSetupTime							= 7;		// tcs - tds - 1

			memCtrlNandBusDepth = 16;
			break;

#if 1 // yeom_20200915 [start]
		case 0xd3:		//	8Bit 8GBit //MT29F8G08ABACAWP
			hMemCtrlNAND.Init.MemoryDataWidth			= FMC_NAND_MEM_BUS_WIDTH_8;

			hMemCtrlNAND.Config.PageSize				= 4096;		// 4096;
			hMemCtrlNAND.Config.SpareAreaSize			= 224;		// 224;
			hMemCtrlNAND.Config.BlockSize				= 64;
			hMemCtrlNAND.Config.BlockNbr				= 4096;
			hMemCtrlNAND.Config.PlaneNbr				= 2;
			hMemCtrlNAND.Config.PlaneSize				= 2048;
			hMemCtrlNAND.Config.ExtraCommandEnable		= ENABLE;

			timing.SetupTime							= 4;		// tcs - twp - 1
			timing.WaitSetupTime						= 9;		// (twp or trp) - 1
			timing.HoldSetupTime						= 5;		// tch
			timing.HiZSetupTime							= 7;		// tcs - tds - 1

			memCtrlNandBusDepth = 8;
			break;
#endif // yeom_20200915 [end]

		case 0xc5:		//	16Bit 16GBit
			hMemCtrlNAND.Init.MemoryDataWidth			= FMC_NAND_MEM_BUS_WIDTH_16;

			hMemCtrlNAND.Config.PageSize				= 2048;		// 4096;
			hMemCtrlNAND.Config.SpareAreaSize			= 112;		// 224;
			hMemCtrlNAND.Config.BlockSize				= 64;
			hMemCtrlNAND.Config.BlockNbr				= 8192;
			hMemCtrlNAND.Config.PlaneNbr				= 4;
			hMemCtrlNAND.Config.PlaneSize				= 2048;
			hMemCtrlNAND.Config.ExtraCommandEnable		= ENABLE;

			timing.SetupTime							= 4;		// tcs - twp - 1
			timing.WaitSetupTime						= 9;		// (twp or trp) - 1
			timing.HoldSetupTime						= 5;		// tch
			timing.HiZSetupTime							= 7;		// tcs - tds - 1

			memCtrlNandBusDepth = 16;
			break;

		case 0xd5:		//	8Bit 16GBit
			hMemCtrlNAND.Init.MemoryDataWidth			= FMC_NAND_MEM_BUS_WIDTH_8;

			hMemCtrlNAND.Config.PageSize				= 4096;		// 4096;
			hMemCtrlNAND.Config.SpareAreaSize			= 224;		// 224;
			hMemCtrlNAND.Config.BlockSize				= 64;
			hMemCtrlNAND.Config.BlockNbr				= 8192;
			hMemCtrlNAND.Config.PlaneNbr				= 4;
			hMemCtrlNAND.Config.PlaneSize				= 2048;
			hMemCtrlNAND.Config.ExtraCommandEnable		= ENABLE;

			timing.SetupTime							= 4;		// tcs - twp - 1
			timing.WaitSetupTime						= 9;		// (twp or trp) - 1
			timing.HoldSetupTime						= 5;		// tch
			timing.HiZSetupTime							= 7;		// tcs - tds - 1

			memCtrlNandBusDepth = 8;
			break;

		default:
			return result;
			break;
	}

	rtn = HAL_NAND_Init(&hMemCtrlNAND, &timing, &timing);
	if(rtn != HAL_OK)									errCnt += 1;

	rtn = HAL_NAND_Reset(&hMemCtrlNAND);
	if(rtn != HAL_OK)									errCnt += 1;

	HAL_Delay(10);

	if(errCnt == 0)
	{
		memCtrlNandStatus = 1;
		result = 1;
	}

	return result;
}

static u8 MEMCTRL_FPGA_Init()
{
	u8	result = 0;
	HAL_StatusTypeDef	rtn;
	FMC_NORSRAM_TimingTypeDef	timing;

	memset(&hMemCtrlFPGA, NULL, sizeof(hMemCtrlFPGA));

	hMemCtrlFPGA.Instance						= FMC_NORSRAM_DEVICE;
	hMemCtrlFPGA.Extended						= FMC_NORSRAM_EXTENDED_DEVICE;

	hMemCtrlFPGA.Init.NSBank					= FMC_NORSRAM_BANK1;
	hMemCtrlFPGA.Init.DataAddressMux			= FMC_DATA_ADDRESS_MUX_DISABLE;
	hMemCtrlFPGA.Init.MemoryType				= FMC_MEMORY_TYPE_SRAM;
	hMemCtrlFPGA.Init.MemoryDataWidth			= FMC_NORSRAM_MEM_BUS_WIDTH_32;
	hMemCtrlFPGA.Init.BurstAccessMode			= FMC_BURST_ACCESS_MODE_ENABLE;
	hMemCtrlFPGA.Init.WaitSignalPolarity		= FMC_WAIT_SIGNAL_POLARITY_LOW;
	hMemCtrlFPGA.Init.WaitSignalActive			= FMC_WAIT_TIMING_BEFORE_WS;
	hMemCtrlFPGA.Init.WriteOperation			= FMC_WRITE_OPERATION_ENABLE;
	hMemCtrlFPGA.Init.WaitSignal				= FMC_WAIT_SIGNAL_DISABLE;
	hMemCtrlFPGA.Init.ExtendedMode				= FMC_EXTENDED_MODE_DISABLE;
	hMemCtrlFPGA.Init.AsynchronousWait			= FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hMemCtrlFPGA.Init.WriteBurst				= FMC_WRITE_BURST_DISABLE;
	hMemCtrlFPGA.Init.ContinuousClock			= FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
	hMemCtrlFPGA.Init.WriteFifo					= FMC_WRITE_FIFO_DISABLE;
	hMemCtrlFPGA.Init.PageSize					= FMC_PAGE_SIZE_NONE;

	timing.AddressSetupTime						= 2;
	timing.AddressHoldTime						= 3;
//	timing.DataSetupTime						= 6; // 20210601 revision
	timing.DataSetupTime						= 3;
	timing.BusTurnAroundDuration				= 2;
	timing.CLKDivision							= 2;
	timing.DataLatency							= 4;
	timing.AccessMode							= FMC_ACCESS_MODE_A;
/*
	timing.AddressSetupTime						= 0;
	timing.AddressHoldTime						= 1;
	timing.DataSetupTime						= 4;
	timing.BusTurnAroundDuration				= 1;
	timing.CLKDivision							= 2;
	timing.DataLatency							= 2;
*/
	rtn = HAL_SRAM_Init(&hMemCtrlFPGA, &timing, NULL);
	if(rtn == HAL_OK)						result = 1;

	return result;
}

u8 MEMCTRL_Init()
{
	u8	result = 0, rtn, errCnt = 0;

	rtn = MEMCTRL_IoInit();
	if(rtn == 0)			errCnt += 1;

	rtn = MEMCTRL_SDRAM_Init();
	if(rtn == 0)			errCnt += 1;

	rtn = MEMCTRL_FPGA_Init();
	if(rtn == 0)			errCnt += 1;

	rtn = MEMCTRL_NAND_Init();
	if(rtn == 0)			errCnt += 1;

	rtn = MEMCTRL_NAND_ModelCheck();
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 MEMCTRL_NAND_StatusCheck()
{
	u8	result = 0;
	u32	chkr;

	if(memCtrlNandStatus)
	{
		chkr = HAL_NAND_Read_Status(&hMemCtrlNAND);
		if(chkr == NAND_READY)		result = 1;
	}
	
	return result;
}

u8 MEMCTRL_NAND_AdrsCalculator(u32 pageNo, NAND_AddressTypeDef *pAdrs)
{
	u8	result = 0;
	u32	pageMaxSize, blockCnt;

	pageMaxSize		= hMemCtrlNAND.Config.BlockSize;
	pageMaxSize		*= hMemCtrlNAND.Config.BlockNbr;

	if(pageNo >= pageMaxSize)		return result;

	pAdrs->Page		= pageNo % hMemCtrlNAND.Config.BlockSize;

	blockCnt		= pageNo / hMemCtrlNAND.Config.BlockSize;
	
	pAdrs->Block	= blockCnt % hMemCtrlNAND.Config.PlaneSize;

	pAdrs->Plane	= blockCnt / hMemCtrlNAND.Config.PlaneSize;

	result = 1;

	return result;
}

u32 MEMCTRL_NAND_GetSectorCnt()
{
	u32	result = 0;

	switch(memCtrlNandBusDepth)
	{
		case 8:
		case 16:
			result = hMemCtrlNAND.Config.BlockSize * hMemCtrlNAND.Config.BlockNbr;
			break;
	}

	return result;
}

u32 MEMCTRL_NAND_GetSectorSize()
{
	u32	result = 0;

	switch(memCtrlNandBusDepth)
	{
		case 8:
			result = hMemCtrlNAND.Config.PageSize;
			break;

		case 16:
			result = hMemCtrlNAND.Config.PageSize * 2;
			break;
	}

	return result;
}

u32 MEMCTRL_NAND_GetBlockSize()
{
	u32	result = 0;

	switch(memCtrlNandBusDepth)
	{
		case 8:
		case 16:
			result = hMemCtrlNAND.Config.BlockSize;
			break;
	}

	return result;
}

u8 MEMCTRL_NAND_ReadPage(NAND_AddressTypeDef *pAdrs, u8 *pData, u32 size)
{
	u8	result = 0;
	HAL_StatusTypeDef	rtn = HAL_ERROR;

	switch(memCtrlNandBusDepth)
	{
		case 8:
			rtn = HAL_NAND_Read_Page_8b(&hMemCtrlNAND, pAdrs, pData, size);
			break;

		case 16:
			rtn = HAL_NAND_Read_Page_16b(&hMemCtrlNAND, pAdrs, (u16*)pData, size);
			break;
	}

	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 MEMCTRL_NAND_WritePage(NAND_AddressTypeDef *pAdrs, u8 *pData, u32 size)
{
	u8	result = 0, chkr;
	HAL_StatusTypeDef	rtn = HAL_ERROR;

	if(size == 0)
	{
		rtn = HAL_OK;
		return rtn;
	}

	if((pAdrs->Page == 0) && (size == hMemCtrlNAND.Config.BlockSize))
	{
		MEMCTRL_NAND_WriteProtectCtrl(HIGH);

		rtn = HAL_NAND_Erase_Block(&hMemCtrlNAND, pAdrs);
		if(rtn != HAL_OK)
		{
			MEMCTRL_NAND_WriteProtectCtrl(LOW);

			return result;
		}

		switch(memCtrlNandBusDepth)
		{
			case 8:
				rtn = HAL_NAND_Write_Page_8b(&hMemCtrlNAND, pAdrs, pData, size);
				break;

			case 16:
				rtn = HAL_NAND_Write_Page_16b(&hMemCtrlNAND, pAdrs, (uint16_t*)pData, size);
				break;

			default:
				rtn = HAL_ERROR;
				break;
		}

		MEMCTRL_NAND_WriteProtectCtrl(LOW);
	}
	else
	{
		u8	*pBackup;
		u16	savePage;
		u32	readCnt;
		u32	pageSize;
		u32	backupAdrs, changeSize;

		pBackup		= (u8*)MEMORY_MAP_SDRAM_NAND_BACKUP_ADRS;

		savePage	= pAdrs->Page;

		pAdrs->Page	= 0;

		readCnt		= hMemCtrlNAND.Config.BlockSize;

		chkr = MEMCTRL_NAND_ReadPage(pAdrs, pBackup, readCnt);
		if(chkr == 0)		return rtn;

		MEMCTRL_NAND_WriteProtectCtrl(HIGH);

		rtn = HAL_NAND_Erase_Block(&hMemCtrlNAND, pAdrs);
		if(rtn != HAL_OK)
		{
			MEMCTRL_NAND_WriteProtectCtrl(LOW);

			return rtn;
		}

		pageSize	= MEMCTRL_NAND_GetSectorSize();

		backupAdrs	= MEMORY_MAP_SDRAM_NAND_BACKUP_ADRS;
		backupAdrs	+= pageSize * savePage;

		changeSize	= pageSize * size;

		memcpy((void*)backupAdrs, pData, changeSize);

		switch(memCtrlNandBusDepth)
		{
			case 8:
				rtn = HAL_NAND_Write_Page_8b(&hMemCtrlNAND, pAdrs, pBackup, readCnt);
				break;

			case 16:
				rtn = HAL_NAND_Write_Page_16b(&hMemCtrlNAND, pAdrs, (uint16_t*)pBackup, readCnt);
				break;

			default:
				rtn = HAL_ERROR;
				break;
		}

		MEMCTRL_NAND_WriteProtectCtrl(LOW);
	}

	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 MEMCTRL_SDRAM_Clear()
{
	u8	result = 0;
	u8	*pData = (u8*)MEMORY_MAP_SDRAM_BASE_ADRS;

	memset(pData, NULL, 128 * 1024 * 1024);

	result = 1;

	return result;
}


