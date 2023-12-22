#include "bsp.h"
#include "factory_io.h"

/*
 * BSP_LED_Init()
 * Initialize LED pin (PA5) as a High-Speed Push-Pull output
 * Set LED initial state to OFF
 */
extern uint8_t rx_dma_buffer[FRAME_LENGTH];
extern uint8_t tx_dma_buffer[FRAME_LENGTH];

/*
 * BSP_Console_Init()
 * USART2 @ 57600 Full Duplex
 * 1 start - 8-bit - 1 stop
 * TX -> PA2 (AF1)
 * RX -> PA3 (AF1)
 */

void BSP_Console_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA2 and PA3 as Alternate function
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);
	GPIOA->MODER |= (0x02 <<GPIO_MODER_MODER2_Pos) | (0x02 <<GPIO_MODER_MODER3_Pos);

	// Set PA2 and PA3 to AF1 (USART2)
	GPIOA->AFR[0] &= ~(0x0000FF00);
	GPIOA->AFR[0] |=  (0x00001100);

	// Enable USART2 clock
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;

	// Clear USART2 configuration (reset state)
	// 8-bit, 1 start, 1 stop, CTS/RTS disabled
	USART2->CR1 = 0x00000000;
	USART2->CR2 = 0x00000000;
	USART2->CR3 = 0x00000000;

	// Baud Rate = 57600
	// With OVER8=0 and Fck=1MHz, USARTDIV = 48E6/9600 = 17

	//USART2->CR1 |=USART_CR1_OVER8_Msk;
	USART2->BRR = 17;//For 1MHz

	// Enable both Transmitter and Receiver
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;

	// Setup TX on DMA Channel 4 //

	// Start DMA clock
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	// Reset DMA1 Channel 4 configuration
	DMA1_Channel4->CCR = 0x00000000;

	// Set direction Memory -> Peripheral
	DMA1_Channel4->CCR = DMA_CCR_DIR;

	// Peripheral is USART2 TDR
	DMA1_Channel4->CPAR = (uint32_t)&USART2->TDR;

	// Peripheral data size is 8-bit (byte)
	DMA1_Channel4->CCR |= (0x00 <<DMA_CCR_PSIZE_Pos);

	// Enable Transfer complete interrupt enable
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;

	// Memory is tx_dma_buffer
	DMA1_Channel4->CMAR = (uint32_t)tx_dma_buffer;

	// Memory data size is 8-bit (byte)
	DMA1_Channel4->CCR |= (0x00 <<DMA_CCR_MSIZE_Pos);

	// Enable auto-increment Memory address
	DMA1_Channel4->CCR |= DMA_CCR_MINC;

	// Setup RX on DMA Channel 5 //

	// Reset DMA1 Channel 5 configuration
	DMA1_Channel5->CCR = 0x00000000;

	// Set direction Peripheral -> Memory
	DMA1_Channel5->CCR &= ~DMA_CCR_DIR;

	// Peripheral is USART2 RDR
	DMA1_Channel5->CPAR = (uint32_t)&USART2->RDR;

	// Peripheral data size is 8-bit (byte)
	DMA1_Channel5->CCR |= (0x00 <<DMA_CCR_PSIZE_Pos);

	// Disable auto-increment Peripheral address
	DMA1_Channel5->CCR &= ~DMA_CCR_PINC;

	// Memory is rx_dma_buffer
	DMA1_Channel5->CMAR = (uint32_t)rx_dma_buffer;

	// Memory data size is 8-bit (byte)
	DMA1_Channel5->CCR |= (0x00 <<DMA_CCR_MSIZE_Pos);

	// Enable auto-increment Memory address
	DMA1_Channel5->CCR |= DMA_CCR_MINC;

	// Set Memory Buffer size
	DMA1_Channel5->CNDTR = FRAME_LENGTH;

	// DMA mode is circular
	DMA1_Channel5->CCR |= DMA_CCR_CIRC;

	// Enable DMA1 Channel 5
	DMA1_Channel5->CCR |= DMA_CCR_EN;

	// Enable USART2 DMA Request on RX
	USART2->CR3 |= USART_CR3_DMAR;

	// Enable USART2
	USART2->CR1 |= USART_CR1_UE;
}
