#ifndef BSP_INC_BSP_H_
#define BSP_INC_BSP_H_

#include "stm32f0xx.h"
#include "FreeRTOSConfig.h"

/*
 * LED driver functions
 */

void	BSP_LED_Init	(void);
void	BSP_LED_On	(void);
void	BSP_LED_Off	(void);
void	BSP_LED_Toggle	(void);

/*
 * Push-Button driver functions
 */

void		BSP_PB_Init		(void);
uint8_t	BSP_PB_GetState	(void);

/*
 * Timer functions
 */

void BSP_TIMER_Timebase_Init	(void);

/*
 * NVIC Init
 */

void BSP_NVIC_Init(void);

/*
 * Debug Console init
 */

void	BSP_Console_Init	(void);

#endif /* BSP_INC_BSP_H_ */
