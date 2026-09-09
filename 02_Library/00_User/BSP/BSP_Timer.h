#ifndef	_BSP_TIMER_H
#define	_BSP_TIMER_H

#include	"BSP.h"

#define	BSP_TIMER_COUNT		14

typedef	struct{
	void	(* Base_MspInit)				(void);
	void	(* Base_MspDeInit)				(void);
	void	(* OC_MspInit)					(void);
	void	(* OC_MspDeInit)				(void);
	void	(* PWM_MspInit)					(void);
	void	(* PWM_MspDeInit)				(void);
	void	(* IC_MspInit)					(void);
	void	(* IC_MspDeInit)				(void);
	void	(* OnePulse_MspInit)			(void);
	void	(* OnePulse_MspDeInit)			(void);
	void	(* Encoder_MspInit)				(void);
	void	(* Encoder_MspDeInit)			(void);
	void	(* HallSensor_MspInit)			(void);
	void	(* HallSensor_MspDeInit)		(void);
	void	(* PeriodElapsedCallback)		(void);
	void	(* OC_DelayElapsedCallback)		(void);
	void	(* IC_CaptureCallback)			(void);
	void	(* PWM_PulseFinishedCallback)	(void);
	void	(* TriggerCallback)				(void);
	void	(* ErrorCallback)				(void);
	void	(* CommutationCallback)			(void);
	void	(* BreakCallback)				(void);
}bspTimerFops_t;

enum{
	BSP_TIMER1 = 0,
	BSP_TIMER2,
	BSP_TIMER3,
	BSP_TIMER4,
	BSP_TIMER5,
	BSP_TIMER6,
	BSP_TIMER7,
	BSP_TIMER8,
	BSP_TIMER9,
	BSP_TIMER10,
	BSP_TIMER11,
	BSP_TIMER12,
	BSP_TIMER13,
	BSP_TIMER14,
};

extern	TIM_HandleTypeDef	hTimer[BSP_TIMER_COUNT];

u32 BSP_Timer_GetHandle(u8 data);
u8 BSP_Timer_RegisterInterface(TIM_HandleTypeDef *pHandle, void *pVoid);

#endif	//	_BSP_TIMER_H
