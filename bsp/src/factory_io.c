#include "stm32f0xx.h"
#include "factory_io.h"
#include "main.h"
#include "packages.h"

extern uint8_t rx_dma_buffer[FRAME_LENGTH];
extern uint8_t tx_dma_buffer[FRAME_LENGTH];
void FACTORY_IO_Actuators_Set(uint32_t cmd, uint32_t msk, uint8_t* buffer)
{
	static uint32_t current_actuators_state;

	current_actuators_state = ((current_actuators_state & ~msk) | cmd);

	// Prepare frame buffer
	buffer[0] = TAG_ACTUATORS; 							// Actuators tag

	buffer[1] = (uint8_t) (current_actuators_state & 0x000000FF);			// data byte #1
	buffer[2] = (uint8_t)((current_actuators_state & 0x0000FF00) >>8U );	// data byte #2
	buffer[3] = (uint8_t)((current_actuators_state & 0x00FF0000) >>16U);	// data byte #2
	buffer[4] = (uint8_t)((current_actuators_state & 0xFF000000) >>24U);	// data byte #2

	buffer[5] = 0x00;									// CRC (not yet implemented)
	buffer[6] = '\n';									// End byte
}

uint8_t	FACTORY_IO_Sensors_Check(uint32_t msk, uint32_t desired_state)
{
	uint32_t	sstates;

	// Build 32-bit sensors states representation
	sstates = 0x00000000;

	sstates |= rx_dma_buffer[1];
	sstates |= (rx_dma_buffer[2] <<8U );
	sstates |= (rx_dma_buffer[3] <<16U);
	sstates |= (rx_dma_buffer[4] <<24U);

	// Perform logical comparison with mask
	if ((sstates & msk) == desired_state)
	{
		return 1;
	}

	else
	{
		return 0;
	}
}
