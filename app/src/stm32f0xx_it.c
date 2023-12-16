/**
 ******************************************************************************
 * @file    Templates/Src/stm32f0xx_it.c
 * @author  MCD Application Team
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"


/** @addtogroup STM32F0xx_HAL_Examples
 * @{
 */

/** @addtogroup Templates
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}



/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
 * This function handles EXTI line 13 interrupt request.
 */

extern xSemaphoreHandle xSem_UART_TC;
void USART2_IRQHandler()
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	// Test for TXE pending interrupt
	if ((USART2->ISR & USART_ISR_TC) == USART_ISR_TC)
	{
		// We must clear the TC flag when reading the RDR
		USART2->ICR |= USART_ICR_TCCF;

		// Release the semaphore
		xSemaphoreGiveFromISR(xSem_UART_TC, &xHigherPriorityTaskWoken);

		// Perform a context switch to the waiting task
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}

}

/*
 * This function handles DMA1 Channel 4 (USART2 TX) interrupts
 */
extern xSemaphoreHandle xSem_DMA_TC;

void DMA1_Channel4_5_6_7_IRQHandler()
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	// Test for Channel 4 Transfer Complete
	if ((DMA1->ISR & DMA_ISR_TCIF4) == DMA_ISR_TCIF4)
	{
		// Clear the interrupt pending bit
		DMA1->IFCR |= DMA_IFCR_CTCIF4;

		// Release the semaphore
		xSemaphoreGiveFromISR(xSem_DMA_TC, &xHigherPriorityTaskWoken);

		// Perform a context switch to the waiting task
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}

/**
 * @}
 */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
