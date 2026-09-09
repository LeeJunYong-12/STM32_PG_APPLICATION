#include	"Cmd_HingeTest.h"


static u8 Cmd_HingeTestItem(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	testItem = CMD_StrToUL(pData->pArgv[0]);

	result  = 1;

	return	result;
}

static u8 Cmd_HingeTestVoltageLimit(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pData->pArgv[0], "pp5v1"))
	{
		if(CMD_Compare(pData->pArgv[1], "vddx"))
		{
			hVoltageMin[0] = CMD_AToF(pData->pArgv[2]);
			hVoltageMax[0] = CMD_AToF(pData->pArgv[3]);
		}
	}
	else if(CMD_Compare(pData->pArgv[0], "pp1v2"))
	{
		if(CMD_Compare(pData->pArgv[1], "touch"))
		{
			hVoltageMin[1] = CMD_AToF(pData->pArgv[2]);
			hVoltageMax[1] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vddcore"))
		{
			hVoltageMin[2] = CMD_AToF(pData->pArgv[2]);
			hVoltageMax[2] = CMD_AToF(pData->pArgv[3]);
		}
	}	
	else if(CMD_Compare(pData->pArgv[0], "pp1v8"))
	{
		if(CMD_Compare(pData->pArgv[1], "dvdd"))
		{
			hVoltageMin[3] = CMD_AToF(pData->pArgv[2]);
			hVoltageMax[3] = CMD_AToF(pData->pArgv[3]);
		}
	}	
	else if(CMD_Compare(pData->pArgv[0], "pp3v15"))
	{
		if(CMD_Compare(pData->pArgv[1], "touch"))
		{
			hVoltageMin[4] = CMD_AToF(pData->pArgv[2]);
			hVoltageMax[4] = CMD_AToF(pData->pArgv[3]);
		}
	}	
	else if(CMD_Compare(pData->pArgv[0], "pp1v24"))
	{
		if(CMD_Compare(pData->pArgv[1], "display"))
		{
			hVoltageMin[5] = CMD_AToF(pData->pArgv[2]);
			hVoltageMax[5] = CMD_AToF(pData->pArgv[3]);
		}
	}	
	else if(CMD_Compare(pData->pArgv[0], "vddel"))
	{
		hVoltageMin[6] = CMD_AToF(pData->pArgv[1]);
		hVoltageMax[6] = CMD_AToF(pData->pArgv[2]);
	}	
	else if(CMD_Compare(pData->pArgv[0], "pp1vx"))
	{
		if(CMD_Compare(pData->pArgv[1], "display"))
		{
			hVoltageMin[7] = CMD_AToF(pData->pArgv[2]);
			hVoltageMax[7] = CMD_AToF(pData->pArgv[3]);
		}
	}
	else if(CMD_Compare(pData->pArgv[0], "vssel"))
	{
		hVoltageMin[8] = CMD_AToF(pData->pArgv[1]);
		hVoltageMax[8] = CMD_AToF(pData->pArgv[2]);
	}

	result  = 1;

	return	result;
}

static u8 Cmd_HingeTestPortDirInit(void *pVoid)		
{
	u8	result = 0, rtn;

	////////////////////////////////////////////////////
	// IOEXPANDER OUTPUT PORT ALL LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
		
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
		
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_IN_EXPANDER_1, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_IN_EXPANDER_1, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_IN_EXPANDER_2, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
		
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_IN_EXPANDER_2, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_INOUT_EXPANDER, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_INOUT_EXPANDER, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;

	if(CMD_Compare(pModel, "D854"))
	{
		////////////////////////////////////////////////////
		// IOEXPANDER PORT DIRECTION INIT
		////////////////////////////////////////////////////
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, PORT_OUTPUT);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTB, PORT_OUTPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTA, PORT_INPUT);
		if(rtn == 0)				return	result;
			
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTB, PORT_INPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTA, PORT_OUTPUT);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTB, PORT_OUTPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTA, PORT_OUTPUT);
		if(rtn == 0)				return	result;
			
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTB, PORT_OUTPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_IN_EXPANDER_1, PORTA, PORT_INPUT);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_IN_EXPANDER_1, PORTB, PORT_INPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_IN_EXPANDER_2, PORTA, PORT_INPUT);
		if(rtn == 0)				return	result;
			
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_IN_EXPANDER_2, PORTB, PORT_INPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_INOUT_EXPANDER, PORTA, PORT_OUTPUT);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_INOUT_EXPANDER, PORTB, PORT_INPUT);
		if(rtn == 0)				return	result;
	}
	else if(CMD_Compare(pModel, "D852"))
	{
		////////////////////////////////////////////////////
		// IOEXPANDER PORT DIRECTION INIT
		////////////////////////////////////////////////////
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, PORT_OUTPUT);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTB, PORT_OUTPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTA, PORT_OUTPUT);
		if(rtn == 0)				return	result;
			
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTB, 0xEA);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTA, PORT_OUTPUT);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTB, PORT_OUTPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTA, PORT_OUTPUT);
		if(rtn == 0)				return	result;
			
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTB, PORT_OUTPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_IN_EXPANDER_1, PORTA, PORT_INPUT);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_IN_EXPANDER_1, PORTB, PORT_INPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_IN_EXPANDER_2, PORTA, PORT_INPUT);
		if(rtn == 0)				return	result;
			
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_IN_EXPANDER_2, PORTB, PORT_INPUT);
		if(rtn == 0)				return	result;

		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_INOUT_EXPANDER, PORTA, PORT_INPUT);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, HT_INOUT_EXPANDER, PORTB, PORT_INPUT);
		if(rtn == 0)				return	result;
	}
	else	return	result;

	result = 1;

	return	result;
}

static u8 Cmd_HingeTestOpenShort(void *pVoid)
{
	u8	result = 0, openshort[34] = {0, }, rtn, arr = 0;
	u8	upper, lower;
	const char *string[51];	
	bitCtrl8_t	readData;

	rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTB, 0x80);
	if(rtn == 0)				return	result;

	HAL_Delay(500);
	
	if(CMD_Compare(pModel, "D854"))
	{
		string[0] = "I2C_SDA_TOUCH_EEPROM_1V8";
		string[1] = "I2C_SCL_TOUCH_EEPROM_1V8";
		string[2] = "IO_AOP_TO_TOUCH_BSYNC0_CONN";
		string[3] = "IO_AOP_TO_TOUCH_BSYNC1_CONN";
		string[4] = "IO_TOUCH_TO_MANY_SCAN_ACTIVE";
		string[5] = "IO_KRAKEN_TO_TOUCH_AID_CONN";
		string[6] = "GPIO_MTP_TO_TOUCH_RESET_L_CONN";
		string[7] = "SPI0_MTP_FROM_TOUCH_MISO_CONN";
		string[8] = "SPI0_MTP_TO_TOUCH_MOSI_CONN";
		string[9] = "SPI0_MTP_TO_TOUCH_SCLK_CONN";
		string[10] = "SPI0_MTP_TO_TOUCH_SSIN_CONN";
		string[11] = "EMPTY";
		string[12] = "90_LPDP_AP_TO_DISP_TX_D2_CONN_N";
		string[13] = "90_LPDP_AP_TO_DISP_TX_D2_CONN_P";
		string[14] = "90_LPDP_AP_TO_DISP_TX_D1_CONN_N";
		string[15] = "90_LPDP_AP_TO_DISP_TX_D1_CONN_P";
		string[16] = "90_LPDP_AP_TO_DISP_TX_D0_CONN_N";
		string[17] = "90_LPDP_AP_TO_DISP_TX_D0_CONN_P";
		string[18] = "90_LPDP_AP_TO_DISP_AUX_CONN_N";
		string[19] = "90_LPDP_AP_TO_DISP_AUX_CONN_P";
		string[20] = "I2C_DISPLAY_SCL_1V8";
		string[21] = "I2C_DISPLAY_SDA_1V8";
		string[22] = "ALPDP_AP_FROM_DISPLAY_HPD";
		string[23] = "CLK_GPIO_SEQ_PMU_TO_DISPLAY_32K_CONN";
		string[24] = "IO_AOP_PMU_DPMIC_TO_DISPLAY_PANICB_L";
		string[25] = "IO_AOP_TO_DISPLAY_XSYNC_CONN";
		string[26] = "GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN";
		string[27] = "GPIO_MTP_FROM_TOUCH_INT1_L_CONN";
		string[28] = "GPIO_MTP_FROM_TOUCH_INT0_L_CONN";
		string[29] = "SWD_NUB_BI_TOUCH_SWDIO_CONN";
		string[30] = "SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN";
		string[31] = "GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN";
		string[32] = "PP6V8_DISPLAY_AVDDH_CONN";
		string[33] = "PNVAR_DISPLAY_VGL_CONN";
		string[34] = "SPI_ALPDP_WPB_VPP";

		string[35] = "HTP_KELVIN_GND_0";
		string[36] = "AB_VBAT_DUT_GND_0";
		string[37] = "FTP_KELVIN_DG_1";
		string[38] = "FTP_KELVIN_DG_0";

		string[39] = "ID_BITS_DC_PARTS";

		CMD_Printf("\r\n======================== OPEN SHORT TEST ========================\r\n");

		CMD_Printf("  -- BOARD VERSION/ECO CHECK --\r\n");

		rtn = IoExp_PortInputRead(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTA, &readData.u8Data);
		if(rtn == 0)				return	result;

		upper = readData.u8Data << 4;
		lower = readData.u8Data >> 4;

		readData.u8Data = upper | lower;

		if(readData.u8Data == 0x80)
		{		
			CMD_Printf("%d. IO TEST OK (%s / 0x%02x)\r\n", ++indexCnt, string[39], readData.u8Data);
		}
		else
		{
			CMD_Printf("%d. IO TEST NG (%s / 0x%02x)\r\n", ++indexCnt, string[39], readData.u8Data);
			BoardTest_NgCheck(OPENSHORT);
		}
	 
		rtn = IoExp_PortInputRead(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTB, &readData.u8Data);
		if(rtn == 0)				return	result;

		readData.u8Data &= 0x0F;

		for(u8 cnt = 0; cnt < 4; cnt++)
		{			
			if(!(readData.u8Data &= (1 << cnt)))
			{
				CMD_Printf("%d. IO TEST OK (%s)\r\n", ++indexCnt, string[35 + cnt]);
			}
			else
			{
				CMD_Printf("%d. IO TEST NG (%s)\r\n", ++indexCnt, string[35 + cnt]);
				BoardTest_NgCheck(OPENSHORT);
			}
		}

		CMD_Printf("\r\n");
		
		////////////////////////////////////////////////////
		// IO OPENSHORT CHECK
		////////////////////////////////////////////////////
		for(u8 cnt = 0; cnt < 35; cnt++)
		{
			if(cnt < 8)
			{
				openshort[arr++] = IoExp_ShortCheck_D854Hinge(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTA, P0 << cnt);
			}
			else if(cnt < 16)
			{
				if(cnt == 11)	continue;

				openshort[arr++] = IoExp_ShortCheck_D854Hinge(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTB, P10 << (cnt - 8));
			}
			else if(cnt < 24)
			{
				openshort[arr++] = IoExp_ShortCheck_D854Hinge(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTA, P0 << (cnt - 16));
			}
			else if(cnt < 32)
			{	
				openshort[arr++] = IoExp_ShortCheck_D854Hinge(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTB, P10 << (cnt - 24));
			}
			else if(cnt < 35)
			{			
				openshort[arr++] = IoExp_ShortCheck_D854Hinge(pVoid, BSP_I2C2, HT_INOUT_EXPANDER, PORTA, P2 << (cnt - 32));
			}

			if(cnt != 11)
			{
				if(openshort[arr - 1])	CMD_Printf("%d. IO TEST OK (%s)\r\n", ++indexCnt, string[cnt]);
				else
				{
					CMD_Printf("%d. IO TEST NG (%s)\r\n", ++indexCnt, string[cnt]);
					BoardTest_NgCheck(OPENSHORT);
				}
			}
		}
	}
	else if(CMD_Compare(pModel, "D852"))
	{
		string[0] = "90_MIPI_AP_TO_DISPLAY_D0_CONN_N";
		string[1] = "90_MIPI_AP_TO_DISPLAY_D0_CONN_P";
		string[2] = "90_MIPI_AP_TO_DISPLAY_D1_CONN_N";
		string[3] = "90_MIPI_AP_TO_DISPLAY_D1_CONN_P";
		string[4] = "90_MIPI_AP_TO_DISPLAY_CLK_CONN_N";
		string[5] = "90_MIPI_AP_TO_DISPLAY_CLK_CONN_P";
		string[6] = "90_MIPI_AP_TO_DISPLAY_D2_CONN_N";
		string[7] = "90_MIPI_AP_TO_DISPLAY_D2_CONN_P";
		string[8] = "CLK_GPIO_SEQ_PMU_TO_DISPLAY_32K_1V8_CONN";
		string[9] = "EMPTY";
		string[10] = "IO_TOUCH_FROM_DISPLAY_MSYNC_1V8_CONN";
		string[11] = "EMPTY";
		string[12] = "SPI_MIPI_WPB_VPP";
		string[13] = "EMPTY";
		string[14] = "EMPTY";
		string[15] = "EMPTY";
		string[16] = "SWD_NUB_BI_TOUCH_SWDIO";
		string[17] = "SWD_NUB_TO_PMU_TOUCH_SWCLK";
		string[18] = "I2C3_AP_SCL_1V8";
		string[19] = "I2C3_AP_SDA_1V8";
		string[20] = "SPI0_MTP_TO_TOUCH_CS_L";
		string[21] = "SPI0_MTP_TO_TOUCH_SCLK_CONN";
		string[22] = "SPI0_MTP_TO_TOUCH_MOSI_CONN";
		string[23] = "SPI0_MTP_FROM_TOUCH_MISO_CONN";
		string[24] = "IO_KRAKEN_TO_TOUCH_AID_CONN";
		string[25] = "GPIO_MTP_TO_TOUCH_RESET_L_CONN";
		string[26] = "GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN";
		string[27] = "EMPTY";
		string[28] = "EMPTY";
		string[29] = "EMPTY";
		string[30] = "EMPTY";
		string[31] = "EMPTY";
		string[32] = "IO_AOP_TOUCH_FROM_DISPLAY_BSYNC1_CONN";
		string[33] = "PP7V3_DISPLAY_AVDDH_CONN";
		string[34] = "EMPTY";
		string[35] = "IO_DISPLAY_TO_DPMIC_AVDDH_EN_1V8_CONN";
		string[36] = "IO_DISPLAY_TO_DPMIC_VDDEL_VSSEL_EN_1V8_CONN";
		string[37] = "EMPTY";
		string[38] = "EMPTY";
		string[39] = "EMPTY";
		string[40] = "IO_AOP_TOUCH_FROM_DISPLAY_TE";
		string[41] = "GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_1V8_CONN";
		string[42] = "IO_AOP_SEQ_PMU_DPMIC_TO_DISPLAY_PANICB_L_1V8";
		string[43] = "I2C_DISPLAY_SCL_1V8";
		string[44] = "I2C_DISPLAY_SDA_1V8";
		string[45] = "GPIO_MTP_FROM_TOUCH_INT1_L_CONN";
		string[46] = "GPIO_MTP_FROM_TOUCH_INT0_L_CONN";
		string[47] = "IO_TOUCH_TO_MANY_SCAN_ACTIVE";

		string[48] = "FTP_KELVIN_DG_0";
		string[49] = "FTP_KELVIN_DG_1";
		string[50] = "AB_VBAT_DUT_GND_0";

		CMD_Printf("\r\n======================== OPEN SHORT TEST ========================\r\n");
	 
		rtn = IoExp_PortInputRead(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTB, &readData.u8Data);
		if(rtn == 0)				return	result;

		readData.u8Data &= 0x2A;

		if(!(readData.b1))
		{
			CMD_Printf("%d. IO TEST OK (%s)\r\n", ++indexCnt, string[48]);
		}
		else
		{
			CMD_Printf("%d. IO TEST NG (%s)\r\n", ++indexCnt, string[48]);
			BoardTest_NgCheck(OPENSHORT);
		}

		if(!(readData.b3))
		{
			CMD_Printf("%d. IO TEST OK (%s)\r\n", ++indexCnt, string[49]);
		}
		else
		{
			CMD_Printf("%d. IO TEST NG (%s)\r\n", ++indexCnt, string[49]);
			BoardTest_NgCheck(OPENSHORT);
		}
		/* R664 No stuff
		if(!(readData.b5))
		{
			CMD_Printf("%d. IO TEST OK (%s)\r\n", ++indexCnt, string[50]);
		}
		else
		{
			CMD_Printf("%d. IO TEST NG (%s)\r\n", ++indexCnt, string[50]);
			BoardTest_NgCheck(OPENSHORT);
		}
		*/
		CMD_Printf("\r\n");
		
		////////////////////////////////////////////////////
		// IO OPENSHORT CHECK
		////////////////////////////////////////////////////
		for(u8 cnt = 0; cnt < 48; cnt++)
		{
			if(cnt == 9 || cnt == 11 || cnt == 13 || cnt == 14 || cnt == 15 ||
			cnt == 27 || cnt == 28 || cnt == 29 || cnt == 30 || cnt == 31 ||
			cnt == 34 || cnt == 37 || cnt == 38 || cnt == 39)					continue;
				
			if(cnt < 8)
			{
				openshort[arr++] = IoExp_ShortCheck_D852Hinge(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTA, P0 << cnt);
			}
			else if(cnt < 16)
			{
				openshort[arr++] = IoExp_ShortCheck_D852Hinge(pVoid, BSP_I2C2, HT_VER_EXPANDER, PORTB, P10 << (cnt - 8));
			}
			else if(cnt < 24)
			{
				openshort[arr++] = IoExp_ShortCheck_D852Hinge(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTA, P0 << (cnt - 16));
			}
			else if(cnt < 32)
			{	
				openshort[arr++] = IoExp_ShortCheck_D852Hinge(pVoid, BSP_I2C2, HT_OUT_EXPANDER_1, PORTB, P10 << (cnt - 24));
			}
			else if(cnt < 40)
			{			
				openshort[arr++] = IoExp_ShortCheck_D852Hinge(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTA, P0 << (cnt - 32));
			}
			else if(cnt < 48)
			{				
				openshort[arr++] = IoExp_ShortCheck_D852Hinge(pVoid, BSP_I2C2, HT_OUT_EXPANDER_2, PORTB, P0 << (cnt - 40));
			}

			if(openshort[arr - 1])	CMD_Printf("%d. IO TEST OK (%s)\r\n", ++indexCnt, string[cnt]);
			else
			{
				CMD_Printf("%d. IO TEST NG (%s)\r\n", ++indexCnt, string[cnt]);
				BoardTest_NgCheck(OPENSHORT);
			}
			
		}
	}

	testItem--;

	result = 1;

	return	result;
}

static u8 Cmd_HingeTestPower(void *pVoid)
{
	u8	result = 0, rtn, num;
	double	ReadData[2];
	const char *string[9];	
	
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pModel, "D854"))
	{	
		string[0] = "PP5V1_TOUCH_VDDH_CONN_SENSE"; 				//vm3
		string[1] = "PP1V2_TOUCH_DISPLAY_S2_CONN_SENSE"; 		//vm4
		string[2] = "PP1V2_TOUCH_VDDCORE_CONN_SENSE"; 			//3.3v dcdc
		string[3] = "PP1V8_TOUCH_DISPLAY_DVDD_CONN_SENSE";		//vm2
		string[4] = "PP3V15_TOUCH_DISPLAY_S2_CONN_SENSE"; 		//vm0
		string[5] = "PP1V24_DISPLAY_S2_CONN_SENSE"; 			//vm6
		string[6] = "PP2V8_DISPLAY_VDDEL_CONN_V_MEAS"; 			//3.3v dcdc
		string[7] = "PP1VX_DISPLAY_S2_CONN_SENSE"; 				//vm1     
		string[8] = "PNVAR_DISPLAY_VSSEL_CONN"; 				//vm5	
	}
	else if(CMD_Compare(pModel, "D852"))
	{
		string[0] = "PP5V1_TOUCH_VDDTX_S2_CONN_SENSE"; 			//vm3
		string[1] = "PP1V2_TOUCH_DISPLAY_S2_CONN_SENSE"; 		//vm4
		string[2] = "PP1V2_TOUCH_VDDCORE_S2_CONN_SENSE"; 		//3.3v dcdc
		string[3] = "PP1V8_TOUCH_DISPLAY_DVDD_S2_CONN_SENSE";	//vm2
		string[4] = "PP3V15_TOUCH_DISPLAY_S2_CONN_SENSE"; 		//vm0
		string[5] = "EMPTY"; 									//vm6
		string[6] = "PP2V8_DISPLAY_VDDEL_CONN_V_MEAS"; 			//3.3v dcdc
		string[7] = "PP1VX_DISPLAY_S2_CONN_SENSE"; 				//vm1     
		string[8] = "PNVAR_DISPLAY_VSSEL_CONN"; 				//vm5
	}
	
	CMD_Printf("\r\n========================== POWER TEST ==========================\r\n");

	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		 if(CMD_Compare(pModel, "D852"))
		 {
			if(cnt == 5 || cnt == 6)		continue;
			else if(cnt == 7)				num = cnt - 2;
			else							num = cnt;
		 }
		 else	num = cnt;
		 
		////////////////////////////////////////////////////
		// AMUX Enable
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, P4 | P5);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, P5);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10);
		
		////////////////////////////////////////////////////
		// AMUX Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
	
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, AMUX_S1 + num);
		if(rtn == 0)		return	result;
		
		HAL_Delay(10);
	
		////////////////////////////////////////////////////
		// POWER MODULE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &ReadData[0]); 
		if(rtn == 0)				return	result;

		////////////////////////////////////////////////////
		// DC AMUX Enable
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, P4 | P5);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, P4);
		if(rtn == 0)				return	result;
		
		HAL_Delay(10);

		////////////////////////////////////////////////////
		// POWER MODULE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &ReadData[1]); 
		if(rtn == 0)				return	result;
		
		for(u8 cmp = 0; cmp < 2; cmp++)
		{
			if(ReadData[cmp] < hVoltageMin[cnt] || ReadData[cmp] > hVoltageMax[cnt])
			{
				if(cmp == 0)	CMD_Printf("%d. POWER TEST NG (%s / %f V)\r\n", ++indexCnt, string[cnt], ReadData[cmp]);
				else			CMD_Printf("%d. POWER TEST NG (DC_%s / %f V)\r\n", ++indexCnt, string[cnt], ReadData[cmp]);
				BoardTest_NgCheck(POWER);
			}
			else
			{
				if(cmp == 0)	CMD_Printf("%d. POWER TEST OK (%s / %f V)\r\n", ++indexCnt, string[cnt], ReadData[cmp]);
				else			CMD_Printf("%d. POWER TEST OK (DC_%s / %f V)\r\n", ++indexCnt, string[cnt], ReadData[cmp]);
			}
		}
		
		CMD_Printf("\r\n");
	}

	if(CMD_Compare(pModel, "D854"))
	{
		////////////////////////////////////////////////////
		// VSSEL MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &ReadData[0]); 
		if(rtn == 0)				return	result;

		if(ReadData[0] < hVoltageMin[8] || ReadData[0] > hVoltageMax[8])
		{
			CMD_Printf("%d. POWER TEST NG (%s / %f V)\r\n", ++indexCnt, string[8], ReadData[0]);
			BoardTest_NgCheck(POWER);
		}
		else
		{
			CMD_Printf("%d. POWER TEST OK (%s / %f V)\r\n", ++indexCnt, string[8], ReadData[0]);
		}
	}
	else if(CMD_Compare(pModel, "D852"))
	{
		for(u8 cmp = 0; cmp < 2; cmp++)
		{
			////////////////////////////////////////////////////
			// AMUX Latch
			////////////////////////////////////////////////////
			rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, P0 | P1 | P2);
			if(rtn == 0)				return	result;
			
			rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, AMUX_S7 + cmp);
			if(rtn == 0)				return	result;
			
			HAL_Delay(10);

			if(cmp == 0)	num = 6;
			else			num = 8;
			
			////////////////////////////////////////////////////
			// POWER MODULE MEASURE
			////////////////////////////////////////////////////
			rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &ReadData[0]); 
			
			if(ReadData[0] < hVoltageMin[num] || ReadData[0] > hVoltageMax[num])
			{
				CMD_Printf("%d. POWER TEST NG (%s / %f V)\r\n", ++indexCnt, string[num], ReadData[0]);
				BoardTest_NgCheck(POWER);
			}
			else
			{
				CMD_Printf("%d. POWER TEST OK (%s / %f V)\r\n", ++indexCnt, string[num], ReadData[0]);
			}
		}
	}
		
	testItem--;
	
	result = 1;

	return	result;
}

static u8 Cmd_HingeTestFinalResult(void *pVoid)
{
	u8	result = 0, rtn, errIdx = 0, chk = 0, sum = 0;
	u32 temp;

	CMD_Printf("\r\n========================== TEST END ==========================\r\n");

	for(u8 i = 0; i < 8; i++)
	{
		sum += ngCnt[i];
	}

	sprintf((char*)pModel, "NONE");

	////////////////////////////////////////////////////
	// TEST LED INIT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, PORT_ALL);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTB, PORT_ALL);
	if(rtn == 0)				return	result;
	
	if(sum == 0 && testItem == 0)
	{
		CMD_Printf("TOTAL RESULT : OK\r\n");
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTB, 0x08);
		if(rtn == 0)				return	result;
	}
	else
	{
		CMD_Printf("TOTAL RESULT : NG\r\n");
#if 0
		CMD_Printf("\r\n");
		CMD_Printf("NG INDEX : ");

		for(u8 i = 0; i < 3; i++)
		{
			for(u8 j = 0; j < BIT32_FULL_CNT; j++)
			{
				temp = ngNum[i].u32Data & (1 << j);

				if(temp >= 1)	
				{
					if(i == 0)	errIdx = j + 1 ;		
					else 		errIdx = j + 1 + BIT32_FULL_CNT * i;
		
					chk++;
				}
				else				return	result;

				if(chk)		CMD_Printf("%d ", errIdx);
				
				chk = 0;
			}
		}
#endif

		rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTA, 0x40);
		if(rtn == 0)				return	result;
		
		if(ngCnt[1])
		{
			rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTB, 0x02);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[2])
		{
			rtn = IoExp_PortSet(pVoid, BSP_I2C2, HT_ETC_EXPANDER, PORTB, 0x01);
			if(rtn == 0)				return	result;
		}
	}

	CMD_Printf("\r\n");

	memset(ngNum, 0, sizeof(ngNum));
	memset(ngCnt, 0, sizeof(ngCnt));
	memset(&indexCnt, 0, sizeof(indexCnt));
	memset(&remove_io_ng, 0, sizeof(remove_io_ng));
	memset(&testItem, 0xff, sizeof(testItem));
	
	result = 1;

	return	result;
}

static u8 Cmd_HingeTestModel(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;
	
	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "D854"))	sprintf((char*)pModel, "D854");
	else if(CMD_Compare(pData->pArgv[0], "D852"))	sprintf((char*)pModel, "D852");
	else
	{
		LOG_Printf("%d. TEST MODEL CHECK NG (NEED TO SET TEST MODEL!)\r\n", ++indexCnt);
		CMD_Printf("%d. TEST MODEL CHECK NG (NEED TO SET TEST MODEL!)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	
		return	result;
	}

	result = 1;

	return 	result;		
}

static u8 Cmd_HingeTestVersion(void *pVoid)
{
	u8	result = 0;
	
	CMD_Printf("\r\n");
	CMD_Printf("FIRMWARE VERSION : v1.0.2\r\n");	
	CMD_Printf("REVISION DATE : 2022.03.21\r\n");	

	result = 1;

	return	result;
}	

/******************************************************************************
* Revision History
*
* v1.0.0 : D854 Hinge Test Function complete, Test OK 
* v1.0.1 : Cmd_HingeTestTestItem -> Cmd_HingeTestItem Function name changed
           Remove Function Cmd_HingeTestRemoveNgIndex
           Add D852 Hinge Test Function
           Modified Cmd_HingeTestVoltageLimit
* v1.0.2 : Modified Cmd_HingeTestVersion         
******************************************************************************/		

const	commandList_t	cmdList_Hingetest[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"version",				Cmd_HingeTestVersion,		NULL,		NULL,		"",						NULL},
												{	"model",				Cmd_HingeTestModel,			NULL,		NULL,		"",						NULL},
												{	"test.item",			Cmd_HingeTestItem,			NULL,		NULL,		"",						NULL},
												{	"voltage.limit",		Cmd_HingeTestVoltageLimit,	NULL,		NULL,		"",						NULL},
												{	"io",					Cmd_HingeTestOpenShort,		NULL,		NULL,		"",						NULL},
												{	"power",				Cmd_HingeTestPower,			NULL,		NULL,		"",						NULL},
												{	"port.init",			Cmd_HingeTestPortDirInit,	NULL,		NULL,		"",						NULL},
												{	"final.result",			Cmd_HingeTestFinalResult,	NULL,		NULL,		"",						NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};
