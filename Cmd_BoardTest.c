#include	"Cmd_BoardTest.h"

bitCtrl32_t	ngNum[3];


void BoardTest_NgCheck(u8	ngName)
{
	u8	temp;

	switch(ngName)
	{
		case I2C:
		case OPENSHORT:	
		case VID:
		case DP:
		case POWER:
		case PMIC:
		case OSC:
			ngCnt[ngName]++;
	}

	temp = indexCnt - 1;

	if(temp < BIT32_FULL_CNT)	
	{
		ngNum[0].u32Data |= 1 << temp;
	}
	else if(temp < (BIT32_FULL_CNT * 2))
	{
		temp -= BIT32_FULL_CNT;
		
		if(temp < BIT32_FULL_CNT)	ngNum[1].u32Data |= 1 << temp;
	}	
	else if(temp < (BIT32_FULL_CNT * 3))
	{
		temp -= BIT32_FULL_CNT * 2;
		
		if(temp < BIT32_FULL_CNT)	ngNum[2].u32Data |= 1 << temp;	
	}	
}

static u8 Cmd_BoardTestItem(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	testItem = CMD_StrToUL(pData->pArgv[0]);

	result  = 1;

	return	result;
}

static u8 BoardTest_Uart1(u8 position, u8 line, u8	state)
{
	u8	result = 0, rtn;
	u32	cmd;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	if(line == UART_RX)			cmd = SB_CMD_PWR_M_UART1_RX;
	else						cmd = SB_CMD_PWR_M_UART1_TX;
	
	sbParsingData.channel						= position;
	sbParsingData.cmd							= cmd;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	#if 0
	sbParsingData.channel						= 3;
	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;
	
	if(sbParsingData.rxSize != 0)				return result;
	#endif
	
	result = 1;

	return result;
}

static u8 Cmd_BoardTestCurrentLimit(void *pVoid)
{
	u8 result = 0, vm, range;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pData->pArgv[0], "vm"))
	{
		vm = CMD_StrToUL(pData->pArgv[1]);
		range = CMD_StrToUL(pData->pArgv[2]);

		if(vm > 6)			return	result;
		if(range > 3)		return	result;

		switch(vm)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				switch(range)
				{
					case 0:
					case 1:
					case 2:
					case 3:
						vmCurrentMin[vm][range] = CMD_AToF(pData->pArgv[3]);
						vmCurrentMax[vm][range] = CMD_AToF(pData->pArgv[4]);
						break;
				}
				break;
		}
	}

	result  = 1;

	return	result;
}

static u8 Cmd_BoardTestVoltageLimit(void *pVoid)
{
	u8	result = 0, vm;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pData->pArgv[0], "vm"))
	{
		vm = CMD_StrToUL(pData->pArgv[1]);

		if(vm > 6)			return	result;

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
	else if(CMD_Compare(pData->pArgv[0], "vs"))
	{
		if(CMD_Compare(pData->pArgv[1], "p"))
		{
			vspVoltageMin = CMD_AToF(pData->pArgv[2]);
			vspVoltageMax = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "n"))
		{
			vsnVoltageMin = CMD_AToF(pData->pArgv[2]);
			vsnVoltageMax = CMD_AToF(pData->pArgv[3]);
		}
	}

	result  = 1;

	return	result;
}

static u8 Cmd_BoardTestI2c(void *pVoid)
{
	u8	result = 0, channel, rCnt, rtn;
	u8	eeprom_adrs = 0;
	u8	ioexp_adrs[2] = {0, };
	u8	*pRxBuffer;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	CMD_Printf("\r\n======================== I2C DEVICE CHECK ========================\r\n");

	for(u8 cnt = 0; cnt < 2; cnt++)
	{		
		channel = BSP_I2C1 + cnt;
	
		rtn = LCM_I2CGetSlaveAdrs(pData->position, channel, pRxBuffer, &rCnt);
		if(rtn == 0)	return	result;

		if(channel == BSP_I2C1)
		{
			for(u8 i = 0; i < rCnt; i++)
			{
				if(pRxBuffer[i] == 0x40)		ioexp_adrs[0] = pRxBuffer[i];
				else if(pRxBuffer[i] == 0x42)	ioexp_adrs[1] = pRxBuffer[i];
			}
		}
		else if(channel == BSP_I2C2)
		{
			for(u8 i = 0; i < rCnt; i++)
			{
				if(pRxBuffer[i] == 0xA2)		eeprom_adrs = pRxBuffer[i];
			}
		}
		else	return	result;
	}
	
	if(ioexp_adrs[0] == 0x40)
	{
		CMD_Printf("%d. I2C TEST OK (IO EXPANDER ADDRESS 1 OK / 0x%02x)\r\n", ++indexCnt, ioexp_adrs[0]);
	}
	else
	{
		CMD_Printf("%d. I2C TEST NG (IO EXPANDER ADDRESS 1 NG / 0x%02x)\r\n", ++indexCnt, ioexp_adrs[0]);
		BoardTest_NgCheck(I2C);
	}

	if(ioexp_adrs[1] == 0x42)
	{
		CMD_Printf("%d. I2C TEST OK (IO EXPANDER ADDRESS 2 OK / 0x%02x)\r\n", ++indexCnt, ioexp_adrs[1]);
	}
	else
	{
		CMD_Printf("%d. I2C TEST NG (IO EXPANDER ADDRESS 2 NG / 0x%02x)\r\n", ++indexCnt, ioexp_adrs[1]);
		BoardTest_NgCheck(I2C);
	}

	if(eeprom_adrs == 0xA2)
	{
		CMD_Printf("%d. I2C TEST OK (EEPROM ADDRESS OK / 0x%02x)\r\n", ++indexCnt, eeprom_adrs);
	}
	else
	{
		CMD_Printf("%d. I2C TEST NG (EEPROM ADDRESS NG / 0x%02x)\r\n", ++indexCnt, eeprom_adrs);
		BoardTest_NgCheck(I2C);
	}

	testItem--;
	
	result = 1;

	return	result;
}

static u8 Cmd_BoardTestOpenShort(void *pVoid)
{
	u8	result = 0, writeData = 0;
	u8	temp[2], calc, rtn, mask;
	bitCtrl8_t	gpio[8][2];
	bitCtrl8_t	spi[4][2];
	bitCtrl8_t	i2c_attn[2][2];
	bitCtrl8_t	xsync[2];
	bitCtrl8_t	te[2];
	bitCtrl8_t	uart[2][2];

	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(ngCnt[I2C])			return	result;

	CMD_Printf("\r\n======================== OPEN SHORT TEST ========================\r\n");

	////////////////////////////////////////////////////
	// BT EXPANDER DIRECTION SETTING
	////////////////////////////////////////////////////	
	rtn = IoExp_PortDir(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, PORT_INPUT);
	if(rtn == 0)	return	result;

	rtn = IoExp_PortDir(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTB, PORT_OUTPUT);
	if(rtn == 0)	return	result;

	rtn = IoExp_PortDir(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, PORT_INPUT);
	if(rtn == 0)	return	result;

	rtn = IoExp_PortDir(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTB, PORT_OUTPUT);
	if(rtn == 0)	return	result;

	for(u8 cnt = 0; cnt < GPIO_NUM_MAX; cnt++)
	{	
		writeData = 0;

		rtn = LCM_GpioWrite(pData->position, writeData |= 1 << cnt); 
		if(rtn == 0)	return	result;

		rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, &gpio[cnt][HIGH].u8Data);
		if(rtn == 0)	return	result;

		rtn = LCM_GpioClear(pData->position, writeData);
		if(rtn == 0)	return	result;
		
		rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, &gpio[cnt][LOW].u8Data);
		if(rtn == 0)	return	result;

		temp[HIGH] = gpio[cnt][HIGH].u8Data;
		temp[LOW] = gpio[cnt][LOW].u8Data;

		calc = temp[HIGH] - temp[LOW];

		if(calc != writeData)
		{
			CMD_Printf("%d. OPEN SHORT TEST NG (GPIO %d)\r\n", ++indexCnt, cnt);
			BoardTest_NgCheck(OPENSHORT);
		}	
		else
		{
			CMD_Printf("%d. OPEN SHORT TEST OK (GPIO %d)\r\n", ++indexCnt, cnt);
		}
	}

	////////////////////////////////////////////////////
	// SPI LINE CHECK
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 4; cnt++)
	{
		writeData = 0;
		
		if(cnt == MISO)
		{
			rtn = IoExp_PortWrite(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTB, writeData |= 1 << 0);
			if(rtn == 0)	return	result;

			rtn = LCM_SpiIoRead(pData->position, &spi[cnt][HIGH].u8Data);
			if(rtn == 0)	return	result;

			rtn = IoExp_PortWrite(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTB, 0x00);
			if(rtn == 0)	return	result;

			rtn = LCM_SpiIoRead(pData->position, &spi[cnt][LOW].u8Data);
			if(rtn == 0)	return	result;
		}
		else
		{
			rtn = LCM_SpiIoWrite(pData->position, 0x00);
			if(rtn == 0)	return	result;
			
			rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, &spi[cnt][LOW].u8Data);
			if(rtn == 0)	return	result;
			
			rtn = LCM_SpiIoWrite(pData->position, writeData |= 1 << cnt);
			if(rtn == 0)	return	result;
			
			rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, &spi[cnt][HIGH].u8Data);
			if(rtn == 0)	return	result;
		}

		if(cnt == MISO)			mask = 0x01;
		else					mask = 0x07; 
		
		temp[HIGH] = spi[cnt][HIGH].u8Data & mask;
		temp[LOW] = spi[cnt][LOW].u8Data & mask;

		calc = temp[HIGH] - temp[LOW];

		if(cnt == CS)			calc = calc >> 2;
		else if(cnt == MOSI)	calc = calc << 2;

		if(calc != writeData)
		{
			switch(cnt)
			{
				case 0:
					CMD_Printf("%d. OPEN SHORT TEST NG (SPI CS)\r\n", ++indexCnt);
					break;

				case 1:
					CMD_Printf("%d. OPEN SHORT TEST NG (SPI SCK)\r\n", ++indexCnt);
					break;

				case 2:
					CMD_Printf("%d. OPEN SHORT TEST NG (SPI MOSI)\r\n", ++indexCnt);
					break;

				case 3:
					CMD_Printf("%d. OPEN SHORT TEST NG (SPI MISO)\r\n", ++indexCnt);
					break;
					
			}
			BoardTest_NgCheck(OPENSHORT);
			
		}	
		else
		{
			switch(cnt)
			{
				case 0:
					CMD_Printf("%d. OPEN SHORT TEST OK (SPI CS)\r\n", ++indexCnt);
					break;

				case 1:
					CMD_Printf("%d. OPEN SHORT TEST OK (SPI SCK)\r\n", ++indexCnt);
					break;

				case 2:
					CMD_Printf("%d. OPEN SHORT TEST OK (SPI MOSI)\r\n", ++indexCnt);
					break;

				case 3:
					CMD_Printf("%d. OPEN SHORT TEST OK (SPI MISO)\r\n", ++indexCnt);
					break;
			}
		}
	}
	////////////////////////////////////////////////////
	// I2C0 ATTN / I2C1 ATTN CHECK  
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 2; cnt++)
	{
		writeData = 0;

		rtn = LCM_I2CInit(pData->position, BSP_I2C1);
		if(rtn == 0)	return	result;

		rtn = IoExp_PortWrite(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTB, writeData |= 1 << (4 + cnt));
		if(rtn == 0)	return	result;

		rtn = LCM_I2CDeInit(pData->position, BSP_I2C1 + cnt);
		if(rtn == 0)	return	result;

		rtn = LCM_I2CIoRead(pData->position, BSP_I2C1 + cnt, &i2c_attn[cnt][HIGH].u8Data);
		if(rtn == 0)	return	result;

		rtn = LCM_I2CInit(pData->position, BSP_I2C1);
		if(rtn == 0)	return	result;
		
		rtn = IoExp_PortWrite(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTB, 0x00);
		if(rtn == 0)	return	result;

		rtn = LCM_I2CDeInit(pData->position, BSP_I2C1 + cnt);
		if(rtn == 0)	return	result;

		rtn = LCM_I2CIoRead(pData->position, BSP_I2C1 + cnt, &i2c_attn[cnt][LOW].u8Data);
		if(rtn == 0)	return	result;

		mask = 0xff;
	
		temp[HIGH] = i2c_attn[cnt][HIGH].u8Data & mask;
		temp[LOW] = i2c_attn[cnt][LOW].u8Data & mask;

		calc = temp[HIGH] - temp[LOW];

		if(calc != 0x07)
		{
			switch(cnt)
			{
				case BSP_I2C1:
					CMD_Printf("%d. OPEN SHORT TEST NG (I2C%d ATTN)\r\n", ++indexCnt, cnt);
					break;

				case BSP_I2C2:
					CMD_Printf("%d. OPEN SHORT TEST NG (I2C%d ATTN)\r\n", ++indexCnt, cnt);
					break;
			}
			BoardTest_NgCheck(OPENSHORT);
		}	
		else
		{
			switch(cnt)
			{
				case BSP_I2C1:
					CMD_Printf("%d. OPEN SHORT TEST OK (I2C%d ATTN)\r\n", ++indexCnt, cnt);
					break;

				case BSP_I2C2:
					CMD_Printf("%d. OPEN SHORT TEST OK (I2C%d ATTN)\r\n", ++indexCnt, cnt);
					break;
			}
		}
	}

	////////////////////////////////////////////////////
	// UART CHECK
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 2; cnt++)
	{
		rtn = BoardTest_Uart1(pData->position, UART_RX + cnt, HIGH);
		if(rtn == 0)	return result;

		rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, &uart[cnt][HIGH].u8Data);
		if(rtn == 0)	return	result;
		
		rtn = BoardTest_Uart1(pData->position, UART_RX + cnt, LOW);
		if(rtn == 0)	return result;
		
		rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, &uart[cnt][LOW].u8Data);
		if(rtn == 0)	return	result;

		mask = 0x18;
	
		temp[HIGH] = uart[cnt][HIGH].u8Data & mask;
		temp[LOW] = uart[cnt][LOW].u8Data & mask;

		calc = temp[HIGH] - temp[LOW];

		switch(cnt)
		{
			case UART_RX:
				if(calc != 0x08)
				{
					CMD_Printf("%d. OPEN SHORT TEST NG (UART RX)\r\n", ++indexCnt);
					BoardTest_NgCheck(OPENSHORT);
				}
				else
				{
					CMD_Printf("%d. OPEN SHORT TEST OK (UART RX)\r\n", ++indexCnt);
				}
				break;
				
			case UART_TX:
				if(calc != 0x10)
				{
					CMD_Printf("%d. OPEN SHORT TEST NG (UART TX)\r\n", ++indexCnt);
					BoardTest_NgCheck(OPENSHORT);
				}
				else
				{
					CMD_Printf("%d. OPEN SHORT TEST OK (UART TX)\r\n", ++indexCnt);
				}
				break;
		}
	}

	////////////////////////////////////////////////////
	// XSYNC CHECK
	////////////////////////////////////////////////////
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x11);
	if(rtn == 0)		return	result;

	rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, &xsync[HIGH].u8Data);
	if(rtn == 0)		return	result;
	
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x10);
	if(rtn == 0)		return 	result;
	
	rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, &xsync[LOW].u8Data);
	if(rtn == 0)		return	result;

	mask = 0x20;
	
	temp[HIGH] = xsync[HIGH].u8Data & mask;
	temp[LOW] = xsync[LOW].u8Data & mask;

	calc = temp[HIGH] - temp[LOW];

	if(calc != 0x20)
	{
		CMD_Printf("%d. OPEN SHORT TEST NG (XSYNC)\r\n", ++indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		CMD_Printf("%d. OPEN SHORT TEST OK (XSYNC)\r\n", ++indexCnt);
	}

	////////////////////////////////////////////////////
	// TE CHECK
	////////////////////////////////////////////////////
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x12);
	if(rtn == 0)		return result;

	rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, &te[HIGH].u8Data);
	if(rtn == 0)		return	result;
	
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x10);
	if(rtn == 0)		return result;
	
	rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_2, PORTA, &te[LOW].u8Data);
	if(rtn == 0)		return	result;

	mask = 0x40;
	
	temp[HIGH] = te[HIGH].u8Data & mask;
	temp[LOW] = te[LOW].u8Data & mask;

	calc = temp[HIGH] - temp[LOW];

	if(calc != 0x40)
	{
		CMD_Printf("%d. OPEN SHORT TEST NG (TE)\r\n", ++indexCnt);
		BoardTest_NgCheck(OPENSHORT);
	}
	else
	{
		CMD_Printf("%d. OPEN SHORT TEST OK (TE)\r\n", ++indexCnt);
	}

	testItem--;

	result = 1;

	return	result;
}	

static u8 Cmd_BoardTestPower(void *pVoid)
{
	u8	result = 0, rtn;
	u8	pwrSrc, IoExp_RangeSel;
	double	ReadCurrent;
	double	ReadVoltage;
	
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(ngCnt[I2C])			return	result;
	
	CMD_Printf("\r\n========================== POWER TEST ==========================\r\n");

	for(u8 cnt = 0; cnt < PWR_MEASURE_MODE_END; cnt++)
	{		
		rtn = PWR_ExtVoltageMeasure(pData->position, cnt, &ReadVoltage);
		if(rtn == 0)	return	result;

		if(cnt == PWR_MEASURE_MODE_VOLTAGE)
		{
			ReadVoltage *= 4;

			if(ReadVoltage < vspVoltageMin || ReadVoltage > vspVoltageMax)
			{
				CMD_Printf("%d. POWER TEST NG (VSP / %f V)\r\n", ++indexCnt, ReadVoltage);
				BoardTest_NgCheck(POWER);
			}
			else
			{
				CMD_Printf("%d. POWER TEST OK (VSP / %f V)\r\n", ++indexCnt, ReadVoltage);
			}
		}
		else
		{
			ReadVoltage *= -4;

			if(ReadVoltage < vsnVoltageMin || ReadVoltage > vsnVoltageMax)
			{
				CMD_Printf("%d. POWER TEST NG (VSN / %f V)\r\n", ++indexCnt, ReadVoltage);
				BoardTest_NgCheck(POWER);
			}
			else
			{
				CMD_Printf("%d. POWER TEST OK (VSN / %f V)\r\n", ++indexCnt, ReadVoltage);
			}
		}
	}

	rtn = IoExp_PortWrite(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, 0x00);
	if(rtn == 0)	return	result;

	rtn = IoExp_PortWrite(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTB, 0x00);
	if(rtn == 0)	return	result;

	rtn = IoExp_PortDir(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, PORT_OUTPUT);
	if(rtn == 0)	return	result;

	rtn = IoExp_PortDir(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTB, PORT_OUTPUT);
	if(rtn == 0)	return	result;

	for(s8 range = 3; range >= 0; range--)
	{
		CMD_Printf("\r\n");
		CMD_Printf("  -- POWER RANGE %d --\r\n", range);

		IoExp_RangeSel = 0;

		IoExp_RangeSel |= 0x01 << range;
		
		rtn = IoExp_PortWrite(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTB, IoExp_RangeSel);
		if(rtn == 0)	return	result;

		for(u8 vm = 0; vm < 7; vm++)
		{
			pwrSrc = PWR_SRC_VM_0 + vm;
			
			rtn = PWR_OutputEnable(pData->position, pwrSrc, DISABLE);	
			if(rtn == 0)	return	result;
			
			HAL_Delay(20);
			
			rtn = PWR_MeasureRange(pData->position, pwrSrc, range);
			if(rtn == 0)	return	result;
			
			HAL_Delay(20);
			
			rtn = PWR_OutputEnable(pData->position, pwrSrc, ENABLE);	
			if(rtn == 0)	return	result;
			
			HAL_Delay(400);
			
			rtn = PWR_MesureVI(pData->position, pwrSrc, &ReadVoltage, &ReadCurrent);
			if(rtn == 0)	return	result;
			
			HAL_Delay(20);

			rtn = PWR_OutputEnable(pData->position, pwrSrc, DISABLE);	
			if(rtn == 0)	return	result;

			if(ReadVoltage < vmVoltageMin[vm] || ReadVoltage > vmVoltageMax[vm] || 
			ReadCurrent < vmCurrentMin[vm][range] || ReadCurrent > vmCurrentMax[vm][range])
			{
				CMD_Printf("%d. POWER TEST NG (VM%d / %f V  %f mA)\r\n", ++indexCnt, vm, ReadVoltage, ReadCurrent);
				BoardTest_NgCheck(POWER);
			}
			else
			{
				CMD_Printf("%d. POWER TEST OK (VM%d / %f V  %f mA)\r\n", ++indexCnt, vm, ReadVoltage, ReadCurrent);
			}
		}
	}

	testItem--;

	result = 1;

	return	result;
}		
		
static u8 Cmd_BoardTestFinalResult(void *pVoid)
{
	u8	result = 0, sum = 0, errIdx = 0, chk = 0;
	u32 temp;
	
	for(u8 i = 0; i < 8; i++)
	{
		sum += ngCnt[i];
	}

	CMD_Printf("\r\n========================== TEST END ==========================\r\n");

	if(sum == 0 && testItem == 0)
	{
		CMD_Printf("TOTAL RESULT : OK\r\n");
	}
	else
	{
		CMD_Printf("TOTAL RESULT : NG\r\n");
		CMD_Printf("\r\n");
		CMD_Printf("NG INDEX : ");

		for(u8 i = 0; i < 2; i++)
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
				else		return	result;

				if(chk)		CMD_Printf("%d ", errIdx);
				
				chk = 0;
			}
		}
	}

	CMD_Printf("\r\n");

	memset(ngCnt, 0, sizeof(ngCnt));
	memset(ngNum, 0, sizeof(ngNum));
	memset(&indexCnt, 0, sizeof(indexCnt));
	memset(&testItem, 0xff, sizeof(testItem));

	result = 1;

	return	result;
}		

static u8 Cmd_BoardTestVersion(void *pVoid)
{
	u8	result = 0;
	
	CMD_Printf("\r\n");
	CMD_Printf("BOARD TEST : E7602_MC2CH\r\n");
	CMD_Printf("BOARD TEST VERSION : v1.0.1\r\n");	
	CMD_Printf("BOARD TEST DATE : 2022.02.10\r\n");	

	result = 1;

	return	result;
}	

/******************************************************************************
* Revision History
*
* v1.0.0 : Voltage, Current Limit Command function modify
* v1.0.1 : when Function return error, total result NG print out
           when measure power, increase delay 20ms -> 400ms
******************************************************************************/	
const	commandList_t	cmdList_BoardTest[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"version",				Cmd_BoardTestVersion,			0x01,		NULL,		"",						NULL},
												{	"test.item",			Cmd_BoardTestItem,				0x01,		NULL,		"",						NULL},
												{	"voltage.limit",		Cmd_BoardTestVoltageLimit,		0x01,		NULL,		"",						NULL},
												{	"current.limit",		Cmd_BoardTestCurrentLimit,		0x01,		NULL,		"",						NULL},
												{	"i2c",					Cmd_BoardTestI2c,				0x01,		NULL,		"",						NULL},
												{	"io",					Cmd_BoardTestOpenShort,			0x01,		NULL,		"",						NULL},
												{	"power",				Cmd_BoardTestPower,				0x01,		NULL,		"",						NULL},
												{	"final.result",			Cmd_BoardTestFinalResult,		0x01,		NULL,		"",						NULL},
												{	NULL,					NULL,							0x01,		NULL,		NULL,					NULL},
											};
