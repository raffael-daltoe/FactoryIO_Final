#include "main.h"
#include "stm32f0xx_it.h"
extern xSemaphoreHandle xSem_DMA_TC;

void DMA1_Channel4_5_6_7_IRQHandler(){
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if((DMA1->ISR & DMA_ISR_TCIF4_Msk) != 0){
		DMA1->IFCR = DMA_IFCR_CTCIF4;
		xSemaphoreGiveFromISR(xSem_DMA_TC, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}