#include	"App_Timer.h"

TIM_HandleTypeDef	*phTimer;
static	u32	timerTick = 0;

static void Timer_Base_MspInit()
{
	__HAL_RCC_TIM3_CLK_ENABLE();
}

static void Timer_PeriodElapsedCallback()
{
	timerTick += 10;
}

u8 Timer_Init()
{
	u8	result = 0, rtn;
	bspTimerFops_t	fops;
	HAL_StatusTypeDef	chkr;

	phTimer = (TIM_HandleTypeDef*)BSP_Timer_GetHandle(BSP_TIMER3);
	if(phTimer == NULL)			return result;

	memset(&fops, NULL, sizeof(fops));

	fops.Base_MspInit				= Timer_Base_MspInit;
	fops.PeriodElapsedCallback		= Timer_PeriodElapsedCallback;

	rtn = BSP_Timer_RegisterInterface(phTimer, &fops);
	if(rtn == 0)				return result;

	phTimer->Init.Prescaler				= ((SystemCoreClock / 2) / 1000000) - 1;
	phTimer->Init.CounterMode			= TIM_COUNTERMODE_UP;
	phTimer->Init.Period				= 10 - 1;
	phTimer->Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	phTimer->Init.RepetitionCounter		= 0;
	phTimer->Init.AutoReloadPreload		= TIM_AUTORELOAD_PRELOAD_DISABLE;

	chkr = HAL_TIM_Base_Init(phTimer);
	if(chkr == HAL_OK)					result = 1;

	HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	return result;
}

void Timer_Start()
{
	HAL_TIM_Base_Start_IT(phTimer);
}

void Timer_Stop()
{
	HAL_TIM_Base_Stop_IT(phTimer);
}

void Timer_TickClear()
{
	timerTick = 0;
}

u32 Timer_GetTick()
{
	return timerTick;
}

void Timer_Delay(u32 cnt)
{
	u32	sTick, rTick;

	sTick = Timer_GetTick();

	do{
		rTick = Timer_GetTick();
	}while((rTick - sTick) < cnt);
}

