#include "stm32f7xx_it.h"

#if	(BSP_USE_RTOS)
extern	void vPortSVCHandler();
extern	void xPortPendSVHandler();
extern	void xPortSysTickHandler();
#endif	//	BSP_USE_RTOS

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
	BSP_ERROR_Loop(BSP_ERROR_POS_HARDFAULT);
}

void MemManage_Handler(void)
{
	BSP_ERROR_Loop(BSP_ERROR_POS_MEMMANAGE);
}

void BusFault_Handler(void)
{
	BSP_ERROR_Loop(BSP_ERROR_POS_BUSFAULT);
}

void UsageFault_Handler(void)
{
	BSP_ERROR_Loop(BSP_ERROR_POS_USAGEFAULT);
}

void SVC_Handler(void)
{
#if	(BSP_USE_RTOS)
	vPortSVCHandler();
#endif	//	BSP_USE_RTOS
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
#if	(BSP_USE_RTOS)
	xPortPendSVHandler();
#endif	//	BSP_USE_RTOS
}

void SysTick_Handler(void)
{
#if	(BSP_USE_RTOS)
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		xPortSysTickHandler();
	}
#else
	HAL_IncTick();
	BSP_SysTick_Process();
#endif	//	BSP_USE_RTOS
}


void FMC_IRQHandler(void)
{
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER1]);
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER9]);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER1]);
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER10]);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER1]);
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER11]);
}

void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER1]);
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER2]);
}

void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER3]);
}

void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER4]);
}

void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER5]);
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER6]);
}

void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER7]);
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER8]);
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER12]);
}

void TIM8_UP_TIM13_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER8]);
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER13]);
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER8]);
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER14]);
}

void TIM8_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer[BSP_TIMER8]);
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void I2C1_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&hI2C[BSP_I2C1]);
}

void I2C1_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&hI2C[BSP_I2C1]);
}

void I2C2_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&hI2C[BSP_I2C2]);
}

void I2C2_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&hI2C[BSP_I2C2]);
}

void I2C3_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&hI2C[BSP_I2C3]);
}

void I2C3_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&hI2C[BSP_I2C3]);
}

void I2C4_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&hI2C[BSP_I2C4]);
}

void I2C4_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&hI2C[BSP_I2C4]);
}

void QUADSPI_IRQHandler(void)
{
	HAL_QSPI_IRQHandler(&hQSPI);
}

void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hSPI[BSP_SPI1]);
}

void SPI2_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hSPI[BSP_SPI2]);
}

void SPI3_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hSPI[BSP_SPI3]);
}

void SPI4_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hSPI[BSP_SPI4]);
}

void SPI5_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hSPI[BSP_SPI5]);
}

void SPI6_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hSPI[BSP_SPI6]);
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&hUART[BSP_UART1]);
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&hUART[BSP_UART2]);
}

void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&hUART[BSP_UART3]);
}

void UART4_IRQHandler(void)
{
	HAL_UART_IRQHandler(&hUART[BSP_UART4]);
}

void UART5_IRQHandler(void)
{
	HAL_UART_IRQHandler(&hUART[BSP_UART5]);
}

void USART6_IRQHandler(void)
{
	HAL_UART_IRQHandler(&hUART[BSP_UART6]);
}

void UART7_IRQHandler(void)
{
	HAL_UART_IRQHandler(&hUART[BSP_UART7]);
}

void UART8_IRQHandler(void)
{
	HAL_UART_IRQHandler(&hUART[BSP_UART8]);
}

void OTG_FS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hUSB_FS);
}

void OTG_HS_EP1_OUT_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hUSB_HS);
}

void OTG_HS_EP1_IN_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hUSB_HS);
}

void OTG_HS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hUSB_HS);
}

void SDMMC1_IRQHandler(void)
{
	HAL_SD_IRQHandler(&hSD[BSP_SD1]);
}

void SDMMC2_IRQHandler(void)
{
	HAL_SD_IRQHandler(&hSD[BSP_SD2]);
}

void ETH_IRQHandler(void)
{
	HAL_ETH_IRQHandler(&hEthernet);
}

void DMA1_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA1_S0]);
}

void DMA1_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA1_S1]);
}

void DMA1_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA1_S2]);
}

void DMA1_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA1_S3]);
}

void DMA1_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA1_S4]);
}

void DMA1_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA1_S5]);
}

void DMA1_Stream6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA1_S6]);
}

void DMA1_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA1_S7]);
}

void DMA2_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA2_S0]);
}

void DMA2_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA2_S1]);
}

void DMA2_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA2_S2]);
}

void DMA2_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA2_S3]);
}

void DMA2_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA2_S4]);
}

void DMA2_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA2_S5]);
}

void DMA2_Stream6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA2_S6]);
}

void DMA2_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hDMA[BSP_DMA2_S7]);
}



//	NOT USE

void ETH_WKUP_IRQHandler(void)
{
}

void ADC_IRQHandler(void)
{
}

void JPEG_IRQHandler(void)
{
}

void OTG_FS_WKUP_IRQHandler(void)
{
}

void CAN1_TX_IRQHandler(void)
{
}

void OTG_HS_WKUP_IRQHandler(void)
{
}

void CAN1_RX0_IRQHandler(void)
{
}

void CAN1_RX1_IRQHandler(void)
{
}

void CAN1_SCE_IRQHandler(void)
{
}

void CAN2_TX_IRQHandler(void)
{
}

void CAN2_RX0_IRQHandler(void)
{
}

void CAN2_RX1_IRQHandler(void)
{
}

void CAN2_SCE_IRQHandler(void)
{
}

void CAN3_TX_IRQHandler(void)
{
}

void CAN3_RX0_IRQHandler(void)
{
}

void CAN3_RX1_IRQHandler(void)
{
}

void CAN3_SCE_IRQHandler(void)
{
}

void WWDG_IRQHandler(void)
{
}

void PVD_IRQHandler(void)
{
}

void TAMP_STAMP_IRQHandler(void)
{
}

void RTC_WKUP_IRQHandler(void)
{
}

void RTC_Alarm_IRQHandler(void)
{
}

void FLASH_IRQHandler(void)
{
}

void RCC_IRQHandler(void)
{
}

void DCMI_IRQHandler(void)
{
}

void RNG_IRQHandler(void)
{
}

void FPU_IRQHandler(void)
{
}

void SAI1_IRQHandler(void)
{
}

void SAI2_IRQHandler(void)
{
}

void LTDC_IRQHandler(void)
{
}

void LTDC_ER_IRQHandler(void)
{
}

void DMA2D_IRQHandler(void)
{
}

void LPTIM1_IRQHandler(void)
{
}

void CEC_IRQHandler(void)
{
}

void SPDIF_RX_IRQHandler(void)
{
}

void DFSDM1_FLT0_IRQHandler(void)
{
}

void DFSDM1_FLT1_IRQHandler(void)
{
}

void DFSDM1_FLT2_IRQHandler(void)
{
}

void DFSDM1_FLT3_IRQHandler(void)
{
}

void MDIOS_IRQHandler(void)
{
}

