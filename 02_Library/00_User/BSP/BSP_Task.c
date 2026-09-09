#include	"BSP_Task.h"

#if	(BSP_USE_RTOS)
static	TaskHandle_t		bspTask_TickProcess, bspTask_Reboot;
static	SemaphoreHandle_t	bspTask_SempReboot;

static void BSP_Task_TickProcess(void *pVoid)
{
	TickType_t	lastWakeTime;

	lastWakeTime = xTaskGetTickCount();

	while(1)
	{
		BSP_SysTick_Process();

		vTaskDelayUntil(&lastWakeTime, 50);
	}
}

static void BSP_Task_Reboot(void *pVoid)
{
	bspTask_SempReboot = xSemaphoreCreateBinary();

	if(bspTask_SempReboot == NULL)
	{
		//	Semephore Create Fail, This Task Delete.

		vTaskDelete(NULL);
	}

	xSemaphoreTake(bspTask_SempReboot, portMAX_DELAY);

	vTaskDelay(1000);

	HAL_NVIC_SystemReset();
}

void BSP_Task_SystemReboot()
{
	if(bspTask_SempReboot != NULL)
	{
		xSemaphoreGive(bspTask_SempReboot);
	}
}

u8 BSP_Task_Init()
{
	u8	result = 0, errCnt = 0;
	BaseType_t	rtn;

	rtn = xTaskCreate(BSP_Task_Reboot, "Task_Reboot", configMINIMAL_STACK_SIZE, NULL, BSP_TASK_PRIORITY_VERY_CRITICAL, &bspTask_Reboot);
	if(rtn != pdTRUE)		errCnt += 1;

	rtn = xTaskCreate(BSP_Task_TickProcess, "Task_TickProcess", configMINIMAL_STACK_SIZE, NULL, BSP_TASK_PRIORITY_IDLE, &bspTask_TickProcess);
	if(rtn != pdTRUE)		errCnt += 1;

	if(errCnt == 0)			result = 1;
	
	return result;
}
#endif	//	BSP_USE_RTOS