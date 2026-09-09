#include	"BSP_Clock.h"

static u8 BSP_Clock_MpuInit()
{
	u8	result = 0;

#if	(BSP_CLOCK_MPU_ENABLE == 1)

	MPU_Region_InitTypeDef	mpu;

	HAL_MPU_Disable();

	// SRAM1
	mpu.Enable						= MPU_REGION_ENABLE;
	mpu.BaseAddress					= 0x20000000;
	mpu.Size						= MPU_REGION_SIZE_512KB;
	mpu.AccessPermission			= MPU_REGION_FULL_ACCESS;
	mpu.IsBufferable				= MPU_ACCESS_BUFFERABLE;
	mpu.IsCacheable					= MPU_ACCESS_CACHEABLE;
	mpu.IsShareable					= MPU_ACCESS_SHAREABLE;
	mpu.Number						= MPU_REGION_NUMBER0;
	mpu.TypeExtField				= MPU_TEX_LEVEL0;
	mpu.SubRegionDisable			= 0x00;
	mpu.DisableExec					= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&mpu);

	// FMC BANK_1_2
	mpu.Enable						= MPU_REGION_ENABLE;
	mpu.BaseAddress					= 0x60000000;
	mpu.Size						= MPU_REGION_SIZE_512MB;
	mpu.AccessPermission			= MPU_REGION_FULL_ACCESS;
	mpu.IsBufferable				= MPU_ACCESS_NOT_BUFFERABLE;
	mpu.IsCacheable					= MPU_ACCESS_NOT_CACHEABLE;
	mpu.IsShareable					= MPU_ACCESS_NOT_SHAREABLE;
	mpu.Number						= MPU_REGION_NUMBER2;
	mpu.TypeExtField				= MPU_TEX_LEVEL0;
	mpu.SubRegionDisable			= 0x00;
	mpu.DisableExec					= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&mpu);

	// FMC BANK_3_4
	mpu.Enable						= MPU_REGION_ENABLE;
	mpu.BaseAddress					= 0x80000000;
	mpu.Size						= MPU_REGION_SIZE_512MB;
	mpu.AccessPermission			= MPU_REGION_FULL_ACCESS;
	mpu.IsBufferable				= MPU_ACCESS_NOT_BUFFERABLE;
	mpu.IsCacheable					= MPU_ACCESS_NOT_CACHEABLE;
	mpu.IsShareable					= MPU_ACCESS_NOT_SHAREABLE;
	mpu.Number						= MPU_REGION_NUMBER3;
	mpu.TypeExtField				= MPU_TEX_LEVEL0;
	mpu.SubRegionDisable			= 0x00;
	mpu.DisableExec					= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&mpu);

	// FMC BANK_5
	mpu.Enable						= MPU_REGION_ENABLE;
	mpu.BaseAddress					= 0xC0000000;
	mpu.Size						= MPU_REGION_SIZE_256MB;
	mpu.AccessPermission			= MPU_REGION_FULL_ACCESS;
	mpu.IsBufferable				= MPU_ACCESS_NOT_BUFFERABLE;
	mpu.IsCacheable					= MPU_ACCESS_NOT_CACHEABLE;
	mpu.IsShareable					= MPU_ACCESS_NOT_SHAREABLE;
	mpu.Number						= MPU_REGION_NUMBER4;
	mpu.TypeExtField				= MPU_TEX_LEVEL0;
	mpu.SubRegionDisable			= 0x00;
	mpu.DisableExec					= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&mpu);

	// FMC BANK_6
	mpu.Enable						= MPU_REGION_ENABLE;
	mpu.BaseAddress					= 0xD0000000;
	mpu.Size						= MPU_REGION_SIZE_256MB;
	mpu.AccessPermission			= MPU_REGION_FULL_ACCESS;
	mpu.IsBufferable				= MPU_ACCESS_NOT_BUFFERABLE;
	mpu.IsCacheable					= MPU_ACCESS_NOT_CACHEABLE;
	mpu.IsShareable					= MPU_ACCESS_NOT_SHAREABLE;
	mpu.Number						= MPU_REGION_NUMBER5;
	mpu.TypeExtField				= MPU_TEX_LEVEL0;
	mpu.SubRegionDisable			= 0x00;
	mpu.DisableExec					= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&mpu);

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

#else

	HAL_MPU_Disable();

#endif	//	BSP_CLOCK_MPU_ENABLE

	result = 1;

	return result;
}

u8 BSP_Clock_PllInit()
{
	u8	result = 1;
	HAL_StatusTypeDef	rtn;

	RCC_OscInitTypeDef			osc;
	RCC_ClkInitTypeDef			clk;
	RCC_PeriphCLKInitTypeDef	periClk;

	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	osc.OscillatorType				=	RCC_OSCILLATORTYPE_HSE;
	osc.HSEState					=	RCC_HSE_ON;
	osc.LSEState					=	RCC_LSE_OFF;
	osc.HSIState					=	RCC_HSI_OFF;
	osc.HSICalibrationValue			=	RCC_HSICALIBRATION_DEFAULT;
	osc.LSIState					=	RCC_LSI_OFF;
	osc.PLL.PLLState				=	RCC_PLL_ON;
	osc.PLL.PLLSource				=	RCC_PLLSOURCE_HSE;
	osc.PLL.PLLM					=	25;
	osc.PLL.PLLN					=	432;
	osc.PLL.PLLP					=	RCC_PLLP_DIV2;
	osc.PLL.PLLQ					=	9;

	clk.ClockType					=	RCC_CLOCKTYPE_HCLK;
	clk.ClockType					|=	RCC_CLOCKTYPE_SYSCLK;
	clk.ClockType					|=	RCC_CLOCKTYPE_PCLK1;
	clk.ClockType					|=	RCC_CLOCKTYPE_PCLK2;
	clk.SYSCLKSource				=	RCC_SYSCLKSOURCE_PLLCLK;
	clk.AHBCLKDivider				=	RCC_SYSCLK_DIV1;
	clk.APB1CLKDivider				=	RCC_HCLK_DIV4;
	clk.APB2CLKDivider				=	RCC_HCLK_DIV2;

	periClk.PeriphClockSelection	=	RCC_PERIPHCLK_USART1;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_USART2;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_USART3;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_UART4;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_UART5;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_USART6;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_UART7;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_UART8;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_I2C1;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_I2C2;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_I2C3;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_I2C4;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_SDMMC1;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_SDMMC2;
	periClk.PeriphClockSelection	|=	RCC_PERIPHCLK_CLK48;

	periClk.PLLI2S.PLLI2SN			=	200;
	periClk.PLLI2S.PLLI2SP			=	RCC_PLLI2SP_DIV2;
	periClk.PLLI2S.PLLI2SQ			=	2;
	periClk.PLLI2S.PLLI2SR			=	2;

	periClk.PLLSAI.PLLSAIN			=	384;
	periClk.PLLSAI.PLLSAIP			=	RCC_PLLSAIP_DIV8;
	periClk.PLLSAI.PLLSAIQ			=	2;
	periClk.PLLSAI.PLLSAIR			=	2;

	periClk.Usart1ClockSelection	=	RCC_USART1CLKSOURCE_PCLK2;
	periClk.Usart2ClockSelection	=	RCC_USART2CLKSOURCE_PCLK1;
	periClk.Usart3ClockSelection	=	RCC_USART3CLKSOURCE_PCLK1;
	periClk.Uart4ClockSelection		=	RCC_UART4CLKSOURCE_PCLK1;
	periClk.Uart5ClockSelection		=	RCC_UART5CLKSOURCE_PCLK1;
	periClk.Usart6ClockSelection	=	RCC_USART6CLKSOURCE_PCLK2;
	periClk.Uart7ClockSelection		=	RCC_UART7CLKSOURCE_PCLK1;
	periClk.Uart8ClockSelection		=	RCC_UART8CLKSOURCE_PCLK1;
	periClk.I2c1ClockSelection		=	RCC_I2C1CLKSOURCE_PCLK1;
	periClk.I2c2ClockSelection		=	RCC_I2C2CLKSOURCE_PCLK1;
	periClk.I2c3ClockSelection		=	RCC_I2C3CLKSOURCE_PCLK1;
	periClk.I2c4ClockSelection		=	RCC_I2C4CLKSOURCE_PCLK1;
	periClk.Clk48ClockSelection		=	RCC_CLK48SOURCE_PLL;
	periClk.Sdmmc1ClockSelection	=	RCC_SDMMC1CLKSOURCE_CLK48;
	periClk.Sdmmc2ClockSelection	=	RCC_SDMMC2CLKSOURCE_CLK48;

	rtn = HAL_RCC_OscConfig(&osc);
	if(rtn != HAL_OK)		BSP_ERROR_Loop(BSP_ERROR_POS_MCU_CLK);

	rtn = HAL_PWREx_EnableOverDrive();
	if(rtn != HAL_OK)		BSP_ERROR_Loop(BSP_ERROR_POS_MCU_CLK);

	rtn = HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_7);
	if(rtn != HAL_OK)		BSP_ERROR_Loop(BSP_ERROR_POS_MCU_CLK);

	rtn = HAL_RCCEx_PeriphCLKConfig(&periClk);
	if(rtn != HAL_OK)		BSP_ERROR_Loop(BSP_ERROR_POS_MCU_CLK);

	return result;
}

static u8 BSP_Clock_CacheInit()
{
	u8	result = 0;

	SCB_InvalidateICache();	

//	Enable branch prediction
	SCB->CCR |= (1 <<18); 
	__DSB();

	SCB_InvalidateICache();
#if (BSP_CLOCK_ICACHE_ENABLE == 1)
	SCB_EnableICache();
#else
	SCB_DisableICache();
#endif

	SCB_InvalidateDCache();
#if (BSP_CLOCK_DCACHE_ENABLE == 1)
	SCB_EnableDCache();
#else
	SCB_DisableDCache();
#endif

	result = 1;

	return result;
}

static u8 BSP_Clock_GpioInit()
{
	u8	result = 1;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_GPIOK_CLK_ENABLE();

	return result;
}

u8 BSP_Clock_Init()
{
	u8	result = 0, rtn, errCnt = 0;

	rtn = BSP_DataInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_Clock_MpuInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_Clock_CacheInit();
	if(rtn == 0)		errCnt += 1;

	HAL_Init();

	rtn = BSP_Clock_PllInit();
	if(rtn == 0)		errCnt += 1;

	rtn = BSP_Clock_GpioInit();
	if(rtn == 0)		errCnt += 1;

	if(errCnt == 0)		result = 1;

	return result;
}
