#include	"App_FPGA.h"

static	bspGpio_t	fpgaIoCtrl[4], fpgaIoDNC, fpgaIoRST, fpgaIoPWR;

static	vu32	*pFpgaCmd	= (vu32*)MEMORY_MAP_FPGA_COMMAND_ADRS;
static	vu32	*pFpgaData	= (vu32*)MEMORY_MAP_FPGA_DATA_ADRS;

u8 FPGA_GpioInit()
{
	u8	result = 0, rtn, errCnt = 0;

	fpgaIoCtrl[0].Port		= GPIOF;
	fpgaIoCtrl[0].Pin		= GPIO_PIN_6;
	fpgaIoCtrl[0].Mode		= BSP_GPIO_MODE_INPUT;
	fpgaIoCtrl[0].wData		= LOW;

	fpgaIoCtrl[1].Port		= GPIOF;
	fpgaIoCtrl[1].Pin		= GPIO_PIN_7;
	fpgaIoCtrl[1].Mode		= BSP_GPIO_MODE_INPUT;
	fpgaIoCtrl[1].wData		= LOW;

	fpgaIoCtrl[2].Port		= GPIOF;
	fpgaIoCtrl[2].Pin		= GPIO_PIN_8;
	fpgaIoCtrl[2].Mode		= BSP_GPIO_MODE_INPUT;
	fpgaIoCtrl[2].wData		= LOW;

	fpgaIoCtrl[3].Port		= GPIOF;
	fpgaIoCtrl[3].Pin		= GPIO_PIN_9;
	fpgaIoCtrl[3].Mode		= BSP_GPIO_MODE_INPUT;
	fpgaIoCtrl[3].wData		= LOW;

	fpgaIoDNC.Port			= GPIOI;
	fpgaIoDNC.Pin			= GPIO_PIN_12;
	fpgaIoDNC.Mode			= BSP_GPIO_MODE_OUTPUT_PU;
	fpgaIoDNC.wData			= LOW;

	fpgaIoRST.Port			= GPIOI;
	fpgaIoRST.Pin			= GPIO_PIN_13;
	fpgaIoRST.Mode			= BSP_GPIO_MODE_OUTPUT_PU;
	fpgaIoRST.wData			= LOW;

	fpgaIoPWR.Port			= GPIOI;
	fpgaIoPWR.Pin			= GPIO_PIN_15;
	fpgaIoPWR.Mode			= BSP_GPIO_MODE_OUTPUT_PU;
	fpgaIoPWR.wData			= LOW;

	rtn = BSP_GPIO_Init(&fpgaIoCtrl[0]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&fpgaIoCtrl[1]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&fpgaIoCtrl[1]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&fpgaIoCtrl[1]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&fpgaIoDNC);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&fpgaIoRST);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&fpgaIoPWR);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

u8 FPGA_CtrlIoDirection(u8 position, u8 state)
{
	u8	result = 0, rtn;

	if(position > 4)		return result;
	if(state > 1)			return result;

	if(state)				fpgaIoCtrl[position].Mode = BSP_GPIO_MODE_OUTPUT_PU;
	else					fpgaIoCtrl[position].Mode = BSP_GPIO_MODE_INPUT;

	rtn = BSP_GPIO_Init(&fpgaIoCtrl[position]);
	if(rtn != 0)			result = 1;

	return result;
}

u8 FPGA_CtrlIoWrite(u8 position, u8 state)
{
	u8	result = 0, rtn;

	if(position > 4)		return result;
	if(state > 1)			return result;

	rtn = BSP_GPIO_WritePin(&fpgaIoCtrl[position], state);
	if(rtn != 0)			result = 1;

	return result;
}

u8 FPGA_CtrlIoRead(u8 position, u8 *pData)
{
	u8	result = 0;

	if(position > 4)		return result;

	*pData = BSP_GPIO_ReadPin(&fpgaIoCtrl[position]);

	result = 1;

	return result;
}

u8 FPGA_DncIoCtrl(u8 state)
{
	u8	result = 0, rtn;

	if(state > 1)					return result;
	if(fpgaIoDNC.wData == state)	return result;

	rtn = BSP_GPIO_WritePin(&fpgaIoDNC, state);
	if(rtn != 0)			result = 1;

	return result;
}

u8 FPGA_RstIoCtrl(u8 state)
{
	u8	result = 0, rtn;

	if(state > 1)			return result;

	rtn = BSP_GPIO_WritePin(&fpgaIoRST, state);
	if(rtn != 0)			result = 1;

	return result;
}

u8 FPGA_PwrIoCtrl(u8 state)
{
	u8	result = 0, rtn;

	if(state > 1)			return result;

	rtn = BSP_GPIO_WritePin(&fpgaIoPWR, state);
	if(rtn != 0)			result = 1;

	return result;
}

u8 FPGA_Init()
{
	u8	result = 0;

	FPGA_PwrIoCtrl(LOW);
	FPGA_RstIoCtrl(LOW);

	HAL_Delay(100);

	FPGA_PwrIoCtrl(HIGH);

	HAL_Delay(200);

	FPGA_RstIoCtrl(HIGH);

	HAL_Delay(500);

	result = 1;

	return result;
}

void FPGA_Read(u32 cmd, u32 *pData, u32 length)
{
	u32 cnt;

	FPGA_DncIoCtrl(FPGA_MODE_COMMAND);

	*pFpgaCmd = cmd;

	FPGA_DncIoCtrl(FPGA_MODE_DATA);

	for(cnt = 0; cnt < length; cnt++)
	{
		pData[cnt] = *pFpgaData;
	}
}

void FPGA_Write(u32 cmd, u32 *pData, u32 length)
{
	u32 cnt;

	FPGA_DncIoCtrl(FPGA_MODE_COMMAND);

	*pFpgaCmd = cmd;

	FPGA_DncIoCtrl(FPGA_MODE_DATA);

	for(cnt = 0; cnt < length; cnt++)
	{
		*pFpgaData = pData[cnt] ;
	}
}

void FPGA_WriteCMD(u32 cmd)
{
	FPGA_DncIoCtrl(FPGA_MODE_COMMAND);

	*pFpgaCmd = cmd;
}

void FPGA_WriteDATA(u32 data)
{
	FPGA_DncIoCtrl(FPGA_MODE_DATA);

	*pFpgaData = data;
}

u32	FPGA_ReadDATA()
{
	u32	result;

	FPGA_DncIoCtrl(FPGA_MODE_DATA);

	result = *pFpgaData;

	return result;
}

u32 FPGA_ReadSingle(u32 cmd)
{
	u32 data;

	FPGA_Read(cmd, &data, 1);

	return data;
}

void FPGA_WriteSingle(u32 cmd, u32 data)
{
	FPGA_Write(cmd, &data, 1);
}

u8 FPGA_Check()
{
	u8	result = 0;
	u32	regData;

	regData = FPGA_ReadSingle(FPGA_CMD_VERSION);
	if(regData != 0)		result = 1;

	return result;
}

void FPGA_Reset()
{
	FPGA_WriteSingle(FPGA_CMD_RESET, 0);

	HAL_Delay(5);

	FPGA_WriteSingle(FPGA_CMD_RESET, 1);

	HAL_Delay(5);
}

u8 FPGA_GetVersion(u8 *pData)
{
	u8	result = 0;
	u8	msb, lsb;
	u32	regData;

	regData = FPGA_ReadSingle(FPGA_CMD_VERSION);

	msb = lsb = (u8)regData;

	msb >>= 4;
	lsb &= 0x0f;

	sprintf((char*)pData, "%01X.%01X", msb, lsb);

	result = 1;

	return result;
}

u8 FPGA_GetBuildDate(u8 *pData)
{
	u8	result = 0;
	u8	month, day;
	u16	year;
	u32	regData;

	regData = FPGA_ReadSingle(FPGA_CMD_VERSION);

	year	= (u16)(regData >> 24);
	month	= (u8)(regData >> 16);
	day		= (u8)(regData >> 8);

	year	&= 0x00ff;
	year	|= 0x2000;

	sprintf((char*)pData, "%04X.%02X.%02X", year, month, day);

	result = 1;

	return result;
}

