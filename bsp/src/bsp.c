/*
 * bsp.c
 *  Created on: 28 févr. 2023
 */
#include "bsp.h"
#include "FreeRTOS.h"
#include "factory_io.h"

/*
 * BSP_LED_Init()
 * Initialize LED pin (PA5) as a High-Speed Push-Pull output
 * Set LED initial state to OFF
 */

void BSP_LED_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA5 as output
	GPIOA->MODER &= ~GPIO_MODER_MODER5_Msk;
	GPIOA->MODER |= (0x01 << GPIO_MODER_MODER5_Pos);

	// Configure PA5 as Push-Pull output
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;

	// Configure PA5 as High-Speed Output
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR5_Msk;
	GPIOA->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR5_Pos);

	// Disable PA5 Pull-up/Pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5_Msk;

	// Set Initial State OFF
	GPIOA->BSRR |= GPIO_BSRR_BR_5;
}

/*
 * BSP_LED_On()
 * Turn ON LED on PA5
 */

void BSP_LED_On()
{
	GPIOA->BSRR = GPIO_BSRR_BS_5;
}

/*
 * BSP_LED_Off()
 * Turn OFF LED on PA5
 */

void BSP_LED_Off()
{
	GPIOA->BSRR = GPIO_BSRR_BR_5;
}

/*
 * BSP_LED_Toggle()
 * Toggle LED on PA5
 */

void BSP_LED_Toggle()
{
	GPIOA->ODR ^= GPIO_ODR_5;
}

/*
 * BSP_PB_Init()
 * Initialize Push-Button pin (PC13) as input without Pull-up/Pull-down
 * Enable EXTI13 on falling edge
 */

void BSP_PB_Init()
{
	// Enable GPIOC clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Configure PC13 as input
	GPIOC->MODER &= ~GPIO_MODER_MODER13_Msk;
	GPIOC->MODER |= (0x00 << GPIO_MODER_MODER13_Pos);

	// Disable PC13 Pull-up/Pull-down
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13_Msk;

	// Enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Select Port C as interrupt source for EXTI line 13
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_Msk;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	// Enable EXTI line 13
	EXTI->IMR |= EXTI_IMR_IM13;

	// Disable Rising / Enable Falling trigger
	EXTI->RTSR &= ~EXTI_RTSR_RT13;
	EXTI->FTSR |= EXTI_FTSR_FT13;
}

/*
 * BSP_PB_GetState()
 * Returns the state of the button (0=released, 1=pressed)
 */

uint8_t BSP_PB_GetState()
{
	uint8_t state;

	if ((GPIOC->IDR & GPIO_IDR_13) == GPIO_IDR_13)
	{
		state = 0;
	}
	else
	{
		state = 1;
	}

	return state;
}

/*
 * BSP_Console_Init()
 * USART2 @ 115200 Full Duplex
 * 1 start - 8-bit - 1 stop
 * TX -> PA2 (AF1)
 * RX -> PA3 (AF1)
 */

extern uint8_t tx_dma_buffer[7];
extern uint8_t rx_dma_buffer[FRAME_LENGTH];

void BSP_Console_Init()
{
	
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA2 and PA3 as Alternate function
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER2_Pos) | (0x02 << GPIO_MODER_MODER3_Pos);

	// Set PA2 and PA3 to AF1 (USART2)
	GPIOA->AFR[0] &= ~(0x0000FF00);
	GPIOA->AFR[0] |= (0x00001100);

	// Enable USART2 clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// Clear USART2 configuration (reset state)
	// 8-bit, 1 start, 1 stop, CTS/RTS disabled
	USART2->CR1 = 0x00000000;
	USART2->CR2 = 0x00000000;
	USART2->CR3 = 0x00000000;

	// Select PCLK (APB1) as clock source
	// PCLK -> 48 MHz
	RCC->CFGR3 &= ~RCC_CFGR3_USART2SW_Msk;

	// Baud Rate = 115200
	// With OVER8=1 and Fck=48MHz, USARTDIV = 2*48E6/115200 = 833.3333
	// BRR = 833 -> Actual BaudRate = 115246.0984 -> 0.04% error (better choice)

	USART2->CR1 &= ~USART_CR1_OVER8;
	// USART2->BRR = 833;
	USART2->BRR = 5000; // 9600 bauds
	// Permet de ne pas bloquer l'USART quand on tape un texte
	USART2->CR3 |= USART_CR3_OVRDIS;

	// Enable Transfer Complete ISR
	USART2->CR1 |= USART_CR1_TCIE;

	// Enable both Transmitter and Receiver
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;

	// Start DMA clock
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;







	
	/*
	 * DMA en reception RX channel 5
	 */
	// Reset DMA1 Channel 5 configuration
	DMA1_Channel5->CCR = 0x00000000;

	// Set direction Peripheral -> Memory
	DMA1_Channel5->CCR &= ~DMA_CCR_DIR;

	// Peripheral is USART2 RDR
	DMA1_Channel5->CPAR = (uint32_t)&USART2->RDR;

	// Peripheral data size is 8-bit (byte)
	DMA1_Channel5->CCR |= (0x00 << DMA_CCR_PSIZE_Pos);

	// Disable auto-increment Peripheral address
	DMA1_Channel5->CCR &= ~DMA_CCR_PINC;

	// Memory is rx_dma_buffer
	DMA1_Channel5->CMAR = (uint32_t)rx_dma_buffer;

	// Memory data size is 8-bit (byte)
	DMA1_Channel5->CCR |= (0x00 << DMA_CCR_MSIZE_Pos);

	// Enable auto-increment Memory address
	DMA1_Channel5->CCR |= DMA_CCR_MINC;

	// Set Memory Buffer size
	DMA1_Channel5->CNDTR = 14;

	// DMA mode is circular
	DMA1_Channel5->CCR |= DMA_CCR_CIRC;

	// Enable DMA1 Channel 5
	DMA1_Channel5->CCR |= DMA_CCR_EN;

	// Enable USART2 DMA Request on RX
	USART2->CR3 |= USART_CR3_DMAR;

	/*
	 * DMA en transmission TX channel 4
	 */

	// Reset DMA1 Channel 4 configuration
	DMA1_Channel4->CCR = 0x00000000;

	// Set direction Memory -> Peripheral
	DMA1_Channel4->CCR |= DMA_CCR_DIR;

	// Peripheral is USART2 TDR
	DMA1_Channel4->CPAR = (uint32_t)&USART2->TDR;

	// Peripheral data size is 8-bit (byte)
	DMA1_Channel4->CCR |= (0x00 << DMA_CCR_PSIZE_Pos);

	// Disable auto-increment Peripheral address
	DMA1_Channel4->CCR &= ~DMA_CCR_PINC;

	// Memory is tx_dma_buffer
	DMA1_Channel4->CMAR = (uint32_t)&tx_dma_buffer;

	// Memory data size is 8-bit (byte)
	DMA1_Channel4->CCR |= (0x00 << DMA_CCR_MSIZE_Pos);

	// Enable auto-increment Memory address
	DMA1_Channel4->CCR |= DMA_CCR_MINC;

	// DMA mode is circular disable
	// DMA1_Channel4->CCR &= ~DMA_CCR_CIRC;				AQQQQ

	// Enable DMA TC interrupts
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;

	// Enable USART2
	USART2->CR1 |= USART_CR1_UE;
}

/*
 * ADC_Init()
 * Initialize ADC for single channel conversion
 * - Channel 11 -> pin PC1
 */
void BSP_ADC_Init()
{
	// Enable GPIOC clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Configure pin PC1 as analog
	GPIOC->MODER &= ~(GPIO_MODER_MODER1_Msk);
	GPIOC->MODER |= (0x03 << GPIO_MODER_MODER1_Pos);

	// Enable ADC clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	// Reset ADC configuration
	ADC1->CR = 0x00000000;
	ADC1->CFGR1 = 0x00000000;
	ADC1->CFGR2 = 0x00000000;
	ADC1->CHSELR = 0x00000000;

	// Enable 12-bit resolution, continuous conversion mode
	ADC1->CFGR1 |= (0x00 << ADC_CFGR1_RES_Pos) | ADC_CFGR1_CONT;

	// Select PCLK/2 as ADC clock
	ADC1->CFGR2 |= (0x01 << ADC_CFGR2_CKMODE_Pos);

	// Set sampling time to 28.5 ADC clock cycles
	ADC1->SMPR = 0x03;

	// Select channel 11
	ADC1->CHSELR |= ADC_CHSELR_CHSEL11;

	// Enable ADC
	ADC1->CR |= ADC_CR_ADEN;

	// Start conversion
	ADC1->CR |= ADC_CR_ADSTART;
}

void BSP_TIMER_IC_Init()
{
	// Enable GPIOB clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Configure PB4 as Alternate function
	GPIOB->MODER &= ~(GPIO_MODER_MODER4_Msk);
	GPIOB->MODER |= (0x02 << GPIO_MODER_MODER4_Pos);

	// Set PB4 to AF1 (TIM3_CH1)
	GPIOB->AFR[0] &= ~(0x000F0000);
	GPIOB->AFR[0] |= (0x00010000);

	// Enable TIM3 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// Reset TIM3 configuration
	TIM3->CR1 = 0x0000;
	TIM3->CR2 = 0x0000;
	TIM3->CCER = 0x0000;

	// Set TIM3 prescaler
	// Fck = 48MHz -> /48000 = 1KHz counting frequency
	TIM3->PSC = (uint16_t)48000 - 1;

	// Set Auto-Reload to maximum value
	TIM3->ARR = (uint16_t)0xFFFF;

	// Setup Input Capture
	TIM3->CCMR1 = 0x0000;
	TIM3->CCMR2 = 0x0000;

	// Channel 1 input on TI1
	TIM3->CCMR1 |= (0x01 << TIM_CCMR1_CC1S_Pos);

	// Channel 2 input also on TI1
	TIM3->CCMR1 |= (0x02 << TIM_CCMR1_CC2S_Pos);

	// Filter with N=8
	TIM3->CCMR1 |= (0x03 << TIM_CCMR1_IC1F_Pos) | (0x03 << TIM_CCMR1_IC2F_Pos);

	// Select falling edge for channel 1
	TIM3->CCER |= (0x00 << TIM_CCER_CC1NP_Pos) | (0x01 << TIM_CCER_CC1P_Pos);

	// Select rising edge for channel 2
	TIM3->CCER |= (0x00 << TIM_CCER_CC2NP_Pos) | (0x00 << TIM_CCER_CC2P_Pos);

	// Enable capture on channel 1 & channel 2
	TIM3->CCER |= (0x01 << TIM_CCER_CC1E_Pos) | (0x01 << TIM_CCER_CC2E_Pos);

	// Choose Channel 1 as trigger input
	TIM3->SMCR |= (0x05 << TIM_SMCR_TS_Pos);

	// Slave mode -> Resets counter when trigger occurs
	TIM3->SMCR |= (0x4 << TIM_SMCR_SMS_Pos);

	// Enable TIM3
	TIM3->CR1 |= TIM_CR1_CEN;
}

/*
 * BSP_TIMER_PWM_Init()
 * TIM1 as Output Compare PWM mode
 * Channel 1 -> PA8 (AF2)
 * Channel 2 -> PA9 (AF2)
 */

void BSP_TIMER_PWM_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA8 and PA9 as Alternate Function
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_Msk | GPIO_MODER_MODER9_Msk);
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER8_Pos) | (0x02 << GPIO_MODER_MODER9_Pos);

	// Set PA8 and PA9 to AF2 (TIM1)
	GPIOA->AFR[1] &= ~(0x000000FF);
	GPIOA->AFR[1] |= (0x00000022);

	// Enable TIM1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	// Reset TIM1 configuration
	TIM1->CR1 = 0x0000;
	TIM1->CR2 = 0x0000;
	TIM1->CCER = 0x0000;

	// Set TIM1 prescaler
	// Fck = 48MHz -> /48 = 1MHz counting frequency (1µs resolution)
	TIM1->PSC = (uint16_t)48 - 1;

	// Set Auto-Reload to period = 11ms
	TIM1->ARR = (uint16_t)11000;

	// Enable Auto-Reload Preload register
	TIM1->CR1 |= TIM_CR1_ARPE;

	// Setup Input Capture
	TIM1->CCMR1 = 0x0000;
	TIM1->CCMR2 = 0x0000;

	// Setup PWM mode 1 output
	TIM1->CCMR1 |= (0x06 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;
	TIM1->CCMR1 |= (0x06 << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE;

	// Set default PWM values
	TIM1->CCR1 = 1500;
	TIM1->CCR2 = 1500;

	// Enable Outputs
	TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

	// Enable Main output
	TIM1->BDTR |= TIM_BDTR_MOE;

	// Enable TIM1
	TIM1->CR1 |= TIM_CR1_CEN;
}

/*
 * DAC_Init()
 * Initialize DAC for a single output
 * on channel 1 -> pin PA4
 */

void BSP_DAC_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure pin PA4 as analog
	GPIOA->MODER &= ~GPIO_MODER_MODER4_Msk;
	GPIOA->MODER |= (0x03 << GPIO_MODER_MODER4_Pos);

	// Enable DAC clock
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;

	// Reset DAC configuration
	DAC->CR = 0x00000000;

	// Enable Channel 1
	DAC->CR |= DAC_CR_EN1;
}

/*
 * BSP_TIMER_Timebase_Init()
 * TIM6 at 48MHz
 * Prescaler   = 48000  -> Counting frequency is 1kHz
 * Auto-reload = 10 	-> Update period is 10ms
 */
void BSP_TIMER_Timebase_Init()
{
	// Enable TIM6 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	// Reset TIM6 configuration
	TIM6->CR1 = 0x0000;
	TIM6->CR2 = 0x0000;

	// Set TIM6 prescaler
	// Fck = 48MHz -> /48000 = 1KHz counting frequency
	TIM6->PSC = (uint16_t) 48000 -1;

	// Set TIM6 auto-reload register for 1s
	TIM6->ARR = (uint16_t) 1000 -1;

	// Enable auto-reload preload
	TIM6->CR1 |= TIM_CR1_ARPE;

	// Start TIM6 counter
	TIM6->CR1 |= TIM_CR1_CEN;
}

/*
 * BSP_I2C1_Init()
 * Initialize I2C1 peripheral @100kHz
 */
void BSP_I2C1_Init()
{
	// Pin configuration for I2C2 pins
	// SCL -> PB8
	// SDA -> PB9

	// Enable GPIOB clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Configure PB8, PB9 as AF mode
	GPIOB->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
	GPIOB->MODER |= (0x02 << 16U) | (0x02 << 18U);

	// Connect to I2C1 (AF1)
	GPIOB->AFR[1] &= ~(0x000000FF);
	GPIOB->AFR[1] |= 0x00000011;

	// Setup Open-Drain
	GPIOB->OTYPER |= GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9;

	// Select SYSCLK as I2C1 clock (48MHz)
	RCC->CFGR3 |= RCC_CFGR3_I2C1SW;

	// Enable I2C1 clock
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	// Make sure I2C1 is disabled
	I2C1->CR1 &= ~I2C_CR1_PE;

	// Reset I2C1 Configuration to default values
	I2C1->CR1 = 0x00000000;
	I2C1->CR2 = 0x00000000;
	I2C1->TIMINGR = 0x00000000;

	// Configure timing for 100kHz, 50% duty cycle
	I2C1->TIMINGR |= ((4 - 1) << I2C_TIMINGR_PRESC_Pos); // Clock prescaler /4 -> 12MHz
	I2C1->TIMINGR |= (60 << I2C_TIMINGR_SCLH_Pos);		 // High half-period = 5µs
	I2C1->TIMINGR |= (60 << I2C_TIMINGR_SCLL_Pos);		 // Low  half-period = 5µs

	// Enable I2C1
	I2C1->CR1 |= I2C_CR1_PE;
}

void BSP_DBG_Pins_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure pin PA0 as General output purpose
	GPIOA->MODER &= ~GPIO_MODER_MODER0_Msk;
	GPIOA->MODER |= (0x01 << GPIO_MODER_MODER0_Pos);

	// Setup as Push Pull (reset state) PA0
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_0;

	// Configure pin PA1 as General output purpose
	GPIOA->MODER &= ~GPIO_MODER_MODER1_Msk;
	GPIOA->MODER |= (0x01 << GPIO_MODER_MODER1_Pos);

	// Setup as Push Pull (reset state) PA1
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1;

	// Configure pin PA4 as General output purpose
	GPIOA->MODER &= ~GPIO_MODER_MODER4_Msk;
	GPIOA->MODER |= (0x01 << GPIO_MODER_MODER4_Pos);

	// Setup as Push Pull (reset state) PA4
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;

	// Enable GPIOC clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Configure pin PC1 as General output purpose
	GPIOC->MODER &= ~GPIO_MODER_MODER1_Msk;
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER1_Pos);

	// Setup as Push Pull (reset state)
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_1;
}

uint8_t BSP_I2C1_Read(uint8_t device_address,
					  uint8_t register_address,
					  uint8_t *buffer,
					  uint8_t nbytes)
{
	uint32_t timeout; // Flag waiting timeout
	uint8_t n;		  // Loop counter

	// Set device address
	I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
	I2C1->CR2 |= ((device_address << 1U) << I2C_CR2_SADD_Pos);

	// Set I2C in Write mode
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;

	// Transfer NBYTES = 1, no AUTOEND
	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= (1 << 16U);
	I2C1->CR2 &= ~I2C_CR2_AUTOEND;

	// Start I2C transaction
	I2C1->CR2 |= I2C_CR2_START;

	// Wait for TXIS with timeout
	timeout = 100000;
	while (((I2C1->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS)
	{
		timeout--;
		if (timeout == 0)
			return 1;
	}

	// Send Register address
	I2C1->TXDR = register_address;

	// Wait for TC with timeout
	timeout = 100000;
	while (((I2C1->ISR) & I2C_ISR_TC) != I2C_ISR_TC)
	{
		timeout--;
		if (timeout == 0)
			return 2;
	}

	// Set I2C in Read mode
	I2C1->CR2 |= I2C_CR2_RD_WRN;

	// Transfer NBYTES, no AUTOEND
	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= (nbytes << 16U);
	I2C1->CR2 &= ~I2C_CR2_AUTOEND;

	// Re-Start transaction
	I2C1->CR2 |= I2C_CR2_START;

	n = nbytes;

	while (n > 0)
	{
		// Wait for RXNE with timeout
		timeout = 100000;
		while (((I2C1->ISR) & I2C_ISR_RXNE) != I2C_ISR_RXNE)
		{
			timeout--;
			if (timeout == 0)
				return 3;
		}

		// Store data into buffer
		*buffer = I2C1->RXDR;
		buffer++;
		n--;
	}

	// Generate STOP condition
	I2C1->CR2 |= I2C_CR2_STOP;

	// Wait for STOPF with timeout
	timeout = 100000;
	while (((I2C1->ISR) & I2C_ISR_STOPF) != I2C_ISR_STOPF)
	{
		timeout--;
		if (timeout == 0)
			return 4;
	}

	// Return success
	return 0;
}

uint8_t BSP_I2C1_Write(uint8_t device_address,
					   uint8_t register_address,
					   uint8_t *buffer, uint8_t nbytes)
{
	uint32_t timeout; // Flag waiting timeout
	uint8_t n;		  // Loop counter

	// Set device address
	I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
	I2C1->CR2 |= ((device_address << 1U) << I2C_CR2_SADD_Pos);

	// Set I2C in Write mode
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;

	// Transfer NBYTES, with AUTOEND
	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= ((nbytes + 1) << 16U);
	I2C1->CR2 |= I2C_CR2_AUTOEND;

	// Clear STOPF flag
	I2C1->ICR |= I2C_ICR_STOPCF;

	// Start I2C transaction
	I2C1->CR2 |= I2C_CR2_START;

	// Wait for TXIS with timeout
	timeout = 100000;
	while (((I2C1->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS)
	{
		timeout--;
		if (timeout == 0)
			return 1;
	}

	// Send register address
	I2C1->TXDR = register_address;

	n = nbytes;

	while (n > 0)
	{
		// Wait for TXIS with timeout
		timeout = 100000;
		while (((I2C1->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS)
		{
			timeout--;
			if (timeout == 0)
				return 2;
		}

		// Send data
		I2C1->TXDR = *buffer;
		buffer++;
		n--;
	}

	// Wait for STOPF with timeout
	timeout = 100000;
	while (((I2C1->ISR) & I2C_ISR_STOPF) != I2C_ISR_STOPF)
	{
		timeout--;
		if (timeout == 0)
			return 3;
	}

	// Return success
	return 0;
}

/*
 * BSP_SPI1_Init()
 * Initialize SPI1 peripheral @375kHz
 */
void BSP_SPI1_Init()
{
	// SPI_SCK  -> PB3 (AF0)
	// SPI_MISO -> PB4 (AF0)
	// SPI_MOSI -> PB5 (AF0)
	// CS pin	-> PC7 (GPIO)

	// Configure PC7 as CS pin

	// Enable GPIOC clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Configure PC7 as output
	GPIOC->MODER &= ~GPIO_MODER_MODER7_Msk;
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER7_Pos);

	// Configure PC7 as Push-Pull output
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_7;

	// Configure PC7 as High-Speed Output
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR7_Msk;
	GPIOC->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR7_Pos);

	// Disable PC7 Pull-up/Pull-down
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR7_Msk;

	// Idle state is high
	GPIOC->BSRR = GPIO_BSRR_BS_7;

	// Configure PB3, PB5, PB5 as SPI1 pins (AF0)

	// Enable GPIOB clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Configure PB3, PB4, PB5 as AF mode
	GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
	GPIOB->MODER |= (0x02 << 6U) | (0x02 << 8U) | (0x02 << 10U);

	// Connect to SPI1 (AF0)
	GPIOB->AFR[0] &= ~(0x00FFF000);
	GPIOB->AFR[0] |= (0x00000000);

	// Enable SPI1 Clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// Configure SPI with default config
	// 4-wire = full-duplex (MISO/MOSI on separated lines)
	// 8-bit, no CRC, MSB first
	// CPHA = 0, CPOL = 0 -> SPI mode 0 (first clock rising edge)
	SPI1->CR1 = 0x0000;
	SPI1->CR2 = 0x0000;

	//	// Set the baudrate to 48MHz /128 = 375kHz (slow, but easy to debug)
	//	SPI1->CR1 |= 0x06 <<SPI_CR1_BR_Pos;

	// Set the baudrate to 48MHz /8 = 6MHz
	SPI1->CR1 |= 0x02 << SPI_CR1_BR_Pos;

	// Set data size to 8-bit
	SPI1->CR2 |= 0x07 << SPI_CR2_DS_Pos;

	// Set as master (SSI must be high), with software managed NSS
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM;

	// Enable SPI1
	SPI1->CR1 |= SPI_CR1_SPE;
}

uint8_t BSP_SPI_SendReceive(uint8_t tx_byte)
{
	uint8_t rx_byte;

	// Make sure TXE is set before sending data
	while ((SPI1->SR & SPI_SR_TXE_Msk) == 0)
		;

	// Send tx_byte
	*(__IO uint8_t *)&SPI1->DR = tx_byte;

	// Wait until incoming data has arrived
	while ((SPI1->SR & SPI_SR_RXNE_Msk) == 0)
		;

	// Read data
	rx_byte = *(__IO uint8_t *)&SPI1->DR;

	return rx_byte;
}

void BSP_LPS25H_Read(uint8_t register_address, uint8_t *buffer, uint8_t nbytes)
{
	uint8_t ms_bit_msk;
	uint8_t n;

	// Set the M/S bit depending on the number of byte to read
	if (nbytes > 1)
		ms_bit_msk = 0x40; // bit-6 mask
	else
		ms_bit_msk = 0x00;

	// Set FIFO threshold to 1-byte
	SPI1->CR2 |= SPI_CR2_FRXTH;

	// Select slave (CS -> low)
	GPIOC->BSRR = GPIO_BSRR_BR_7;

	// Send register address to read from
	BSP_SPI_SendReceive(register_address | 0x80 | ms_bit_msk);

	// Read loop
	n = nbytes;

	while (n > 0)
	{
		*buffer = BSP_SPI_SendReceive(0x00);
		buffer++;
		n--;
	}

	// Release slave (CS -> High)
	GPIOC->BSRR = GPIO_BSRR_BS_7;
}

void BSP_LPS25H_Write(uint8_t register_address, uint8_t data)
{
	// Set FIFO threshold to 1-byte
	SPI1->CR2 |= SPI_CR2_FRXTH;

	// Select slave (CS -> low)
	GPIOC->BSRR = GPIO_BSRR_BR_7;

	// Send register address to write to
	BSP_SPI_SendReceive(register_address);

	// Send data to write
	BSP_SPI_SendReceive(data);

	// Release slave (CS -> High)
	GPIOC->BSRR = GPIO_BSRR_BS_7;
}

void BSP_LPS25H_ReadFast(uint8_t register_address, uint8_t *buffer, uint8_t nbytes)
{
	uint8_t ms_bit_msk;
	uint8_t n_tx, n_rx;

	// Set the M/S bit depending on the number of byte to read
	if (nbytes > 1)
		ms_bit_msk = 0x40;
	else
		ms_bit_msk = 0x00;

	// Set FIFO threshold to 1-byte
	SPI1->CR2 |= SPI_CR2_FRXTH;

	// Select slave (CS -> low)
	GPIOC->BSRR = GPIO_BSRR_BR_7;

	n_tx = nbytes + 1; // Add 1 byte to take register address into account
	n_rx = nbytes + 1;

	while (n_rx > 0)
	{
		// Write to TX_FIFO if possible
		if (((SPI1->SR & SPI_SR_TXE_Msk) != 0) & (n_tx > 0))
		{
			if (n_tx == (nbytes + 1))
			{
				// If this is first byte, send register address
				*(__IO uint8_t *)&SPI1->DR = (register_address | 0x80 | ms_bit_msk);
			}
			else
			{
				// Otherwise send zeros
				*(__IO uint8_t *)&SPI1->DR = 0x00;
			}
			n_tx--;
		}

		// Read from RX_FIFO if not empty
		if ((SPI1->SR & SPI_SR_RXNE_Msk) != 0)
		{
			if (n_rx == (nbytes + 1))
			{
				// Dummy access to SPI_DR (first byte)
				*(__IO uint8_t *)&SPI1->DR;
			}
			else
			{
				// Read from slave
				*buffer = *(__IO uint8_t *)&SPI1->DR;
				buffer++;
			}
			n_rx--;
		}
	}

	// Release slave (CS -> High)
	GPIOC->BSRR = GPIO_BSRR_BS_7;
}

/*
 * BSP_NVIC_Init()
 * Setup NVIC controller for desired interrupts
 */

void BSP_NVIC_Init()
{
	// Remove interrupt channel settings from here
}

/*
 * BSP_DBG_Pin_Init()
 * Initialize Debug pin (PA6) as a High-Speed Push-Pull output
 * Set pin initial state to OFF
 */
void BSP_DBG_Pin_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA6 as output
	GPIOA->MODER &= ~GPIO_MODER_MODER6_Msk;
	GPIOA->MODER |= (0x01 << GPIO_MODER_MODER6_Pos);

	// Configure PA6 as Push-Pull output
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;

	// Configure PA6 as High-Speed Output
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR6_Msk;
	GPIOA->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR6_Pos);

	// Disable PA6 Pull-up/Pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6_Msk;

	// Set Initial State OFF
	GPIOA->BSRR |= GPIO_BSRR_BR_6;
}
