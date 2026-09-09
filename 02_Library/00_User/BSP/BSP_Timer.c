#include	"BSP_Timer.h"

static	bspTimerFops_t	bspTimerFops[BSP_TIMER_COUNT];

TIM_HandleTypeDef	hTimer[BSP_TIMER_COUNT];

u8 BSP_Timer_DataInit()
{
	u8	result = 0;

	memset(bspTimerFops, NULL, sizeof(bspTimerFops));
	memset(hTimer, NULL, sizeof(hTimer));

	result = 1;

	return result;
}

u32 BSP_Timer_GetHandle(u8 data)
{
	u32	result = NULL;

	switch(data)
	{
		case BSP_TIMER1:
			hTimer[BSP_TIMER1].Instance		= TIM1;
			result = (u32)&hTimer[BSP_TIMER1];
			break;

		case BSP_TIMER2:
			hTimer[BSP_TIMER2].Instance		= TIM2;
			result = (u32)&hTimer[BSP_TIMER2];
			break;

		case BSP_TIMER3:
			hTimer[BSP_TIMER3].Instance		= TIM3;
			result = (u32)&hTimer[BSP_TIMER3];
			break;

		case BSP_TIMER4:
			hTimer[BSP_TIMER4].Instance		= TIM4;
			result = (u32)&hTimer[BSP_TIMER4];
			break;

		case BSP_TIMER5:
			hTimer[BSP_TIMER5].Instance		= TIM5;
			result = (u32)&hTimer[BSP_TIMER5];
			break;

		case BSP_TIMER6:
			hTimer[BSP_TIMER6].Instance		= TIM6;
			result = (u32)&hTimer[BSP_TIMER6];
			break;

		case BSP_TIMER7:
			hTimer[BSP_TIMER7].Instance		= TIM7;
			result = (u32)&hTimer[BSP_TIMER7];
			break;

		case BSP_TIMER8:
			hTimer[BSP_TIMER8].Instance		= TIM8;
			result = (u32)&hTimer[BSP_TIMER8];
			break;

		case BSP_TIMER9:
			hTimer[BSP_TIMER9].Instance		= TIM9;
			result = (u32)&hTimer[BSP_TIMER9];
			break;

		case BSP_TIMER10:
			hTimer[BSP_TIMER10].Instance	= TIM10;
			result = (u32)&hTimer[BSP_TIMER10];
			break;

		case BSP_TIMER11:
			hTimer[BSP_TIMER11].Instance	= TIM11;
			result = (u32)&hTimer[BSP_TIMER11];
			break;

		case BSP_TIMER12:
			hTimer[BSP_TIMER12].Instance	= TIM12;
			result = (u32)&hTimer[BSP_TIMER12];
			break;

		case BSP_TIMER13:
			hTimer[BSP_TIMER13].Instance	= TIM13;
			result = (u32)&hTimer[BSP_TIMER13];
			break;

		case BSP_TIMER14:
			hTimer[BSP_TIMER14].Instance	= TIM14;
			result = (u32)&hTimer[BSP_TIMER14];
			break;
	}

	return result;
}

u8 BSP_Timer_RegisterInterface(TIM_HandleTypeDef *pHandle, void *pVoid)
{
	u8	result = 1;

	if(pHandle->Instance == TIM1)			memcpy(&bspTimerFops[BSP_TIMER1], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM2)		memcpy(&bspTimerFops[BSP_TIMER2], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM3)		memcpy(&bspTimerFops[BSP_TIMER3], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM4)		memcpy(&bspTimerFops[BSP_TIMER4], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM5)		memcpy(&bspTimerFops[BSP_TIMER5], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM6)		memcpy(&bspTimerFops[BSP_TIMER6], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM7)		memcpy(&bspTimerFops[BSP_TIMER7], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM8)		memcpy(&bspTimerFops[BSP_TIMER8], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM9)		memcpy(&bspTimerFops[BSP_TIMER9], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM10)		memcpy(&bspTimerFops[BSP_TIMER10], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM11)		memcpy(&bspTimerFops[BSP_TIMER11], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM12)		memcpy(&bspTimerFops[BSP_TIMER12], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM13)		memcpy(&bspTimerFops[BSP_TIMER13], pVoid, sizeof(bspTimerFops_t));
	else if(pHandle->Instance == TIM14)		memcpy(&bspTimerFops[BSP_TIMER14], pVoid, sizeof(bspTimerFops_t));
	else									result = 0;

	return	result;
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].Base_MspInit != NULL)
	{
		bspTimerFops[cnt].Base_MspInit();
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].Base_MspDeInit != NULL)
	{
		bspTimerFops[cnt].Base_MspDeInit();
	}
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].OC_MspInit != NULL)
	{
		bspTimerFops[cnt].OC_MspInit();
	}
}

void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].OC_MspDeInit != NULL)
	{
		bspTimerFops[cnt].OC_MspDeInit();
	}
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].PWM_MspInit != NULL)
	{
		bspTimerFops[cnt].PWM_MspInit();
	}
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].PWM_MspDeInit != NULL)
	{
		bspTimerFops[cnt].PWM_MspDeInit();
	}
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].IC_MspInit != NULL)
	{
		bspTimerFops[cnt].IC_MspInit();
	}
}

void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].IC_MspDeInit != NULL)
	{
		bspTimerFops[cnt].IC_MspDeInit();
	}
}

void HAL_TIM_OnePulse_MspInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].OnePulse_MspInit != NULL)
	{
		bspTimerFops[cnt].OnePulse_MspInit();
	}
}

void HAL_TIM_OnePulse_MspDeInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].OnePulse_MspDeInit != NULL)
	{
		bspTimerFops[cnt].OnePulse_MspDeInit();
	}
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].Encoder_MspInit != NULL)
	{
		bspTimerFops[cnt].Encoder_MspInit();
	}
}

void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].Encoder_MspDeInit != NULL)
	{
		bspTimerFops[cnt].Encoder_MspDeInit();
	}
}

void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef* htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].HallSensor_MspInit != NULL)
	{
		bspTimerFops[cnt].HallSensor_MspInit();
	}
}

void HAL_TIMEx_HallSensor_MspDeInit(TIM_HandleTypeDef* htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].HallSensor_MspDeInit != NULL)
	{
		bspTimerFops[cnt].HallSensor_MspDeInit();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].PeriodElapsedCallback != NULL)
	{
		bspTimerFops[cnt].PeriodElapsedCallback();
	}
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].OC_DelayElapsedCallback != NULL)
	{
		bspTimerFops[cnt].OC_DelayElapsedCallback();
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].IC_CaptureCallback != NULL)
	{
		bspTimerFops[cnt].IC_CaptureCallback();
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].PWM_PulseFinishedCallback != NULL)
	{
		bspTimerFops[cnt].PWM_PulseFinishedCallback();
	}
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].TriggerCallback != NULL)
	{
		bspTimerFops[cnt].TriggerCallback();
	}
}

void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].ErrorCallback != NULL)
	{
		bspTimerFops[cnt].ErrorCallback();
	}
}

void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef* htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].CommutationCallback != NULL)
	{
		bspTimerFops[cnt].CommutationCallback();
	}
}

void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef* htim)
{
	u8	cnt;

	if(htim->Instance == TIM1)			cnt = BSP_TIMER1;
	else if(htim->Instance == TIM2)		cnt = BSP_TIMER2;
	else if(htim->Instance == TIM3)		cnt = BSP_TIMER3;
	else if(htim->Instance == TIM4)		cnt = BSP_TIMER4;
	else if(htim->Instance == TIM5)		cnt = BSP_TIMER5;
	else if(htim->Instance == TIM6)		cnt = BSP_TIMER6;
	else if(htim->Instance == TIM7)		cnt = BSP_TIMER7;
	else if(htim->Instance == TIM8)		cnt = BSP_TIMER8;
	else if(htim->Instance == TIM9)		cnt = BSP_TIMER9;
	else if(htim->Instance == TIM10)	cnt = BSP_TIMER10;
	else if(htim->Instance == TIM11)	cnt = BSP_TIMER11;
	else if(htim->Instance == TIM12)	cnt = BSP_TIMER12;
	else if(htim->Instance == TIM13)	cnt = BSP_TIMER13;
	else if(htim->Instance == TIM14)	cnt = BSP_TIMER14;
	else								return;

	if(bspTimerFops[cnt].BreakCallback != NULL)
	{
		bspTimerFops[cnt].BreakCallback();
	}
}


