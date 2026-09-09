#include	"Cmd_EtTest.h"

static	u8	mode = 0;


static u8 Cmd_EtTestItem(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	testItem = CMD_StrToUL(pData->pArgv[0]);

	result  = 1;

	return	result;
}

static u8 Cmd_EtTestMode(void *pVoid)
{
	u8 result = 0;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pData->pArgv[0], "oc"))
	{
		if(CMD_Compare(pData->pArgv[1], "te"))
		{
			if(CMD_Compare(pData->pArgv[2], "on"))
			{
				mode = OC;
			}
			else if(CMD_Compare(pData->pArgv[2], "off"))
			{
				mode = OC_TE_SKIP;
			}
		}
		else	return result;
	}
	else if(CMD_Compare(pData->pArgv[0], "ft"))
	{
		mode = FT;
	}
	else	return	result;

	result = 1;

	return	result;
}

static u8 Cmd_EtTestClockLimit(void *pVoid)
{
	u8 result = 0;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pData->pArgv[0], "osc"))
	{
		oscMin = CMD_AToF(pData->pArgv[1]);
		oscMax = CMD_AToF(pData->pArgv[2]);
	}

	result = 1;

	return	result;
}

static u8 Cmd_EtTestCurrentLimit(void *pVoid)
{
	u8 	result = 0;
	u8 	vm;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

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
		if(CMD_Compare(pData->pArgv[1], "vssel"))
		{
			pmicCurrentMin[VSSEL] = CMD_AToF(pData->pArgv[2]);
			pmicCurrentMax[VSSEL] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vgl"))
		{
			pmicCurrentMin[VGL] = CMD_AToF(pData->pArgv[2]);
			pmicCurrentMax[VGL] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "avddh"))
		{
			pmicCurrentMin[AVDDH] = CMD_AToF(pData->pArgv[2]);
			pmicCurrentMax[AVDDH] = CMD_AToF(pData->pArgv[3]);
		}
		else	return	result;
	}

	result = 1;

	return	result;
}

static u8 Cmd_EtTestVoltageLimit(void *pVoid)
{
	u8 	result = 0;
	u8 	vm;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

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
		if(CMD_Compare(pData->pArgv[1], "vssel"))
		{
			pmicVoltageMin[VSSEL] = CMD_AToF(pData->pArgv[2]);
			pmicVoltageMax[VSSEL] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vgl"))
		{
			pmicVoltageMin[VGL] = CMD_AToF(pData->pArgv[2]);
			pmicVoltageMax[VGL] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "avddh"))
		{
			pmicVoltageMin[AVDDH] = CMD_AToF(pData->pArgv[2]);
			pmicVoltageMax[AVDDH] = CMD_AToF(pData->pArgv[3]);
		}
		else if(CMD_Compare(pData->pArgv[1], "vddel"))
		{
			pmicVoltageMin[VDDEL] = CMD_AToF(pData->pArgv[2]);
			pmicVoltageMax[VDDEL] = CMD_AToF(pData->pArgv[3]);
		}
		else	return	result;
	}

	result = 1;

	return	result;
}

static u8 Cmd_EtTestI2c(void *pVoid)
{
	u8	result = 0, rCnt, rtn;
	u8	ioexpAdrs = 0;
	u8	tmpsensAdrs = 0;
	u8	*pRxBuffer;
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	pRxBuffer = (u8*)MEMORY_MAP_SDRAM_CMD_TEMP_BUFFER_1_ADRS;

	CMD_Printf("\r\n======================== I2C DEVICE CHECK ========================\r\n");

	rtn = LCM_I2CGetSlaveAdrs(pData->position, BSP_I2C1, pRxBuffer, &rCnt);
	if(rtn == 0)	return	result;
		
	for(u8 i = 0; i < rCnt; i++)
	{
		if(pRxBuffer[i] == 0x40)		ioexpAdrs = pRxBuffer[i];
		else if(pRxBuffer[i] == 0xB6)	tmpsensAdrs = pRxBuffer[i];
	}
		
	if(ioexpAdrs)
	{
		CMD_Printf("%d. I2C TEST OK (IO EXPANDER ADDRESS OK / 0x%02x)\r\n", ++indexCnt, ioexpAdrs);
	}
	else
	{
		CMD_Printf("%d. I2C TEST NG (IO EXPANDER ADDRESS NG / 0x%02x)\r\n", ++indexCnt, ioexpAdrs);
		BoardTest_NgCheck(I2C);
	}

	if(mode != FT)
	{
		if(tmpsensAdrs)
		{
			CMD_Printf("%d. I2C TEST OK (TEMP SENSOR OK / 0x%02x)\r\n", ++indexCnt, tmpsensAdrs);
		}
		else
		{
			CMD_Printf("%d. I2C TEST NG (TEMP SENSOR NG / 0x%02x)\r\n", ++indexCnt, tmpsensAdrs);
			BoardTest_NgCheck(I2C);
		}
	}

	testItem--;
	
	result = 1;

	return	result;
}

static u8 Cmd_EtTestOpenShort(void *pVoid)
{
	u8	result = 0;
	u8	rtn, cnt_end, calc;
	u8	wData[2], sum;
	bitCtrl8_t	ReadData[2];
	
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(ngCnt[0])			return	result;

	CMD_Printf("\r\n======================== OPEN SHORT TEST ========================\r\n");

	if(mode == FT || mode == OC_TE_SKIP)
		cnt_end = 5;
	else	
		cnt_end = 6;
	
	for(u8 cnt = 0; cnt < cnt_end; cnt++)
	{
		if(cnt == 0)
		{	
			wData[HIGH] = 0x06; //nReset HIGH
			wData[LOW] = 0x07; //nReset LOW
		}
		else if(cnt == 1)
		{
			wData[HIGH] = 0x05; //PANICB HIGH	
			wData[LOW] = 0x07; //PANICB LOW
		}
		else if(cnt == 2)
		{
			wData[HIGH] = 0x47; //BSYNC_GPIO3 HIGH
			wData[LOW] = 0x07; //BSYNC_GPIO3 LOW
		}
		else if(cnt == 3)
		{
			wData[HIGH] = 0x87; //BSYNC_GPIO4 HIGH	
			wData[LOW] = 0x07; //BSYNC_GPIO4 LOW
		}
		else if(cnt == 4)
		{
			wData[HIGH] = 0x11; //DPTX_XSYNC_OUT HIGH
			wData[LOW] = 0x10; //DPTX_XSYNC_OUT LOW
		}
		else if(cnt == 5)
		{
			wData[HIGH] = 0x12; //DPTX_VBLANK_OUT HIGH
			wData[LOW] = 0x10; //DPTX_VBLANK_OUT LOW
		}
		
		if(cnt < 4)
		{
			rtn = LCM_GpioWrite(pData->position, wData[HIGH]); 
			if(rtn == 0)	return	result;

			rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, &ReadData[HIGH].u8Data);
			if(rtn == 0)	return	result;

			rtn = LCM_GpioWrite(pData->position, wData[LOW]); 
			if(rtn == 0)	return	result;

			rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, &ReadData[LOW].u8Data);
			if(rtn == 0)	return	result;
		}
		else
		{
			rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, wData[HIGH]);
			if(rtn == 0)	return	result;

			rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, &ReadData[HIGH].u8Data);
			if(rtn == 0)	return	result;

			rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, wData[LOW]);
			if(rtn == 0)	return	result;

			rtn = IoExp_PortInputRead(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, &ReadData[LOW].u8Data);
			if(rtn == 0)	return	result;
		}

		//TE skip masking
		if(mode == FT || mode == OC_TE_SKIP)
		{
			ReadData[HIGH].u8Data &= 0x1F;
			ReadData[LOW].u8Data &= 0x1F;
		}
	
		//Open Filter
		ReadData[HIGH].u8Data -= ReadData[LOW].u8Data;

		//Short Filter
		sum = 0;
		
		for(u8 chk = 0; chk < 8; chk++)
		{
			calc = ReadData[HIGH].u8Data & (0x01 << chk);

			if(calc >= 1)	sum++;
		}

		switch(cnt)
		{
			case 0:
				if(sum == 1 && ReadData[HIGH].b0 == HIGH && ReadData[LOW].b0 == LOW)
				{
					CMD_Printf("%d. OPEN SHORT TEST OK (nRESET)\r\n", ++indexCnt, cnt);
				}	
				else
				{
					CMD_Printf("%d. OPEN SHORT TEST NG (nRESET)\r\n", ++indexCnt, cnt);
					BoardTest_NgCheck(OPENSHORT);
				}
				break;

			case 1:
				if(sum == 1 && ReadData[HIGH].b1 == HIGH && ReadData[LOW].b1 == LOW)
				{
					CMD_Printf("%d. OPEN SHORT TEST OK (PANICB_L)\r\n", ++indexCnt, cnt);
				}	
				else
				{
					CMD_Printf("%d. OPEN SHORT TEST NG (PANICB_L)\r\n", ++indexCnt, cnt);
					BoardTest_NgCheck(OPENSHORT);
				}	
				break;

			case 2:
				if(sum == 1 && ReadData[HIGH].b2 == HIGH && ReadData[LOW].b2 == LOW)
				{
					CMD_Printf("%d. OPEN SHORT TEST OK (BSYNC_GPIO3)\r\n", ++indexCnt, cnt);
				}	
				else
				{
					CMD_Printf("%d. OPEN SHORT TEST NG (BSYNC_GPIO3)\r\n", ++indexCnt, cnt);
					BoardTest_NgCheck(OPENSHORT);
				}
				break;

			case 3:
				if(sum == 1 && ReadData[HIGH].b3 == HIGH && ReadData[LOW].b3 == LOW)
				{
					CMD_Printf("%d. OPEN SHORT TEST OK (BSYNC_GPIO4)\r\n", ++indexCnt, cnt);
				}	
				else
				{
					CMD_Printf("%d. OPEN SHORT TEST NG (BSYNC_GPIO4)\r\n", ++indexCnt, cnt);
					BoardTest_NgCheck(OPENSHORT);
				}
				break;
				
			case 4:
				if(sum == 1 && ReadData[HIGH].b4 == HIGH && ReadData[LOW].b4 == LOW)
				{
					CMD_Printf("%d. OPEN SHORT TEST OK (DPTX_XSYNC_OUT)\r\n", ++indexCnt, cnt);
				}	
				else
				{
					CMD_Printf("%d. OPEN SHORT TEST NG (DPTX_XSYNC_OUT)\r\n", ++indexCnt, cnt);
					BoardTest_NgCheck(OPENSHORT);
				}
				break;
			
			case 5:
				if(sum == 1 && ReadData[HIGH].b5 == HIGH && ReadData[LOW].b5 == LOW)
				{
					CMD_Printf("%d. OPEN SHORT TEST OK (DPTX_VBLANK_OUT)\r\n", ++indexCnt, cnt);
				}	
				else
				{
					CMD_Printf("%d. OPEN SHORT TEST NG (DPTX_VBLANK_OUT)\r\n", ++indexCnt, cnt);
					BoardTest_NgCheck(OPENSHORT);
				}
				break;
		}
	}

	///////////////////////////////////////////////////
	// DPTX CORE IO LOW
	////////////////////////////////////////////////////	
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x00);
	if(rtn == 0)	return	result;

	testItem--;

	result = 1;

	return	result;
}	

static u8 Cmd_EtTestPower(void *pVoid)
{
	u8	result = 0, rtn;
	double	voltage;
	double	current;
	
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(ngCnt[0])			return	result;
	
	CMD_Printf("\r\n========================== POWER TEST ==========================\r\n");

	////////////////////////////////////////////////////
	// POWER ENABLE VM
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 7; cnt++)
	{
		if(cnt == PWR_SRC_VM_3)		continue;
		
		rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, HIGH);
		if(rtn == 0)				return	result;
		
		HAL_Delay(20);
	}

	////////////////////////////////////////////////////
	// POWER MEASURE VM
	////////////////////////////////////////////////////
	for(u8 pwrSrc = 0; pwrSrc < PWR_SRC_VM_6 + 1; pwrSrc++)
	{
		if(pwrSrc == PWR_SRC_VM_3)		continue;

		HAL_Delay(400);
	
		rtn = PWR_MesureVI(pData->position, pwrSrc, &voltage, &current);	
		if(rtn == 0)	return	result;

		if(voltage < vmVoltageMin[pwrSrc] || voltage > vmVoltageMax[pwrSrc] || 
			current < vmCurrentMin[pwrSrc][0] || current > vmCurrentMax[pwrSrc][0])
		{
			CMD_Printf("%d. POWER TEST NG (VM%d / %f V  %f mA)\r\n", ++indexCnt, pwrSrc, voltage, current);
			BoardTest_NgCheck(POWER);
		}
		else
		{
			CMD_Printf("%d. POWER TEST OK (VM%d / %f V  %f mA)\r\n", ++indexCnt, pwrSrc, voltage, current);
		}
	}
	
	////////////////////////////////////////////////////
	// POWER DISABLE EXCEPT VM3
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 7; cnt++)
	{
		if(cnt == PWR_SRC_VM_3)
		{
			rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, HIGH);
			if(rtn == 0)				return	result;
		}
		else
		{
			rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, LOW);
			if(rtn == 0)				return	result;			
		}
		
		HAL_Delay(20);
	}

	testItem--;

	result = 1;

	return	result;
}	

static u8 Cmd_EtTestPmic(void *pVoid)
{
	u8	result = 0, rtn;
	u8	gpioData[4];
	double	voltage;
	double	current;
	
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;

	if(ngCnt[0])			return	result;
	
	CMD_Printf("\r\n========================== PMIC TEST ==========================\r\n");
	
	////////////////////////////////////////////////////
	// POWER DISABLE EXCEPT VM3
	////////////////////////////////////////////////////
	for(u8 cnt = 0; cnt < 7; cnt++)
	{
		if(cnt == PWR_SRC_VM_3)
		{
			rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, HIGH);
			if(rtn == 0)				return	result;
		}
		else
		{
			rtn = PWR_OutputEnable(pData->position, PWR_SRC_VM_0 + cnt, LOW);
			if(rtn == 0)				return	result;			
		}
		
		HAL_Delay(20);
	}

	gpioData[0] = 0x24;	//VSSEL
	gpioData[1] = 0x0C;	//VGL
	gpioData[2] = 0x14;	//AVDDH
	gpioData[3] = 0x1C;	//VDDEL

	for(u8 cnt = 0; cnt < 4; cnt++)
	{		
		rtn = LCM_GpioWrite(pData->position, gpioData[cnt]); 
		if(rtn == 0)	return	result;

		HAL_Delay(400);

		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_VOLTAGE, &voltage);
		if(rtn == 0)	return	result;
	
		rtn = PWR_ExtVoltageMeasure(pData->position, PWR_MEASURE_MODE_CURRENT, &current);
		if(rtn == 0)	return	result;

		if(cnt == VSSEL)			voltage *= -9;
		else if(cnt == VGL)			voltage *= -4;		
		else if(cnt == AVDDH)		voltage *= 2;	
		else if(cnt == VDDEL)		voltage *= 2;
		else						return	result;

		current *= 1000;
		current /= 41;
		current /= 10;

		if(cnt == 3)
		{
			if(voltage < pmicVoltageMin[cnt] || voltage > pmicVoltageMax[cnt])
			{
				CMD_Printf("%d. PMIC TEST NG ", ++indexCnt);
				
				switch(cnt)
				{
					case VDDEL:
						CMD_Printf("(VDDEL / %f V)\r\n", voltage);
						break;
				}
				
				BoardTest_NgCheck(PMIC);
			}
			else
			{
				CMD_Printf("%d. PMIC TEST OK ", ++indexCnt);

				switch(cnt)
				{
					case VDDEL:
						CMD_Printf("(VDDEL / %f V)\r\n", voltage);
						break;
				}
			}
		}
		else
		{
			if(voltage < pmicVoltageMin[cnt] || voltage > pmicVoltageMax[cnt] || 
			current < pmicCurrentMin[cnt] || current > pmicCurrentMax[cnt])
			{
				CMD_Printf("%d. PMIC TEST NG ", ++indexCnt);
				
				switch(cnt)
				{
					case VSSEL:
						CMD_Printf("(VSSEL / %f V  %f mA)\r\n", voltage, current);
						break;
					
					case VGL:
						CMD_Printf("(VGL / %f V  %f mA)\r\n", voltage, current);
						break;

					case AVDDH:
						CMD_Printf("(AVDDH / %f V  %f mA)\r\n", voltage, current);
						break;
				}
				
				BoardTest_NgCheck(PMIC);
			}
			else
			{
				CMD_Printf("%d. PMIC TEST OK ", ++indexCnt);

				switch(cnt)
				{
					case VSSEL:
						CMD_Printf("(VSSEL / %f V  %f mA)\r\n", voltage, current);
						break;
					
					case VGL:
						CMD_Printf("(VGL / %f V  %f mA)\r\n", voltage, current);
						break;

					case AVDDH:
						CMD_Printf("(AVDDH / %f V  %f mA)\r\n", voltage, current);
						break;
				}
			}
		}

	}

	testItem--;

	result = 1;

	return	result;
}

static u8 Cmd_EtTestOSC(void *pVoid)
{
	u8	result = 0, rtn;
	u32	ReadData;
	double	osc_clock;
	
	cmdFunctionData_t	*pData;
		
	pData = (cmdFunctionData_t*)pVoid;

	if(ngCnt[0])			return	result;

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

	CMD_Printf("\r\n=========================== OSC TEST ===========================\r\n");
	
	if(osc_clock < oscMin || osc_clock > oscMax)
	{
		CMD_Printf("%d. OSC TEST NG (OSC / %.3f kHz)\r\n", ++indexCnt, osc_clock);
		BoardTest_NgCheck(OSC);
	}
	else
	{
		CMD_Printf("%d. OSC TEST OK (OSC / %.3f kHz)\r\n", ++indexCnt, osc_clock);
	}

	testItem--;

	result = 1;

	return	result;
}	

static u8 Cmd_EtTestPortDirInit(void *pVoid)		
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;
		
	pData = (cmdFunctionData_t*)pVoid;
	
	////////////////////////////////////////////////////
	// DPTX CORE IO LOW
	////////////////////////////////////////////////////	
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x00);
	if(rtn == 0)	return	result;
	
	////////////////////////////////////////////////////
	// BT EXPANDER PORT ALL LOW
	////////////////////////////////////////////////////	
	rtn = IoExp_PortClear(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, 0xFF);
	if(rtn == 0)	return	result;

	rtn = IoExp_PortClear(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTB, 0xFF);
	if(rtn == 0)	return	result;

	////////////////////////////////////////////////////
	// BT EXPANDER DIRECTION SETTING
	////////////////////////////////////////////////////	
	rtn = IoExp_PortDir(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTA, PORT_INPUT);
	if(rtn == 0)	return	result;

	rtn = IoExp_PortDir(pVoid, BSP_I2C1, BT_EXPANDER_1, PORTB, PORT_OUTPUT);
	if(rtn == 0)	return	result;
	
	result = 1;

	return	result;
}
		
static u8 Cmd_EtTestFinalResult(void *pVoid)
{
	u8	result = 0, sum = 0, errIndex = 0, chk = 0;
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
		CMD_Printf("\r\n");

		for(u8 i = 0; i < 2; i++)
		{
			for(u8 j = 0; j < BIT32_FULL_CNT; j++)
			{
				temp = ngNum[i].u32Data & (1 << j);

				if(i == 0)	
				{
					if(temp >= 1)	
					{
						errIndex = j + 1 ;
						chk++;
					}
				}
				else if(i == 1)
				{
					if(temp >= 1)
					{
						errIndex = j + 1 + BIT32_FULL_CNT;
						chk++;
					}
				}
				else		return	result;

				if(chk)		CMD_Printf("%d ", errIndex);
				
				chk = 0;
			}
		}
	}

	memset(&mode, 0, sizeof(mode));
	memset(ngCnt, 0, sizeof(ngCnt));
	memset(ngNum, 0, sizeof(ngNum));
	memset(&indexCnt, 0, sizeof(indexCnt));
	memset(&testItem, 0xff, sizeof(testItem));
	
	result = 1;

	return	result;
}		

static u8 Cmd_EtTestVersion(void *pVoid)
{
	u8	result = 0;
	
	CMD_Printf("\r\n");
	CMD_Printf("ET TEST : D854_FT_OC\r\n");
	CMD_Printf("ET TEST VERSION : v1.0.3\r\n");	
	CMD_Printf("ET TEST DATE : 2022.04.07\r\n");	

	result = 1;

	return	result;
}	

/******************************************************************************
* Revision History
*
* v1.0.0 : Change Global Variable with MBTEST Global Variable
* v1.0.1 : when Function return error, total result NG print out
           when measure power, increase delay 20ms -> 400ms
* v1.0.2 : Temp Version
           Remove OC/FT Mode in Openshort test
           VBlank(TE signal) Test masking  
* v1.0.3 : function openshort modified >> add open filter, short filter
           add OC_TE_SKIP mode
******************************************************************************/			
const	commandList_t	cmdList_EtTest[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"version",				Cmd_EtTestVersion,			0x01,		NULL,		"",						NULL},
												{	"test.item",			Cmd_EtTestItem,				0x01,		NULL,		"",						NULL},
												{	"mode",					Cmd_EtTestMode,				0x01,		NULL,		"",						NULL},
												{	"clock.limit",			Cmd_EtTestClockLimit,		0x01,		NULL,		"",						NULL},
												{	"voltage.limit",		Cmd_EtTestVoltageLimit,		0x01,		NULL,		"",						NULL},
												{	"current.limit",		Cmd_EtTestCurrentLimit,		0x01,		NULL,		"",						NULL},
												{	"i2c",					Cmd_EtTestI2c,				0x01,		NULL,		"",						NULL},
												{	"io",					Cmd_EtTestOpenShort,		0x01,		NULL,		"",						NULL},
												{	"power",				Cmd_EtTestPower,			0x01,		NULL,		"",						NULL},
												{	"pmic",					Cmd_EtTestPmic,				0x01,		NULL,		"",						NULL},
												{	"osc",					Cmd_EtTestOSC,				0x01,		NULL,		"",						NULL},
												{	"port.init",			Cmd_EtTestPortDirInit,		0x01,		NULL,		"",						NULL},
												{	"final.result",			Cmd_EtTestFinalResult,		0x01,		NULL,		"",						NULL},
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};
