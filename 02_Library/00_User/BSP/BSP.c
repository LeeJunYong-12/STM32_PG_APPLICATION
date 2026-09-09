#include	"BSP.h"

extern	u8 BSP_Ethernet_DataInit();
extern	u8 BSP_GPIO_DataInit();
extern	u8 BSP_I2C_DataInit();
extern	u8 BSP_QSPI_DataInit();
extern	u8 BSP_SD_DataInit();
extern	u8 BSP_SPI_DataInit();
extern	u8 BSP_SysTick_DataInit();
extern	u8 BSP_Timer_DataInit();
extern	u8 BSP_UART_DataInit();
extern	u8 BSP_USB_DataInit();

void DelayCycles(u32 cnt)
{
	while(cnt--);
}

u8 BSP_DataInit()
{
	u8	result = 0, rtn, errCnt = 0;

	rtn = BSP_Ethernet_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_GPIO_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_I2C_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_QSPI_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_SD_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_SPI_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_SysTick_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_Timer_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_UART_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_USB_DataInit();
	if(rtn == 0)		errCnt += 1;

	if(errCnt == 0)		result = 1;

	return result;
}

