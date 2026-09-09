#include	"Cmd_MBTest.h"

u8	ngCnt[7] = {0, };
u8	indexCnt = 0;
u8	pModel[10];
u8 	errDetector = 0;

double	hVoltageMin[9], hVoltageMax[9];
double	vmVoltageMin[7], vmVoltageMax[7];
double	vmCurrentMin[7][4], vmCurrentMax[7][4];

double	pmicVoltageMin[4], pmicVoltageMax[4];
double	pmicCurrentMin[5], pmicCurrentMax[5];	

double	oscMin, oscMax;

double	vol_adjavddhMin, vol_adjavddhMax;
double	vol_adjvsselMin, vol_adjvsselMax;
double	vol_adjvglMin, vol_adjvglMax;
double	vol_adjvddelMin, vol_adjvddelMax;

double	cur_1v2Min, cur_1v2Max; 
double	cur_2v5Min, cur_2v5Max;
double	vol_1v2Min, vol_1v2Max;
double	vol_2v5Min, vol_2v5Max;

double	vspVoltageMin, vspVoltageMax;
double	vsnVoltageMin, vsnVoltageMax;

u32	remove_io_ng = 0;
u8	testItem = 0xff;

u8 Cmd_MBTestTestItem(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	testItem = CMD_StrToUL(pData->pArgv[0]);

	result  = 1;

	return	result;
}

static u8 Cmd_MBTestRemoveNgIndex(void *pVoid)
{
	u8	result = 0;
	u32	temp;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	temp = CMD_StrToUL(pData->pArgv[0]);

	remove_io_ng |= temp;

	result  = 1;

	return	result;
}

static u8 RXDPC_CoreRegSet(u8 position, u16 adrs, u32	data)
{
	u8	result = 0, rtn;
	u32 temp;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_REG_READ;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	temp = SYS_HexToWord(&sbParsingData.pRxData[0]);


	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_REG_WRITE;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_WordToHex(data | temp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 RXDPC_CoreRegClear(u8 position, u16 adrs, u32	data)
{
	u8	result = 0, rtn;
	u32 temp;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_REG_READ;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	temp = SYS_HexToWord(&sbParsingData.pRxData[0]);

	
	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_REG_WRITE;

	SYS_HWordToHex(adrs, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_WordToHex(~data & temp, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 4;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

#if 0
static u8 RXDPC_PBTestStart(u8 position)
{
	u8	result = 0, rtn;
	u32 iostate;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_PBTEST_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	iostate = SYS_HexToWord(&sbParsingData.pRxData[0]);

	if(iostate == 0)						
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_PMU_DPMIC_TO_DISPLAY_PANICB_L)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_PMU_DPMIC_TO_DISPLAY_PANICB_L)\r\n", indexCnt);
	}
	else 
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_AOP_PMU_DPMIC_TO_DISPLAY_PANICB_L)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_AOP_PMU_DPMIC_TO_DISPLAY_PANICB_L)\r\n", indexCnt);
		ngCnt[1]++;
	}

	result = 1;

	return result;
}
#endif

static u8 RXDPC_IOTest2Start(u8 position)
{
	u8	result = 0, rtn;
	u32 iostate_open, iostate_short;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_IOTEST2_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 8)		return result;

	iostate_open = SYS_HexToWord(&sbParsingData.pRxData[0]);
	iostate_short = SYS_HexToWord(&sbParsingData.pRxData[4]);

	iostate_open &= ~remove_io_ng;
	iostate_short &= ~remove_io_ng;

	if(iostate_open & 0x00000001 || iostate_short & 0x00000001)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000002 || iostate_short & 0x00000002)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000004 || iostate_short & 0x00000004)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000008 || iostate_short & 0x00000008)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000010 || iostate_short & 0x00000010)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000020 || iostate_short & 0x00000020)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_KRAKEN_TO_TOUCH_AID_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_KRAKEN_TO_TOUCH_AID_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_KRAKEN_TO_TOUCH_AID_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_KRAKEN_TO_TOUCH_AID_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_KRAKEN_TO_TOUCH_AID_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_KRAKEN_TO_TOUCH_AID_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000040 || iostate_short & 0x00000040)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000080 || iostate_short & 0x00000080)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000100 || iostate_short & 0x00000100)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000200 || iostate_short & 0x00000200)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000400 || iostate_short & 0x00000400)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_I2C_SDA_TOUCH_EEPROM_1V8) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_I2C_SDA_TOUCH_EEPROM_1V8) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_I2C_SDA_TOUCH_EEPROM_1V8) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_I2C_SDA_TOUCH_EEPROM_1V8) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_I2C_SDA_TOUCH_EEPROM_1V8)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_I2C_SDA_TOUCH_EEPROM_1V8)\r\n", indexCnt);
	}

	if(iostate_open & 0x00000800 || iostate_short & 0x00000800)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_I2C_SCL_TOUCH_EEPROM_1V8) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_I2C_SCL_TOUCH_EEPROM_1V8) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_I2C_SCL_TOUCH_EEPROM_1V8) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_I2C_SCL_TOUCH_EEPROM_1V8) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_I2C_SCL_TOUCH_EEPROM_1V8)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_I2C_SCL_TOUCH_EEPROM_1V8)\r\n", indexCnt);
	}

	if(iostate_open & 0x00001000 || iostate_short & 0x00001000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN)\r\n", indexCnt);
	}
	
#if 0
	if(iostate_open & 0x00002000 || iostate_short & 0x00002000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN) -- SHORT\r\n", indexCnt);
		}
		ngCnt[1]++;
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", indexCnt);
	}
#endif 

	if(iostate_open & 0x00004000 || iostate_short & 0x00004000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00008000 || iostate_short & 0x00008000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1)\r\n", indexCnt);
	}

	if(iostate_open & 0x00010000 || iostate_short & 0x00010000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1)\r\n", indexCnt);
	}

	if(iostate_open & 0x00020000 || iostate_short & 0x00020000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2)\r\n", indexCnt);
	}

	if(iostate_open & 0x00040000 || iostate_short & 0x00040000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2)\r\n", indexCnt);
	}

	if(iostate_open & 0x00080000 || iostate_short & 0x00080000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (FRAME_GND_MEAS_IN) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (FRAME_GND_MEAS_IN) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (FRAME_GND_MEAS_IN) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (FRAME_GND_MEAS_IN) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (FRAME_GND_MEAS_IN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (FRAME_GND_MEAS_IN)\r\n", indexCnt);
	}

	if(iostate_open & 0x00100000 || iostate_short & 0x00100000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (FTP_KELVIN_DG_0) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (FTP_KELVIN_DG_0) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (FTP_KELVIN_DG_0) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (FTP_KELVIN_DG_0) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (FTP_KELVIN_DG_0)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (FTP_KELVIN_DG_0)\r\n", indexCnt);
	}

	if(iostate_open & 0x00200000 || iostate_short & 0x00200000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (FTP_KELVIN_DG_1) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (FTP_KELVIN_DG_1) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (FTP_KELVIN_DG_1) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (FTP_KELVIN_DG_1) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (FTP_KELVIN_DG_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (FTP_KELVIN_DG_1)\r\n", indexCnt);
	}	

	if(iostate_open & 0x00400000 || iostate_short & 0x00400000)
	{
		if(iostate_open)
		{
			LOG_Printf("%d. IO TEST NG (POGO_VTP_DUT_GND_0) -- OPEN\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (POGO_VTP_DUT_GND_0) -- OPEN\r\n", indexCnt);
		}
		else if(iostate_short)
		{
			LOG_Printf("%d. IO TEST NG (POGO_VTP_DUT_GND_0) -- SHORT\r\n", ++indexCnt);
			CMD_Printf("%d. IO TEST NG (POGO_VTP_DUT_GND_0) -- SHORT\r\n", indexCnt);
		}
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (POGO_VTP_DUT_GND_0)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (POGO_VTP_DUT_GND_0)\r\n", indexCnt);
	}		
	
	result = 1;

	return result;
}

static u8 RXDPC_IOTestXSync(u8 position)
{
	u8	result = 0, rtn, data, errCnt = 0;
	u32 iostate;

	// xsync = 1
	rtn = DPC_DirectRegWriteSingle(position, 0x008A, 0x11);
	if(rtn == 0)						return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_IOTEST_XSYNC;

	data = 1;

	SYS_WordToHex(data, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	iostate = SYS_HexToWord(&sbParsingData.pRxData[0]);

	if(iostate)		errCnt++;					

	// xsync = 0
	rtn = DPC_DirectRegWriteSingle(position, 0x008A, 0x10);
	if(rtn == 0)						return result;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_IOTEST_XSYNC;

	data = 0;

	SYS_WordToHex(data, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 4)		return result;

	iostate = SYS_HexToWord(&sbParsingData.pRxData[0]);

	if(iostate)		errCnt++;	

	if(errCnt > 1)	
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_DISPLAY_XSYNC_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_DISPLAY_XSYNC_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_DISPLAY_XSYNC_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_DISPLAY_XSYNC_CONN)\r\n", indexCnt);
	}
	
	result = 1;

	return result;
}

static u8 RXDPC_CRCTestStart(u8	position)
{
	u8	result = 0, rtn;
	u32 tx_crc_R, tx_crc_G, tx_crc_B, rx_crc_R, rx_crc_G, rx_crc_B;
	dptxSFunction_t	sFunction;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_CRCTEST_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 12)		return result;

	rx_crc_R = SYS_HexToWord(&sbParsingData.pRxData[0]);
	rx_crc_G = SYS_HexToWord(&sbParsingData.pRxData[4]);
	rx_crc_B = SYS_HexToWord(&sbParsingData.pRxData[8]);

	memset(&sFunction, NULL, sizeof(sFunction));
	sFunction.channel	= position + 1;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= 0x0030;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)									tx_crc_R = sFunction.buffer[1];
	else									return	result;
	sFunction.buffer[0]	= 0x0031;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)									tx_crc_G = sFunction.buffer[1];
	else									return	result;
	sFunction.buffer[0]	= 0x0032;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)									tx_crc_B = sFunction.buffer[1];
	else									return	result;

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- CRC TEST --\r\n");
	CMD_Printf("  -- CRC TEST --\r\n");

	if(tx_crc_R != rx_crc_R)
	{
		LOG_Printf("%d. DP TEST NG (CRC_R / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", ++indexCnt,  tx_crc_R, rx_crc_R);
		CMD_Printf("%d. DP TEST NG (CRC_R / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", indexCnt,  tx_crc_R, rx_crc_R);
		BoardTest_NgCheck(DP);
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (CRC_R / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", ++indexCnt,  tx_crc_R, rx_crc_R);
		CMD_Printf("%d. DP TEST OK (CRC_R / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", indexCnt,  tx_crc_R, rx_crc_R);
	}

	if(tx_crc_G != rx_crc_G)
	{
		LOG_Printf("%d. DP TEST NG (CRC_G / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", ++indexCnt,  tx_crc_G, rx_crc_G);
		CMD_Printf("%d. DP TEST NG (CRC_G / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", indexCnt,  tx_crc_G, rx_crc_G);
		BoardTest_NgCheck(DP);
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (CRC_G / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", ++indexCnt,  tx_crc_G, rx_crc_G);
		CMD_Printf("%d. DP TEST OK (CRC_G / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", indexCnt,  tx_crc_G, rx_crc_G);
	}

	if(tx_crc_B != rx_crc_B)
	{
		LOG_Printf("%d. DP TEST NG (CRC_B / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", ++indexCnt,  tx_crc_B, rx_crc_B);
		CMD_Printf("%d. DP TEST NG (CRC_B / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", indexCnt,  tx_crc_B, rx_crc_B);
		BoardTest_NgCheck(DP);
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (CRC_B / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", ++indexCnt,  tx_crc_B, rx_crc_B);
		CMD_Printf("%d. DP TEST OK (CRC_B / DPTX : 0x%08X  DPRX : 0x%08X)\r\n", indexCnt,  tx_crc_B, rx_crc_B);
	}
	
	result = 1;

	return result;
}

static u8 RXDPC_SymbolTestStart(u8	position)
{
	u8	result = 0, rtn, lanecnt;
	u32	tx_control, mask = 0x8000000F, regData;
	u16 err_lane[4];
	dptxSFunction_t	sFunction;

	memset(&sFunction, NULL, sizeof(sFunction));
	sFunction.channel	= position + 1;
	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_READ;
	sFunction.buffer[0]	= 0x00;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn)								tx_control = sFunction.buffer[1];
	else								return result;

	lanecnt = (tx_control & 0x000003E0) >> 5;	// Lane Count = bit [9:5]
	
	regData = tx_control & ~mask;
	regData |= 0x0B;		// TP = 0x0B(PRBS7)

	sFunction.mode		= RP_DP_TX_SFUNCTION_MODE_DP_REG_WRITE;
	sFunction.buffer[0]	= 0x00;
	sFunction.buffer[1]	= regData;
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn == 0)						return result;

	HAL_Delay(500);

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_SYMBOLTEST_START;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 8)		return result;

	err_lane[0] = SYS_HexToWord(&sbParsingData.pRxData[0]);
	err_lane[1] = SYS_HexToWord(&sbParsingData.pRxData[2]);
	err_lane[2] = SYS_HexToWord(&sbParsingData.pRxData[4]);
	err_lane[3] = SYS_HexToWord(&sbParsingData.pRxData[6]);

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- SYMBOL TEST --\r\n");
	CMD_Printf("  -- SYMBOL TEST --\r\n");

	for(int cnt = 0; cnt < lanecnt; cnt++)
	{
		if(err_lane[cnt])
		{
			LOG_Printf("%d. DP TEST NG (SYMBOL / LANE : %d  ERROR COUNT : %d)\r\n", ++indexCnt,  cnt, err_lane[cnt]);
			CMD_Printf("%d. DP TEST NG (SYMBOL / LANE : %d  ERROR COUNT : %d)\r\n", indexCnt,  cnt, err_lane[cnt]);
			BoardTest_NgCheck(DP);
		}
		else
		{
			LOG_Printf("%d. DP TEST OK (SYMBOL / LANE : %d  ERROR COUNT : %d)\r\n", ++indexCnt,  cnt, err_lane[cnt]);
			CMD_Printf("%d. DP TEST OK (SYMBOL / LANE : %d  ERROR COUNT : %d)\r\n", indexCnt,  cnt, err_lane[cnt]);
		}
	}

	sFunction.buffer[1]	= tx_control;
	
	rtn = DPTX_SpecialFunction(&sFunction);
	if(rtn == 0)						return result;

	result = 1;

	return result;
}

static u8 RXDPC_DCTestInit(u8 position)
{
	u8	result = 0, rtn;
	u16	testframes, pattern;

	testframes		= 30;
	pattern			= 24;
	
	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_DCTEST_INIT;

	SYS_HWordToHex(testframes, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;
	SYS_HWordToHex(pattern, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize				+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 0)		return result;

	result = 1;

	return result;
}

static u8 RXDPC_DCTestStart(u8 position)
{
	u8	result = 0, rtn, doneState, errorState;
	u32 errorCnt;

	rtn = SB_DataInit();
	if(rtn == 0)						return result;

	sbParsingData.channel				= position;
	sbParsingData.cmd					= SB_CMD_PWR_M_DPCORE_DCTEST_START;
	sbParsingData.waitTime				= 5000;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)						return result;

	if(sbParsingData.rxSize != 6)		return result;

	doneState = sbParsingData.pRxData[0];
	errorState = sbParsingData.pRxData[1];
	errorCnt = SYS_HexToWord(&sbParsingData.pRxData[2]);

	LOG_Printf("\r\n=========================== DP TEST ===========================\r\n");
	CMD_Printf("\r\n=========================== DP TEST ===========================\r\n");

	LOG_Printf("  -- DATA COMPARE TEST --\r\n");
	CMD_Printf("  -- DATA COMPARE TEST --\r\n");

	if(doneState)
	{
		if(errorState == 0)
		{
			LOG_Printf("%d. DP TEST OK (ERROR COUNT / %d)\r\n", ++indexCnt, errorCnt);
			CMD_Printf("%d. DP TEST OK (ERROR COUNT / %d)\r\n", indexCnt, errorCnt);
		}
		else if(errorState == 1)
		{
			LOG_Printf("%d. DP TEST NG (NO VIDEO DATA)\r\n", ++indexCnt);
			CMD_Printf("%d. DP TEST NG (NO VIDEO DATA)\r\n", indexCnt);
			BoardTest_NgCheck(DP);
		}	
		else if(errorState == 2)
		{
			LOG_Printf("%d. DP TEST NG (ERROR COUNT / %d)\r\n", ++indexCnt, errorCnt);
			CMD_Printf("%d. DP TEST NG (ERROR COUNT / %d)\r\n", indexCnt, errorCnt);
			BoardTest_NgCheck(DP);
		}	
		else		return result;
	}
	
	result = 1;

	return result;
}

u8 Cmd_MBTestCurrentLimit(void *pVoid)
{
	u8 	result = 0;
	u8	vm;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;
	
	if(pData->argc == 0 || pData->argc >= 5)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm"))
	{
		vm = CMD_StrToUL(pData->pArgv[1]);

		switch(vm)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				vmCurrentMin[vm][0] = CMD_AToF(pData->pArgv[2]);
				vmCurrentMax[vm][0] = CMD_AToF(pData->pArgv[3]);
				break;
		}
	}
	else if(CMD_Compare(pData->pArgv[0], "pmic"))
	{
		if(CMD_Compare(pData->pArgv[1], "avddh"))
		{
			pmicCurrentMin[AVDDH] = CMD_AToF(pData->pArgv[2]);
			pmicCurrentMax[AVDDH] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vssel"))
		{
			pmicCurrentMin[VSSEL] = CMD_AToF(pData->pArgv[2]);
			pmicCurrentMax[VSSEL] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vssel_1"))
		{
			pmicCurrentMin[VSSEL_L] = CMD_AToF(pData->pArgv[2]);
			pmicCurrentMax[VSSEL_L] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vgl"))
		{
			pmicCurrentMin[VGL] = CMD_AToF(pData->pArgv[2]);
			pmicCurrentMax[VGL] = CMD_AToF(pData->pArgv[3]);
		}
	}
	else if(CMD_Compare(pData->pArgv[0], "touch"))
	{
		if(CMD_Compare(pData->pArgv[1], "1v2"))
		{
			cur_1v2Min = CMD_AToF(pData->pArgv[2]);
			cur_1v2Max = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "2v5"))
		{
			cur_2v5Min = CMD_AToF(pData->pArgv[2]);
			cur_2v5Max = CMD_AToF(pData->pArgv[3]);
		}
	}

	result  = 1;

	return	result;
}

u8 Cmd_MBTestVoltageLimit(void *pVoid)
{
	u8	result = 0;
	u8	vm;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0 || pData->argc >= 6)		return result;

	if(CMD_Compare(pData->pArgv[0], "vm"))
	{
		vm = CMD_StrToUL(pData->pArgv[1]);

		switch(vm)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				vmVoltageMin[vm] = CMD_AToF(pData->pArgv[2]);
				vmVoltageMax[vm] = CMD_AToF(pData->pArgv[3]);
				break;
		}
	}
	else if(CMD_Compare(pData->pArgv[0], "pmic"))
	{
		if(CMD_Compare(pData->pArgv[1], "avddh"))
		{
			pmicVoltageMin[AVDDH] = CMD_AToF(pData->pArgv[2]);
			pmicVoltageMax[AVDDH] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vssel"))
		{
			pmicVoltageMin[VSSEL] = CMD_AToF(pData->pArgv[2]);
			pmicVoltageMax[VSSEL] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vgl"))
		{
			pmicVoltageMin[VGL] = CMD_AToF(pData->pArgv[2]);
			pmicVoltageMax[VGL] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vddel"))
		{
			pmicVoltageMin[VDDEL] = CMD_AToF(pData->pArgv[2]);
			pmicVoltageMax[VDDEL] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "adj"))
		{
			if(CMD_Compare(pData->pArgv[2], "avddh"))
			{
				vol_adjavddhMin = CMD_AToF(pData->pArgv[3]);
				vol_adjavddhMax = CMD_AToF(pData->pArgv[4]);
			}
			else if(CMD_Compare(pData->pArgv[2], "vssel"))
			{
				vol_adjvsselMin = CMD_AToF(pData->pArgv[3]);
				vol_adjvsselMax = CMD_AToF(pData->pArgv[4]);
			}
			else if(CMD_Compare(pData->pArgv[2], "vgl"))
			{
				vol_adjvglMin = CMD_AToF(pData->pArgv[3]);
				vol_adjvglMax = CMD_AToF(pData->pArgv[4]);
			}
			else if(CMD_Compare(pData->pArgv[2], "vddel"))
			{
				vol_adjvddelMin = CMD_AToF(pData->pArgv[3]);
				vol_adjvddelMax = CMD_AToF(pData->pArgv[4]);
			}
		}
	}
	else if(CMD_Compare(pData->pArgv[0], "touch"))
	{
		if(CMD_Compare(pData->pArgv[1], "1v2"))
		{
			vol_1v2Min = CMD_AToF(pData->pArgv[2]);
			vol_1v2Max = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "2v5"))
		{
			vol_2v5Min = CMD_AToF(pData->pArgv[2]);
			vol_2v5Max = CMD_AToF(pData->pArgv[3]);
		}
	}
	else if(CMD_Compare(pData->pArgv[0], "osc"))
	{
		oscMin = CMD_AToF(pData->pArgv[1]);
		oscMax = CMD_AToF(pData->pArgv[2]);
	}

	result  = 1;

	return	result;
}

static u8 Cmd_MBTestI2c(void *pVoid)
{
	u8	result = 0, pass[7] = {0, }, rCnt, rtn;
	u8	dc_eeprom_adrs;
	u8	mb_eeprom_adrs;
	u8	mbexpander_adrs[2];
	u8	mb_brd;
	u8	dc_brd;
	u8	*pRxBuffer;
	u8	pmic_adrs;
	u8	pmic_ver;
	u8	pmic_id;
	u8	dpRetimer_adrs;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	////////////////////////////////////////////////////
	// Set NG Flag
	// When I2C Error in I2C Test, skip all test
	////////////////////////////////////////////////////
	errDetector = 1;

	////////////////////////////////////////////////////
	// POWER DISABLE VM
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 7; cnt++)
	{
		if(cnt == PWR_SRC_VM_5)		continue;
		
		rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, LOW);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);
	}

	////////////////////////////////////////////////////
	// TEST LED INIT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, 0xFF);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTB, 0xFF);
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// TEST LED ON
	////////////////////////////////////////////////////
	rtn = IoExp_LED_Ctrl(pVoid, TESTING);
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
	// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);		
	
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);	

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN LOW
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0042, GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_042);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L LOW
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, AP_TO_CHIANTI_PANICB_L_IO_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L HIGH
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegSet(pData->position, 0x0044, AP_TO_CHIANTI_PANICB_L_IO_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN HIGH
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegSet(pData->position, 0x0042, GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_042);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// I2C0 Line slave address check
	// -> 0x25(MB_IoExpander_1) check
	// -> 0xA2, 0xA4(EEPROM) check
	////////////////////////////////////////////////////
	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	rtn = LCM_I2CGetSlaveAdrs(pData->position, BSP_I2C1, pRxBuffer, &rCnt);
	if(rtn == 0)				return	result;

	for(u8 i = 0; i < rCnt; i++)
	{
		if(pRxBuffer[i] == 0xA4)
		{
			pass[0] = 1;
			dc_eeprom_adrs = pRxBuffer[i];
		}
		else if(pRxBuffer[i] == 0xA2)
		{
			pass[1] = 1;
			mb_eeprom_adrs = pRxBuffer[i];
		}
		else if(pRxBuffer[i] == 0x4E)
		{
			mbexpander_adrs[0] = pRxBuffer[i];

			//D854 : PORTA = DC_BRD    PORTB = MB_BRD	
			//D847 : PORTA = DC_BRD    PORTB = MB_BRD
			//D878 : PORTA = MB_BRD    PORTB = DC_BRD

			rtn = LCM_I2CMemRead(pData->position, BSP_I2C1, 0x4E, 0x00, 0, &dc_brd, 1);
			if(rtn == 0)				return	result;
			
			HAL_Delay(5);

			rtn = LCM_I2CMemRead(pData->position, BSP_I2C1, 0x4E, 0x01, 0, &mb_brd, 1);
			if(rtn == 0)				return	result;
			
			HAL_Delay(5);
			
			if(mb_brd == 0x80)
			{
				pass[2] = 1;
			}

			if(dc_brd == 0x80)
			{
				pass[3] = 1;
			}
		}
		else if(pRxBuffer[i] == 0x46)
		{
			pass[4] = 1;
			mbexpander_adrs[1] = pRxBuffer[i];
		}
		else if(pRxBuffer[i] == 0x06)
		{
			pass[6] = 1;
			dpRetimer_adrs = pRxBuffer[i];
		}
	}

	rtn = LCM_I2CGetSlaveAdrs(pData->position, BSP_I2C2, pRxBuffer, &rCnt);
	if(rtn == 0)				return	result;

	for(u8 i = 0; i < rCnt; i++)
	{
		if(pRxBuffer[i] == 0xA0)
		{
			u8	rxSize = 1;
			u8	mode = 0;
			u8	memAdrs = 0x18;
			
			pmic_adrs = pRxBuffer[i];

			rtn = LCM_I2CMemRead(pData->position, BSP_I2C2, pmic_adrs, memAdrs, mode, &pmic_ver, rxSize);
			if(rtn == 0)				return	result;

			memAdrs = 0x0C;

			rtn = LCM_I2CMemRead(pData->position, BSP_I2C2, pmic_adrs, memAdrs, mode, &pmic_id, rxSize);
			if(rtn == 0)				return	result;
			
			pass[5] = 1;			
		}
	}

	////////////////////////////////////////////////////
	// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
	// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);		
	
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);	

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN LOW
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0042, GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_042);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L LOW
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, AP_TO_CHIANTI_PANICB_L_IO_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	LOG_Printf("\r\n======================== I2C DEVICE CHECK ========================\r\n");
	CMD_Printf("\r\n======================== I2C DEVICE CHECK ========================\r\n");

	LOG_Printf("  -- MB BOARD EEPROM --\r\n");
	CMD_Printf("  -- MB BOARD EEPROM --\r\n");

	if(pass[0])	
	{
		LOG_Printf("%d. I2C TEST OK (DC EEPROM ADDRESS / 0x%02x)\r\n", ++indexCnt, dc_eeprom_adrs);
		CMD_Printf("%d. I2C TEST OK (DC EEPROM ADDRESS / 0x%02x)\r\n", indexCnt, dc_eeprom_adrs);
	}
	else
	{
		LOG_Printf("%d. I2C TEST NG (DC EEPROM ADDRESS / 0x%02x)\r\n", ++indexCnt, dc_eeprom_adrs);
		CMD_Printf("%d. I2C TEST NG (DC EEPROM ADDRESS / 0x%02x)\r\n", indexCnt, dc_eeprom_adrs);
		BoardTest_NgCheck(I2C);
	}
	
	if(pass[1])
	{
		LOG_Printf("%d. I2C TEST OK (MB EEPROM ADDRESS / 0x%02x)\r\n", ++indexCnt, mb_eeprom_adrs);
		CMD_Printf("%d. I2C TEST OK (MB EEPROM ADDRESS / 0x%02x)\r\n", indexCnt, mb_eeprom_adrs);
	}
	else
	{
		LOG_Printf("%d. I2C TEST NG (MB EEPROM ADDRESS / 0x%02x)\r\n", ++indexCnt, mb_eeprom_adrs);
		CMD_Printf("%d. I2C TEST NG (MB EEPROM ADDRESS / 0x%02x)\r\n", indexCnt, mb_eeprom_adrs);
		BoardTest_NgCheck(I2C);
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- BOARD VERSION / ECO CHECK --\r\n");
	CMD_Printf("  -- BOARD VERSION / ECO CHECK --\r\n");

	if(pass[2])
	{
		LOG_Printf("%d. I2C TEST OK (MB ID_BITS_MB_PARTS / 0x%02x)\r\n", ++indexCnt, mb_brd);
		CMD_Printf("%d. I2C TEST OK (MB ID_BITS_MB_PARTS / 0x%02x)\r\n", indexCnt, mb_brd);
	}
	else
	{
		LOG_Printf("%d. I2C TEST NG (MB ID_BITS_MB_PARTS / 0x%02x)\r\n", ++indexCnt, mb_brd);
		CMD_Printf("%d. I2C TEST NG (MB ID_BITS_MB_PARTS / 0x%02x)\r\n", indexCnt, mb_brd);
		BoardTest_NgCheck(I2C);
	}

	if(pass[3])
	{
		LOG_Printf("%d. I2C TEST OK (MB ID_BITS_DC_PARTS / 0x%02x)\r\n", ++indexCnt, dc_brd);
		CMD_Printf("%d. I2C TEST OK (MB ID_BITS_DC_PARTS / 0x%02x)\r\n", indexCnt, dc_brd);
	}
	else
	{
		LOG_Printf("%d. I2C TEST NG (MB ID_BITS_DC_PARTS / 0x%02x)\r\n", ++indexCnt, dc_brd);
		CMD_Printf("%d. I2C TEST NG (MB ID_BITS_DC_PARTS / 0x%02x)\r\n", indexCnt, dc_brd);
		BoardTest_NgCheck(I2C);
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- MB BOARD EXPANDER --\r\n");
	CMD_Printf("  -- MB BOARD EXPANDER --\r\n");

	if(pass[2] && pass[3])
	{
		LOG_Printf("%d. I2C TEST OK (MB IO EXPANDER 1/ 0x%02x)\r\n", ++indexCnt, mbexpander_adrs[0]);
		CMD_Printf("%d. I2C TEST OK (MB IO EXPANDER 1/ 0x%02x)\r\n", indexCnt, mbexpander_adrs[0]);
	}
	else
	{
		LOG_Printf("%d. I2C TEST NG (MB IO EXPANDER 1/ 0x%02x)\r\n", ++indexCnt, mbexpander_adrs[0]);
		CMD_Printf("%d. I2C TEST NG (MB IO EXPANDER 1/ 0x%02x)\r\n", indexCnt, mbexpander_adrs[0]);
		BoardTest_NgCheck(I2C);
	}

	if(pass[4])
	{
		LOG_Printf("%d. I2C TEST OK (MB IO EXPANDER 2/ 0x%02x)\r\n", ++indexCnt, mbexpander_adrs[1]);
		CMD_Printf("%d. I2C TEST OK (MB IO EXPANDER 2/ 0x%02x)\r\n", indexCnt, mbexpander_adrs[1]);
	}
	else
	{
		LOG_Printf("%d. I2C TEST NG (MB IO EXPANDER 2/ 0x%02x)\r\n", ++indexCnt, mbexpander_adrs[1]);
		CMD_Printf("%d. I2C TEST NG (MB IO EXPANDER 2/ 0x%02x)\r\n", indexCnt, mbexpander_adrs[1]);
		BoardTest_NgCheck(I2C);
	}
	
	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- %s PMIC I2C CHECK --\r\n", pModel);
	CMD_Printf("  -- %s PMIC I2C CHECK --\r\n", pModel);

	if(pass[5])
	{
		LOG_Printf("%d. I2C TEST OK (PMIC ADDRESS / 0x%02x)\r\n", ++indexCnt, pmic_adrs);
		CMD_Printf("%d. I2C TEST OK (PMIC ADDRESS / 0x%02x)\r\n", indexCnt, pmic_adrs);

		if(pmic_id == 0x61)
		{
			LOG_Printf("%d. I2C TEST OK (DEVICE ID / 0x%02x)\r\n", ++indexCnt, pmic_id);
			CMD_Printf("%d. I2C TEST OK (DEVICE ID / 0x%02x)\r\n", indexCnt, pmic_id);
		}
		else
		{
			LOG_Printf("%d. I2C TEST NG (DEVICE ID / 0x%02x)\r\n", ++indexCnt, pmic_id);
			CMD_Printf("%d. I2C TEST NG (DEVICE ID / 0x%02x)\r\n", indexCnt, pmic_id);
			BoardTest_NgCheck(I2C);
			
			rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_5, LOW);
			if(rtn == 0)				return	result;
		}
	}	
	else
	{		
		LOG_Printf("%d. I2C TEST NG (PMIC ADDRESS / 0x%02x)\r\n", ++indexCnt, pmic_adrs);
		CMD_Printf("%d. I2C TEST NG (PMIC ADDRESS / 0x%02x)\r\n", indexCnt, pmic_adrs);
		BoardTest_NgCheck(I2C);
		
		rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_5, LOW);
		if(rtn == 0)				return	result;
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- DP REDRIVER I2C CHECK --\r\n");
	CMD_Printf("  -- DP REDRIVER I2C CHECK --\r\n");

	if(pass[6])
	{
		LOG_Printf("%d. I2C TEST OK (DP REDRIVER ADDRESS / 0x%02x)\r\n", ++indexCnt, dpRetimer_adrs);
		CMD_Printf("%d. I2C TEST OK (DP REDRIVER ADDRESS / 0x%02x)\r\n", indexCnt, dpRetimer_adrs);
	}	
	else
	{		
		LOG_Printf("%d. I2C TEST NG (DP REDRIVER ADDRESS / 0x%02x)\r\n", ++indexCnt, dpRetimer_adrs);
		CMD_Printf("%d. I2C TEST NG (DP REDRIVER ADDRESS / 0x%02x)\r\n", indexCnt, dpRetimer_adrs);
		BoardTest_NgCheck(I2C);
	}

	testItem--;
	
	////////////////////////////////////////////////////
	// REMOVE NG FLAG
	// When I2C Error in I2C Test, skip all test
	////////////////////////////////////////////////////
	errDetector = 0;

	result = 1;

	return	result;
}

static u8 Cmd_MBTestSerialChk(void *pVoid)
{
	u8	result = 0, rtn;
	u8	channel, slaveAdrs, mode;
	u8	*pRxBuffer;
	u16	memAdrs;
	u32	rxSize;
	cmdFunctionData_t	*pData;
	u8 Y_copy;
	u8 W_copy, W1_copy, W2_copy;
	u8 D_copy;
	u8 S1_copy, S2_copy, S3_copy, S4_copy;
	//u8 P1_copy =16, P2_copy = 8, P3_copy = 24;
	//u8 E1_copy =23, E2_copy = 22, E3_copy = 13, E4_copy = 8;
	u8 R_copy = 0;
	u16 x1, x2, x;
	u8 x_result;
	u8 	ptxbuffer2[16];
	u16	cnt = 0;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return 	result;

	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	LOG_Printf("\r\n======================== VENTURA ID CHECK ========================\r\n");
	CMD_Printf("\r\n======================== VENTURA ID CHECK ========================\r\n");

	if(setpppflag == 0 || seteeeeflag == 0)
	{
		P_1 = 'F';
		P_2	= 'F';
		P_3 = 'F';

		E1 = 'F';
		E2 = 'F';
		E3 = 'F';
		E4 = 'F';
					
		LOG_Printf("%d. VENTURA ID CHECK NG (NEED TO SET TEST MODEL!)\r\n", ++indexCnt);
		CMD_Printf("%d. VENTURA ID CHECK NG (NEED TO SET TEST MODEL!)\r\n", indexCnt);
		BoardTest_NgCheck(VID);
	}

	channel = pData->option - 1;

	slaveAdrs = 0xA2;

	memAdrs = 0xFA;

	rxSize = 6;

	mode = 0;

	rtn = LCM_I2CMemRead(pData->position, channel, slaveAdrs, memAdrs, mode, pRxBuffer, rxSize);
	if(rtn == 0)				return	result;
	
	HAL_Delay(5);

	////////////////////////////////////////////////////
	// EEPROM FORMAT
	////////////////////////////////////////////////////
	memset(ptxbuffer, 0xFF, sizeof(ptxbuffer));

	memAdrs = 0x00;

	do{
		rtn = LCM_I2CMemWrite(pData->position, channel, 0xA2, memAdrs, mode, ptxbuffer, 16);
		if(rtn == 0)				return	result;
		
		HAL_Delay(5);

		rtn = LCM_I2CMemWrite(pData->position, channel, 0xA4, memAdrs, mode, ptxbuffer, 16);
		if(rtn == 0)				return	result;
		
		HAL_Delay(5);
		memAdrs = (cnt + 1) << 4;

		cnt++;

		if(memAdrs > 0xf0)	break;
	}while(1);
	
//Y calculate
	if(pRxBuffer[2] == 0x00)
	{
		Y_copy=0;
	}
	else if(pRxBuffer[2] == 0x01)
	{
		Y_copy= 1;
	}
	else if(pRxBuffer[2] == 0x12)
	{
		Y_copy=8;
	}
	sprintf(&Y, "%d", Y_copy);
//WW calculate
	W_copy = (pRxBuffer[3]/7)+1;
	W1_copy = W_copy/10;
	W2_copy = W_copy-W1_copy*10;
	sprintf(&W1, "%d", W1_copy);
	sprintf(&W2, "%d", W2_copy);
//D calculate
	D_copy = pRxBuffer[3]%7;
	D_copy = D_copy +1;
	sprintf(&D, "%d", D_copy);
//SSSS calculate
	S1_copy = (pRxBuffer[4]>>4);
	S2_copy = (pRxBuffer[4]-(S1_copy<<4));
	S3_copy = (pRxBuffer[5]>>4);
	S4_copy = (pRxBuffer[5]-(S3_copy<<4));
	sprintf(&S1, "%x", S1_copy);
	if(S1 >0x60) S1=S1-32;
	sprintf(&S2, "%x", S2_copy);
	if(S2 >0x60) S2=S2-32;
	sprintf(&S3, "%x", S3_copy);
	if(S3 >0x60) S3=S3-32;
	sprintf(&S4, "%x", S4_copy);
	if(S4 >0x60) S4=S4-32;

//PPP, EEEE Transfer
	x1 = (P2_copy + Y_copy + W2_copy + S1_copy + S3_copy + E1_copy + E3_copy + R_copy)*3;
	x2 =  P1_copy + P3_copy + W1_copy + D_copy + S2_copy + S4_copy + E2_copy + E4_copy;
	x= x1+x2;
	x = x%34;
	x_result = (u8)(34-x);
	//x_result = 30;
	if(x_result>15)
	{
	   if(x_result == 16)
	   {
		 X='G';
	   }
	   if(x_result == 17)
	   {
		 X='H';
	   }
	   if(x_result == 18)
	   {
		 X='J';
	   }
	   if(x_result == 19)
	   {
		 X='K';
	   }
	   if(x_result == 20)
	   {
		 X='L';
	   }
	   if(x_result == 21)
	   {
		 X='M';
	   }
	   if(x_result == 22)
	   {
		 X='N';
	   }
	   if(x_result == 23)
	   {
		 X='P';
	   }
	   if(x_result == 24)
	   {
		 X='Q';
	   }
	   if(x_result == 25)
	   {
		 X='R';
	   }
	   if(x_result == 26)
	   {
		 X='S';
	   }
	   if(x_result == 27)
	   {
		 X='T';
	   }
	   if(x_result == 28)
	   {
		 X='U';
	   }
	   if(x_result == 29)
	   {
		 X='V';
	   }
	   if(x_result == 30)
	   {
		 X='W';
	   }
	   if(x_result == 31)
	   {
		 X='X';
	   }
	   if(x_result == 32)
	   {
		 X='Y';
	   }
	   if(x_result == 33)
	   {
		 X='Z';
	   }
	   if(x_result == 34)
	   {
		 X='0';
	   }
	}

	if(x_result<=15)
	{
	  sprintf(&X, "%x", x_result);
	  if(X >0x60) X=X-32;
	}

	ptxbuffer[0] = P_1; ptxbuffer[1] = P_2; ptxbuffer[2] = P_3; ptxbuffer[3] = Y; ptxbuffer[4] = W1; ptxbuffer[5] = W2; ptxbuffer[6] = D; ptxbuffer[7] = S1; ptxbuffer[8] = S2;
	ptxbuffer[9] = S3; ptxbuffer[10] = S4; ptxbuffer[11] = E1; ptxbuffer[12] = E2; ptxbuffer[13] = E3; ptxbuffer[14] = E4; ptxbuffer[15] = R; ptxbuffer[16] = X;

	LOG_Printf("  -- 0xA2 EEPROM READ --\r\n");
	CMD_Printf("  -- 0xA2 EEPROM READ --\r\n");
	
	ptxbuffer2[0] = 0x61;
	
	for(u16 cnt = 0; cnt < 15; cnt++)
	{
	  ptxbuffer2[cnt + 1] = ptxbuffer[cnt];
	}

	rtn = LCM_I2CTransmitData(pData->position, channel, slaveAdrs, ptxbuffer2, 16);
	if(rtn == 0)				return	result;
	
	HAL_Delay(5);
	
	ptxbuffer2[0] = 0x70;
	ptxbuffer2[1] = ptxbuffer[15];
	ptxbuffer2[2] = ptxbuffer[16];

	rtn = LCM_I2CTransmitData(pData->position, channel, slaveAdrs, ptxbuffer2, 3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(5);

	slaveAdrs = 0xA2;

	memAdrs = 0x61;

	rxSize = 17;

	mode = 0;
	
	rtn = LCM_I2CMemRead(pData->position, channel, slaveAdrs, memAdrs, mode, prxbuffer, rxSize);
	if(rtn == 0)				return	result;
	
	HAL_Delay(5);

	for(u8 cnt = 0; cnt < 17; cnt++)
	{
		if(ptxbuffer[cnt] != prxbuffer[cnt])	ngCnt[VID]++;
	}

	LOG_Printf("%d. VENTURA ID HEX SOURCE (", ++indexCnt);
	CMD_Printf("%d. VENTURA ID HEX SOURCE (", indexCnt);
	
	for(u8 cnt = 0; cnt < 6; cnt++)
	{
		if(cnt < 5)
		{
			LOG_Printf("%02x", pRxBuffer[cnt]);
			CMD_Printf("%02x", pRxBuffer[cnt]);
		}
		else if(cnt == 5)
		{
			LOG_Printf("%02x)\r\n", pRxBuffer[cnt]);
			CMD_Printf("%02x)\r\n", pRxBuffer[cnt]);
		}	
	}

	LOG_Printf("%d. VENTURA ID DEC SOURCE (", ++indexCnt);
	CMD_Printf("%d. VENTURA ID DEC SOURCE (", indexCnt);
	
	for(u8 cnt = 0; cnt < 6; cnt++)
	{
		if(cnt < 5)
		{
			LOG_Printf("%d ", pRxBuffer[cnt]);
			CMD_Printf("%d ", pRxBuffer[cnt]);
		}
		else if(cnt == 5)
		{
			LOG_Printf("%d)\r\n", pRxBuffer[cnt]);
			CMD_Printf("%d)\r\n", pRxBuffer[cnt]);
		}	
	}

	LOG_Printf("%d. VENTURA ID ASCII (", ++indexCnt);
	CMD_Printf("%d. VENTURA ID ASCII (", indexCnt);

	for(u8 cnt = 0; cnt < 17; cnt++)
	{
		if(cnt < 16)
		{
			LOG_Printf("%c", ptxbuffer[cnt]);
			CMD_Printf("%c", ptxbuffer[cnt]);
		}
		else if(cnt == 16)
		{
			LOG_Printf("%c)\r\n", ptxbuffer[cnt]);
			CMD_Printf("%c)\r\n", ptxbuffer[cnt]);
		}
	}
	
	if(ngCnt[VID] == 0)
	{
		LOG_Printf("%d. VENTURA ID WRITE OK (", ++indexCnt);
		CMD_Printf("%d. VENTURA ID WRITE OK (", indexCnt);
	}
	else
	{
		LOG_Printf("%d. VENTURA ID WRITE NG (", ++indexCnt);
		CMD_Printf("%d. VENTURA ID WRITE NG (", indexCnt);
	}

	for(u8 cnt = 0; cnt < 17; cnt++)
	{
		if(cnt == 0)
		{
			LOG_Printf("0x%02x", ptxbuffer[cnt]);
			CMD_Printf("0x%02x", ptxbuffer[cnt]);
		}
		else if(cnt == 16)
		{
			LOG_Printf(" 0x%02x)\r\n", ptxbuffer[cnt]);
			CMD_Printf(" 0x%02x)\r\n", ptxbuffer[cnt]);
		}
		else
		{
			LOG_Printf(" 0x%02x", ptxbuffer[cnt]);
			CMD_Printf(" 0x%02x", ptxbuffer[cnt]);
		}	
	}

	if(ngCnt[VID] == 0)
	{
		LOG_Printf("%d. VENTURA ID VERIFY OK (", ++indexCnt);
		CMD_Printf("%d. VENTURA ID VERIFY OK (", indexCnt);
	}
	else
	{
		LOG_Printf("%d. VENTURA ID VERIFY NG (", ++indexCnt);
		CMD_Printf("%d. VENTURA ID VERIFY NG (", indexCnt);
	}

	for(u8 cnt = 0; cnt < 17; cnt++)
	{
		if(cnt == 0)
		{
			LOG_Printf("0x%02x", prxbuffer[cnt]);
			CMD_Printf("0x%02x", prxbuffer[cnt]);
		}
		else if(cnt == 16)
		{
			LOG_Printf(" 0x%02x)\r\n", prxbuffer[cnt]);
			CMD_Printf(" 0x%02x)\r\n", prxbuffer[cnt]);
		}
		else
		{
			LOG_Printf(" 0x%02x", prxbuffer[cnt]);
			CMD_Printf(" 0x%02x", prxbuffer[cnt]);
		}
	}

	slaveAdrs = 0xA4;

	memAdrs = 0xFA;

	rxSize = 6;

	mode = 0;

	rtn = LCM_I2CMemRead(pData->position, channel, slaveAdrs, memAdrs, mode, pRxBuffer, rxSize);
	if(rtn == 0)				return	result;
	
	HAL_Delay(5);

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");
	
	LOG_Printf("  -- 0xA4 EEPROM READ --\r\n");
	CMD_Printf("  -- 0xA4 EEPROM READ --\r\n");
	
	LOG_Printf("%d. DC EEPROM HEX SOURCE (", ++indexCnt);
	CMD_Printf("%d. DC EEPROM HEX SOURCE (", indexCnt);
	
	for(u8 cnt = 0; cnt < 6; cnt++)
	{
		if(cnt < 5)
		{
			LOG_Printf("%02x", pRxBuffer[cnt]);
			CMD_Printf("%02x", pRxBuffer[cnt]);
		}
		else if(cnt == 5)
		{
			LOG_Printf("%02x)\r\n", pRxBuffer[cnt]);
			CMD_Printf("%02x)\r\n", pRxBuffer[cnt]);
		}	
	}

	LOG_Printf("%d. DC EEPROM DEC SOURCE (", ++indexCnt);
	CMD_Printf("%d. DC EEPROM DEC SOURCE (", indexCnt);

	for(u8 cnt = 0; cnt < 6; cnt++)
	{
		if(cnt < 5)
		{
			LOG_Printf("%d ", pRxBuffer[cnt]);
			CMD_Printf("%d ", pRxBuffer[cnt]);
		}
		else if(cnt == 5)
		{
			LOG_Printf("%d)\r\n", pRxBuffer[cnt]);
			CMD_Printf("%d)\r\n", pRxBuffer[cnt]);
		}	
	}

	memset(ptxbuffer, 0, sizeof(ptxbuffer));
	memset(prxbuffer, 0, sizeof(prxbuffer));

	setpppflag = 0;
	seteeeeflag = 0;

	testItem--;

	result = 1;

	return	result;
}

static u8 Cmd_MBTestOpenShort(void *pVoid)
{
	u8	result = 0, rtn, high = 0, low = 0;
	bitCtrl8_t	Rxbuf;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;

	LOG_Printf("\r\n======================== OPEN SHORT TEST ========================\r\n");
	CMD_Printf("\r\n======================== OPEN SHORT TEST ========================\r\n");

	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTB, P2);
	if(rtn == 0)			return result;

	HAL_Delay(10);

	rtn = IoExp_PortInputRead(pVoid, BSP_I2C2, 0x40, PORTA, &Rxbuf.u8Data);
	if(rtn == 0)			return result;

	if(Rxbuf.b5 == 0)	low = 1;

	rtn = IoExp_PortSet(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTB, P2);
	if(rtn == 0)			return result;

	HAL_Delay(10);

	rtn = IoExp_PortInputRead(pVoid, BSP_I2C2, 0x40, PORTA, &Rxbuf.u8Data);
	if(rtn == 0)			return result;

	if(Rxbuf.b5 == 1)	high = 1;
	
	if(high == 0 || low == 0)
	{
		LOG_Printf("%d. IO TEST NG (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", indexCnt);
	}

	rtn = RXDPC_IOTest2Start(pData->position);
	if(rtn == 0)				return	result;
	
	rtn = RXDPC_IOTestXSync(pData->position);
	if(rtn == 0)				return result;

	//rtn = RXDPC_PBTestStart(pData->position);
	//if(rtn == 0)				return result;

	testItem--;

	result = 1;

	return	result;
}	

static u8 Cmd_MBTestPower(void *pVoid)
{
	u8	result = 0, rtn;
	
	double	pp1v24_dp_s2_conn_sense[2];
	double	pp1v2_tc_dp_s2_conn_sense[2];
	double	spi_alpdp_wpb_vpp_sense[2];
	double	pp1v8_tc_dp_dvdd_conn_sense[2];
	double	pp1vx_dp_s2_conn_sense[2];
	double	pp3v15_tc_dp_s2_conn_sense[2];

	double	dc_pp1v24_dp_s2_conn_sense;
	double	dc_pp1v2_tc_dp_s2_conn_sense;
	double	dc_spi_alpdp_wpb_vpp_sense;
	double	dc_pp1v8_tc_dp_dvdd_conn_sense;
	double	dc_pp1vx_dp_s2_conn_sense;
	double	dc_pp3v15_tc_dp_s2_conn_sense;

	double	pp1v2_tc_vddcore_conn_sense[2];
	double	htp_s_0[2];
	double	vtp_s_1[2];

	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;

	////////////////////////////////////////////////////
	// POWER ENABLE VM
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 7; cnt++)
	{
		if(cnt == PWR_SRC_VM_5)		continue;
		
		rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, HIGH);
		if(rtn == 0)					return	result;
		
		HAL_Delay(20);
	}

	//*****************************
	// 
	// POWER VOLTAGE CHECK
	//
	//*****************************

	////////////////////////////////////////////////////
	// VOUT VOLTAGE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_0, &pp3v15_tc_dp_s2_conn_sense[0], &pp3v15_tc_dp_s2_conn_sense[1]);
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_1, &pp1vx_dp_s2_conn_sense[0], &pp1vx_dp_s2_conn_sense[1]); 
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_2, &pp1v8_tc_dp_dvdd_conn_sense[0], &pp1v8_tc_dp_dvdd_conn_sense[1]);
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_3, &spi_alpdp_wpb_vpp_sense[0], &spi_alpdp_wpb_vpp_sense[1]);
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_4, &pp1v2_tc_dp_s2_conn_sense[0], &pp1v2_tc_dp_s2_conn_sense[1]);
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_6, &pp1v24_dp_s2_conn_sense[0], &pp1v24_dp_s2_conn_sense[1]);	
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// VOLTAGE SENSEING ADMUX ENABLE
	// 전압 측정을 위해 VOLTAGE AMUX EN 할 때, 
	// P2 혹은 P3 같이 SET 하지 않으면 측정 전압 Drop 증상 발생
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P3 | P4 | P5);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3 | P4);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// AMUX_S1 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// POWER MODULE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dc_pp1vx_dp_s2_conn_sense);	
	if(rtn == 0)				return	result;
	
	////////////////////////////////////////////////////
	// AMUX_S2 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// POWER MODULE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dc_pp1v2_tc_dp_s2_conn_sense);	
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S3 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// POWER MODULE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dc_pp1v8_tc_dp_dvdd_conn_sense);	
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S4 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S4);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// POWER MODULE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dc_pp3v15_tc_dp_s2_conn_sense);	
	if(rtn == 0)				return	result;
	
	////////////////////////////////////////////////////
	// AMUX_S5 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S5);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// TOUCH VOLTAGE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &pp1v2_tc_vddcore_conn_sense[0]);
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S6 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S6);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_0 ENABLE
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, UTIL_1V8_GPIO_0_044 | UTIL_1V8_GPIO_1_044);
	if(rtn == 0)				return	result;
	
	rtn = RXDPC_CoreRegSet(pData->position, 0x0044, UTIL_1V8_GPIO_0_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);	

	////////////////////////////////////////////////////
	// TOUCH VOLTAGE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &vtp_s_1[0]); 	
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_1 ENABLE
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, UTIL_1V8_GPIO_0_044 | UTIL_1V8_GPIO_1_044);
	if(rtn == 0)				return	result;
	
	rtn = RXDPC_CoreRegSet(pData->position, 0x0044, UTIL_1V8_GPIO_1_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);	

	////////////////////////////////////////////////////
	// TOUCH VOLTAGE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &htp_s_0[0]);  
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S7 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S7);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// POWER MODULE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dc_spi_alpdp_wpb_vpp_sense);	
	if(rtn == 0)				return	result;
	
	////////////////////////////////////////////////////
	// AMUX_S8 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S8);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// POWER MODULE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dc_pp1v24_dp_s2_conn_sense);	
	if(rtn == 0)				return	result;

	//*****************************
	// 
	// POWER CURRENT CHECK
	//
	//*****************************
	
	////////////////////////////////////////////////////
	// CURRENT SENSEING ADMUX ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P3 | P4 | P5);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// ADMUX S1 CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);
	
	////////////////////////////////////////////////////
	// TOUCH CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &pp1v2_tc_vddcore_conn_sense[1]);	
	if(rtn == 0)				return	result;

	pp1v2_tc_vddcore_conn_sense[1] *= 1000;
	pp1v2_tc_vddcore_conn_sense[1] /= 41; // Gain = 41
	pp1v2_tc_vddcore_conn_sense[1] /= TOUCH_SHUNT;

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_0 ENABLE
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, UTIL_1V8_GPIO_0_044 | UTIL_1V8_GPIO_1_044);
	if(rtn == 0)				return	result;
	
	rtn = RXDPC_CoreRegSet(pData->position, 0x0044, UTIL_1V8_GPIO_0_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);	
	
	////////////////////////////////////////////////////
	// ADMUX S3 CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// TOUCH CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &vtp_s_1[1]); 	
	if(rtn == 0)				return	result;

	vtp_s_1[1] *= 1000;
	vtp_s_1[1] /= 41; // Gain = 41
	vtp_s_1[1] /= TOUCH_SHUNT;

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_1 ENABLE
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, UTIL_1V8_GPIO_0_044 | UTIL_1V8_GPIO_1_044);
	if(rtn == 0)				return	result;
	
	rtn = RXDPC_CoreRegSet(pData->position, 0x0044, UTIL_1V8_GPIO_1_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);	
	
	////////////////////////////////////////////////////
	// ADMUX S2 CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// TOUCH CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &htp_s_0[1]);
	if(rtn == 0)				return	result;

	htp_s_0[1] *= 1000;
	htp_s_0[1] /= 41; // Gain = 41
	htp_s_0[1] /= TOUCH_SHUNT;

	////////////////////////////////////////////////////
	// POWER DISABLE VM
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 7; cnt++)
	{
		if(cnt == PWR_SRC_VM_5)		continue;
		
		rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, LOW);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);
	}

	LOG_Printf("\r\n========================== POWER TEST ==========================\r\n");
	CMD_Printf("\r\n========================== POWER TEST ==========================\r\n");

	LOG_Printf("  -- POGO POWER CHECK --\r\n");
	CMD_Printf("  -- POGO POWER CHECK --\r\n");

	if(pp3v15_tc_dp_s2_conn_sense[0] < vmVoltageMin[0] || pp3v15_tc_dp_s2_conn_sense[0] > vmVoltageMax[0] || 
		pp3v15_tc_dp_s2_conn_sense[1] < vmCurrentMin[0][0] || pp3v15_tc_dp_s2_conn_sense[1] > vmCurrentMax[0][0])
	{
		LOG_Printf("%d. POWER TEST NG (PP3V15_TOUCH_DISPLAY_S2_CONN / %f V %f mA)\r\n", ++indexCnt, pp3v15_tc_dp_s2_conn_sense[0], pp3v15_tc_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (PP3V15_TOUCH_DISPLAY_S2_CONN / %f V %f mA)\r\n", indexCnt, pp3v15_tc_dp_s2_conn_sense[0], pp3v15_tc_dp_s2_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (PP3V15_TOUCH_DISPLAY_S2_CONN / %f V %f mA)\r\n", ++indexCnt, pp3v15_tc_dp_s2_conn_sense[0], pp3v15_tc_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (PP3V15_TOUCH_DISPLAY_S2_CONN / %f V %f mA)\r\n", indexCnt, pp3v15_tc_dp_s2_conn_sense[0], pp3v15_tc_dp_s2_conn_sense[1]);
	}

	if(pp1vx_dp_s2_conn_sense[0] < vmVoltageMin[1] || pp1vx_dp_s2_conn_sense[0] > vmVoltageMax[1] || 
		pp1vx_dp_s2_conn_sense[1] < vmCurrentMin[1][0] || pp1vx_dp_s2_conn_sense[1] > vmCurrentMax[1][0])
	{
		LOG_Printf("%d. POWER TEST NG (PP1VX_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1vx_dp_s2_conn_sense[0], pp1vx_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (PP1VX_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1vx_dp_s2_conn_sense[0], pp1vx_dp_s2_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (PP1VX_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1vx_dp_s2_conn_sense[0], pp1vx_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (PP1VX_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1vx_dp_s2_conn_sense[0], pp1vx_dp_s2_conn_sense[1]);
	}
	
	if(pp1v8_tc_dp_dvdd_conn_sense[0] < vmVoltageMin[2] || pp1v8_tc_dp_dvdd_conn_sense[0] > vmVoltageMax[2] || 
		pp1v8_tc_dp_dvdd_conn_sense[1] < vmCurrentMin[2][0] || pp1v8_tc_dp_dvdd_conn_sense[1] > vmCurrentMax[2][0])
	{
		LOG_Printf("%d. POWER TEST NG (PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v8_tc_dp_dvdd_conn_sense[0], pp1v8_tc_dp_dvdd_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V  %f mA)\r\n", indexCnt, pp1v8_tc_dp_dvdd_conn_sense[0], pp1v8_tc_dp_dvdd_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v8_tc_dp_dvdd_conn_sense[0], pp1v8_tc_dp_dvdd_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V  %f mA)\r\n", indexCnt, pp1v8_tc_dp_dvdd_conn_sense[0], pp1v8_tc_dp_dvdd_conn_sense[1]);
	}

	if(spi_alpdp_wpb_vpp_sense[0] < vmVoltageMin[3] || spi_alpdp_wpb_vpp_sense[0] > vmVoltageMax[3] || 
		spi_alpdp_wpb_vpp_sense[1] < vmCurrentMin[3][0] || spi_alpdp_wpb_vpp_sense[1] > vmCurrentMax[3][0])
	{
		LOG_Printf("%d. POWER TEST NG (SPI_ALPDP_WPB_VPP / %f V  %f mA)\r\n", ++indexCnt, spi_alpdp_wpb_vpp_sense[0], spi_alpdp_wpb_vpp_sense[1]);
		CMD_Printf("%d. POWER TEST NG (SPI_ALPDP_WPB_VPP / %f V  %f mA)\r\n", indexCnt, spi_alpdp_wpb_vpp_sense[0], spi_alpdp_wpb_vpp_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (SPI_ALPDP_WPB_VPP / %f V  %f mA)\r\n", ++indexCnt, spi_alpdp_wpb_vpp_sense[0], spi_alpdp_wpb_vpp_sense[1]);
		CMD_Printf("%d. POWER TEST OK (SPI_ALPDP_WPB_VPP / %f V  %f mA)\r\n", indexCnt, spi_alpdp_wpb_vpp_sense[0], spi_alpdp_wpb_vpp_sense[1]);
	}

	if(pp1v2_tc_dp_s2_conn_sense[0] < vmVoltageMin[4] || pp1v2_tc_dp_s2_conn_sense[0] > vmVoltageMax[4] || 
		pp1v2_tc_dp_s2_conn_sense[1] < vmCurrentMin[4][0] || pp1v2_tc_dp_s2_conn_sense[1] > vmCurrentMax[4][0])
	{
		LOG_Printf("%d. POWER TEST NG (PP1V2_TOUCH_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v2_tc_dp_s2_conn_sense[0], pp1v2_tc_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (PP1V2_TOUCH_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1v2_tc_dp_s2_conn_sense[0], pp1v2_tc_dp_s2_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (PP1V2_TOUCH_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v2_tc_dp_s2_conn_sense[0], pp1v2_tc_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (PP1V2_TOUCH_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1v2_tc_dp_s2_conn_sense[0], pp1v2_tc_dp_s2_conn_sense[1]);
	}

	if(pp1v24_dp_s2_conn_sense[0] < vmVoltageMin[6] || pp1v24_dp_s2_conn_sense[0] > vmVoltageMax[6] || 
		pp1v24_dp_s2_conn_sense[1] < vmCurrentMin[6][0] || pp1v24_dp_s2_conn_sense[1] > vmCurrentMax[6][0])
	{
		LOG_Printf("%d. POWER TEST NG (PP1V24_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v24_dp_s2_conn_sense[0], pp1v24_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (PP1V24_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1v24_dp_s2_conn_sense[0], pp1v24_dp_s2_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (PP1V24_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v24_dp_s2_conn_sense[0], pp1v24_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (PP1V24_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1v24_dp_s2_conn_sense[0], pp1v24_dp_s2_conn_sense[1]);
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");
	
	LOG_Printf("  -- MODULE POWER CHECK --\r\n");
	CMD_Printf("  -- MODULE POWER CHECK --\r\n");

	if(dc_pp3v15_tc_dp_s2_conn_sense < vmVoltageMin[0] || dc_pp3v15_tc_dp_s2_conn_sense > vmVoltageMax[0])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP3V15_TOUCH_DISPLAY_S2_CONN / %f V)\r\n", ++indexCnt, dc_pp3v15_tc_dp_s2_conn_sense);
		CMD_Printf("%d. POWER TEST NG (DC_PP3V15_TOUCH_DISPLAY_S2_CONN / %f V)\r\n", indexCnt, dc_pp3v15_tc_dp_s2_conn_sense);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP3V15_TOUCH_DISPLAY_S2_CONN / %f V)\r\n", ++indexCnt, dc_pp3v15_tc_dp_s2_conn_sense);
		CMD_Printf("%d. POWER TEST OK (DC_PP3V15_TOUCH_DISPLAY_S2_CONN / %f V)\r\n", indexCnt, dc_pp3v15_tc_dp_s2_conn_sense);
	}

	if(dc_pp1vx_dp_s2_conn_sense < vmVoltageMin[1] || dc_pp1vx_dp_s2_conn_sense > vmVoltageMax[1])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1VX_DISPLAY_S2_CONN / %f V)\r\n", ++indexCnt, dc_pp1vx_dp_s2_conn_sense);
		CMD_Printf("%d. POWER TEST NG (DC_PP1VX_DISPLAY_S2_CONN / %f V)\r\n", indexCnt, dc_pp1vx_dp_s2_conn_sense);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1VX_DISPLAY_S2_CONN / %f V)\r\n", ++indexCnt, dc_pp1vx_dp_s2_conn_sense);
		CMD_Printf("%d. POWER TEST OK (DC_PP1VX_DISPLAY_S2_CONN / %f V)\r\n", indexCnt, dc_pp1vx_dp_s2_conn_sense);
	}

	if(dc_pp1v8_tc_dp_dvdd_conn_sense < vmVoltageMin[2] || dc_pp1v8_tc_dp_dvdd_conn_sense > vmVoltageMax[2])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V)\r\n", ++indexCnt, dc_pp1v8_tc_dp_dvdd_conn_sense);
		CMD_Printf("%d. POWER TEST NG (DC_PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V)\r\n", indexCnt, dc_pp1v8_tc_dp_dvdd_conn_sense);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V)\r\n", ++indexCnt, dc_pp1v8_tc_dp_dvdd_conn_sense);
		CMD_Printf("%d. POWER TEST OK (DC_PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V)\r\n", indexCnt, dc_pp1v8_tc_dp_dvdd_conn_sense);
	}

	if(dc_spi_alpdp_wpb_vpp_sense < vmVoltageMin[3] || dc_spi_alpdp_wpb_vpp_sense > vmVoltageMax[3])
	{
		LOG_Printf("%d. POWER TEST NG (DC_SPI_ALPDP_WPB_VPP / %f V)\r\n", ++indexCnt, dc_spi_alpdp_wpb_vpp_sense);
		CMD_Printf("%d. POWER TEST NG (DC_SPI_ALPDP_WPB_VPP / %f V)\r\n", indexCnt, dc_spi_alpdp_wpb_vpp_sense);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_SPI_ALPDP_WPB_VPP / %f V)\r\n", ++indexCnt, dc_spi_alpdp_wpb_vpp_sense);
		CMD_Printf("%d. POWER TEST OK (DC_SPI_ALPDP_WPB_VPP / %f V)\r\n", indexCnt, dc_spi_alpdp_wpb_vpp_sense);
	}

	if(dc_pp1v2_tc_dp_s2_conn_sense < vmVoltageMin[4] || dc_pp1v2_tc_dp_s2_conn_sense > vmVoltageMax[4])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1V2_TOUCH_DISPLAY_S2_CONN / %f V)\r\n", ++indexCnt, dc_pp1v2_tc_dp_s2_conn_sense);
		CMD_Printf("%d. POWER TEST NG (DC_PP1V2_TOUCH_DISPLAY_S2_CONN / %f V)\r\n", indexCnt, dc_pp1v2_tc_dp_s2_conn_sense);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1V2_TOUCH_DISPLAY_S2_CONN / %f V)\r\n", ++indexCnt, dc_pp1v2_tc_dp_s2_conn_sense);
		CMD_Printf("%d. POWER TEST OK (DC_PP1V2_TOUCH_DISPLAY_S2_CONN / %f V)\r\n", indexCnt, dc_pp1v2_tc_dp_s2_conn_sense);
	}

	if(dc_pp1v24_dp_s2_conn_sense < vmVoltageMin[6] || dc_pp1v24_dp_s2_conn_sense > vmVoltageMax[6])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1V24_DISPLAY_S2_CONN / %f V)\r\n", ++indexCnt, dc_pp1v24_dp_s2_conn_sense);
		CMD_Printf("%d. POWER TEST NG (DC_PP1V24_DISPLAY_S2_CONN / %f V)\r\n", indexCnt, dc_pp1v24_dp_s2_conn_sense);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1V24_DISPLAY_S2_CONN / %f V)\r\n", ++indexCnt, dc_pp1v24_dp_s2_conn_sense);
		CMD_Printf("%d. POWER TEST OK (DC_PP1V24_DISPLAY_S2_CONN / %f V)\r\n", indexCnt, dc_pp1v24_dp_s2_conn_sense);
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- TOUCH POWER FEEDBACK CHECK --\r\n");
	CMD_Printf("  -- TOUCH POWER FEEDBACK CHECK --\r\n");

	if(pp1v2_tc_vddcore_conn_sense[0] < vol_1v2Min || pp1v2_tc_vddcore_conn_sense[0] > vol_1v2Max || 
		pp1v2_tc_vddcore_conn_sense[1] < cur_1v2Min || pp1v2_tc_vddcore_conn_sense[1] > cur_1v2Max)
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1V2_TOUCH_VDDCORE_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v2_tc_vddcore_conn_sense[0], pp1v2_tc_vddcore_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (DC_PP1V2_TOUCH_VDDCORE_CONN / %f V  %f mA)\r\n", indexCnt,	pp1v2_tc_vddcore_conn_sense[0], pp1v2_tc_vddcore_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1V2_TOUCH_VDDCORE_CONN / %f V  %f mA)\r\n", ++indexCnt,  pp1v2_tc_vddcore_conn_sense[0], pp1v2_tc_vddcore_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (DC_PP1V2_TOUCH_VDDCORE_CONN / %f V  %f mA)\r\n", indexCnt,	pp1v2_tc_vddcore_conn_sense[0], pp1v2_tc_vddcore_conn_sense[1]);
	}

	if(vtp_s_1[0] < vol_2v5Min || vtp_s_1[0] > vol_2v5Max || vtp_s_1[1] < cur_2v5Min || vtp_s_1[1] > cur_2v5Max)
	{
		LOG_Printf("%d. POWER TEST NG (VTP_S_1 / %f V  %f mA)\r\n", ++indexCnt, vtp_s_1[0], vtp_s_1[1]);
		CMD_Printf("%d. POWER TEST NG (VTP_S_1 / %f V  %f mA)\r\n", indexCnt,	vtp_s_1[0], vtp_s_1[1]);
		BoardTest_NgCheck(POWER);
	}
	else
	{
		LOG_Printf("%d. POWER TEST OK (VTP_S_1 / %f V  %f mA)\r\n", ++indexCnt,  vtp_s_1[0], vtp_s_1[1]);
		CMD_Printf("%d. POWER TEST OK (VTP_S_1 / %f V  %f mA)\r\n", indexCnt,	vtp_s_1[0], vtp_s_1[1]);
	}

	if(htp_s_0[0] < vol_2v5Min || htp_s_0[0] > vol_2v5Max || htp_s_0[1] < cur_2v5Min || htp_s_0[1] > cur_2v5Max)
	{
		LOG_Printf("%d. POWER TEST NG (HTP_S_0 / %f V  %f mA)\r\n", ++indexCnt, htp_s_0[0], htp_s_0[1]);
		CMD_Printf("%d. POWER TEST NG (HTP_S_0 / %f V  %f mA)\r\n", indexCnt,	htp_s_0[0], htp_s_0[1]);
		BoardTest_NgCheck(POWER);
	}
	else
	{
		LOG_Printf("%d. POWER TEST OK (HTP_S_0 / %f V  %f mA)\r\n", ++indexCnt,  htp_s_0[0], htp_s_0[1]);
		CMD_Printf("%d. POWER TEST OK (HTP_S_0 / %f V  %f mA)\r\n", indexCnt,	htp_s_0[0], htp_s_0[1]);
	}

	testItem--;

	result = 1;

	return	result;

}		
	
static u8 Cmd_MBTestPMIC(void *pVoid)
{
	u8	result = 0, rtn;
	u8	pmic_adrs;
	u8	pmic_reg[4];
	
	double	temp;

	double	adj_vssel;
	double	adj_vddel;
	double	adj_avddh;
	double	adj_vgl;
	
	double	vssel[3];
	double	avddh[2];
	double	vgl[2];
	double	vddel;
	
	double	dc_vssel_conn;
	double	dc_vddel_conn;
	double	dc_avddh_conn;
	double	dc_vgl_conn;

	double	vbat;
	double	ibat;

	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;
	
	if(pData->argc != 0)		return 	result;

	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;

	//*****************************
	// 
	// PMIC DISABLE
	//
	//*****************************
	
	////////////////////////////////////////////////////
	// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
	// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);		
	
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);	

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN LOW
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0042, GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_042);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L LOW
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, AP_TO_CHIANTI_PANICB_L_IO_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	//*****************************
	// 
	// PMIC ENABLE
	//
	//*****************************

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L HIGH
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegSet(pData->position, 0x0044, AP_TO_CHIANTI_PANICB_L_IO_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN HIGH
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegSet(pData->position, 0x0042, GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_042);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
	// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegSet(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);	

	rtn = RXDPC_CoreRegSet(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);	
	
	//*****************************
	// 
	// PMIC CURRENT CHECK
	//
	//*****************************

	////////////////////////////////////////////////////
	// CURRENT SENSEING ADMUX ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P3 | P4 | P5);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// CURRENT MEAS MUX S2A CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// MB IOEXPNADER
	// GPIO_3V3_EN_AVDDH_I_MEAS
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P3 | P2 | P1 | P0);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P2);
	if(rtn == 0)				return	result;

	HAL_Delay(10);

	////////////////////////////////////////////////////
	// PMIC CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &avddh[1]); 	
	if(rtn == 0)				return	result;

	avddh[1] *= 1000;
	avddh[1] /= 41; // Gain = 41
	avddh[1] /= AVDDH_SHUNT;

	////////////////////////////////////////////////////
	// CURRENT MEAS MUX S3A CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// MB IOEXPNADER
	// GPIO_3V3_EN_VGL_I_MEAS
	//
	// P0 LOW => SHUNT ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P3 | P2 | P1 | P0);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// EN_PMIC_LOAD (GPIO1) ON	
	// EN_PMIC_LOAD_1 (GPIO2) OFF
	////////////////////////////////////////////////////
	rtn = LCM_GpioClear(pData->position, 0x04);
	if(rtn == 0)				return	result;
	
	rtn = LCM_GpioSet(pData->position, 0x02);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// V/IBAT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_MesureAutoVI(pData->position, PWR_SRC_VM_5, &vbat, &ibat);
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// PMIC CURRENT MEASURE	
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &vgl[1]); 
	if(rtn == 0)				return	result;

	vgl[1] *= 1000;
	vgl[1] /= 41; // Gain = 41
	vgl[1] /= VGL_SHUNT;

	////////////////////////////////////////////////////
	// CURRENT MEAS MUX S1A CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);
	
	////////////////////////////////////////////////////
	// MB IOEXPNADER
	// GPIO_3V3_EN_ELVSS_1 HIGH
	// GPIO_3V3_EN_ELVSS_2 LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P3 | P2 | P1 | P0);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);
	
	////////////////////////////////////////////////////
	// EN_PMIC_LOAD (GPIO1) ON
	// EN_PMIC_LOAD_1 (GPIO2) OFF
	////////////////////////////////////////////////////
	rtn = LCM_GpioClear(pData->position, 0x04);
	if(rtn == 0)				return	result;
	
	rtn = LCM_GpioSet(pData->position, 0x02);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// PMIC CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &vssel[1]); 
	if(rtn == 0)				return	result;

	vssel[1] *= 1000;
	vssel[1] /= 41; // Gain = 41
	vssel[1] /= VSSEL_SHUNT;

	////////////////////////////////////////////////////
	// MB IOEXPNADER
	// GPIO_3V3_EN_ELVSS_1 HIGH
	// GPIO_3V3_EN_ELVSS_2 HIGH
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P3 | P2 | P1 | P0);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P1 | P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);
	
	////////////////////////////////////////////////////
	// EN_PMIC_LOAD (GPIO1) OFF
	// EN_PMIC_LOAD_1 (GPIO2) ON
	////////////////////////////////////////////////////
	rtn = LCM_GpioClear(pData->position, 0x02);
	if(rtn == 0)				return	result;
	
	rtn = LCM_GpioSet(pData->position, 0x04);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	////////////////////////////////////////////////////
	// PMIC CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &vssel[2]); 
	if(rtn == 0)				return	result;

	vssel[2] *= 1000;
	vssel[2] /= 41; // Gain = 41
	vssel[2] /= VSSEL_L_SHUNT;

	////////////////////////////////////////////////////
	// CURRENT SENSEING ADMUX DISABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P3);
	if(rtn == 0)				return	result;
	
	////////////////////////////////////////////////////
	// EN_PMIC_LOAD (GPIO1) OFF
	// EN_PMIC_LOAD_1 (GPIO2) OFF
	////////////////////////////////////////////////////
	rtn = LCM_GpioClear(pData->position, 0x02);
	if(rtn == 0)				return	result;
	
	rtn = LCM_GpioClear(pData->position, 0x04);
	if(rtn == 0)				return	result;
	
	HAL_Delay(10);

	//*****************************
	// 
	// PMIC VOLTAGE CHECK
	//
	//*****************************
	for(u8 cnt = 0; cnt < 2; cnt++)
	{
		if(cnt == 1)
		{
			////////////////////////////////////////////////////
			// PMIC VOLTAGE ADJUSTING
			////////////////////////////////////////////////////
			if(CMD_Compare(pModel, "D854"))
			{
				pmic_adrs = 0xA0;
				
				pmic_reg[VSSEL] = 0x50;		// -5.0v
				pmic_reg[VDDEL] = 0x05;		// 2.5V
				pmic_reg[AVDDH] = 0x0C;		// 7.7v
				pmic_reg[VGL] = 0x87;		// -8.5V

				rtn = LCM_I2CMemWrite(pData->position, BSP_I2C2, pmic_adrs, 0x01, 0, &pmic_reg[VSSEL], 1);
				if(rtn == 0)				return	result;
				
				rtn = LCM_I2CMemWrite(pData->position, BSP_I2C2, pmic_adrs, 0x02, 0, &pmic_reg[VDDEL], 1);
				if(rtn == 0)				return	result;
				
				rtn = LCM_I2CMemWrite(pData->position, BSP_I2C2, pmic_adrs, 0x03, 0, &pmic_reg[AVDDH], 1);
				if(rtn == 0)				return	result;
				
				rtn = LCM_I2CMemWrite(pData->position, BSP_I2C2, pmic_adrs, 0x05, 0, &pmic_reg[VGL], 1);
				if(rtn == 0)				return	result;

				HAL_Delay(50);
			}
		}

		////////////////////////////////////////////////////
		// VOLTAGE SENSEING ADMUX ENABLE
		// 전압 측정을 위해 VOLTAGE AMUX EN 할 때, 
		// P2 혹은 P3 같이 SET 하지 않으면 측정 전압 Drop 증상 발생
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P3 | P4 | P5);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3 | P5);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10);

		////////////////////////////////////////////////////
		// AMUX_S1 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S1);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10);

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;
		
		if(cnt == 0)	vssel[0] = temp * -2;
		else			adj_vssel = temp * -2;
	
		////////////////////////////////////////////////////
		// AMUX_S2 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S2);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10);

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;

		if(cnt == 0)	avddh[0] = temp * 3;
		else			adj_avddh = temp * 3;

	 	////////////////////////////////////////////////////
	 	// AMUX_S3 Latch
	 	////////////////////////////////////////////////////
	 	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
	 	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S3);
		if(rtn == 0)				return	result;
		
	 	HAL_Delay(10);
	 
	 	////////////////////////////////////////////////////
	 	// PMIC VOLTAGE MEASURE
	 	////////////////////////////////////////////////////
	 	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;
	 
	 	if(cnt == 0)	vgl[0] = (temp + 0.3) * -4;
		else			adj_vgl = temp * -4;

		////////////////////////////////////////////////////
		// AMUX_S4 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S4);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10); 
	        
		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;

		if(cnt == 0)	vddel = temp;
		else			adj_vddel = temp;

		////////////////////////////////////////////////////
		// VOLTAGE SENSEING ADMUX ENABLE
		// 전압 측정을 위해 VOLTAGE AMUX EN 할 때, 
		// P2 혹은 P3 같이 SET 하지 않으면 측정 전압 Drop 증상 발생
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P3 | P4 | P5);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P5);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10);

		////////////////////////////////////////////////////
		// AMUX_S5 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S5);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10); 

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;

		if(cnt == 0)	dc_vssel_conn = temp * -2;

		////////////////////////////////////////////////////
		// AMUX_S6 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S6);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10); 

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;

		if(cnt == 0)	dc_avddh_conn = temp * 3;

		////////////////////////////////////////////////////
		// AMUX_S7 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S7);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10); 

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;

		if(cnt == 0)	dc_vgl_conn = (temp - 0.7) * -4;

		////////////////////////////////////////////////////
		// AMUX_S8 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S8);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10); 

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;

		if(cnt == 0)	dc_vddel_conn = temp;
	}

	//*****************************
	// 
	// PMIC DISABLE
	//
	//*****************************

	////////////////////////////////////////////////////
	// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
	// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);		
	
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);	

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN LOW
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0042, GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_042);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L LOW
	////////////////////////////////////////////////////
	rtn = RXDPC_CoreRegClear(pData->position, 0x0044, AP_TO_CHIANTI_PANICB_L_IO_044);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	LOG_Printf("\r\n========================== PMIC TEST ==========================\r\n");
	CMD_Printf("\r\n========================== PMIC TEST ==========================\r\n");

	LOG_Printf("  -- V/IBAT CHECK --\r\n");
	CMD_Printf("  -- V/IBAT CHECK --\r\n");

	if(vbat < vmVoltageMin[5] || vbat > vmVoltageMax[5] || ibat < vmCurrentMin[5][0] || ibat > vmCurrentMax[5][0])
	{
		LOG_Printf("%d. PMIC TEST NG (V/IBAT : %f V  %f mA)\r\n", ++indexCnt, vbat, ibat);
		CMD_Printf("%d. PMIC TEST NG (V/IBAT : %f V  %f mA)\r\n", indexCnt, vbat, ibat);
		BoardTest_NgCheck(PMIC);
	}
	else
	{	
		LOG_Printf("%d. PMIC TEST OK (V/IBAT : %f V  %f mA)\r\n", ++indexCnt, vbat, ibat);
		CMD_Printf("%d. PMIC TEST OK (V/IBAT : %f V  %f mA)\r\n", indexCnt, vbat, ibat);
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- CURRENT READ --\r\n");
	CMD_Printf("  -- CURRENT READ --\r\n");

	if(vssel[1] < pmicCurrentMin[VSSEL] || vssel[1] > pmicCurrentMax[VSSEL])
	{
		LOG_Printf("%d. PMIC TEST NG (VSSEL CURRENT / %f mA)\r\n", ++indexCnt, vssel[1]);
		CMD_Printf("%d. PMIC TEST NG (VSSEL CURRENT / %f mA)\r\n", indexCnt, vssel[1]);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VSSEL CURRENT / %f mA)\r\n", ++indexCnt, vssel[1]);
		CMD_Printf("%d. PMIC TEST OK (VSSEL CURRENT / %f mA)\r\n", indexCnt, vssel[1]);
	}

	if(vssel[2] < pmicCurrentMin[VSSEL_L] || vssel[2] > pmicCurrentMax[VSSEL_L])
	{
		LOG_Printf("%d. PMIC TEST NG (VSSEL_L CURRENT / %f mA)\r\n", ++indexCnt, vssel[2]);
		CMD_Printf("%d. PMIC TEST NG (VSSEL_L CURRENT / %f mA)\r\n", indexCnt, vssel[2]);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VSSEL_L CURRENT / %f mA)\r\n", ++indexCnt, vssel[2]);
		CMD_Printf("%d. PMIC TEST OK (VSSEL_L CURRENT / %f mA)\r\n", indexCnt, vssel[2]);
	}

	if(avddh[1] < pmicCurrentMin[AVDDH] || avddh[1] > pmicCurrentMax[AVDDH])
	{
		LOG_Printf("%d. PMIC TEST NG (AVDDH CURRENT / %f mA)\r\n", ++indexCnt, avddh[1]);
		CMD_Printf("%d. PMIC TEST NG (AVDDH CURRENT / %f mA)\r\n", indexCnt, avddh[1]);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (AVDDH CURRENT / %f mA)\r\n", ++indexCnt, avddh[1]);
		CMD_Printf("%d. PMIC TEST OK (AVDDH CURRENT / %f mA)\r\n", indexCnt, avddh[1]);
	}

	if(vgl[1] < pmicCurrentMin[VGL] || vgl[1] > pmicCurrentMax[VGL])
	{
		LOG_Printf("%d. PMIC TEST NG (VGL CURRENT / %f mA)\r\n", ++indexCnt, vgl[1]);
		CMD_Printf("%d. PMIC TEST NG (VGL CURRENT / %f mA)\r\n", indexCnt, vgl[1]);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VGL CURRENT / %f mA)\r\n", ++indexCnt, vgl[1]);
		CMD_Printf("%d. PMIC TEST OK (VGL CURRENT / %f mA)\r\n", indexCnt, vgl[1]);
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- POGO VOLTAGE READ --\r\n");
	CMD_Printf("  -- POGO VOLTAGE READ --\r\n");

	if(vssel[0] < pmicVoltageMin[VSSEL] || vssel[0] > pmicVoltageMax[VSSEL])
	{
		LOG_Printf("%d. PMIC TEST NG (VSSEL VOLTAGE / %f V)\r\n", ++indexCnt, vssel[0]);
		CMD_Printf("%d. PMIC TEST NG (VSSEL VOLTAGE / %f V)\r\n", indexCnt, vssel[0]);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VSSEL VOLTAGE / %f V)\r\n", ++indexCnt, vssel[0]);
		CMD_Printf("%d. PMIC TEST OK (VSSEL VOLTAGE / %f V)\r\n", indexCnt, vssel[0]);
	}

	if(avddh[0] < pmicVoltageMin[AVDDH] || avddh[0] > pmicVoltageMax[AVDDH])
	{
		LOG_Printf("%d. PMIC TEST NG (AVDDH VOLTAGE / %f V)\r\n", ++indexCnt, avddh[0]);
		CMD_Printf("%d. PMIC TEST NG (AVDDH VOLTAGE / %f V)\r\n", indexCnt, avddh[0]);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (AVDDH VOLTAGE / %f V)\r\n", ++indexCnt, avddh[0]);
		CMD_Printf("%d. PMIC TEST OK (AVDDH VOLTAGE / %f V)\r\n", indexCnt, avddh[0]);
	}

	if(vgl[0] < pmicVoltageMin[VGL] || vgl[0] > pmicVoltageMax[VGL])
	{
		LOG_Printf("%d. PMIC TEST NG (VGL VOLTAGE / %f V)\r\n", ++indexCnt, vgl[0]);
		CMD_Printf("%d. PMIC TEST NG (VGL VOLTAGE / %f V)\r\n", indexCnt, vgl[0]);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VGL VOLTAGE / %f V)\r\n", ++indexCnt, vgl[0]);
		CMD_Printf("%d. PMIC TEST OK (VGL VOLTAGE / %f V)\r\n", indexCnt, vgl[0]);
	}

	if(vddel < pmicVoltageMin[VDDEL] || vddel > pmicVoltageMax[VDDEL])
	{
		LOG_Printf("%d. PMIC TEST NG (VDDEL VOLTAGE / %f V)\r\n", ++indexCnt, vddel);
		CMD_Printf("%d. PMIC TEST NG (VDDEL VOLTAGE / %f V)\r\n", indexCnt, vddel);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VDDEL VOLTAGE / %f V)\r\n", ++indexCnt, vddel);
		CMD_Printf("%d. PMIC TEST OK (VDDEL VOLTAGE / %f V)\r\n", indexCnt, vddel);
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- MODULE VOLTAGE READ --\r\n");
	CMD_Printf("  -- MODULE VOLTAGE READ --\r\n");

	if(dc_vssel_conn < pmicVoltageMin[VSSEL] || dc_vssel_conn > pmicVoltageMax[VSSEL])
		{
			LOG_Printf("%d. PMIC TEST NG (DC VSSEL VOLTAGE / %f V)\r\n", ++indexCnt, dc_vssel_conn);
			CMD_Printf("%d. PMIC TEST NG (DC VSSEL VOLTAGE / %f V)\r\n", indexCnt, dc_vssel_conn);
			BoardTest_NgCheck(PMIC);
		}
		else
		{
			LOG_Printf("%d. PMIC TEST OK (DC VSSEL VOLTAGE / %f V)\r\n", ++indexCnt, dc_vssel_conn);
			CMD_Printf("%d. PMIC TEST OK (DC VSSEL VOLTAGE / %f V)\r\n", indexCnt, dc_vssel_conn);
		}
	
		if(dc_avddh_conn < pmicVoltageMin[AVDDH] || dc_avddh_conn > pmicVoltageMax[AVDDH])
		{
			LOG_Printf("%d. PMIC TEST NG (DC AVDDH VOLTAGE / %f V)\r\n", ++indexCnt, dc_avddh_conn);
			CMD_Printf("%d. PMIC TEST NG (DC AVDDH VOLTAGE / %f V)\r\n", indexCnt, dc_avddh_conn);
			BoardTest_NgCheck(PMIC);
		}
		else
		{
			LOG_Printf("%d. PMIC TEST OK (DC AVDDH VOLTAGE / %f V)\r\n", ++indexCnt, dc_avddh_conn);
			CMD_Printf("%d. PMIC TEST OK (DC AVDDH VOLTAGE / %f V)\r\n", indexCnt, dc_avddh_conn);
		}
	
		if(dc_vgl_conn < pmicVoltageMin[VGL] || dc_vgl_conn > pmicVoltageMax[VGL])
		{
			LOG_Printf("%d. PMIC TEST NG (DC VGL VOLTAGE / %f V)\r\n", ++indexCnt, dc_vgl_conn);
			CMD_Printf("%d. PMIC TEST NG (DC VGL VOLTAGE / %f V)\r\n", indexCnt, dc_vgl_conn);
			BoardTest_NgCheck(PMIC);
		}
		else
		{
			LOG_Printf("%d. PMIC TEST OK (DC VGL VOLTAGE / %f V)\r\n", ++indexCnt, dc_vgl_conn);
			CMD_Printf("%d. PMIC TEST OK (DC VGL VOLTAGE / %f V)\r\n", indexCnt, dc_vgl_conn);
		}
	
		if(dc_vddel_conn < pmicVoltageMin[VDDEL] || dc_vddel_conn > pmicVoltageMax[VDDEL])
		{
			LOG_Printf("%d. PMIC TEST NG (DC VDDEL VOLTAGE / %f V)\r\n", ++indexCnt, dc_vddel_conn);
			CMD_Printf("%d. PMIC TEST NG (DC VDDEL VOLTAGE / %f V)\r\n", indexCnt, dc_vddel_conn);
			BoardTest_NgCheck(PMIC);
		}
		else
		{
			LOG_Printf("%d. PMIC TEST OK (DC VDDEL VOLTAGE / %f V)\r\n", ++indexCnt, dc_vddel_conn);
			CMD_Printf("%d. PMIC TEST OK (DC VDDEL VOLTAGE / %f V)\r\n", indexCnt, dc_vddel_conn);
		}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("  -- PMIC VOLTAGE ADJUSTMENT --\r\n");
	CMD_Printf("  -- PMIC VOLTAGE ADJUSTMENT --\r\n");

	if(adj_vssel < vol_adjvsselMin || adj_vssel > vol_adjvsselMax)
	{
		LOG_Printf("%d. PMIC TEST NG (VSSEL / %f V)\r\n", ++indexCnt, adj_vssel);
		CMD_Printf("%d. PMIC TEST NG (VSSEL / %f V)\r\n", indexCnt, adj_vssel);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VSSEL / %f V)\r\n", ++indexCnt, adj_vssel);
		CMD_Printf("%d. PMIC TEST OK (VSSEL / %f V)\r\n", indexCnt, adj_vssel);
	}

	if(adj_avddh < vol_adjavddhMin || adj_avddh > vol_adjavddhMax)
	{
		LOG_Printf("%d. PMIC TEST NG (AVDDH / %f V)\r\n", ++indexCnt, adj_avddh);
		CMD_Printf("%d. PMIC TEST NG (AVDDH / %f V)\r\n", indexCnt, adj_avddh);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (AVDDH / %f V)\r\n", ++indexCnt, adj_avddh);
		CMD_Printf("%d. PMIC TEST OK (AVDDH / %f V)\r\n", indexCnt, adj_avddh);
	}

	if(adj_vgl < vol_adjvglMin || adj_vgl > vol_adjvglMax)
	{
		LOG_Printf("%d. PMIC TEST NG (VGL / %f V)\r\n", ++indexCnt, adj_vgl);
		CMD_Printf("%d. PMIC TEST NG (VGL / %f V)\r\n", indexCnt, adj_vgl);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VGL / %f V)\r\n", ++indexCnt, adj_vgl);
		CMD_Printf("%d. PMIC TEST OK (VGL / %f V)\r\n", indexCnt, adj_vgl);
	}

	if(adj_vddel < vol_adjvddelMin || adj_vddel > vol_adjvddelMax)
	{
		LOG_Printf("%d. PMIC TEST NG (VDDEL / %f V)\r\n", ++indexCnt, adj_vddel);
		CMD_Printf("%d. PMIC TEST NG (VDDEL / %f V)\r\n", indexCnt, adj_vddel);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (VDDEL / %f V)\r\n", ++indexCnt, adj_vddel);
		CMD_Printf("%d. PMIC TEST OK (VDDEL / %f V)\r\n", indexCnt, adj_vddel);
	}

	testItem--;

	result = 1;

	return	result;
}

static u8 Cmd_MBTestDP(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;

	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;

	rtn = RXDPC_DCTestInit(pData->position);
	if(rtn == 0)				return	result;

	rtn = RXDPC_DCTestStart(pData->position);
	if(rtn == 0)				return	result;

	rtn = RXDPC_CRCTestStart(pData->position);
	if(rtn == 0)				return	result;

	rtn = RXDPC_SymbolTestStart(pData->position);
	if(rtn == 0)				return	result;

	testItem--;

	result = 1;

	return	result;
}

static u8 Cmd_MBTestOSC(void *pVoid)
{
	u8	result = 0, rtn;
	u32	ReadData;
	u32	period, time;
	u32	mask = 0x0FFF0001;
	double	osc_clock;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;
	
	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;
		
	pData = (cmdFunctionData_t*)pVoid;

	rtn = DPC_DirectRegReadSingle(pData->position, CORE_REG_ADDR_EX_CLK_CNT_START, &ReadData);
	if(rtn == 0)			return result;

	ReadData &= ~mask;
	ReadData |= ((0x0000000A << 16) | 0x00000001);

	rtn = DPC_DirectRegWriteSingle(pData->position, CORE_REG_ADDR_EX_CLK_CNT_START, ReadData);
	if(rtn == 0)			return	result;

	rtn = DPC_DirectRegReadSingle(pData->position, CORE_REG_ADDR_EX_CLK_CNT_VALUE2, &ReadData);
	if(rtn == 0)			return result;

	period = ReadData;

	// calculate pulse count
	rtn = DPC_DirectRegReadSingle(pData->position, CORE_REG_ADDR_EX_CLK_CNT_START, &ReadData);
	if(rtn == 0)			return result;

	time = (ReadData >> 16) & 0x0FFF;	// time(0.1sec)

	osc_clock = (double)(time * 100 * 1000) / period;	// period(usec)
	osc_clock /= 1000;

	LOG_Printf("\r\n=========================== OSC TEST ===========================\r\n");
	CMD_Printf("\r\n=========================== OSC TEST ===========================\r\n");
	
	if(osc_clock < oscMin || osc_clock > oscMax)
	{
		LOG_Printf("%d. OSC TEST NG (OSC / %.3f kHz)\r\n", ++indexCnt, osc_clock);
		CMD_Printf("%d. OSC TEST NG (OSC / %.3f kHz)\r\n", indexCnt, osc_clock);
		BoardTest_NgCheck(OSC);
	}
	else
	{
		LOG_Printf("%d. OSC TEST OK (OSC / %.3f kHz)\r\n", ++indexCnt, osc_clock);
		CMD_Printf("%d. OSC TEST OK (OSC / %.3f kHz)\r\n", indexCnt, osc_clock);
	}

	testItem--;

	result = 1;

	return	result;
}

static u8 Cmd_MBTestPortDirInit(void *pVoid)		
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;

	////////////////////////////////////////////////////
	// IOEXPANDER OUTPUT PORT ALL LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTB, PORT_ALL); 
	if(rtn == 0)				return	result;
	
	////////////////////////////////////////////////////
	// IOEXPANDER PORT DIRECTION INIT
	////////////////////////////////////////////////////
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, PORT_OUTPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTB, PORT_OUTPUT);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortDir(pVoid, BSP_I2C1, MB_EXPANDER_1, PORTA, PORT_INPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C1, MB_EXPANDER_1, PORTB, PORT_INPUT);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortDir(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, PORT_OUTPUT);
	if(rtn == 0)				return	result;
	
	//rtn = IoExp_PortDir(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTB, P12);
	rtn = IoExp_PortDir(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTB, PORT_OUTPUT);
	if(rtn == 0)				return	result;

	//temp
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, 0x40, PORTA, PORT_INPUT);
	if(rtn == 0)			return result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, 0x40, PORTB, PORT_INPUT);
	if(rtn == 0)			return result;


	result = 1;

	return	result;
}

static u8 Cmd_MBTestFinalResult(void *pVoid)
{
	u8	result = 0, rtn, errIdx = 0, chk = 0, sum = 0;
	u32 temp;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;

	LOG_Printf("\r\n========================== TEST END ==========================\r\n");
	CMD_Printf("\r\n========================== TEST END ==========================\r\n");

	for(u8 i = 0; i < 8; i++)
	{
		sum += ngCnt[i];
	}

	sprintf((char*)pModel, "NONE");

	////////////////////////////////////////////////////
	// TEST LED INIT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;
	
	if(sum == 0 && testItem == 0)
	{
		LOG_Printf("TOTAL RESULT : OK\r\n");
		CMD_Printf("TOTAL RESULT : OK\r\n");
		
		rtn = IoExp_LED_Ctrl(pVoid, TOTAL_OK);
		if(rtn == 0)				return	result;
	}
	else
	{
		LOG_Printf("TOTAL RESULT : NG\r\n");
		CMD_Printf("TOTAL RESULT : NG\r\n");
		LOG_Printf("\r\n");
		CMD_Printf("\r\n");
		LOG_Printf("NG INDEX : ");
		CMD_Printf("NG INDEX : ");

		for(u8 i = 0; i < 3; i++)
		{
			for(u8 j = 0; j < BIT32_FULL_CNT; j++)
			{
				temp = ngNum[i].u32Data & (1 << j);

				if(i == 0)	
				{
					if(temp >= 1)	
					{
						errIdx = j + 1 ;
						chk++;
					}
				}
				else if(i == 1)
				{
					if(temp >= 1)
					{
						errIdx = j + 1 + BIT32_FULL_CNT;
						chk++;
					}
				}
				else if(i == 2)
				{
					if(temp >= 1)
					{
						errIdx = j + 1 + (BIT32_FULL_CNT * 2);
						chk++;
					}
				}
				else		return	result;

				if(chk)		CMD_Printf("%d ", errIdx);
				
				chk = 0;
			}
		}

		LOG_Printf("\r\n");
		CMD_Printf("\r\n");
		
		rtn = IoExp_LED_Ctrl(pVoid, TOTAL_NG);
		if(rtn == 0)				return	result;
		
		if(ngCnt[I2C])
		{
			rtn = IoExp_LED_Ctrl(pVoid, I2C_NG);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[OPENSHORT])
		{
			rtn = IoExp_LED_Ctrl(pVoid, IO_NG);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[POWER])
		{
			rtn = IoExp_LED_Ctrl(pVoid, POWER_NG);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[DP])
		{
			rtn = IoExp_LED_Ctrl(pVoid, DP_NG);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[OSC])
		{
			rtn = IoExp_LED_Ctrl(pVoid, OSC_NG);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[VID])
		{
			rtn = IoExp_LED_Ctrl(pVoid, VENTURA_NG);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[PMIC])
		{
			rtn = IoExp_LED_Ctrl(pVoid, PMIC_NG);
			if(rtn == 0)				return	result;
		}
	}

	memset(ngNum, 0, sizeof(ngNum));
	memset(ngCnt, 0, sizeof(ngCnt));
	memset(&indexCnt, 0, sizeof(indexCnt));
	memset(&remove_io_ng, 0, sizeof(remove_io_ng));
	memset(&testItem, 0xff, sizeof(testItem));
	memset(&errDetector, 0, sizeof(errDetector));
	
	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("##################################################################################\r\n");
	LOG_Printf("##################################################################################\r\n");

	result = 1;

	return	result;
}	

static u8 Cmd_MBTestModel(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;
	
	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "D854"))	sprintf((char*)pModel, "D854");
	else
	{
		LOG_Printf("%d. TEST MODEL CHECK NG (NEED TO SET TEST MODEL!)\r\n", ++indexCnt);
		CMD_Printf("%d. TEST MODEL CHECK NG (NEED TO SET TEST MODEL!)\r\n", indexCnt);
		BoardTest_NgCheck(I2C);
	
		return	result;
	}

	result = 1;

	return 	result;		
}

static u8 Cmd_MBTestVersion(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;
	
	CMD_Printf("\r\n");
	CMD_Printf("MBTEST MODEL : D854_E1\r\n");
	CMD_Printf("MBTEST DEVICE : E7602_E2\r\n");
	CMD_Printf("MBTEST VERSION : v1.0.5\r\n");	
	CMD_Printf("MBTEST DATE : 2022.05.31\r\n");	

	result = 1;

	return	result;
}	

/******************************************************************************
 Revision History

 v1.0.2 : New CCJ ET Target
	      OSC test : time check -> period check 
		  Change Global Variable
          remove_io_ng Variable init ==> final result 
          Cmd_MBTestVoltageLimit modify
          Cmd_MBTestCurrentLimit modify
          Change pmic reg local variable, apply delay 100ms -> 50ms
          Final result LED function => ioexpander.c modify
          Cmd_MBTestI2c : Function Return value modify
          Add command mbtest.test.item
          Addd ngIndex Print out
          openshort test : PANEL_ID pin test with Ioexp(Ioexp port init change)

 v1.0.3 : NG Index Count 64bit -> 96bit  
 v1.0.4 : Power MeasureVIAuto -> Power MeasureVI 
          I2C ERROR in I2c Test => All Test skip
            =>first, I2C NG Flag set, when I2c test OK, remove I2C NG Flag
          Arguments count Check
 v1.0.5 : Modified Cmd_MBTestSerialChk
            =>remove serial search function
            =>Add to format EEPROM with 0xFF 
******************************************************************************/		
const	commandList_t	cmdList_MBtest[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"version",				Cmd_MBTestVersion,			NULL,		NULL,		"",						NULL},
												{	"model",				Cmd_MBTestModel,			NULL,		NULL,		"",						NULL},
												{	"test.item",			Cmd_MBTestTestItem,			NULL,		NULL,		"",						NULL},
												{	"remove.ng",			Cmd_MBTestRemoveNgIndex,	NULL,		NULL,		"",						NULL},
												{	"voltage.range",		Cmd_MBTestVoltageLimit,		NULL,		NULL,		"",						NULL},
												{	"current.range",		Cmd_MBTestCurrentLimit,		NULL,		NULL,		"",						NULL},
												{	"i2c",					Cmd_MBTestI2c,				NULL,		NULL,		"",						NULL},
												{	"io",					Cmd_MBTestOpenShort,		NULL,		NULL,		"",						NULL},
												{	"power",				Cmd_MBTestPower,			NULL,		NULL,		"",						NULL},
												{	"pmic",					Cmd_MBTestPMIC,				NULL,		NULL,		"",						NULL},
												{	"osc",					Cmd_MBTestOSC,				NULL,		NULL,		"",						NULL},
												{	"serial",				Cmd_MBTestSerialChk,		0x01,		NULL,		"",						NULL},
												{	"dp",					Cmd_MBTestDP,				NULL,		NULL,		"",						NULL},
												{	"port.init",			Cmd_MBTestPortDirInit,		NULL,		NULL,		"",						NULL},
												{	"final.result",			Cmd_MBTestFinalResult,		NULL,		NULL,		"",						NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};
