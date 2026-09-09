#include	"BSP_SysTick.h"

static bspSysTickFops_t	bspSysTickFops[BSP_SYSTICK_FUNCTION_MAX_COUNT];

#if	(BSP_USE_RTOS)
static	TIM_HandleTypeDef	*phBspSysTickTimer;
#endif	//	BSP_USE_RTOS

u8 BSP_SysTick_DataInit()
{
	u8	result = 0;

	memset(bspSysTickFops, NULL, sizeof(bspSysTickFops));

	result = 1;

	return result;
}

u8 BSP_SysTick_RegisterInterface(u8 pos, void *pVoid)
{
	u8	result = 0;

	if(pos >= BSP_SYSTICK_FUNCTION_MAX_COUNT)	return result;

	memcpy(&bspSysTickFops[pos], pVoid, sizeof(bspSysTickFops_t));

	result = 1;

	return result;
}

void BSP_SysTick_Process()
{
	u8	cnt;

	for(cnt = 0; cnt < BSP_SYSTICK_FUNCTION_MAX_COUNT; cnt++)
	{
		if(bspSysTickFops[cnt].SysTickFunction != NULL)
		{
			bspSysTickFops[cnt].SysTickFunction();
		}
	}
}

#if	(BSP_USE_RTOS)
static void BSP_SysTick_Timer_MspInit()
{
	__HAL_RCC_TIM6_CLK_ENABLE();
}

static void BSP_SysTick_Timer_MspDeInit()
{
	__HAL_RCC_TIM6_CLK_DISABLE();
}

static void BSP_SysTick_Timer_PeriodElapsedCallback()
{
	HAL_IncTick();
}

void HAL_SuspendTick(void)
{
	__HAL_TIM_DISABLE_IT(phBspSysTickTimer, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void)
{
	__HAL_TIM_ENABLE_IT(phBspSysTickTimer, TIM_IT_UPDATE);
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	HAL_StatusTypeDef	result = HAL_ERROR;
	HAL_StatusTypeDef	rtn;
	RCC_ClkInitTypeDef	clkConfig;
	u8	errCnt = 0;
	u32	clk, prescaler, period;
	u32	fLatency;

	bspTimerFops_t		fops;

	memset(&fops, NULL, sizeof(fops));

	phBspSysTickTimer = (TIM_HandleTypeDef*)BSP_Timer_GetHandle(BSP_TIMER6);

	if(phBspSysTickTimer == NULL)				return result;

	fops.Base_MspInit				= BSP_SysTick_Timer_MspInit;
	fops.Base_MspDeInit				= BSP_SysTick_Timer_MspDeInit;
	fops.PeriodElapsedCallback		= BSP_SysTick_Timer_PeriodElapsedCallback;

	rtn = (HAL_StatusTypeDef)BSP_Timer_RegisterInterface(phBspSysTickTimer, &fops);
	if(rtn == 0)								return result;

	HAL_RCC_GetClockConfig(&clkConfig, (uint32_t*)&fLatency);

	if(clkConfig.APB1CLKDivider == RCC_HCLK_DIV1)
	{
		clk = HAL_RCC_GetPCLK1Freq();
	}
	else
	{
		clk = HAL_RCC_GetPCLK1Freq() * 2;
	}

	prescaler	= (clk / 1000000) - 1;
	period		= (1000000 / 1000) - 1;

	phBspSysTickTimer->Init.Prescaler			= prescaler;
	phBspSysTickTimer->Init.CounterMode			= TIM_COUNTERMODE_UP;
	phBspSysTickTimer->Init.Period				= period;
	phBspSysTickTimer->Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	phBspSysTickTimer->Init.RepetitionCounter	= 0;

	rtn = HAL_TIM_Base_Init(phBspSysTickTimer);
	if(rtn != HAL_OK)		errCnt += 1;

	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority, 0);

	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	rtn = HAL_TIM_Base_Start_IT(phBspSysTickTimer);
	if(rtn != HAL_OK)		errCnt += 1;

	if(errCnt == 0)			result = HAL_OK;

	return result;
}
#endif	//	BSP_USE_RTOS

