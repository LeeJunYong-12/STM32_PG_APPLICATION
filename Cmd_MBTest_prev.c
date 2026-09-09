#include	"Cmd_MBTest_prev.h"

double	dp_D0pMin, dp_D0pMax;
double	dp_D1pMin, dp_D1pMax;
double	dp_D2pMin, dp_D2pMax;
double	dp_AuxpMin, dp_AuxpMax;

double	dp_D0nMin, dp_D0nMax;
double	dp_D1nMin, dp_D1nMax;
double	dp_D2nMin, dp_D2nMax;
double	dp_AuxnMin, dp_AuxnMax;

static u8 Cmd_MBTestPrevI2c(void *pVoid)
{
	u8	result = 0, pass[7] = {0, }, rCnt, rtn;
	u8	dc_eeprom_adrs;
	u8	mb_eeprom_adrs;
	u8	mbexpander_adrs[2];
	u8	mb_brd;
	u8	dc_brd;
	u8	*pRxBuffer;
	u8	pmic_adrs;
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
	rtn = IoExp_LED_TEST_RUN(pVoid, BSP_I2C2);
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
	// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);	

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P17);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN LOW
	/////////////////////////////////////////////////// /
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P5);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L HIGH
	////////////////////////////////////////////////////
	rtn = IoExp_PortWrite(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P5);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN HIGH
	////////////////////////////////////////////////////
	rtn = IoExp_PortWrite(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// I2C0 Line slave address check
	// -> 0x25(MBBD_IOExpander_1) check
	// -> 0xA0, 0xA1(EEPROM) check
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
			u16	memAdrs = 0x0C;

			pmic_adrs = pRxBuffer[i];
 			
			rtn = LCM_I2CMemRead(pData->position, BSP_I2C2, pmic_adrs, memAdrs, 0, &pmic_id, 1);
			if(rtn == 0)				return	result;
			
			HAL_Delay(5);
			
			pass[5] = 1;			
		}
	}

	////////////////////////////////////////////////////
	// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
	// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P17);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P5);
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

	LOG_Printf("  -- BOARD VERSION/ECO CHECK --\r\n");
	CMD_Printf("  -- BOARD VERSION/ECO CHECK --\r\n");

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

	LOG_Printf("  -- PMIC I2C CHECK --\r\n");
	CMD_Printf("  -- PMIC I2C CHECK --\r\n");

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
			
			rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_5, LOW);
			if(rtn == 0)				return	result;

			BoardTest_NgCheck(I2C);
		}
	}	
	else
	{		
		LOG_Printf("%d. I2C TEST NG (PMIC ADDRESS / 0x%02x)\r\n", ++indexCnt, pmic_adrs);
		CMD_Printf("%d. I2C TEST NG (PMIC ADDRESS / 0x%02x)\r\n", indexCnt, pmic_adrs);

		PWR_OutputEnable(pData->position, PWR_SRC_VM_5, LOW);
		BoardTest_NgCheck(I2C);
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

static u8 Cmd_MBTestPrevSerialChk(void *pVoid)
{
	u8	result = 0, rtn;
	u8	slaveAdrs, mode;
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

	slaveAdrs = 0xA2;

	memAdrs = 0xFA;

	rxSize = 6;

	mode = 0;

	rtn = LCM_I2CMemRead(pData->position, BSP_I2C1, slaveAdrs, memAdrs, mode, pRxBuffer, rxSize);
	if(rtn == 0)				return	result;
	
	HAL_Delay(5);

	////////////////////////////////////////////////////
	// EEPROM FORMAT
	////////////////////////////////////////////////////
	memset(ptxbuffer, 0xFF, sizeof(ptxbuffer));

	memAdrs = 0x00;

	do{
		rtn = LCM_I2CMemWrite(pData->position, BSP_I2C1, 0xA2, memAdrs, mode, ptxbuffer, 16);
		if(rtn == 0)				return	result;
		
		HAL_Delay(5);

		rtn = LCM_I2CMemWrite(pData->position, BSP_I2C1, 0xA4, memAdrs, mode, ptxbuffer, 16);
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

	if(CMD_Compare(pData->pArgv[0], "search"))
	{
		LOG_Printf("%d. VENTURA ID EXTRACTION (", ++indexCnt);
		CMD_Printf("%d. VENTURA ID EXTRACTION (", indexCnt);
		
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
	}
	else
	{
		ptxbuffer2[0] = 0x61;
		
		for(u16 cnt = 0; cnt < 15; cnt++)
		{
		  ptxbuffer2[cnt+1] = ptxbuffer[cnt];
		}

		rtn = LCM_I2CTransmitData(pData->position, BSP_I2C1, slaveAdrs, ptxbuffer2, 16);
		if(rtn == 0)				return	result;
		
		HAL_Delay(5);
		
		ptxbuffer2[0] = 0x70;
		ptxbuffer2[1] = ptxbuffer[15];
		ptxbuffer2[2] = ptxbuffer[16];

		rtn = LCM_I2CTransmitData(pData->position, BSP_I2C1, slaveAdrs, ptxbuffer2, 3);
		if(rtn == 0)				return	result;
		
		HAL_Delay(5);

		slaveAdrs = 0xA2;

		memAdrs = 0x61;

		rxSize = 17;

		mode = 0;
		
		rtn = LCM_I2CMemRead(pData->position, BSP_I2C1, slaveAdrs, memAdrs, mode, prxbuffer, rxSize);
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
	}

	slaveAdrs = 0xA4;

	memAdrs = 0xFA;

	rxSize = 6;

	mode = 0;

	rtn = LCM_I2CMemRead(pData->position, BSP_I2C1, slaveAdrs, memAdrs, mode, pRxBuffer, rxSize);
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


static u8 Cmd_MBTestPrevOpenShort(void *pVoid)
{
	u8	result = 0, rtn;
	bitCtrl8_t	readData;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;

	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;

	LOG_Printf("\r\n======================== OPEN SHORT TEST ========================\r\n");
	CMD_Printf("\r\n======================== OPEN SHORT TEST ========================\r\n");

	rtn = IoExp_PortInputRead(pVoid, BSP_I2C2, MT_IN_EXPANDER_2, PORTA, &readData.u8Data);
	if(rtn == 0)				return	result;

	if(!readData.b3)
	{
		LOG_Printf("%d. IO TEST OK (FTP_KELVIN_DG_0)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (FTP_KELVIN_DG_0)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (FTP_KELVIN_DG_0)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (FTP_KELVIN_DG_0)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(!readData.b4)
	{
		LOG_Printf("%d. IO TEST OK (FTP_KELVIN_DG_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (FTP_KELVIN_DG_1)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (FTP_KELVIN_DG_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (FTP_KELVIN_DG_1)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(!readData.b6)
	{
		LOG_Printf("%d. IO TEST OK (POGO_VTP_DUT_GND_0)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (POGO_VTP_DUT_GND_0)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (POGO_VTP_DUT_GND_0)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (POGO_VTP_DUT_GND_0)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(!readData.b7)
	{
		LOG_Printf("%d. IO TEST OK (HTP_KELVIN_GND_0)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (HTP_KELVIN_GND_0)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (HTP_KELVIN_GND_0)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (HTP_KELVIN_GND_0)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_0/1 DISABLE
	// UTIL_1V8_GPIO_3/4 ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P16 | P17);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P12 | P13);
	if(rtn == 0)				return	result;

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, P0))
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_TO_TOUCH_RESET_L_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, P1))
	{
		LOG_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SCLK_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, P2))
	{
		LOG_Printf("%d. IO TEST OK (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_FROM_TOUCH_MISO_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, P3))
	{
		LOG_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_MOSI_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, P4))
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT0_L_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, P5))
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_KRAKEN_TO_TOUCH_AID_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_KRAKEN_TO_TOUCH_AID_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_KRAKEN_TO_TOUCH_AID_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_KRAKEN_TO_TOUCH_AID_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, P7))
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_1)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P10))
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_1)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P11))
	{
		LOG_Printf("%d. IO TEST OK (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN)\r\n", +indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_SWD_NUB_TO_PMU_TOUCH_SWCLK_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P12))
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC0_CONN_2)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P13))
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_TOUCH_BSYNC1_CONN_2)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P14))
	{
		LOG_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_SPI0_MTP_TO_TOUCH_SSIN_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}
	
	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P15))
	{
		LOG_Printf("%d. IO TEST OK (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_SWD_NUB_BI_TOUCH_SWDIO_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P3))
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P4))
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_TOUCH_TO_MANY_SCAN_ACTIVE)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P6))
	{
		LOG_Printf("%d. IO TEST OK (DC_I2C_SDA_TOUCH_EEPROM_1V8)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_I2C_SDA_TOUCH_EEPROM_1V8)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_I2C_SDA_TOUCH_EEPROM_1V8)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_I2C_SDA_TOUCH_EEPROM_1V8)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P7))
	{
		LOG_Printf("%d. IO TEST OK (DC_I2C_SCL_TOUCH_EEPROM_1V8)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_I2C_SCL_TOUCH_EEPROM_1V8)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_I2C_SCL_TOUCH_EEPROM_1V8)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_I2C_SCL_TOUCH_EEPROM_1V8)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P10))
	{
		LOG_Printf("%d. IO TEST OK (FRAME_GND_MEAS_IN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (FRAME_GND_MEAS_IN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (FRAME_GND_MEAS_IN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (FRAME_GND_MEAS_IN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P14))
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_GPIO_MTP_FROM_TOUCH_INT1_L_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P15))
	{
		LOG_Printf("%d. IO TEST OK (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_GPIO_AP_FROM_DISPLAY_PANEL_ID_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}

	if(IoExp_ShortCheck_Prev(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, OUT_XSYNC))
	{
		LOG_Printf("%d. IO TEST OK (DC_IO_AOP_TO_DISPLAY_XSYNC_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_IO_AOP_TO_DISPLAY_XSYNC_CONN)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_IO_AOP_TO_DISPLAY_XSYNC_CONN)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_IO_AOP_TO_DISPLAY_XSYNC_CONN)\r\n", indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}
/*
	if(openshort[20])
	{
		LOG_Printf("%d. IO TEST OK (DC_ALPDP_AP_FROM_DISPLAY_HPD)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST OK (DC_ALPDP_AP_FROM_DISPLAY_HPD)\r\n", indexCnt);
	}
	else
	{
		LOG_Printf("%d. IO TEST NG (DC_ALPDP_AP_FROM_DISPLAY_HPD)\r\n", ++indexCnt);
		CMD_Printf("%d. IO TEST NG (DC_ALPDP_AP_FROM_DISPLAY_HPD)\r\n", indexCnt);
		ngCnt[1]++;
	}

	if(openshort[21])
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
*/	
	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_0/1 DISABLE
	// UTIL_1V8_GPIO_3/4 DISABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P16 | P17);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P12 | P13);
	if(rtn == 0)				return	result;
	
	testItem--;
	
	result = 1;

	return	result;
}	

static u8 Cmd_MBTestPrevPower(void *pVoid)
{
	u8	result = 0, rtn;
	
	double	pp1v24_dp_s2_conn_sense[2];
	double	pp1v2_tc_dp_s2_conn_sense[2];
	double	spi_alpdp_wpb_vpp_sense[2];
	double	pp1v8_tc_dp_dvdd_conn_sense[2];
	double	pp1vx_dp_s2_conn_sense[2];
	double	pp3v15_tc_dp_s2_conn_sense[2];

	double	pp1v2_tc_vddcore_conn_sense[2];
	double	htp_s_0[2];
	double	vtp_s_1[2];
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;

	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;

	////////////////////////////////////////////////////
	// POWER ENABLE VM
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 7; cnt++)
	{
		if(cnt == PWR_SRC_VM_5)		continue;
		
		rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, HIGH);
		if(rtn == 0)				return	result;
		
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
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_0, &pp3v15_tc_dp_s2_conn_sense[0], &pp3v15_tc_dp_s2_conn_sense[1]); 			//3.15v
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_1, &pp1vx_dp_s2_conn_sense[0], &pp1vx_dp_s2_conn_sense[1]);  				//1.xv
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_2, &pp1v8_tc_dp_dvdd_conn_sense[0], &pp1v8_tc_dp_dvdd_conn_sense[1]);  		//1.8v
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_3, &spi_alpdp_wpb_vpp_sense[0], &spi_alpdp_wpb_vpp_sense[1]);				//6.0v
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_4, &pp1v2_tc_dp_s2_conn_sense[0], &pp1v2_tc_dp_s2_conn_sense[1]);			//1.2v
	if(rtn == 0)				return	result;

	HAL_Delay(20);
	
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_6, &pp1v24_dp_s2_conn_sense[0], &pp1v24_dp_s2_conn_sense[1]);				//1.24v
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// VOLTAGE SENSEING ADMUX ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3 | P4);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// AMUX_S5 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S5);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// TOUCH VOLTAGE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &pp1v2_tc_vddcore_conn_sense[0]);				//1.2v
	if(rtn == 0)				return	result;	
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AMUX_S7 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S7);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_0 ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P16 | P17);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P16);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// TOUCH VOLTAGE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &vtp_s_1[0]); 		//2.5v
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_1 ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P16 | P17);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortWrite(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P17);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// TOUCH VOLTAGE MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &htp_s_0[0]);  		//2.5v
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	//*****************************
	// 
	// POWER CURRENT CHECK
	//
	//*****************************
	
	////////////////////////////////////////////////////
	// CURRENT SENSEING ADMUX ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P3);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// ADMUX S1 CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// TOUCH CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &pp1v2_tc_vddcore_conn_sense[1]);		//1.2v
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	pp1v2_tc_vddcore_conn_sense[1] *= 1000;
	pp1v2_tc_vddcore_conn_sense[1] /= 41; // Gain = 41
	pp1v2_tc_vddcore_conn_sense[1] /= TOUCH_SHUNT;

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_0 ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P16 | P17);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P16);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// ADMUX S3 CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// TOUCH CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &vtp_s_1[1]); 	
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	vtp_s_1[1] *= 1000;
	vtp_s_1[1] /= 41; // Gain = 41
	vtp_s_1[1] /= TOUCH_SHUNT;

	////////////////////////////////////////////////////
	// UTIL_1V8_GPIO_1 ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P16 | P17);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, P17);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// ADMUX S2 CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// TOUCH CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &htp_s_0[1]);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

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

	LOG_Printf("  -- MODULE CONNECTOR POWER CHECK --\r\n");
	CMD_Printf("  -- MODULE CONNECTOR POWER CHECK --\r\n");

	if(pp3v15_tc_dp_s2_conn_sense[0] < vmVoltageMin[0] || pp3v15_tc_dp_s2_conn_sense[0] > vmVoltageMax[0] || 
		pp3v15_tc_dp_s2_conn_sense[1] < vmCurrentMin[0][0] || pp3v15_tc_dp_s2_conn_sense[1] > vmCurrentMax[0][0])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP3V15_TOUCH_DISPLAY_S2_CONN / %f V %f mA)\r\n", ++indexCnt, pp3v15_tc_dp_s2_conn_sense[0], pp3v15_tc_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (DC_PP3V15_TOUCH_DISPLAY_S2_CONN / %f V %f mA)\r\n", indexCnt, pp3v15_tc_dp_s2_conn_sense[0], pp3v15_tc_dp_s2_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP3V15_TOUCH_DISPLAY_S2_CONN / %f V %f mA)\r\n", ++indexCnt, pp3v15_tc_dp_s2_conn_sense[0], pp3v15_tc_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (DC_PP3V15_TOUCH_DISPLAY_S2_CONN / %f V %f mA)\r\n", indexCnt, pp3v15_tc_dp_s2_conn_sense[0], pp3v15_tc_dp_s2_conn_sense[1]);
	}

	if(pp1vx_dp_s2_conn_sense[0] < vmVoltageMin[1] || pp1vx_dp_s2_conn_sense[0] > vmVoltageMax[1] ||
		pp1vx_dp_s2_conn_sense[1] < vmCurrentMin[1][0] || pp1vx_dp_s2_conn_sense[1] > vmCurrentMax[1][0])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1VX_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1vx_dp_s2_conn_sense[0], pp1vx_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (DC_PP1VX_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1vx_dp_s2_conn_sense[0], pp1vx_dp_s2_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1VX_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1vx_dp_s2_conn_sense[0], pp1vx_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (DC_PP1VX_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1vx_dp_s2_conn_sense[0], pp1vx_dp_s2_conn_sense[1]);
	}
	
	if(pp1v8_tc_dp_dvdd_conn_sense[0] < vmVoltageMin[2] || pp1v8_tc_dp_dvdd_conn_sense[0] > vmVoltageMax[2] ||
		pp1v8_tc_dp_dvdd_conn_sense[1] < vmCurrentMin[2][0] || pp1v8_tc_dp_dvdd_conn_sense[1] > vmCurrentMax[2][0])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v8_tc_dp_dvdd_conn_sense[0], pp1v8_tc_dp_dvdd_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (DC_PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V  %f mA)\r\n", indexCnt, pp1v8_tc_dp_dvdd_conn_sense[0], pp1v8_tc_dp_dvdd_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v8_tc_dp_dvdd_conn_sense[0], pp1v8_tc_dp_dvdd_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (DC_PP1V8_TOUCH_DISPLAY_DVDD_CONN / %f V  %f mA)\r\n", indexCnt, pp1v8_tc_dp_dvdd_conn_sense[0], pp1v8_tc_dp_dvdd_conn_sense[1]);
	}

	if(spi_alpdp_wpb_vpp_sense[0] < vmVoltageMin[3] || spi_alpdp_wpb_vpp_sense[0] > vmVoltageMax[3] ||
		spi_alpdp_wpb_vpp_sense[1] < vmCurrentMin[3][0] || spi_alpdp_wpb_vpp_sense[1] > vmCurrentMax[3][0])
	{
		LOG_Printf("%d. POWER TEST NG (DC_SPI_ALPDP_WPB_VPP / %f V  %f mA)\r\n", ++indexCnt, spi_alpdp_wpb_vpp_sense[0], spi_alpdp_wpb_vpp_sense[1]);
		CMD_Printf("%d. POWER TEST NG (DC_SPI_ALPDP_WPB_VPP / %f V  %f mA)\r\n", indexCnt, spi_alpdp_wpb_vpp_sense[0], spi_alpdp_wpb_vpp_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_SPI_ALPDP_WPB_VPP / %f V  %f mA)\r\n", ++indexCnt, spi_alpdp_wpb_vpp_sense[0], spi_alpdp_wpb_vpp_sense[1]);
		CMD_Printf("%d. POWER TEST OK (DC_SPI_ALPDP_WPB_VPP / %f V  %f mA)\r\n", indexCnt, spi_alpdp_wpb_vpp_sense[0], spi_alpdp_wpb_vpp_sense[1]);
	}

	if(pp1v2_tc_dp_s2_conn_sense[0] < vmVoltageMin[4] || pp1v2_tc_dp_s2_conn_sense[0] > vmVoltageMax[4] ||
		pp1v2_tc_dp_s2_conn_sense[1] < vmCurrentMin[4][0] || pp1v2_tc_dp_s2_conn_sense[1] > vmCurrentMax[4][0])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1V2_TOUCH_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v2_tc_dp_s2_conn_sense[0], pp1v2_tc_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (DC_PP1V2_TOUCH_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1v2_tc_dp_s2_conn_sense[0], pp1v2_tc_dp_s2_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1V2_TOUCH_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v2_tc_dp_s2_conn_sense[0], pp1v2_tc_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (DC_PP1V2_TOUCH_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1v2_tc_dp_s2_conn_sense[0], pp1v2_tc_dp_s2_conn_sense[1]);
	}

	if(pp1v24_dp_s2_conn_sense[0] < vmVoltageMin[6] || pp1v24_dp_s2_conn_sense[0] > vmVoltageMax[6] ||
		pp1v24_dp_s2_conn_sense[1] < vmCurrentMin[6][0] || pp1v24_dp_s2_conn_sense[1] > vmCurrentMax[6][0])
	{
		LOG_Printf("%d. POWER TEST NG (DC_PP1V24_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v24_dp_s2_conn_sense[0], pp1v24_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST NG (DC_PP1V24_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1v24_dp_s2_conn_sense[0], pp1v24_dp_s2_conn_sense[1]);
		BoardTest_NgCheck(POWER);
	}
	else	
	{
		LOG_Printf("%d. POWER TEST OK (DC_PP1V24_DISPLAY_S2_CONN / %f V  %f mA)\r\n", ++indexCnt, pp1v24_dp_s2_conn_sense[0], pp1v24_dp_s2_conn_sense[1]);
		CMD_Printf("%d. POWER TEST OK (DC_PP1V24_DISPLAY_S2_CONN / %f V  %f mA)\r\n", indexCnt, pp1v24_dp_s2_conn_sense[0], pp1v24_dp_s2_conn_sense[1]);
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

	if(vtp_s_1[0] < vol_2v5Min || vtp_s_1[0] > vol_2v5Max ||
		vtp_s_1[1] < cur_2v5Min || vtp_s_1[1] > cur_2v5Max)
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

	if(htp_s_0[0] < vol_2v5Min || htp_s_0[0] > vol_2v5Max ||
		htp_s_0[1] < cur_2v5Min || htp_s_0[1] > cur_2v5Max)
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

	
static u8 Cmd_MBTestPrevPMIC(void *pVoid)
{
	u8	result = 0, rtn;
	u8	pmic_adrs;
	u8	pmic_reg[4];

	double	adj_vssel;
	double	adj_vddel;
	double	adj_avddh;
	double	adj_vgl;
		
	double	vssel[3];
	double	avddh[2];
	double	vgl[2];
	double	vddel;
	double	vddel_conn;

	double	vbat;
	double	ibat;
	double	temp;
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
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P17);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AP_TO_CHIANTI_PANICB_L LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P5);
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
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P5);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN HIGH
	////////////////////////////////////////////////////
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P3);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
	// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
	////////////////////////////////////////////////////
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P17);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P2);
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
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P3);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// CURRENT MEAS MUX S1A CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// MB IOEXPNADER
	// GPIO_3V3_EN_AVDDH_I_MEAS
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P3 | P2 | P1 | P0);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// PMIC CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &avddh[1]); 	
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

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
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// MB IOEXPNADER
	// GPIO_3V3_EN_VGL_I_MEAS
	//
	// P0 LOW => SHUNT ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P3 | P2 | P1 | P0);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// EN_PMIC_LOAD (GPIO1) ON	
	// EN_PMIC_LOAD_1 (GPIO2) OFF
	////////////////////////////////////////////////////
	rtn = LCM_GpioClear(pData->position, 0x04);
	if(rtn == 0)				return	result;
	
	rtn = LCM_GpioSet(pData->position, 0x02);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// V/IBAT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_MesureVI(pData->position, PWR_SRC_VM_5, &vbat, &ibat);
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// PMIC CURRENT MEASURE	
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &vgl[1]);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	vgl[1] *= 1000;
	vgl[1] /= 41; // Gain = 41
	vgl[1] /= VGL_SHUNT;

	////////////////////////////////////////////////////
	// CURRENT MEAS MUX S1A CHANNEL SELECT
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// MB IOEXPNADER
	// GPIO_3V3_EN_ELVSS_1 HIGH
	// GPIO_3V3_EN_ELVSS_2 LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P3 | P2 | P1 | P0);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// EN_PMIC_LOAD (GPIO1) ON
	// EN_PMIC_LOAD_1 (GPIO2) OFF
	////////////////////////////////////////////////////
	rtn = LCM_GpioClear(pData->position, 0x04);
	if(rtn == 0)				return	result;
	
	rtn = LCM_GpioSet(pData->position, 0x02);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// PMIC CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &vssel[1]); 
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

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
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// EN_PMIC_LOAD (GPIO1) OFF
	// EN_PMIC_LOAD_1 (GPIO2) ON
	////////////////////////////////////////////////////
	rtn = LCM_GpioClear(pData->position, 0x02);
	if(rtn == 0)				return	result;
	
	rtn = LCM_GpioSet(pData->position, 0x04);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// PMIC CURRENT MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &vssel[2]); 
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

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
	
	HAL_Delay(20);
	
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
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3 | P4);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		////////////////////////////////////////////////////
		// AMUX_S1 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S1);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &vssel[0]);	
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		if(cnt == 0)	vssel[0] *= -2;
		else			adj_vssel = vssel[0] * -2;

		////////////////////////////////////////////////////
		// AMUX_S2 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S2);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &avddh[0]);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		if(cnt == 0)	avddh[0] *= 2;
		else			adj_avddh = avddh[0] * 2;

		////////////////////////////////////////////////////
		// AMUX_S3 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S3);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &vgl[0]);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		if(cnt == 0)	vgl[0] *= -4;
		else			adj_vgl = vgl[0] * -4;

		////////////////////////////////////////////////////
		// AMUX_S4 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S4);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20); //modify 150ms
	        
		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &vddel);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		if(cnt == 0)	vddel *= 3;
		else			adj_vddel = vddel * 3;

		////////////////////////////////////////////////////
		// AMUX_S8 Latch
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
		if(rtn == 0)				return	result;
		
		rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, AMUX_S8);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20); //modify 150ms

		////////////////////////////////////////////////////
		// PMIC VOLTAGE MEASURE
		////////////////////////////////////////////////////
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &temp);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		if(cnt == 0) 	vddel_conn = temp * 3;

		////////////////////////////////////////////////////
		// VOLTAGE SENSEING ADMUX DISABLE
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P3 | P4);
		if(rtn == 0)				return	result;

		//*****************************
		// 
		// PMIC DISABLE
		//
		//*****************************

		////////////////////////////////////////////////////
		// IO_DISPLAY_TO_DPMIC_VDDEL_EN_CONN
		// IO_DISPLAY_TO_DPMIC_AVDDH_EN_CONN
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P2);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P17);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		////////////////////////////////////////////////////
		// GPIO_SEQ_PMU_TO_DISPLAY_RESET_L_CONN LOW
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P3);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);

		////////////////////////////////////////////////////
		// AP_TO_CHIANTI_PANICB_L LOW
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, P5);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);
	}

	LOG_Printf("\r\n========================== PMIC TEST ==========================\r\n");
	CMD_Printf("\r\n========================== PMIC TEST ==========================\r\n");
	
	LOG_Printf("  -- V/IBAT CHECK --\r\n");
	CMD_Printf("  -- V/IBAT CHECK --\r\n");

	if(vbat < vmVoltageMin[5] || vbat > vmVoltageMax[5] ||
		ibat < vmCurrentMin[5][0] || ibat > vmCurrentMax[5][0])
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

	if(vssel[1] <  pmicCurrentMin[VSSEL] || vssel[1] > pmicCurrentMax[VSSEL])
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

	LOG_Printf("  -- VOLTAGE READ --\r\n");
	CMD_Printf("  -- VOLTAGE READ --\r\n");

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

	if(vddel_conn < pmicVoltageMin[VDDEL] || vddel_conn > pmicVoltageMax[VDDEL])
	{
		LOG_Printf("%d. PMIC TEST NG (DC VDDEL VOLTAGE / %f V)\r\n", ++indexCnt, vddel_conn);
		CMD_Printf("%d. PMIC TEST NG (DC VDDEL VOLTAGE / %f V)\r\n", indexCnt, vddel_conn);
		BoardTest_NgCheck(PMIC);
	}
	else
	{
		LOG_Printf("%d. PMIC TEST OK (DC VDDEL VOLTAGE / %f V)\r\n", ++indexCnt, vddel_conn);
		CMD_Printf("%d. PMIC TEST OK (DC VDDEL VOLTAGE / %f V)\r\n", indexCnt, vddel_conn);
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

static u8 Cmd_MBTestPrevDP(void *pVoid)
{
	u8	result = 0, rtn;
	double	dp_D0P = 0;
	double	dp_D0N = 0;
	double	dp_D1P = 0;
	double	dp_D1N = 0;
	double	dp_D2P = 0;
	double	dp_D2N = 0;
	double	dp_AUXP = 0;
	double	dp_AUXN = 0;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;

	if(ngCnt[I2C])				return	result;
	if(errDetector)				return	result;

	////////////////////////////////////////////////////
	// ADMUX ENABLE
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P4);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P4);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);

	////////////////////////////////////////////////////
	// AMUX_S1 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// DP SIGNAL LEVEL MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dp_AUXP); 
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S2 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// DP SIGNAL LEVEL MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dp_AUXN); 
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S3 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// DP SIGNAL LEVEL MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dp_D0P); 
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S4 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// DP SIGNAL LEVEL MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dp_D0N); 
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S5 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// DP SIGNAL LEVEL MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dp_D1P); 
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S6 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// DP SIGNAL LEVEL MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dp_D1N); 
	if(rtn == 0)				return	result;

	////////////////////////////////////////////////////
	// AMUX_S7 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P2 | P1);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// DP SIGNAL LEVEL MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dp_D2P); 
	if(rtn == 0)				return	result;
 
	////////////////////////////////////////////////////
	// AMUX_S8 Latch
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, P0 | P1 | P2);
	if(rtn == 0)				return	result;
	
	HAL_Delay(20);
	
	////////////////////////////////////////////////////
	// DP SIGNAL LEVEL MEASURE
	////////////////////////////////////////////////////
	rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &dp_D2N); 
	if(rtn == 0)				return	result;

	LOG_Printf("\r\n=========================== DP TEST ===========================\r\n");
	CMD_Printf("\r\n=========================== DP TEST ===========================\r\n");

	if(dp_D0P < dp_D0pMin || dp_D0P > dp_D0pMax)
	{
		LOG_Printf("%d. DP TEST NG (D0P / %f V)\r\n", ++indexCnt, dp_D0P);
		CMD_Printf("%d. DP TEST NG (D0P / %f V)\r\n", indexCnt, dp_D0P);
		BoardTest_NgCheck(DP);
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (D0P / %f V)\r\n", ++indexCnt, dp_D0P);
		CMD_Printf("%d. DP TEST OK (D0P / %f V)\r\n", indexCnt, dp_D0P);
	}

	if(dp_D0N < dp_D0nMin || dp_D0N > dp_D0nMax)
	{
		LOG_Printf("%d. DP TEST NG (D0N / %f V)\r\n", ++indexCnt, dp_D0N);
		CMD_Printf("%d. DP TEST NG (D0N / %f V)\r\n", indexCnt, dp_D0N);
		BoardTest_NgCheck(DP);
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (D0N / %f V)\r\n", ++indexCnt, dp_D0N);
		CMD_Printf("%d. DP TEST OK (D0N / %f V)\r\n", indexCnt, dp_D0N);
	}

	if(dp_D1P < dp_D1pMin || dp_D1P > dp_D1pMax)
	{
		LOG_Printf("%d. DP TEST NG (D1P / %f V)\r\n", ++indexCnt, dp_D1P);
		CMD_Printf("%d. DP TEST NG (D1P / %f V)\r\n", indexCnt, dp_D1P);
		BoardTest_NgCheck(DP);
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (D1P / %f V)\r\n", ++indexCnt, dp_D1P);
		CMD_Printf("%d. DP TEST OK (D1P / %f V)\r\n", indexCnt, dp_D1P);
	}

	if(dp_D1N < dp_D1nMin || dp_D1N > dp_D1nMax)
	{
		LOG_Printf("%d. DP TEST NG (D1N / %f V)\r\n", ++indexCnt, dp_D1N);
		CMD_Printf("%d. DP TEST NG (D1N / %f V)\r\n", indexCnt, dp_D1N);
		ngCnt[3]++;
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (D1N / %f V)\r\n", ++indexCnt, dp_D1N);
		CMD_Printf("%d. DP TEST OK (D1N / %f V)\r\n", indexCnt, dp_D1N);
	}

	if(dp_D2P < dp_D2pMin || dp_D2P > dp_D2pMax)
	{
		LOG_Printf("%d. DP TEST NG (D2P / %f V)\r\n", ++indexCnt, dp_D2P);
		CMD_Printf("%d. DP TEST NG (D2P / %f V)\r\n", indexCnt, dp_D2P);
		ngCnt[3]++;
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (D2P / %f V)\r\n", ++indexCnt, dp_D2P);
		CMD_Printf("%d. DP TEST OK (D2P / %f V)\r\n", indexCnt, dp_D2P);
	}

	if(dp_D2N < dp_D2nMin || dp_D2N > dp_D2nMax)
	{
		LOG_Printf("%d. DP TEST NG (D2N / %f V)\r\n", ++indexCnt, dp_D2N);
		CMD_Printf("%d. DP TEST NG (D2N / %f V)\r\n", indexCnt, dp_D2N);
		ngCnt[3]++;
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (D2N / %f V)\r\n", ++indexCnt, dp_D2N);
		CMD_Printf("%d. DP TEST OK (D2N / %f V)\r\n", indexCnt, dp_D2N);
	}

	if(dp_AUXP < dp_AuxpMin || dp_AUXP > dp_AuxpMax)
	{
		LOG_Printf("%d. DP TEST NG (AUXP / %f V)\r\n", ++indexCnt, dp_AUXP);
		CMD_Printf("%d. DP TEST NG (AUXP / %f V)\r\n", indexCnt, dp_AUXP);
		ngCnt[3]++;
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (AUXP / %f V)\r\n", ++indexCnt, dp_AUXP);
		CMD_Printf("%d. DP TEST OK (AUXP / %f V)\r\n", indexCnt, dp_AUXP);
	}

	if(dp_AUXN < dp_AuxnMin || dp_AUXN > dp_AuxnMax)
	{
		LOG_Printf("%d. DP TEST NG (AUXN / %f V)\r\n", ++indexCnt, dp_AUXN);
		CMD_Printf("%d. DP TEST NG (AUXN / %f V)\r\n", indexCnt, dp_AUXN);
		ngCnt[3]++;
	}
	else
	{
		LOG_Printf("%d. DP TEST OK (AUXN / %f V)\r\n", ++indexCnt, dp_AUXN);
		CMD_Printf("%d. DP TEST OK (AUXN / %f V)\r\n", indexCnt, dp_AUXN);
	}

	testItem--;

	result = 1;

	return	result;
}


static u8 Cmd_MBTestPrevOSC(void *pVoid)
{
	u8	result = 0, rtn;
	u32	ReadData;
	double	osc_clock;
	
	cmdFunctionData_t	*pData;
		
	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;
	
	if(ngCnt[I2C])				return	result;
	if(errDetector) 			return	result;

	////////////////////////////////////////////////////
	// CORE FPGA CLOCK COUNTING ENABLE
	////////////////////////////////////////////////////
	rtn = DPC_DirectRegWriteSingle(pData->position, CORE_REG_ADDR_EX_CLK_CNT_START, 0x001E0001);
	if(rtn == 0)				return	result;
	
	HAL_Delay(1000);
	
	rtn = DPC_DirectRegReadSingle(pData->position, CORE_REG_ADDR_EX_CLK_CNT_VALUE0, &ReadData);
	if(rtn == 0)				return	result;
	
	osc_clock = (double)ReadData;

	osc_clock /= 1;
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

static u8 Cmd_MBTestPrevPortDirInit(void *pVoid)		
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return	result;

	////////////////////////////////////////////////////
	// IOEXPANDER OUTPUT PORT ALL LOW
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, 0xFF);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, 0xFF);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, 0xFF);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, 0xFF);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_2, PORTB, P10);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, 0xFF);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTB, 0xFF);
	if(rtn == 0)				return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTA, 0xFF);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTB, 0xFF); 
	if(rtn == 0)				return	result;
	
	////////////////////////////////////////////////////
	// IOEXPANDER PORT DIRECTION INIT
	////////////////////////////////////////////////////
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, PORT_OUTPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, PORT_OUTPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTA, PORT_OUTPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_2, PORTB, PORT_OUTPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_IN_EXPANDER_1, PORTA, PORT_INPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_IN_EXPANDER_1, PORTB, PORT_INPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_IN_EXPANDER_2, PORTA, PORT_INPUT);
	if(rtn == 0)				return	result;
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_IN_EXPANDER_2, PORTB, P10);
	if(rtn == 0)				return	result;

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
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C1, MB_EXPANDER_2, PORTB, P12);
	if(rtn == 0)				return	result;

	result = 1;

	return	result;
}
		
static u8 Cmd_MBTestPrevFinalResult(void *pVoid)
{
	u8	result = 0, sum = 0, rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;
	
	if(pData->argc != 0)		return	result;

	sprintf((char*)pModel, "NONE");

	for(u8 x = 0; x < 8; x++)
	{
		sum += ngCnt[x];
	}

	////////////////////////////////////////////////////
	// TEST LED INIT
	////////////////////////////////////////////////////
	rtn =IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTA, 0xff);
	if(rtn == 0)				return	result;
	
	rtn =IoExp_PortClear(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, PORTB, 0xff);
	if(rtn == 0)				return	result;
	
	////////////////////////////////////////////////////
	// LED FINAL STATUS
	////////////////////////////////////////////////////
	if(sum == 0 && testItem == 0)
	{
		rtn = IoExp_LED_TOTAL_OK(pVoid, BSP_I2C2);
		if(rtn == 0)				return	result;
	}
	else
	{
		rtn = IoExp_LED_TOTAL_NG(pVoid, BSP_I2C2);
		if(rtn == 0)				return	result;
		
		if(ngCnt[I2C])
		{
			rtn = IoExp_LED_I2C_NG(pVoid, BSP_I2C2);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[OPENSHORT])
		{
			rtn = IoExp_LED_IO_NG(pVoid, BSP_I2C2);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[POWER])
		{
			rtn = IoExp_LED_POWER_NG(pVoid, BSP_I2C2);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[DP])
		{
			rtn = IoExp_LED_DP_NG(pVoid, BSP_I2C2);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[OSC])
		{
			rtn = IoExp_LED_OSC_NG(pVoid, BSP_I2C2);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[PMIC])
		{
			rtn = IoExp_LED_PMIC_NG(pVoid, BSP_I2C2);
			if(rtn == 0)				return	result;
		}
		if(ngCnt[VID])
		{
			rtn = IoExp_LED_VENTURA_NG(pVoid, BSP_I2C2);
			if(rtn == 0)				return	result;
		}
	}

	memset(ngNum, 0, sizeof(ngNum));
	memset(ngCnt, 0, sizeof(ngCnt));
	memset(&indexCnt, 0, sizeof(indexCnt));
	memset(&errDetector, 0, sizeof(errDetector));
	memset(&testItem, 0xff, sizeof(testItem));

	LOG_Printf("\r\n========================== TEST END ==========================\r\n");
	CMD_Printf("\r\n========================== TEST END ==========================\r\n");

	if(sum == 0)
	{
		LOG_Printf("TOTAL RESULT : OK\r\n");
		CMD_Printf("TOTAL RESULT : OK\r\n");
	}
	else
	{
		LOG_Printf("TOTAL RESULT : NG\r\n");
		CMD_Printf("TOTAL RESULT : NG\r\n");
	}

	LOG_Printf("\r\n");
	CMD_Printf("\r\n");

	LOG_Printf("##################################################################################\r\n");
	LOG_Printf("##################################################################################\r\n");

	result = 1;

	return	result;
}	

static u8 Cmd_MBTestPrevModel(void *pVoid)
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
		ngCnt[0]++;
	
		return	result;
	}

	result = 1;

	return 	result;		
}

static u8 Cmd_MBTestPrevVersion(void *pVoid)
{
	u8	result = 0;
	
	CMD_Printf("\r\n");
	CMD_Printf("MBTEST PREV MODEL : D854\r\n");
	CMD_Printf("MBTEST PREV DEVICE : E7602_R2\r\n");
	CMD_Printf("MBTEST PREV VERSION : v0.0.9\r\n");	
	CMD_Printf("MBTEST PREV DATE : 2022.04.29\r\n");	

	result = 1;

	return	result;
}	

/******************************************************************************
* Revision History
*
* v0.0.8	Add Openshort Test OUT_XSYNC 
* v0.0.9	Power MeasureVIAuto -> Power MeasureVI 
            I2C ERROR in I2c Test => Every Test skip
              =>first, I2C NG Flag set, when I2c test OK, remove I2C NG Flag
            Arguments count Check
            Add PMIC voltage adjustment
              => before : Voltage check --> Current check
                 after : Current Check --> Voltage Check --> Adj Voltage Check
            Add DP Signal Voltage check     
******************************************************************************/



const	commandList_t	cmdList_MBtest_prev[] =	{
  //												{Command,				Function,				   		Option,		Mode, 		RootMessage				Note}
  												{	"version",				Cmd_MBTestPrevVersion,			NULL,		NULL,		"",						NULL},
  												{	"model",				Cmd_MBTestPrevModel,			NULL,		NULL,		"",						NULL},
  												{	"test.item",			Cmd_MBTestTestItem,				NULL,		NULL,		"",						NULL},
  												{	"voltage.range",		Cmd_MBTestVoltageLimit,			NULL,		NULL,		"",						NULL},
  												{	"current.range",		Cmd_MBTestCurrentLimit,			NULL,		NULL,		"",						NULL},
  												{	"i2c",					Cmd_MBTestPrevI2c,				NULL,		NULL,		"",						NULL},
  												{	"io",					Cmd_MBTestPrevOpenShort,		NULL,		NULL,		"",						NULL},
  												{	"pmic",					Cmd_MBTestPrevPMIC,				NULL,		NULL,		"",						NULL},
  												{	"power",				Cmd_MBTestPrevPower,			NULL,		NULL,		"",						NULL},
  												{	"osc",					Cmd_MBTestPrevOSC,				NULL,		NULL,		"",						NULL},
  												{	"serial",				Cmd_MBTestPrevSerialChk,		0x02,		NULL,		"",						NULL},
  												{	"dp",					Cmd_MBTestPrevDP,				NULL,		NULL,		"",						NULL},
  												{	"port.init",			Cmd_MBTestPrevPortDirInit,		NULL,		NULL,		"",						NULL},
  												{	"final.result",			Cmd_MBTestPrevFinalResult,		NULL,		NULL,		"",						NULL},
  												{	NULL,					NULL,							NULL,		NULL,		NULL,					NULL},
											};
