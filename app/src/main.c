/*
 * main.c
 *
 *  Created on: 15/12/2023
 *      Author: Raffael Rocha Daltoe
 */

#include "stm32f0xx.h"
#include "bsp.h"
#include "delay.h"
#include "factory_io.h"
#include "main.h"

// Kernel Objects
// Queue
xQueueHandle xComQueue;
xQueueHandle xSubcribeQueue;

// Semaphores
xSemaphoreHandle xSem_UART_TC,xSem_DMA_TC;
xSemaphoreHandle xSem1,xSem2,xSem3;
xSemaphoreHandle xSemCarton,xSemAscenseur,xSemPalette;

xSemaphoreHandle xConsoleMutex;

// Tasks
void vTaskPalettiseur(void *pvParameters);
void vTaskPalette(void *pvParameters);
void vTaskRead(void *pvParameters);
void vTaskWrite(void *pvParameters);
void vTaskAscenseur(void *pvParameters);

void init_tab(subscribe_message_t *t);
void affiche_tab(subscribe_message_t *t);

// Global variables
extern uint8_t tx_dma_buffer[7];
extern uint8_t rx_dma_buffer[FRAME_LENGTH];
uint8_t sensor[SENSOR_TABLE_SIZE];

// Static functions
static void SystemClock_Config(void);
static void Init_Tapis();
// Main program
int main()
{
	// Configure System Clock
	SystemClock_Config();

	// Initialize LED pin
	BSP_LED_Init();

	// Initialize Debug Console
	BSP_Console_Init();

	// Start Trace Recording
	vTraceEnable(TRC_START);
	
	// Create Semaphore objects
	xSem_UART_TC = xSemaphoreCreateBinary();
	xSem_DMA_TC = xSemaphoreCreateBinary();
	xSem1 = xSemaphoreCreateBinary();
	xSem2 = xSemaphoreCreateBinary();
	xSem3 = xSemaphoreCreateBinary();
	xSemCarton = xSemaphoreCreateBinary();
	xSemPalette = xSemaphoreCreateBinary();
	xSemAscenseur = xSemaphoreCreateBinary();

	// Create a Mutex for accessing the console
	xConsoleMutex = xSemaphoreCreateMutex();
	// Create Queue to hold console messages

	xComQueue = xQueueCreate(10, sizeof(uint32_t*));
	xSubcribeQueue = xQueueCreate(10, sizeof(subscribe_message_t*));

	Init_Tapis();

	// Create Tasks with higher priority for task2
	xTaskCreate(vTaskPalettiseur, "vTaskPalettiseur", 128, NULL, 3, NULL);
	xTaskCreate(vTaskPalette, "vTaskPalette", 128, NULL, 2, NULL);
	xTaskCreate(vTaskAscenseur, "vTaskAscenseur", 128, NULL, 4, NULL);
	xTaskCreate(vTaskRead, "vTaskRead", 256, NULL, 6, NULL);
	xTaskCreate(vTaskWrite, "vTaskWrite", 256, NULL, 5, NULL);
	// Start the Scheduler
	vTaskStartScheduler();

	while(1)
	{
		// The program should never be here...
	}
}


void vTaskPalettiseur(void *pvParameters)
{
	portTickType xLastWakeTime;

	// Timing initialization
	xLastWakeTime = xTaskGetTickCount();
	subscribe_message_t mPalletiseur;
	subscribe_message_t *ps;

	command_message_t *m_cmd;
	command_message_t command;

	command.state = 0;
	command.mask = 0;

	while (1)
	{
		for (int i = 0; i <3; i++)
		{
			//Turn on the cardboard distribution
			command.mask = Distribution_Cartons_MSK;
			command.state = Distribution_Cartons_ON;
			m_cmd = &command;
			xQueueSendToBack(xComQueue, &m_cmd, 0);

			mPalletiseur.sem_id = 1;
			mPalletiseur.sensor_id = Carton_Envoye_MSK;
			mPalletiseur.sensor_state = Carton_Envoye_OFF;
			ps = &mPalletiseur;

			// Send message to Subscribe Queue
			xQueueSendToBack(xSubcribeQueue, &ps, 0);

			// wait forever for the semaphore
			xSemaphoreTake(xSemCarton, portMAX_DELAY);

			//Subscribe for the sensor change
			mPalletiseur.sem_id = 1;
			mPalletiseur.sensor_id = Carton_Distribue_MSK;
			mPalletiseur.sensor_state = Carton_Distribue_OFF;
			ps = &mPalletiseur;

			// Send message to the Subscribe Queue
			xQueueSendToBack(xSubcribeQueue, &ps, 0);

			// wait forever for the semaphore
			xSemaphoreTake(xSemCarton, portMAX_DELAY);

			//Turn off the cardboard distribution
			command.mask = Distribution_Cartons_MSK;
			command.state = Distribution_Cartons_OFF;
			m_cmd = &command;
			xQueueSendToBack(xComQueue, &m_cmd, 0);

			mPalletiseur.sem_id = 1;
			mPalletiseur.sensor_id = Entree_Palettiseur_MSK;
			mPalletiseur.sensor_state = Entree_Palettiseur_OFF;
			ps = &mPalletiseur;
			// Send message to the Subscribe Queue
			xQueueSendToBack(xSubcribeQueue, &ps, 0);
			// wait forever for the semaphore
			xSemaphoreTake(xSemCarton, portMAX_DELAY);


			mPalletiseur.sem_id = 1;
			mPalletiseur.sensor_id = Entree_Palettiseur_MSK;
			mPalletiseur.sensor_state = Entree_Palettiseur_ON;
			ps = &mPalletiseur;
			// Send message to the Subscribe Queue
			xQueueSendToBack(xSubcribeQueue, &ps, 0);
			// wait forever for the semaphore
			xSemaphoreTake(xSemCarton, portMAX_DELAY);


			mPalletiseur.sem_id = 1;
			mPalletiseur.sensor_id = Entree_Palettiseur_MSK;
			mPalletiseur.sensor_state = Entree_Palettiseur_OFF;
			ps = &mPalletiseur;
			// Send message to the Subscribe Queue
			xQueueSendToBack(xSubcribeQueue, &ps, 0);
			// wait forever for the semaphore
			xSemaphoreTake(xSemCarton, portMAX_DELAY);

			//Open the Blockage
			command.mask = (Blocage_Entree_Palettiseur_MSK | Charger_Palettetiseur_MSK);
			command.state = Blocage_Entree_Palettiseur_OFF | Charger_Palettetiseur_ON;
			m_cmd = &command;
			xQueueSendToBack(xComQueue, &m_cmd, 0);

			mPalletiseur.sem_id = 1;
			mPalletiseur.sensor_id = Butee_Carton_MSK;
			mPalletiseur.sensor_state = Butee_Carton_ON;
			ps = &mPalletiseur;
			// Send message to the Subscribe Queue
			xQueueSendToBack(xSubcribeQueue, &ps, 0);
			// wait forever for the semaphore
			xSemaphoreTake(xSemCarton, portMAX_DELAY);

			//Close the barrier
			command.mask = (Blocage_Entree_Palettiseur_MSK | Charger_Palettetiseur_MSK | Poussoir_MSK);
			command.state = Blocage_Entree_Palettiseur_ON | Charger_Palettetiseur_OFF| Poussoir_ON;
			m_cmd = &command;
			xQueueSendToBack(xComQueue, &m_cmd, 0);

			mPalletiseur.sem_id = 1;
			mPalletiseur.sensor_id = Limite_Poussoir_MSK;
			mPalletiseur.sensor_state = Limite_Poussoir_OFF;
			ps = &mPalletiseur;

			// Send message to the Subscribe Queue
			xQueueSendToBack(xSubcribeQueue, &ps, 0);
			// wait forever for the semaphore
			xSemaphoreTake(xSemCarton, portMAX_DELAY);

			mPalletiseur.sem_id = 1;
			mPalletiseur.sensor_id = Limite_Poussoir_MSK;
			mPalletiseur.sensor_state = Limite_Poussoir_ON;
			ps = &mPalletiseur;
			// Send message to the Subscribe Queue
			xQueueSendToBack(xSubcribeQueue, &ps, 0);
			// wait forever for the semaphore
			xSemaphoreTake(xSemCarton, portMAX_DELAY);

			//Return of the "pusher"
			command.mask = (Poussoir_MSK);
			command.state =  Poussoir_OFF;
			m_cmd = &command;
			xQueueSendToBack(xComQueue, &m_cmd, 0);
		}

		xSemaphoreGive(xSem3);
		vTaskDelayUntil(&xLastWakeTime, 100 / portTICK_RATE_MS); //100 ms wait
	}

}

void vTaskAscenseur(void *pvParameters) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    subscribe_message_t mAscenseur;
    subscribe_message_t *ps;
    command_message_t *m_cmd;
    command_message_t command;

    command.state = 0;
    command.mask = 0;

    while (1)
    {
        //Semaphore of sync
        xSemaphoreTake(xSem3, portMAX_DELAY);

        //Move the elevator
        command.mask = (Monter_Ascenseur_MSK | Ascenceur_to_limit_MSK );
        command.state = Monter_Ascenseur_ON | Ascenceur_to_limit_ON ;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        //wait to go up
        mAscenseur.sem_id = 3;
        mAscenseur.sensor_id = Ascenceur_Etage_1_MSK;
        mAscenseur.sensor_state = Ascenceur_Etage_1_ON;
        ps = &mAscenseur;

        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemAscenseur, portMAX_DELAY);

        //Semaphore of sync
        xSemaphoreTake(xSem3, portMAX_DELAY);

        // Clamp -- I don't know the name in English
        command.mask = (Clamp_MSK);
        command.state = Clamp_ON ;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        //wait for the clamp
        mAscenseur.sem_id = 3;
        mAscenseur.sensor_id = Clamped_MSK;
        mAscenseur.sensor_state = Clamped_ON;
        ps = &mAscenseur;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemAscenseur, portMAX_DELAY);

        // open the door
        command.mask = (Porte_MSK |Ascenceur_to_limit_MSK|Monter_Ascenseur_MSK);
        command.state = Porte_ON |Ascenceur_to_limit_OFF|Monter_Ascenseur_OFF;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        // wait to open it
        mAscenseur.sem_id = 3;
        mAscenseur.sensor_id = Porte_Ouverte_MSK ;
        mAscenseur.sensor_state = Porte_Ouverte_ON ;
        ps = &mAscenseur;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemAscenseur, portMAX_DELAY);

        // wait for the door limit
        mAscenseur.sem_id = 3;
        mAscenseur.sensor_id = Limite_Porte_MSK;
        mAscenseur.sensor_state = Limite_Porte_OFF;
        ps = &mAscenseur;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemAscenseur, portMAX_DELAY);

        // go down
        command.mask = (Descendre_Ascenseur_MSK);
        command.state =  Descendre_Ascenseur_ON;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        // wait for the elevator to go down
        mAscenseur.sem_id = 3;
        mAscenseur.sensor_id = Ascenceur_Etage_1_MSK;
        mAscenseur.sensor_state = Ascenceur_Etage_1_OFF;
        ps = &mAscenseur;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemAscenseur, portMAX_DELAY);

        // close the door and turn off the clamp
        command.mask = ( Clamp_MSK|Porte_MSK);
        command.state = Clamp_OFF| Porte_OFF;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        // Semaphore of sync
        xSemaphoreTake(xSem3, portMAX_DELAY);

        // clamp again (second time)
        command.mask = (Clamp_MSK|Porte_MSK);
        command.state = Clamp_ON |Porte_ON;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        // Wait for door open
        mAscenseur.sem_id = 3;
        mAscenseur.sensor_id = Porte_Ouverte_MSK ;
        mAscenseur.sensor_state = Porte_Ouverte_ON ;
        ps = &mAscenseur;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemAscenseur, portMAX_DELAY);

        // Wait for door limit
        mAscenseur.sem_id = 3;
        mAscenseur.sensor_id = Limite_Porte_MSK;
        mAscenseur.sensor_state = Limite_Porte_ON;
        ps = &mAscenseur;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemAscenseur, portMAX_DELAY);

        // descend to the limit
        command.mask = ( Descendre_Ascenseur_MSK | Ascenceur_to_limit_MSK);
        command.state =  Descendre_Ascenseur_ON | Ascenceur_to_limit_ON;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        // Wait for elevator to descend
        mAscenseur.sem_id = 3;
        mAscenseur.sensor_id = Ascenceur_Etage_RDC_MSK;
        mAscenseur.sensor_state = Ascenceur_Etage_RDC_ON;
        ps = &mAscenseur;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemAscenseur, portMAX_DELAY);

        // Close the door and deactivate the limit
        command.mask = (Ascenceur_to_limit_MSK | Porte_MSK | Clamped_MSK | Descendre_Ascenseur_MSK);
        command.state = Ascenceur_to_limit_OFF | Porte_OFF | Clamped_OFF | Descendre_Ascenseur_OFF;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);
        xSemaphoreGive(xSem2);

        vTaskDelayUntil(&xLastWakeTime, 100 / portTICK_RATE_MS); //200 ms wait
    }
}


void vTaskPalette(void *pvParameters)
{
    portTickType xLastWakeTime;
    // Initialize timing
    xLastWakeTime = xTaskGetTickCount();

    //VARIABLES
    subscribe_message_t mPalette;
    subscribe_message_t *ps;

    command_message_t *m_cmd;
    command_message_t command;

    command.state = 0;
    command.mask = 0;

    while (1)
    {

        // Wait for the pallet to exit
        mPalette.sem_id = 2;
        mPalette.sensor_id = Sortie_Palette_MSK;
        mPalette.sensor_state = Sortie_Palette_OFF;
        ps = &mPalette;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // Synchronization semaphore
        xSemaphoreTake(xSemPalette, portMAX_DELAY);

        // Turn on the pallet distribution
        command.mask = Distribution_Palette_MSK;
        command.state = Distribution_Palette_ON;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        // Wait for the distribution
        vTaskDelay(500);

        // Turn off the pallet distribution
        command.mask = Distribution_Palette_MSK;
        command.state = Distribution_Palette_OFF;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);


        // Wait for the pallet to arrive
        mPalette.sem_id = 2;
        mPalette.sensor_id = Entree_Palette_MSK;
        mPalette.sensor_state = Entree_Palette_ON;
        ps = &mPalette;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemPalette, portMAX_DELAY);

        // Turn on the pallet loading conveyor
        command.mask = Charger_Palette_MSK;
        command.state = Charger_Palette_ON;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);


        // Wait for the pallet to arrive at the exit
        mPalette.sem_id = 2;
        mPalette.sensor_id = Sortie_Palette_MSK;
        mPalette.sensor_state = Sortie_Palette_ON;
        ps = &mPalette;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemPalette, portMAX_DELAY);

        // Turn off the pallet loading conveyor
        command.mask = Charger_Palette_MSK;
        command.state = Charger_Palette_OFF;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        // Synchronization semaphore
        xSemaphoreTake(xSem2, portMAX_DELAY);

        // Turn on the pallet loading conveyor
        command.mask = Charger_Palette_MSK;
        command.state = Charger_Palette_ON;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);


        // Wait for the pallet to exit
        mPalette.sem_id = 2;
        mPalette.sensor_id = Sortie_Palette_MSK;
        mPalette.sensor_state = Sortie_Palette_OFF;
        ps = &mPalette;
        // Send message to the Subscribe Queue
        xQueueSendToBack(xSubcribeQueue, &ps, 0);
        // wait forever for the semaphore
        xSemaphoreTake(xSemPalette, portMAX_DELAY);

        // Turn off the pallet loading conveyor
        command.mask = Charger_Palette_MSK;
        command.state = Charger_Palette_OFF;
        m_cmd = &command;
        xQueueSendToBack(xComQueue, &m_cmd, 0);

        vTaskDelayUntil(&xLastWakeTime, 200 / portTICK_RATE_MS); //200 ms wait
    }
}

void vTaskRead(void *pvParameters)
{
    int aux = 1;
    // Initialization of objects, expected type
    subscribe_message_t *s_message;

    // Initialization of an array of defined size to 0
    subscribe_message_t tab[20]; //tamSemaphore

    //xSemaphopreHandle *SemEnvoyer; -->bizzare

    portBASE_TYPE xStatus;
    portTickType xLastWakeTime;

    // Initialize timing
    xLastWakeTime = xTaskGetTickCount();

    uint8_t existe = 0;
    // uint8_t first_press = 0;

    init_tab(tab);
    xSemaphoreGive(xSem1);
    //xSemaphoreGive(xSemPalette);
    // Now enter the task loop
    while (1)
    {
        // BSP_LED_Toggle();

        // Wait here for message with 0s timeout
        xStatus = xQueueReceive(xSubcribeQueue, &s_message, 0);

        // Test if the message is received
        if (xStatus == pdPASS)
        {

            // Checking that there are no same combinations
            for (int i = 0; i < SIZE; i++)
            {
                if ((s_message->sem_id == tab[i].sem_id)
                        && (s_message->sensor_id == tab[i].sensor_id)
                        && (s_message->sensor_state == tab[i].sensor_state))
                {
                    existe = 1;
                    // existe_print = 1;
                }
            }

            // Filling the array on the first empty slot
            for (int i = 0; i < SIZE; i++)
            {
                if ((tab[i].sem_id == 0) && (existe == 0))
                {
                    tab[i].sem_id = s_message->sem_id;
                    tab[i].sensor_id = s_message->sensor_id;
                    tab[i].sensor_state = s_message->sensor_state;
                    existe = 1;
                    // indice = i;
                }
            }

            // Resetting the existence counters
            existe = 0;

        }

        xSemaphoreTake(xConsoleMutex, portMAX_DELAY);

        for (int i = 0; i < SIZE; i++)
        {
            if (tab[i].sem_id != 0) // Checking in the subscription queue if there is a request/demand
            {

                if (FACTORY_IO_Sensors_Get(tab[i].sensor_id) == tab[i].sensor_state) // If the state of the sensors matches the sensor identifier
                {
                    if (tab[i].sem_id == 1)
                    {

                        //xSemaphoreGive(xSem1);xSemCarton
                        xSemaphoreGive(xSemCarton);
                        // Free the slot
                        tab[i].sem_id = 0;
                        tab[i].sensor_id = 0;
                        tab[i].sensor_state = 0;
                    }

                    // Free the semaphores
                    if (tab[i].sem_id == 2)
                    {

                        xSemaphoreGive(xSemPalette);
                        // Free the slot
                        tab[i].sem_id = 0;
                        tab[i].sensor_id = 0;
                        tab[i].sensor_state = 0;
                        aux ++;
                        //Aux to synchronize the task
                        if (aux == 4)
                        {
                            xSemaphoreGive(xSem3);
                            aux =0;
                        }
                    }
                    // Free the semaphores
                    if (tab[i].sem_id == 3)
                    {

                        xSemaphoreGive(xSemAscenseur);
                        // Free the slot
                        tab[i].sem_id = 0;
                        tab[i].sensor_id = 0;
                        tab[i].sensor_state = 0;

                    }
                }
            }
        }

        // Release Mutex
        xSemaphoreGive(xConsoleMutex);
        vTaskDelayUntil(&xLastWakeTime, 200 / portTICK_RATE_MS); //200 ms wait
    }
}


void vTaskWrite(void *pvParameters)
{
    uint32_t memoire = 0;
    command_message_t cmd;

    // Definition of cmd
    portBASE_TYPE  xStatus;
    command_message_t *c_message;

    portTickType xLastWakeTime;

    // Initialize timing
    xLastWakeTime = xTaskGetTickCount();

    // Set maximum priority for EXTI line 4 to 15 interrupts
    NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn,
            configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);

    // Enable EXTI line 4 to 15 (user button on line 13) interrupts
    NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

    //Init_Tapis();

    // Now enter the task loop
    while (1)
    {
        // Wait for something in the message Queue
        xStatus = xQueueReceive(xComQueue, &c_message, portMAX_DELAY);

        // Take Mutex
        xSemaphoreTake(xConsoleMutex, portMAX_DELAY);

        // Test if the message is received
        if (xStatus == pdPASS)
        {
            /********************************************************************************************/
            // Definition of bits for FactorIO
            cmd = *c_message;
            memoire &= ~cmd.mask;
            memoire |= cmd.state;

            // Prepare frame buffer
            tx_dma_buffer[0] = TAG_ACTUATORS;                     // Actuators tag
            tx_dma_buffer[1] = (uint8_t) (memoire & 0x000000FF);  // data byte #1
            tx_dma_buffer[2] = (uint8_t) ((memoire & 0x0000FF00) >> 8U); // data byte #2
            tx_dma_buffer[3] = (uint8_t) ((memoire & 0x00FF0000) >> 16U);// data byte #2
            tx_dma_buffer[4] = (uint8_t) ((memoire & 0xFF000000) >> 24U);// data byte #2
            tx_dma_buffer[5] = 0x00;                                // CRC (not yet implemented)
            tx_dma_buffer[6] = '\n';                                // End byte
            /********************************************************************************************/

            // Set Memory Buffer size
            DMA1_Channel4->CNDTR = 7;

            // Enable DMA1 Channel 4
            DMA1_Channel4->CCR |= DMA_CCR_EN;

            // Enable USART2 DMA Request on T
            USART2->CR3 |= USART_CR3_DMAT;

            // Wait for Semaphore endlessly
            xSemaphoreTake(xSem_DMA_TC, portMAX_DELAY);

            // Disable USART2 DMA Request on Tx
            USART2->CR3 &= ~USART_CR3_DMAT;

            // Disable DMA1 Channel 4
            DMA1_Channel4->CCR &= ~DMA_CCR_EN;

            /****************************************************************************************************/
            // Release Mutex
            xSemaphoreGive(xConsoleMutex);
        }
        vTaskDelayUntil(&xLastWakeTime, 200 / portTICK_RATE_MS); //200 ms wait
    }

}


static void Init_Tapis() {

    command_message_t *message;
    command_message_t command;

    command.state = 0;
    command.mask = 0;

    command.mask = Tapis_Distribution_Cartons_MSK
            | Blocage_Entree_Palettiseur_MSK | Tapis_Carton_vers_Palettiseur_MSK
            | Tapis_Palette_Vers_Ascenseur_MSK | Tapis_Distribution_Palette_MSK
            | Tapis_Fin_MSK | Remover_MSK;
            //|Distribution_Cartons_MSK;

    // TURN ON THE CONCERNED BITS
    command.state = Tapis_Distribution_Cartons_ON | Blocage_Entree_Palettiseur_ON
            | Tapis_Carton_vers_Palettiseur_ON   | Tapis_Palette_Vers_Ascenseur_ON
            | Tapis_Distribution_Palette_ON | Tapis_Fin_ON | Remover_ON; //|Distribution_Cartons_ON;

    // Send message to the Console Queue
    message = &command;
    xQueueSendToBack(xComQueue, &message, 0);

}
static void SystemClock_Config()
{
    uint32_t    HSE_Status;
    uint32_t    PLL_Status;
    uint32_t    SW_Status;
    uint32_t    timeout = 0;

    timeout = 1000000;

    // Start HSE in Bypass Mode
    RCC->CR |= RCC_CR_HSEBYP;
    RCC->CR |= RCC_CR_HSEON;

    // Wait until HSE is ready
    do
    {
        HSE_Status = RCC->CR & RCC_CR_HSERDY_Msk;
        timeout--;
    } while ((HSE_Status == 0) && (timeout > 0));

    // Select HSE as PLL input source
    RCC->CFGR &= ~RCC_CFGR_PLLSRC_Msk;
    RCC->CFGR |= (0x02 <<RCC_CFGR_PLLSRC_Pos);

    // Set PLL PREDIV to /1
    RCC->CFGR2 = 0x00000000;

    // Set PLL MUL to x6
    RCC->CFGR &= ~RCC_CFGR_PLLMUL_Msk;
    RCC->CFGR |= (0x04 <<RCC_CFGR_PLLMUL_Pos);

    // Enable the main PLL
    RCC-> CR |= RCC_CR_PLLON;

    // Wait until PLL is ready
    do
    {
        PLL_Status = RCC->CR & RCC_CR_PLLRDY_Msk;
        timeout--;
    } while ((PLL_Status == 0) && (timeout > 0));

    // Set AHB prescaler to /1
    RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    //Set APB1 prescaler to /1
    RCC->CFGR &= ~RCC_CFGR_PPRE_Msk;
    RCC->CFGR |= RCC_CFGR_PPRE_DIV1;

    // Enable FLASH Prefetch Buffer and set Flash Latency
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

    /* --- Until this point, MCU was still clocked by HSI at 8MHz ---*/
    /* --- Switching to PLL at 48MHz Now!  Fasten your seat belt! ---*/

    // Select the main PLL as system clock source
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Wait until PLL becomes main switch input
    do
    {
        SW_Status = (RCC->CFGR & RCC_CFGR_SWS_Msk);
        timeout--;
    } while ((SW_Status != RCC_CFGR_SWS_PLL) && (timeout > 0));

    /*--- Use PA8 as LSE output ---*/

    // Set MCO source as LSE
    RCC->CFGR &= ~RCC_CFGR_MCO_Msk;
    RCC->CFGR |=  RCC_CFGR_MCOSEL_LSE;     // Change here

    // No prescaler
    RCC->CFGR &= ~RCC_CFGR_MCOPRE_Msk;

    // Enable GPIOA clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Configure PA8 as Alternate function
    GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk;
    GPIOA->MODER |= (0x02 <<GPIO_MODER_MODER8_Pos);

    // Set to AF0 (MCO output)
    GPIOA->AFR[1] &= ~(0x0000000F);
    GPIOA->AFR[1] |=  (0x00000000);

    // Update SystemCoreClock global variable
    SystemCoreClockUpdate();
}

void init_tab(subscribe_message_t *t)
{
    // Initialize the array to 0
    for (int i = 0; i < SIZE; i++)
    {
        t[i].sem_id = 0;
        t[i].sensor_id = 0;
        t[i].sensor_state = 0;
    }
}
