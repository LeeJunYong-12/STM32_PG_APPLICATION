#include	"App_IoExpander.h"

static 	u8	glb_in_expander_1_a_h;
static	u8	glb_in_expander_1_b_h;
static	u8	glb_in_expander_2_a_h;
static	u8	glb_in_expander_2_b_h;
static	u8	glb_in_expander_3_a_h;
static	u8	glb_in_expander_3_b_h;

u8 IoExp_ChannelSelect(u8 expander, u8 status)
{
	u8 slaveAdrs;

	switch(expander)
	{
		case MT_OUT_EXPANDER_1:
			slaveAdrs = 0x44 | status; 	// 0x20(7Bit) + R/W Bit
			break;

		case MT_OUT_EXPANDER_2:
			slaveAdrs = 0x42 | status; 
			break;
			
		case MT_IN_EXPANDER_1:
			slaveAdrs = 0x48 | status;
			break;

		case MT_IN_EXPANDER_2:
			slaveAdrs = 0x4A | status;
			break;

		case MT_IN_EXPANDER_3:
			slaveAdrs = 0x46 | status;
			break;

		case MB_EXPANDER_1:
			slaveAdrs = 0x4E | status;
			break;

		case MB_EXPANDER_2:
			slaveAdrs = 0x46 | status;
			break;

		case BT_EXPANDER_1:
			slaveAdrs = 0x40 | status;
			break;

		case BT_EXPANDER_2:
			slaveAdrs = 0x42 | status;
			break;

		case ET_TEST_EXPANDER_1:
			slaveAdrs = 0x46 | status;
			break;

		case HT_ETC_EXPANDER:
			slaveAdrs = 0x46 | status;
			break;
		
        case HT_VER_EXPANDER:
			slaveAdrs = 0x40 | status;
			break;
		
        case HT_OUT_EXPANDER_1:
			slaveAdrs = 0x42 | status;
			break;
		
        case HT_OUT_EXPANDER_2:
			slaveAdrs = 0x44 | status;
			break;
		
        case HT_IN_EXPANDER_1:
			slaveAdrs = 0x4A | status;
			break;
		
        case HT_IN_EXPANDER_2:
			slaveAdrs = 0x4C | status;
			break;
		
        case HT_INOUT_EXPANDER:
			slaveAdrs = 0x4E | status;
			break;
		
		default:
			break;
	}

	return slaveAdrs;
}

u8 IoExp_I2CTransmit(void *pVoid, u8 i2cNo, u8 devAdrs, u8 *pBuffer, u16 size)
{
	u8	result = 0, rtn;;	
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	rtn = LCM_I2CTransmitData(pData->position, i2cNo, devAdrs, pBuffer, size);
	if(rtn == 0)		return	result;

	result = 1;

	return result;
}

u8 IoExp_I2CMEMTransmit(void *pVoid, u8 i2cNo, u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pBuffer, u16 size)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	rtn = LCM_I2CMemWrite(pData->position, i2cNo, i2cNo, memAdrs, adrsMode, pBuffer, size);
	if(rtn == 0)		return	result;

	result = 1;

	return result;
}

u8 IoExp_I2CMEMReceive(void *pVoid, u8 i2cNo, u8 devAdrs, u16 memAdrs, u8 adrsMode, u8 *pBuffer, u16 size)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	rtn = LCM_I2CMemRead(pData->position, i2cNo, devAdrs, memAdrs, adrsMode, pBuffer, size);
	if(rtn == 0)		return	result;

	result = 1;

	return result;
}

u8 IoExp_PortDir(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 direction)
{
	u8	result = 0, rtn;
	u8	slaveAdrs, wData[2];

	slaveAdrs = IoExp_ChannelSelect(expander, WRITE);

	if(port == PORTA)
	{
		wData[0] = 0x06;  					// Configuration Port 0(GPOA) 
		wData[1] = direction;				// Configuration Port 0(DIR)

		rtn = IoExp_I2CTransmit(pVoid, i2cNo, slaveAdrs, wData, 2);
		if(rtn == 0)	return	result;
	}
	else if(port == PORTB)
	{
		wData[0] = 0x07;  					// Configuration Port 1(GPOB) 
		wData[1] = direction;				// Configuration Port 1(DIR)

		rtn = IoExp_I2CTransmit(pVoid, i2cNo, slaveAdrs, wData, 2);
		if(rtn == 0)	return	result;
	}
	else				return	result;

	result = 1;

	return result;
}

u8 IoExp_PortSet(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData)
{
	u8	result = 0, rtn;
	u8	slaveAdrs, setStatus = 0;
	u8	wData[2];

	slaveAdrs = IoExp_ChannelSelect(expander, READ);

	if(port == PORTA)
	{
		rtn = IoExp_I2CMEMReceive(pVoid, i2cNo, slaveAdrs, 0x02, 0, &setStatus, 1);
		if(rtn == 0)	return	result;
	}
	else if(port == PORTB)
	{
		rtn = IoExp_I2CMEMReceive(pVoid, i2cNo, slaveAdrs, 0x03, 0, &setStatus, 1);
		if(rtn == 0)	return	result;
	}

	setStatus |= setData;
	
	slaveAdrs = IoExp_ChannelSelect(expander, WRITE);

	if(port == PORTA)
	{
		wData[0] = 0x02;				//Output Port 0 Reg
		wData[1] = setStatus;			//Output Port 0 Data

		rtn = IoExp_I2CTransmit(pVoid, i2cNo, slaveAdrs, wData, 2);
		if(rtn == 0)	return	result;
	}
	else if(port == PORTB)
	{
		wData[0] = 0x03;				//Output Port 1 Reg
		wData[1] = setStatus;			//Output Port 1 Data

		rtn = IoExp_I2CTransmit(pVoid, i2cNo, slaveAdrs, wData, 2);
		if(rtn == 0)	return	result;
	}
	else				return	result;
	
	result = 1;

	return result;
}

u8 IoExp_PortWrite(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData)
{
	u8	result = 0, rtn;
	u8	slaveAdrs;
	u8	wData[2];
	
	slaveAdrs = IoExp_ChannelSelect(expander, WRITE);

	if(port == PORTA)
	{
		wData[0] = 0x02;				//Output Port 0 Reg
		wData[1] = setData;				//Output Port 0 Data

		rtn = IoExp_I2CTransmit(pVoid, i2cNo, slaveAdrs, wData, 2);
		if(rtn == 0)	return	result;
	}
	else if(port == PORTB)
	{
		wData[0] = 0x03;				//Output Port 1 Reg
		wData[1] = setData;				//Output Port 1 Data

		rtn = IoExp_I2CTransmit(pVoid, i2cNo, slaveAdrs, wData, 2);
		if(rtn == 0)	return	result;
	}
	else				return	result;
	
	result = 1;

	return result;
}

u8 IoExp_PortClear(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData)
{
	u8	result = 0, rtn;
	u8	slaveAdrs, setStatus = 0;
	u8	wData[2];

	slaveAdrs = IoExp_ChannelSelect(expander, READ);

	if(port == PORTA)
	{
		rtn = IoExp_I2CMEMReceive(pVoid, i2cNo, slaveAdrs, 0x02, 0, &setStatus, 1);
		if(rtn == 0)	return	result;
	}
	else if(port == PORTB)
	{
		rtn = IoExp_I2CMEMReceive(pVoid, i2cNo, slaveAdrs, 0x03, 0, &setStatus, 1);
		if(rtn == 0)	return	result;
	}
	else				return	result;

	setStatus &= ~setData;
	
	slaveAdrs = IoExp_ChannelSelect(expander, WRITE);

	if(port == PORTA)
	{
		wData[0] = 0x02;				//Output Port 0 Reg
		wData[1] = setStatus;			//Output Port 0 Data

		rtn = IoExp_I2CTransmit(pVoid, i2cNo, slaveAdrs, wData, 2);
		if(rtn == 0)	return	result;
	}
	else if(port == PORTB)
	{
		wData[0] = 0x03;				//Output Port 1 Reg
		wData[1] = setStatus;			//Output Port 1 Data

		rtn = IoExp_I2CTransmit(pVoid, i2cNo, slaveAdrs, wData, 2);
		if(rtn == 0)	return	result;
	}
	else				return	result;
	
	result = 1;

	return result;
}

u8 IoExp_PortInputRead(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 *pBuffer)
{
	u8	result = 0, rtn;
	u8	slaveAdrs;

	slaveAdrs = IoExp_ChannelSelect(expander, READ);

	if(port == PORTA)
	{
   		rtn = IoExp_I2CMEMReceive(pVoid, i2cNo, slaveAdrs, 0x00, 0, pBuffer, 1);
		if(rtn == 0)	return	result;
	}
	else if(port == PORTB)
	{
   		rtn = IoExp_I2CMEMReceive(pVoid, i2cNo, slaveAdrs, 0x01, 0, pBuffer, 1);
		if(rtn == 0)	return	result;
	}
	else				return	result;

	result = 1;

	return result;
}

u8 IoExp_LED_Ctrl(void *pVoid, u8 status)
{	
	u8 result = 0, rtn;
	u8 port;
	u8 wData;

	switch(status)
	{
		case TOTAL_NG:
			port = PORTA;
			wData = 0x40;
			break;	
			
		case I2C_NG:
			port = PORTA;
			wData = 0x80;
			break;

		case VENTURA_NG:
			port = PORTB;
			wData = 0x10;
			break;
		
		case IO_NG:
			port = PORTB;
			wData = 0x02;
			break;
			
		case POWER_NG:
			port = PORTB;
			wData = 0x01;
			break;
			
		case DP_NG:
			port = PORTB;
			wData = 0x20;
			break;
			
		case PMIC_NG:
			port = PORTB;
			wData = 0x40;
			break;
			
		case OSC_NG:
			port = PORTB;
			wData = 0x04;
			break;
			
		case TESTING:
			port = PORTB;
			wData = 0x80;
			break;
			
		case TOTAL_OK:
			port = PORTB;
			wData = 0x08;
			break;
	}

	rtn = IoExp_PortSet(pVoid, BSP_I2C2, MT_IN_EXPANDER_3, port, wData);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}

u8 IoExp_ShortCheck_D854Hinge(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData)
{
	u8	result = 0, rtn;
	u8	Sumval[2] = {0, };
	
	bitCtrl8_t	in_expander_1_a[2];
	bitCtrl8_t	in_expander_1_b[2];
	bitCtrl8_t	in_expander_2_a[2];
	bitCtrl8_t	in_expander_2_b[2];
	bitCtrl8_t	in_expander_3_b[2];
	
	////////////////////////////////////////////////////
	// IOEXPANDER ALL PORT LOW 
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortClear(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	

	////////////////////////////////////////////////////
	// IOEXPANDER WRITE
	////////////////////////////////////////////////////
	rtn = IoExp_PortSet(pVoid, i2cNo, expander, port, setData);
	if(rtn == 0)	return	result;	

	////////////////////////////////////////////////////
	// IOEXPANDER Open/Short Check => HIGH SENSING
	////////////////////////////////////////////////////
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTA, &in_expander_1_a[HIGH].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTB, &in_expander_1_b[HIGH].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTA, &in_expander_2_a[HIGH].u8Data);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTB, &in_expander_2_b[HIGH].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTB, &in_expander_3_b[HIGH].u8Data);
	if(rtn == 0)	return	result;	

	in_expander_2_b[HIGH].u8Data &= 0xFE;
	in_expander_3_b[HIGH].u8Data &= 0x1C;
	
	glb_in_expander_1_a_h = in_expander_1_a[HIGH].u8Data;
	glb_in_expander_1_b_h = in_expander_1_b[HIGH].u8Data;
	glb_in_expander_2_a_h = in_expander_2_a[HIGH].u8Data;
	glb_in_expander_2_b_h = in_expander_2_b[HIGH].u8Data;
	glb_in_expander_3_b_h = in_expander_3_b[HIGH].u8Data;

	////////////////////////////////
	// MERGE IN_EXPANDER DATA
	////////////////////////////////
	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		if(in_expander_1_a[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_1_b[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_2_a[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_2_b[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_3_b[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;	
	}

	////////////////////////////////////////////////////
	// IOEXPANDER ALL PORT HIGH
	////////////////////////////////////////////////////
	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	

	////////////////////////////////////////////////////
	// IOEXPANDER CLEAR
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, i2cNo, expander, port, setData);
	if(rtn == 0)	return	result;	
	
	////////////////////////////////////////////////////
	// IOEXPANDER Open/Short Check => LOW SENSING
	////////////////////////////////////////////////////
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTA, &in_expander_1_a[LOW].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTB, &in_expander_1_b[LOW].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTA, &in_expander_2_a[LOW].u8Data);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTB, &in_expander_2_b[LOW].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTB, &in_expander_3_b[LOW].u8Data);
	if(rtn == 0)	return	result;	

	in_expander_2_b[LOW].u8Data |= ~0xFE;
	in_expander_3_b[LOW].u8Data |= ~0x1C;
	
	////////////////////////////////
	// MERGE IN_EXPANDER DATA
	////////////////////////////////
	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		if(in_expander_1_a[LOW].u8Data & (1 << cnt))		Sumval[LOW]++;
		if(in_expander_1_b[LOW].u8Data & (1 << cnt))		Sumval[LOW]++;
		if(in_expander_2_a[LOW].u8Data & (1 << cnt))		Sumval[LOW]++;
		if(in_expander_2_b[LOW].u8Data & (1 << cnt))		Sumval[LOW]++;
		if(in_expander_3_b[LOW].u8Data & (1 << cnt))		Sumval[LOW]++;	
	}
	
	if(expander == HT_OUT_EXPANDER_1 && port == PORTA)
	{
		switch(setData)
		{
			case P0:
				if(in_expander_2_a[HIGH].b0 == 1 && in_expander_2_a[LOW].b0 == 0)	result = 1;
				break;
			case P1:
				if(in_expander_2_a[HIGH].b1 == 1 && in_expander_2_a[LOW].b1 == 0)	result = 1;
				break;
			case P2:
				if(in_expander_2_a[HIGH].b2 == 1 && in_expander_2_a[LOW].b2 == 0)	result = 1;
				break;
			case P3:
				if(in_expander_1_b[HIGH].b0 == 1 && in_expander_1_b[LOW].b0 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_1_b[HIGH].b1 == 1 && in_expander_1_b[LOW].b1 == 0)	result = 1;
				break;
			case P5:
				if(in_expander_1_b[HIGH].b2 == 1 && in_expander_1_b[LOW].b2 == 0)	result = 1;
				break;
			case P6:
				if(in_expander_1_b[HIGH].b3 == 1 && in_expander_1_b[LOW].b3 == 0)	result = 1;
				break;
			case P7:
				if(in_expander_1_b[HIGH].b4 == 1 && in_expander_1_b[LOW].b4 == 0)	result = 1;
				break;
		}
	}	
	else if(expander == HT_OUT_EXPANDER_1 && port == PORTB)
	{
		switch(setData)
		{
			case P10:
				if(in_expander_1_b[HIGH].b5 == 1 && in_expander_1_b[LOW].b5 == 0)	result = 1;
				break;
			case P11:
				if(in_expander_1_b[HIGH].b6 == 1 && in_expander_1_b[LOW].b6 == 0)	result = 1;
				break;
			case P12:
				if(in_expander_1_b[HIGH].b7 == 1 && in_expander_1_b[LOW].b7 == 0)	result = 1;
				break;
			case P13:
				break;
			case P14:
				if(in_expander_1_a[HIGH].b3 == 1 && in_expander_1_a[LOW].b3 == 0)	result = 1;
				break;
			case P15:
				if(in_expander_1_a[HIGH].b2 == 1 && in_expander_1_a[LOW].b2 == 0)	result = 1;
				break;
			case P16:
				if(in_expander_1_a[HIGH].b7 == 1 && in_expander_1_a[LOW].b7 == 0)	result = 1;
				break;
			case P17:
				if(in_expander_1_a[HIGH].b6 == 1 && in_expander_1_a[LOW].b6 == 0)	result = 1;
				break;
		}
	}
	else if(expander == HT_OUT_EXPANDER_2 && port == PORTA)
	{
		switch(setData)
		{
			case P0:
				if(in_expander_1_a[HIGH].b1 == 1 && in_expander_1_a[LOW].b1 == 0)	result = 1;
				break;
			case P1:
				if(in_expander_1_a[HIGH].b0 == 1 && in_expander_1_a[LOW].b0 == 0)	result = 1;
				break;
			case P2:
				if(in_expander_1_a[HIGH].b5 == 1 && in_expander_1_a[LOW].b5 == 0)	result = 1;
				break;
			case P3:
				if(in_expander_1_a[HIGH].b4 == 1 && in_expander_1_a[LOW].b4 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_2_a[HIGH].b3 == 1 && in_expander_2_a[LOW].b3 == 0)	result = 1;
				break;
			case P5:
				if(in_expander_2_a[HIGH].b4 == 1 && in_expander_2_a[LOW].b4 == 0)	result = 1;
				break;
			case P6:
				if(in_expander_2_a[HIGH].b5 == 1 && in_expander_2_a[LOW].b5 == 0)	result = 1;
				break;
			case P7:
				if(in_expander_2_a[HIGH].b6 == 1 && in_expander_2_a[LOW].b6 == 0)	result = 1;
				break;
		}
	}
	else if(expander == HT_OUT_EXPANDER_2 && port == PORTB)
	{
		switch(setData)
		{
			case P10:
				if(in_expander_2_a[HIGH].b7 == 1 && in_expander_2_a[LOW].b7 == 0)	result = 1;
				break;
			case P11:
				if(in_expander_2_b[HIGH].b1 == 1 && in_expander_2_b[LOW].b1 == 0)	result = 1;
				break;
			case P12:
				if(in_expander_2_b[HIGH].b2 == 1 && in_expander_2_b[LOW].b2 == 0)	result = 1;
				break;
			case P13:
				if(in_expander_2_b[HIGH].b3 == 1 && in_expander_2_b[LOW].b3 == 0)	result = 1;
				break;
			case P14:
				if(in_expander_2_b[HIGH].b4 == 1 && in_expander_2_b[LOW].b4 == 0)	result = 1;
				break;
			case P15:
				if(in_expander_2_b[HIGH].b5 == 1 && in_expander_2_b[LOW].b5 == 0)	result = 1;
				break;
			case P16:
				if(in_expander_2_b[HIGH].b6 == 1 && in_expander_2_b[LOW].b6 == 0)	result = 1;
				break;
			case P17:
				if(in_expander_2_b[HIGH].b7 == 1 && in_expander_2_b[LOW].b7 == 0)	result = 1;
				break;
		}
	}
	else if(expander == HT_INOUT_EXPANDER && port == PORTA)
	{
		switch(setData)
		{
			case P0:
			case P1:
			case P5:
			case P6:
			case P7:
				break;
			case P2:
				if(in_expander_3_b[HIGH].b2 == 1 && in_expander_3_b[LOW].b2 == 0)	result = 1;
				break;
			case P3:
				if(in_expander_3_b[HIGH].b3 == 1 && in_expander_3_b[LOW].b3 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_3_b[HIGH].b4 == 1 && in_expander_3_b[LOW].b4 == 0)	result = 1;
				break;
		}
	}

	if(Sumval[LOW] < (34 - 1)) // PIN TO PIN SHORT CHECK
	{	
		u8	calcData[5];
		u8	pinshort = 0;
		u8	sum = 0;
	
		////////////////////////////////////////////////////
		// IOEXPANDER ALL PORT LOW 
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTA, PORT_ALL);
		if(rtn == 0)	return	result; 
	
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTB, PORT_ALL);
		if(rtn == 0)	return	result; 
	
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTA, PORT_ALL);
		if(rtn == 0)	return	result; 
		
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTB, PORT_ALL);
		if(rtn == 0)	return	result; 
	
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTA, PORT_ALL);
		if(rtn == 0)	return	result; 
				
		////////////////////////////////////////////////////
		// IOEXPANDER Open/Short Check => LOW SENSING
		////////////////////////////////////////////////////
		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTA, &in_expander_1_a[LOW].u8Data);
		if(rtn == 0)	return	result;	
		
		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTB, &in_expander_1_b[LOW].u8Data);
		if(rtn == 0)	return	result;	
		
		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTA, &in_expander_2_a[LOW].u8Data);
		if(rtn == 0)	return	result;	

		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTB, &in_expander_2_b[LOW].u8Data);
		if(rtn == 0)	return	result;	
		
		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTB, &in_expander_3_b[LOW].u8Data);
		if(rtn == 0)	return	result;	

		in_expander_2_b[LOW].u8Data &= 0xFE;
		in_expander_3_b[LOW].u8Data &= 0x1C;
	
		calcData[0] = glb_in_expander_1_a_h - in_expander_1_a[LOW].u8Data;
		calcData[1] = glb_in_expander_1_b_h - in_expander_1_b[LOW].u8Data;
		calcData[2] = glb_in_expander_2_a_h - in_expander_2_a[LOW].u8Data;
		calcData[3] = glb_in_expander_2_b_h - in_expander_2_b[LOW].u8Data;
		calcData[4] = glb_in_expander_3_b_h - in_expander_3_b[LOW].u8Data;

		for(u8 cnt = 0; cnt < 5; cnt++)
		{
			for(u8 loop = 0; loop < 8; loop++)
			{
				pinshort = calcData[cnt] & (0x01 << loop);

				if(pinshort >= 1)	sum++;	
			}
		}

		if(sum > 1)		result = 0;
		
	}

	return	result;
}

u8 IoExp_ShortCheck_D852Hinge(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData)
{
	u8	result = 0, rtn;
	u8	Sumval[2] = {0, };
	
	bitCtrl8_t	in_expander_1_a[2];
	bitCtrl8_t	in_expander_1_b[2];
	bitCtrl8_t	in_expander_2_a[2];
	bitCtrl8_t	in_expander_2_b[2];
	bitCtrl8_t	in_expander_3_a[2];
	
	////////////////////////////////////////////////////
	// IOEXPANDER ALL PORT LOW 
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortClear(pVoid, i2cNo, HT_VER_EXPANDER, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortClear(pVoid, i2cNo, HT_VER_EXPANDER, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	////////////////////////////////////////////////////
	// IOEXPANDER WRITE
	////////////////////////////////////////////////////
	rtn = IoExp_PortSet(pVoid, i2cNo, expander, port, setData);
	if(rtn == 0)	return	result;	

	////////////////////////////////////////////////////
	// IOEXPANDER Open/Short Check => HIGH SENSING
	////////////////////////////////////////////////////
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTA, &in_expander_1_a[HIGH].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTB, &in_expander_1_b[HIGH].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTA, &in_expander_2_a[HIGH].u8Data);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTB, &in_expander_2_b[HIGH].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTA, &in_expander_3_a[HIGH].u8Data);
	if(rtn == 0)	return	result;	

	in_expander_2_a[HIGH].u8Data &= 0xF7;
	in_expander_2_b[HIGH].u8Data &= 0x7F;
	in_expander_3_a[HIGH].u8Data &= 0x0F;
	
	glb_in_expander_1_a_h = in_expander_1_a[HIGH].u8Data;
	glb_in_expander_1_b_h = in_expander_1_b[HIGH].u8Data;
	glb_in_expander_2_a_h = in_expander_2_a[HIGH].u8Data;
	glb_in_expander_2_b_h = in_expander_2_b[HIGH].u8Data;
	glb_in_expander_3_a_h = in_expander_3_a[HIGH].u8Data;

	////////////////////////////////
	// MERGE IN_EXPANDER DATA
	////////////////////////////////
	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		if(in_expander_1_a[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_1_b[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_2_a[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_2_b[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_3_a[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;	
	}

	////////////////////////////////////////////////////
	// IOEXPANDER ALL PORT HIGH
	////////////////////////////////////////////////////
	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_VER_EXPANDER, PORTA, PORT_ALL);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortWrite(pVoid, i2cNo, HT_VER_EXPANDER, PORTB, PORT_ALL);
	if(rtn == 0)	return	result;	

	////////////////////////////////////////////////////
	// IOEXPANDER CLEAR
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, i2cNo, expander, port, setData);
	if(rtn == 0)	return	result;	
	
	////////////////////////////////////////////////////
	// IOEXPANDER Open/Short Check => LOW SENSING
	////////////////////////////////////////////////////
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTA, &in_expander_1_a[LOW].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTB, &in_expander_1_b[LOW].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTA, &in_expander_2_a[LOW].u8Data);
	if(rtn == 0)	return	result;	

	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTB, &in_expander_2_b[LOW].u8Data);
	if(rtn == 0)	return	result;	
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTA, &in_expander_3_a[LOW].u8Data);
	if(rtn == 0)	return	result;	

	in_expander_2_a[LOW].u8Data |= ~0xF7;
	in_expander_2_b[LOW].u8Data |= ~0x7F;
	in_expander_3_a[LOW].u8Data |= ~0x0F;
	
	////////////////////////////////
	// MERGE IN_EXPANDER DATA
	////////////////////////////////
	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		if(in_expander_1_a[LOW].u8Data & (1 << cnt))	Sumval[LOW]++;
		if(in_expander_1_b[LOW].u8Data & (1 << cnt))	Sumval[LOW]++;
		if(in_expander_2_a[LOW].u8Data & (1 << cnt))	Sumval[LOW]++;
		if(in_expander_2_b[LOW].u8Data & (1 << cnt))	Sumval[LOW]++;
		if(in_expander_3_a[LOW].u8Data & (1 << cnt))	Sumval[LOW]++;	
	}
	
	if(expander == HT_VER_EXPANDER && port == PORTA)
	{
		switch(setData)
		{
			case P0:
				if(in_expander_1_a[HIGH].b4 == 1 && in_expander_1_a[LOW].b4 == 0)	result = 1;
				break;
			case P1:
				if(in_expander_1_a[HIGH].b5 == 1 && in_expander_1_a[LOW].b5 == 0)	result = 1;
				break;
			case P2:
				if(in_expander_1_a[HIGH].b6 == 1 && in_expander_1_a[LOW].b6 == 0)	result = 1;
				break;
			case P3:
				if(in_expander_1_a[HIGH].b7 == 1 && in_expander_1_a[LOW].b7 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_1_a[HIGH].b0 == 1 && in_expander_1_a[LOW].b0 == 0)	result = 1;
				break;
			case P5:
				if(in_expander_1_a[HIGH].b1 == 1 && in_expander_1_a[LOW].b1 == 0)	result = 1;
				break;
			case P6:
				if(in_expander_1_a[HIGH].b2 == 1 && in_expander_1_a[LOW].b2 == 0)	result = 1;
				break;
			case P7:
				if(in_expander_1_a[HIGH].b3 == 1 && in_expander_1_a[LOW].b3 == 0)	result = 1;
				break;
		}
	}	
	else if(expander == HT_VER_EXPANDER && port == PORTB)
	{
		switch(setData)
		{
			case P10:
				if(in_expander_1_b[HIGH].b0 == 1 && in_expander_1_b[LOW].b0 == 0)	result = 1;
				break;
			case P11:
				break;
			case P12:
				if(in_expander_1_b[HIGH].b4 == 1 && in_expander_1_b[LOW].b4 == 0)	result = 1;
				break;
			case P13:
				break;
			case P14:
				if(in_expander_1_b[HIGH].b5 == 1 && in_expander_1_b[LOW].b5 == 0)	result = 1;
				break;
			case P15:
				break;
			case P16:
				break;
			case P17:
				break;
		}
	}
	else if(expander == HT_OUT_EXPANDER_1 && port == PORTA)
	{
		switch(setData)
		{
			case P0:
				if(in_expander_2_a[HIGH].b7 == 1 && in_expander_2_a[LOW].b7 == 0)	result = 1;
				break;
			case P1:
				if(in_expander_2_b[HIGH].b1 == 1 && in_expander_2_b[LOW].b1 == 0)	result = 1;
				break;
			case P2:
				if(in_expander_3_a[HIGH].b0 == 1 && in_expander_3_a[LOW].b0 == 0)	result = 1;
				break;
			case P3:
				if(in_expander_3_a[HIGH].b1 == 1 && in_expander_3_a[LOW].b1 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_2_b[HIGH].b2 == 1 && in_expander_2_b[LOW].b2 == 0)	result = 1;
				break;
			case P5:
				if(in_expander_2_b[HIGH].b3 == 1 && in_expander_2_b[LOW].b3 == 0)	result = 1;
				break;
			case P6:
				if(in_expander_2_b[HIGH].b4 == 1 && in_expander_2_b[LOW].b4 == 0)	result = 1;
				break;
			case P7:
				if(in_expander_2_b[HIGH].b5 == 1 && in_expander_2_b[LOW].b5 == 0)	result = 1;
				break;
		}
	}
	else if(expander == HT_OUT_EXPANDER_1 && port == PORTB)
	{
		switch(setData)
		{
			case P10:
				if(in_expander_3_a[HIGH].b2 == 1 && in_expander_3_a[LOW].b2 == 0)	result = 1;
				break;
			case P11:
				if(in_expander_3_a[HIGH].b3 == 1 && in_expander_3_a[LOW].b3 == 0)	result = 1;
				break;
			case P12:
				if(in_expander_2_b[HIGH].b6 == 1 && in_expander_2_b[LOW].b6 == 0)	result = 1;
				break;
			case P13:
				break;
			case P14:
				break;
			case P15:
				break;
			case P16:
				break;
			case P17:
				break;
		}
	}
	else if(expander == HT_OUT_EXPANDER_2 && port == PORTA)
	{
		switch(setData)
		{
			case P0:
				if(in_expander_1_b[HIGH].b1 == 1 && in_expander_1_b[LOW].b1 == 0)	result = 1;
				break;
			case P1:
				if(in_expander_1_b[HIGH].b6 == 1 && in_expander_1_b[LOW].b6 == 0)	result = 1;
				break;
			case P2:
				break;
			case P3:
				if(in_expander_1_b[HIGH].b2 == 1 && in_expander_1_b[LOW].b2 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_1_b[HIGH].b3 == 1 && in_expander_1_b[LOW].b3 == 0)	result = 1;
				break;
			case P5:
				break;
			case P6:
				break;
			case P7:
				break;
		}
	}
	else if(expander == HT_OUT_EXPANDER_2 && port == PORTB)
	{
		switch(setData)
		{
			case P0:
				if(in_expander_1_b[HIGH].b7 == 1 && in_expander_1_b[LOW].b7 == 0)	result = 1;
				break;
			case P1:
				if(in_expander_2_a[HIGH].b0 == 1 && in_expander_2_a[LOW].b0 == 0)	result = 1;
				break;
			case P2:
				if(in_expander_2_a[HIGH].b4 == 1 && in_expander_2_a[LOW].b4 == 0)	result = 1;
				break;
			case P3:
				if(in_expander_2_a[HIGH].b1 == 1 && in_expander_2_a[LOW].b1 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_2_a[HIGH].b2 == 1 && in_expander_2_a[LOW].b2 == 0)	result = 1;
				break;
			case P5:
				if(in_expander_2_a[HIGH].b5 == 1 && in_expander_2_a[LOW].b5 == 0)	result = 1;
				break;
			case P6:
				if(in_expander_2_a[HIGH].b6 == 1 && in_expander_2_a[LOW].b6 == 0)	result = 1;
				break;
			case P7:
				if(in_expander_2_b[HIGH].b0 == 1 && in_expander_2_b[LOW].b0 == 0)	result = 1;
				break;
		}
	}

	if(Sumval[LOW] < (34 - 1)) // PIN TO PIN SHORT CHECK
	{	
		u8	calcData[5];
		u8	pinshort = 0;
		u8	sum = 0;
	
		////////////////////////////////////////////////////
		// IOEXPANDER ALL PORT LOW 
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTA, PORT_ALL);
		if(rtn == 0)	return	result; 
	
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_1, PORTB, PORT_ALL);
		if(rtn == 0)	return	result; 
	
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTA, PORT_ALL);
		if(rtn == 0)	return	result; 
		
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_OUT_EXPANDER_2, PORTB, PORT_ALL);
		if(rtn == 0)	return	result; 
	
		rtn = IoExp_PortClear(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTA, PORT_ALL);
		if(rtn == 0)	return	result; 
				
		////////////////////////////////////////////////////
		// IOEXPANDER Open/Short Check => LOW SENSING
		////////////////////////////////////////////////////
		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTA, &in_expander_1_a[LOW].u8Data);
		if(rtn == 0)	return	result;	
		
		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_1, PORTB, &in_expander_1_b[LOW].u8Data);
		if(rtn == 0)	return	result;	
		
		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTA, &in_expander_2_a[LOW].u8Data);
		if(rtn == 0)	return	result;	

		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_IN_EXPANDER_2, PORTB, &in_expander_2_b[LOW].u8Data);
		if(rtn == 0)	return	result;	
		
		rtn = IoExp_PortInputRead(pVoid, i2cNo, HT_INOUT_EXPANDER, PORTA, &in_expander_3_a[LOW].u8Data);
		if(rtn == 0)	return	result;	

		in_expander_2_a[LOW].u8Data &= 0xF7;
		in_expander_2_b[LOW].u8Data &= 0x7F;
		in_expander_3_a[LOW].u8Data &= 0x0F;
	
		calcData[0] = glb_in_expander_1_a_h - in_expander_1_a[LOW].u8Data;
		calcData[1] = glb_in_expander_1_b_h - in_expander_1_b[LOW].u8Data;
		calcData[2] = glb_in_expander_2_a_h - in_expander_2_a[LOW].u8Data;
		calcData[3] = glb_in_expander_2_b_h - in_expander_2_b[LOW].u8Data;
		calcData[4] = glb_in_expander_3_a_h - in_expander_3_a[LOW].u8Data;

		for(u8 cnt = 0; cnt < 5; cnt++)
		{
			for(u8 loop = 0; loop < 8; loop++)
			{
				pinshort = calcData[cnt] & (0x01 << loop);

				if(pinshort >= 1)	sum++;	
			}
		}

		if(sum > 1)		result = 0;
		
	}

	return	result;
}

u8 IoExp_ShortCheck_Prev(void *pVoid, u8 i2cNo, u8 expander, u8 port, u8 setData)
{
	u8	result = 0, rtn;
	u8	Sumval[2] = {0, };
	cmdFunctionData_t	*pData;
	
	pData = (cmdFunctionData_t*)pVoid;
	
	bitCtrl8_t	in_expander_1_a[2];
	bitCtrl8_t	in_expander_1_b[2];
	bitCtrl8_t	in_expander_2_a[2];

	////////////////////////////////////////////////////
	// MT_OUT_EXPANDER_1 DIRECTION INIT
	////////////////////////////////////////////////////
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, PORT_OUTPUT);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, PORT_OUTPUT);
	if(rtn == 0)	return	result; 

	////////////////////////////////////////////////////
	// EXCEPT FOR BSYNC PAIR PIN 
	////////////////////////////////////////////////////
	if(expander == MT_OUT_EXPANDER_1 && port == PORTB && setData == P10)
	{
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P12);
		if(rtn == 0)	return	result; 
	}
	else if(expander == MT_OUT_EXPANDER_1 && port == PORTB && setData == P12)
	{
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P10);
		if(rtn == 0)	return	result; 
	}
	else if(expander == MT_OUT_EXPANDER_1 && port == PORTA && setData == P7)
	{
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTB, P13);
		if(rtn == 0)	return	result; 
	}
	else if(expander == MT_OUT_EXPANDER_1 && port == PORTB && setData == P13)
	{
		rtn = IoExp_PortDir(pVoid, BSP_I2C2, MT_OUT_EXPANDER_1, PORTA, P7);
		if(rtn == 0)	return	result; 
	}
	
	////////////////////////////////////////////////////
	// IOEXPANDER ALL PORT LOW 
	////////////////////////////////////////////////////
	rtn = IoExp_PortClear(pVoid, i2cNo, MT_OUT_EXPANDER_1, PORTA, 0xff);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortClear(pVoid, i2cNo, MT_OUT_EXPANDER_1, PORTB, 0xff);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortClear(pVoid, i2cNo, MT_OUT_EXPANDER_2, PORTA, 0xff);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortClear(pVoid, i2cNo, MT_OUT_EXPANDER_2, PORTB, 0x33);
	if(rtn == 0)	return	result; 
	
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x10);
	if(rtn == 0)	return	result; 
	
	if(setData == OUT_XSYNC)
	{
		rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x11);
		if(rtn == 0)	return	result; 
	}
	else
	{	
		////////////////////////////////////////////////////
		// IOEXPANDER WRITE
		////////////////////////////////////////////////////
		rtn = IoExp_PortWrite(pVoid, i2cNo, expander, port, setData);
		if(rtn == 0)	return	result; 
	}
	
	////////////////////////////////////////////////////
	// IOEXPANDER Open/Short Check => HIGH SENSING
	////////////////////////////////////////////////////
	rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_1, PORTA, &in_expander_1_a[HIGH].u8Data);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_1, PORTB, &in_expander_1_b[HIGH].u8Data);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_2, PORTA, &in_expander_2_a[HIGH].u8Data);
	if(rtn == 0)	return	result; 
		
	glb_in_expander_1_a_h = in_expander_1_a[HIGH].u8Data;
	glb_in_expander_1_b_h = in_expander_1_b[HIGH].u8Data;
	glb_in_expander_2_a_h = in_expander_2_a[HIGH].u8Data;

	////////////////////////////////
	// MERGE IN_EXPANDER DATA
	////////////////////////////////
	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		if(in_expander_1_a[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		if(in_expander_1_b[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;

		if(cnt < 3)
		{
			if(in_expander_2_a[HIGH].u8Data & (1 << cnt))	Sumval[HIGH]++;
		}
	}

	////////////////////////////////////////////////////
	// IOEXPANDER ALL PORT HIGH 
	////////////////////////////////////////////////////
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_OUT_EXPANDER_1, PORTA, 0xff);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_OUT_EXPANDER_1, PORTB, 0xff);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_OUT_EXPANDER_2, PORTA, 0xff);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_OUT_EXPANDER_2, PORTB, 0x33);
	if(rtn == 0)	return	result; 
	
	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x11);
	if(rtn == 0)	return	result; 

	if(setData == OUT_XSYNC)
	{
		rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x10);
		if(rtn == 0)	return	result; 
	}
	else
	{
		////////////////////////////////////////////////////
		// IOEXPANDER CLEAR
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, i2cNo, expander, port, setData);
		if(rtn == 0)	return	result; 
	}
	
	////////////////////////////////////////////////////
	// IOEXPANDER Open/Short Check => LOW SENSING
	////////////////////////////////////////////////////
	rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_1, PORTA, &in_expander_1_a[LOW].u8Data);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_1, PORTB, &in_expander_1_b[LOW].u8Data);
	if(rtn == 0)	return	result; 
	
	rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_2, PORTA, &in_expander_2_a[LOW].u8Data);
	if(rtn == 0)	return	result; 
	
	////////////////////////////////
	// MERGE IN_EXPANDER DATA
	////////////////////////////////
	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		if(in_expander_1_a[LOW].u8Data & (1 << cnt))	Sumval[LOW]++;
		if(in_expander_1_b[LOW].u8Data & (1 << cnt))	Sumval[LOW]++;

		if(cnt < 3)
		{
			if(in_expander_2_a[LOW].u8Data & (1 << cnt))	Sumval[LOW]++;
		}
	}

	if(expander == MT_OUT_EXPANDER_1 && port == PORTA)
	{
		switch(setData)
		{
			case P0:
				if(in_expander_1_a[HIGH].b4 == 1 && in_expander_1_a[LOW].b4 == 0)	result = 1;
				break;
			case P1:
				if(in_expander_1_a[HIGH].b1 == 1 && in_expander_1_a[LOW].b1 == 0)	result = 1;
				break;
			case P2:
				if(in_expander_1_a[HIGH].b3 == 1 && in_expander_1_a[LOW].b3 == 0)	result = 1;
				break;
			case P3:
				if(in_expander_1_a[HIGH].b2 == 1 && in_expander_1_a[LOW].b2 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_1_b[HIGH].b1 == 1 && in_expander_1_b[LOW].b1 == 0)	result = 1;
				break;
			case P5:
				if(in_expander_1_a[HIGH].b5 == 1 && in_expander_1_a[LOW].b5 == 0)	result = 1;
				break;
			case P7:
				if(in_expander_1_b[HIGH].b5 == 1 && in_expander_1_b[LOW].b5 == 0)	result = 1;
				break;
		}
	}
	else if(expander == MT_OUT_EXPANDER_1 && port == PORTB)
	{
		switch(setData)
		{
			case P10:
				if(in_expander_1_b[HIGH].b6 == 1 && in_expander_1_b[LOW].b6 == 0)	result = 1;
				break;
			case P11:
				if(in_expander_1_b[HIGH].b3 == 1 && in_expander_1_b[LOW].b3 == 0)	result = 1;
				break;
			case P12:
				if(in_expander_1_b[HIGH].b6 == 1 && in_expander_1_b[LOW].b6 == 0)	result = 1;
				break;
			case P13:
				if(in_expander_1_b[HIGH].b5 == 1 && in_expander_1_b[LOW].b5 == 0)	result = 1;
				break;
			case P14:
				if(in_expander_1_a[HIGH].b0 == 1 && in_expander_1_a[LOW].b0 == 0)	result = 1;
				break;
			case P15:
				if(in_expander_1_b[HIGH].b2 == 1 && in_expander_1_b[LOW].b2 == 0)	result = 1;
				break;
		}
	}
	else if(expander == MT_OUT_EXPANDER_2 && port == PORTA)
	{
		switch(setData)
		{
			case P3:
				if(in_expander_1_a[HIGH].b7 == 1 && in_expander_1_a[LOW].b7 == 0)	result = 1;
				break;
			case P4:
				if(in_expander_1_b[HIGH].b4 == 1 && in_expander_1_b[LOW].b4 == 0)	result = 1;
				break;
			case P6:
				if(in_expander_2_a[HIGH].b0 == 1 && in_expander_2_a[LOW].b0 == 0)	result = 1;
				break;
			case P7:
				if(in_expander_1_b[HIGH].b7 == 1 && in_expander_1_b[LOW].b7 == 0)	result = 1;
				break;
		}
	}
	else if(expander == MT_OUT_EXPANDER_2 && port == PORTB)
	{
		switch(setData)
		{
			case P10:
				if(in_expander_2_a[HIGH].b2 == 1 && in_expander_2_a[LOW].b2 == 0)	result = 1;
				break;
			case P14:
				if(in_expander_1_b[HIGH].b0 == 1 && in_expander_1_b[LOW].b0 == 0)	result = 1;
				break;
			case P15:
				if(in_expander_2_a[HIGH].b1 == 1 && in_expander_2_a[LOW].b1 == 0)	result = 1;
				break;
		}
	}
	else if(setData == OUT_XSYNC)
	{
		if(in_expander_1_a[HIGH].b6 == 1 && in_expander_1_a[LOW].b6 == 0)	result = 1;
		else	result = 0;
	}

	if(Sumval[LOW] < 18) // PIN TO PIN SHORT CHECK
	{	
		u8	calcData[3];
		u8	pinshort = 0;
		u8	sum = 0;
	
		////////////////////////////////////////////////////
		// IOEXPANDER ALL PORT LOW 
		////////////////////////////////////////////////////
		rtn = IoExp_PortClear(pVoid, i2cNo, MT_OUT_EXPANDER_1, PORTA, 0xff);
		if(rtn == 0)	return	result; 
		
		rtn = IoExp_PortClear(pVoid, i2cNo, MT_OUT_EXPANDER_1, PORTB, 0xff);
		if(rtn == 0)	return	result; 
		
		rtn = IoExp_PortClear(pVoid, i2cNo, MT_OUT_EXPANDER_2, PORTA, 0xff);
		if(rtn == 0)	return	result; 
		
		rtn = IoExp_PortClear(pVoid, i2cNo, MT_OUT_EXPANDER_2, PORTB, 0x33);
		if(rtn == 0)	return	result; 
		
		rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x10);
		if(rtn == 0)	return	result; 
				
		////////////////////////////////////////////////////
		// IOEXPANDER Open/Short Check => HIGH SENSING
		////////////////////////////////////////////////////
		rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_1, PORTA, &in_expander_1_a[LOW].u8Data);
		if(rtn == 0)	return	result; 
		
		rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_1, PORTB, &in_expander_1_b[LOW].u8Data);
		if(rtn == 0)	return	result; 
		
		rtn = IoExp_PortInputRead(pVoid, i2cNo, MT_IN_EXPANDER_2, PORTA, &in_expander_2_a[LOW].u8Data);
		if(rtn == 0)	return	result; 
	
		calcData[0] = glb_in_expander_1_a_h - in_expander_1_a[LOW].u8Data;
		calcData[1] = glb_in_expander_1_b_h - in_expander_1_b[LOW].u8Data;
		calcData[2] = glb_in_expander_2_a_h - in_expander_2_a[LOW].u8Data;

		for(u8 cnt = 0; cnt < 3; cnt++)
		{
			for(u8 loop = 0; loop < 8; loop++)
			{
				pinshort = calcData[cnt] & (0x01 << loop);

				if(pinshort >= 1)	sum++;	
			}
		}

		if(sum > 1)		result = 0;
		
	}

	rtn = DPC_DirectRegWriteSingle(pData->position, 0x008A, 0x00);
	if(rtn == 0)	return	result; 

	return	result;
}

u8 IoExp_LED_I2C_NG(void *pVoid, u8 i2cNo)
{	
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTA, I2C_NG);
	if(rtn == 0)	return	result;	

	result = 1;

	return	result;
}

u8 IoExp_LED_VENTURA_NG(void *pVoid, u8 i2cNo)
{	
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTB, VENTURA_NG);
	if(rtn == 0)	return	result;	

	result = 1;

	return	result;
}

u8 IoExp_LED_IO_NG(void *pVoid, u8 i2cNo)
{	
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTB, IO_NG);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}

u8 IoExp_LED_POWER_NG(void *pVoid, u8 i2cNo)
{
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTB, POWER_NG);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}

u8 IoExp_LED_DP_NG(void *pVoid, u8 i2cNo)
{
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTB, DP_NG);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}

u8 IoExp_LED_PMIC_NG(void *pVoid, u8 i2cNo)
{
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTB, PMIC_NG);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}

u8 IoExp_LED_OSC_NG(void *pVoid, u8 i2cNo)
{
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTB, OSC_NG);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}

u8 IoExp_LED_TEST_RUN(void *pVoid, u8 i2cNo)
{
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTB, TESTING);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}

u8 IoExp_LED_TOTAL_OK(void *pVoid, u8 i2cNo)
{
	u8 result  = 0, rtn;
	
	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTB, TOTAL_OK);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}

u8 IoExp_LED_TOTAL_NG(void *pVoid, u8 i2cNo)
{
	u8 result  = 0, rtn;

	rtn = IoExp_PortWrite(pVoid, i2cNo, MT_IN_EXPANDER_3, PORTA, TOTAL_NG);
	if(rtn == 0)	return	result;	
	
	result = 1;

	return	result;
}
