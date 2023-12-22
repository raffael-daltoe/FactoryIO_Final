#include "stm32f0xx.h"
#include "bsp.h"
#include "main.h"
#include "factory_io.h"
#include "packages.h"

// Global variables
uint32_t free_heap_size;
uint8_t tx_dma_buffer[FRAME_LENGTH];
uint8_t rx_dma_buffer[FRAME_LENGTH];

// FreeRTOS tasks
void vSendBoxes(void *pvParameters);
void vPush(void *pvParameters);
void vTask_Pub(void *pvParameters);
void vTask_Write(void *pvParameters);
void vPalete(void *pvParameters);
void vElevador(void *pvParameters);
void vEstado(void *pvParameters);

// Kernel objects
xQueueHandle xSubscribeQueue;
xQueueHandle xPublishQueue;

// Mutex for synchronization between the task "poussoir" and "etau"
xSemaphoreHandle M_Push;

// One semaphore for each task
xSemaphoreHandle S_SendBox;
xSemaphoreHandle S_Push;
xSemaphoreHandle S_Palete;
xSemaphoreHandle S_Elevador;
xSemaphoreHandle S_Estado;

// Semaphore for DMA TC (Transfer Complete)
xSemaphoreHandle xSem_DMA_TC;

// Event group to store synchronization between tasks
EventGroupHandle_t taskSync;
// Function to configure the system clock
static void SystemClock_Config();

// Main program
int main() {
    // Configure System Clock
    SystemClock_Config();
    // Initialize Timer for delays
    // BSP_DELAY_TIM_init();

    // Initialize Debug Console
    BSP_Console_Init();

    // Start Trace Recording
    vTraceEnable(TRC_START);

    // Create kernel objects to hold console messages
    // Size 10 (twice the amount of tasks we have) to make sure there's enough room
    xSubscribeQueue = xQueueCreate(10, sizeof(subscribe_message_t *));
    xPublishQueue = xQueueCreate(10, sizeof(command_message_t *));

    // 5 semaphores are used to publish/subscribe sensor updates for each module
    S_SendBox = xSemaphoreCreateBinary();
    S_Push = xSemaphoreCreateBinary();
    S_Palete = xSemaphoreCreateBinary();
    S_Elevador = xSemaphoreCreateBinary();
    S_Estado = xSemaphoreCreateBinary();

    // Mutex for synchronization between task "poussoir" and "etau"
    M_Push = xSemaphoreCreateMutex();

    // Semaphore for DMA (Direct Memory Access) Transfer Complete
    xSem_DMA_TC = xSemaphoreCreateBinary();

    // Create Event Group
    taskSync = xEventGroupCreate();

    // Give a nice name to queues/semaphores and mutex in the trace recorder
    vTraceSetQueueName(xSubscribeQueue, "Subscribe Queue");
    vTraceSetQueueName(xPublishQueue, "Publish Queue");

    vTraceSetMutexName(M_Push, "Mutex Stock poussoir");

    vTraceSetSemaphoreName(S_SendBox, "S_SendBox");
    vTraceSetSemaphoreName(S_Push, "S_Push");
    vTraceSetSemaphoreName(S_Palete, "S_Palete");
    vTraceSetSemaphoreName(S_Elevador, "S_Elevador");
    vTraceSetSemaphoreName(S_Estado, "S_Estado");

    vTraceSetSemaphoreName(xSem_DMA_TC, "xSEM_DMA_TC");

    // INITIAL STATE

    command_message_t *pm;
    command_message_t actuator_write_buffer;

    // Activate conveyors, carton distribution, carton turner, barrier, remover, etc.
    FACTORY_IO_Actuators_Set(
        A1_ON | A2_ON | A11_ON | A12_OFF | A14_ON | A18_ON | A19_ON,
        A1_Msk | A2_Msk | A11_Msk | A12_Msk | A14_Msk | A18_Msk | A19_Msk,
        actuator_write_buffer
    );

    pm = &actuator_write_buffer;
    xQueueSendToBack(xPublishQueue, &pm, 0);

    // Default synchronization to 1
    xEventGroupSetBits(taskSync, BOXES_SENDED);

    // Create Tasks // 256 bytes allocated for each task
    xTaskCreate(vSendBoxes, "vSendBoxes", 128, NULL, 5, NULL); 
    xTaskCreate(vPush, "vPush", 128, NULL, 4, NULL);
    xTaskCreate(vPalete, "vPalete", 128, NULL, 1, NULL);
    xTaskCreate(vTask_Pub, "Task_Pub", 128, NULL, 7, NULL);
    xTaskCreate(vTask_Write, "Task_Write", 128, NULL, 6, NULL);
    xTaskCreate(vElevador, "vElevador", 128, NULL, 2, NULL);
    xTaskCreate(vEstado, "vEstado", 128, NULL, 3, NULL);

    // Start the Scheduler
    vTaskStartScheduler();

    while (1) {
        // The program should never reach here...
    }
}
void vSendBoxes(void *pvParameters) {
    command_message_t *pm;
    command_message_t actuator_write_buffer;

    subscribe_message_t sub;
    subscribe_message_t *sub_pm;

    // Activate carton distribution
    FACTORY_IO_Actuators_Set(A0_ON, A0_Msk, actuator_write_buffer);
    pm = &actuator_write_buffer;
    xQueueSendToBack(xPublishQueue, &pm, 0);

    sub.sem_id = &S_SendBox;
    sub.sensor_state = S0_ON;
    sub.sensor_mask = S0_Pos;
    sub_pm = &sub;
    xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
    xSemaphoreTake(S_SendBox, portMAX_DELAY);

    // Carton 1 passing in front of the carton sensor
    sub.sem_id = &S_SendBox;
    sub.sensor_state = S0_OFF;
    sub.sensor_mask = S0_Pos;
    sub_pm = &sub;
    xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
    xSemaphoreTake(S_SendBox, portMAX_DELAY);

    sub.sensor_state = S0_ON;
    sub_pm = &sub;
    xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
    xSemaphoreTake(S_SendBox, portMAX_DELAY);

    sub.sensor_state = S0_OFF;
    sub_pm = &sub;
    xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
    xSemaphoreTake(S_SendBox, portMAX_DELAY);

    // Deactivate carton distribution
    FACTORY_IO_Actuators_Set(A0_OFF, A0_Msk, actuator_write_buffer);
    pm = &actuator_write_buffer;
    xQueueSendToBack(xPublishQueue, &pm, 0);

    while (1) {
        free_heap_size = xPortGetFreeHeapSize(); // Measure free heap size

        // Carton 1 passing in front of the palettizer entry sensor
        sub.sem_id = &S_SendBox;
        sub.sensor_state = S2_OFF;
        sub.sensor_mask = S2_Pos;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_SendBox, portMAX_DELAY);

        // End of Carton 1 passing in front of the palettizer entry sensor
        sub.sensor_state = S2_ON;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_SendBox, portMAX_DELAY);

        // Carton 2 passing in front of the palettizer entry sensor
        sub.sensor_state = S2_OFF;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_SendBox, portMAX_DELAY);

        vTaskDelay(20);

        // Wait for synchronization indicating that the previous cartons have been pushed (initial state is 1)
        xEventGroupWaitBits(taskSync, BOXES_SENDED, pdTRUE, pdTRUE, portMAX_DELAY);

        // Open barrier and start carton distribution
        FACTORY_IO_Actuators_Set(A2_OFF | A0_ON, A2_Msk | A0_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Carton 1 passing in front of the carton sensor
        sub.sem_id = &S_SendBox;
        sub.sensor_state = S0_OFF;
        sub.sensor_mask = S0_Pos;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_SendBox, portMAX_DELAY);

        sub.sensor_state = S0_ON;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_SendBox, portMAX_DELAY);

        sub.sensor_state = S0_OFF;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_SendBox, portMAX_DELAY);

        // Deactivate carton distribution and close barrier
        FACTORY_IO_Actuators_Set(A0_OFF | A2_ON, A0_Msk | A2_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Send synchronization signal indicating that cartons should be pushed
        xEventGroupSetBits(taskSync, SEND_BOXES);

        vTaskDelay(100 / TIMEBASE);
    }
}
void vPush(void *pvParameters) {
    uint8_t stock_poussoir = 3;
    subscribe_message_t sub;
    subscribe_message_t *sub_pm;

    command_message_t actuator_write_buffer;
    command_message_t *pm;

    while (1) {
        // Acquire the Mutex
        xSemaphoreTake(M_Push, portMAX_DELAY);

        // The stock allows managing 3 batches of cartons to push
        // When the stock is 0, the mutex is released to "etau" so that it
        // releases the 6 cartons. Once that is done, the current task
        // (vPush) regains the Mutex, resets the stock to 3, and resumes its cycle.
        while (stock_poussoir != 0) {
            stock_poussoir--;

            // Wait for the arrival of cartons in front of the pusher
            xEventGroupWaitBits(taskSync, SEND_BOXES, pdTRUE, pdTRUE, portMAX_DELAY);

            // Activate the pusher
            FACTORY_IO_Actuators_Set(A4_ON, A4_Msk, actuator_write_buffer);
            pm = &actuator_write_buffer;
            xQueueSendToBack(xPublishQueue, &pm, 0);

            // Wait for the pusher to reach its limit (deactivation and reactivation of the sensor)
            sub.sensor_mask = S4_Pos;
            sub.sensor_state = S4_OFF;
            sub.sem_id = &S_Push;
            sub_pm = &sub;
            xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
            xSemaphoreTake(S_Push, portMAX_DELAY);

            sub.sensor_state = S4_ON;
            sub_pm = &sub;
            xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
            xSemaphoreTake(S_Push, portMAX_DELAY);

            // Deactivate the pusher
            FACTORY_IO_Actuators_Set(A4_OFF, A4_Msk, actuator_write_buffer);
            pm = &actuator_write_buffer;
            xQueueSendToBack(xPublishQueue, &pm, 0);

            // Wait for the pusher to reach its limit (deactivation and reactivation of the sensor)
            sub.sensor_state = S4_OFF;
            sub_pm = &sub;
            xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
            xSemaphoreTake(S_Push, portMAX_DELAY);

            sub.sensor_state = S4_ON;
            sub_pm = &sub;
            xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
            xSemaphoreTake(S_Push, portMAX_DELAY);

            // Send synchronization signal: cartons pushed
            xEventGroupSetBits(taskSync, BOXES_SENDED);
        }

        // Once the 3 batches of cartons are pushed, release the semaphore to "etau"
        xSemaphoreGive(M_Push);

        // Reset the carton stock
        stock_poussoir = 3;

        vTaskDelay(100 / TIMEBASE);
    }
}
void vPalete(void *pvParameters) {
    subscribe_message_t sub;
    subscribe_message_t *sub_pm;

    command_message_t actuator_write_buffer;
    command_message_t *pm;

    while (1) {
        // Activate both palette conveyors (A16 and A17)
        // Activate the distribution (A9)
        FACTORY_IO_Actuators_Set(A16_ON | A17_ON | A9_ON, A16_Msk | A17_Msk | A9_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Wait for the palette to arrive on the support (palette entry)
        sub.sensor_mask = S12_Pos;
        sub.sensor_state = S12_ON;
        sub.sem_id = &S_Palete;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Palete, portMAX_DELAY);

        // Deactivate the distribution (A9) and activate palette loading (A10)
        FACTORY_IO_Actuators_Set(A9_OFF | A10_ON, A9_Msk | A10_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Wait for the palette to reach the end of the support (palette exit)
        sub.sensor_mask = S9_Pos;
        sub.sensor_state = S9_ON;
        sub.sem_id = &S_Palete;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Palete, portMAX_DELAY);

        // Deactivate the distribution, loading, and conveyor to the elevator
        FACTORY_IO_Actuators_Set(A10_OFF | A16_OFF, A10_Msk | A16_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Send synchronization signal to indicate that the palette is in place
        xEventGroupSetBits(taskSync, ARRIVED_PALETE);

        // Wait for synchronization indicating that the palette is ready to be sent
        xEventGroupWaitBits(taskSync, SEND_PALETE, pdTRUE, pdTRUE, portMAX_DELAY);

        // Activate palette loading
        FACTORY_IO_Actuators_Set(A10_ON, A10_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Wait for the palette to leave
        sub.sensor_state = S9_OFF;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Palete, portMAX_DELAY);

        vTaskDelay(100 / TIMEBASE);
    }
}

void vEstado(void *pvParameters) {
    // This delay ensures that this task does not take the Mutex
    // before the pusher task
    vTaskDelay(50 / TIMEBASE);

    subscribe_message_t sub;
    subscribe_message_t *sub_pm;

    command_message_t actuator_write_buffer;
    command_message_t *pm;

    while (1) {
        // Acquire the Mutex from the pusher task
        xSemaphoreTake(M_Push, portMAX_DELAY);

        // Activate the etau
        FACTORY_IO_Actuators_Set(A5_ON, A5_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Wait for the etau to be tightened
        sub.sensor_mask = S5_Pos;
        sub.sensor_state = S5_ON;
        sub.sem_id = &S_Estado;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Estado, portMAX_DELAY);

        // Wait for the authorization to open the trap
        xEventGroupWaitBits(taskSync, OPEN_TRAPE, pdTRUE, pdTRUE, portMAX_DELAY);

        // Open the trap
        FACTORY_IO_Actuators_Set(A3_ON, A3_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Wait for the trap to be opened
        sub.sensor_mask = S3_Pos;
        sub.sensor_state = S3_ON;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Estado, portMAX_DELAY);

        // Deactivate the etau to release the cartons
        FACTORY_IO_Actuators_Set(A5_OFF, A5_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Wait for the etau to return to its initial position
        sub.sensor_mask = S5_Pos;
        sub.sensor_state = S5_OFF;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Estado, portMAX_DELAY);

        // Synchronization to indicate that the cartons are deposited to the elevator
        xEventGroupSetBits(taskSync, BOXES_STOREDS);

        // Delay to allow time for the cartons to fall and the elevator to
        // reposition (downward)
        vTaskDelay(500 / TIMEBASE);

        // Wait for synchronization allowing the trap to be closed
        xEventGroupWaitBits(taskSync, CLOSE_PORT, pdTRUE, pdTRUE, portMAX_DELAY);

        // Close the trap
        FACTORY_IO_Actuators_Set(A3_OFF, A3_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Wait for the trap to be closed
        sub.sensor_mask = S3_Pos;
        sub.sensor_state = S3_OFF;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Estado, portMAX_DELAY);

        // Return the semaphore to the pusher task
        xSemaphoreGive(M_Push);

        vTaskDelay(100 / TIMEBASE);
    }
}

void vElevador(void *pvParameters) {
    subscribe_message_t sub;
    subscribe_message_t *sub_pm;

    command_message_t actuator_write_buffer;
    command_message_t *pm;

    while (1) {
        // Wait for a palette to arrive
        xEventGroupWaitBits(taskSync, ARRIVED_PALETE, pdTRUE, pdTRUE, portMAX_DELAY);

        // Move up to the limit
        FACTORY_IO_Actuators_Set(A6_ON | A8_ON, A6_Msk | A8_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Arrive at floor 1
        sub.sensor_mask = S7_Pos;
        sub.sensor_state = S7_ON;
        sub.sem_id = &S_Elevador;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Elevador, portMAX_DELAY);

        // Command to open the trap
        xEventGroupSetBits(taskSync, OPEN_TRAPE);

        // Deactivate the upward movement and the top limit
        FACTORY_IO_Actuators_Set(A6_OFF | A8_OFF, A6_Msk | A8_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Wait for the deposit of 6 cartons
        xEventGroupWaitBits(taskSync, BOXES_STOREDS, pdTRUE, pdTRUE, portMAX_DELAY);

        // Move down
        FACTORY_IO_Actuators_Set(A7_ON, A7_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Leave floor 7
        sub.sensor_mask = S7_Pos;
        sub.sensor_state = S7_OFF;
        sub.sem_id = &S_Elevador;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Elevador, portMAX_DELAY);

        // Deactivate the descent
        FACTORY_IO_Actuators_Set(A7_OFF, A7_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Command to close the trap
        xEventGroupSetBits(taskSync, CLOSE_PORT);

        // Indicate readiness for a new trap opening
        xEventGroupSetBits(taskSync, OPEN_TRAPE);

        // Wait for the deposit of 6 cartons
        xEventGroupWaitBits(taskSync, BOXES_STOREDS, pdTRUE, pdTRUE, portMAX_DELAY);

        // Move down to the limit
        FACTORY_IO_Actuators_Set(A7_ON | A8_ON, A7_Msk | A8_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        // Reach the ground floor
        sub.sensor_mask = S6_Pos;
        sub.sensor_state = S6_ON;
        sub.sem_id = &S_Elevador;
        sub_pm = &sub;
        xQueueSendToBack(xSubscribeQueue, &sub_pm, 0);
        xSemaphoreTake(S_Elevador, portMAX_DELAY);

        // Command to close the door
        xEventGroupSetBits(taskSync, CLOSE_PORT);

        // Command to send the palette
        xEventGroupSetBits(taskSync, SEND_PALETE);

        // Deactivate the descent and the bottom limit
        FACTORY_IO_Actuators_Set(A7_OFF | A8_OFF, A7_Msk | A8_Msk, actuator_write_buffer);
        pm = &actuator_write_buffer;
        xQueueSendToBack(xPublishQueue, &pm, 0);

        vTaskDelay(100 / TIMEBASE);
    }
}

void vTask_Pub(void *pvParameters) {
    // Take the semaphores once to make sure they are empty
    xSemaphoreTake(S_SendBox, 0);
    xSemaphoreTake(S_Push, 0);
    xSemaphoreTake(S_Palete, 0);
    xSemaphoreTake(S_Elevador, 0);
    xSemaphoreTake(S_Estado, 0);

    // Message received
    subscribe_message_t *message;

    // Status of the queue receive
    portBASE_TYPE xStatus;

    // Init array of subscribers
    subscribe_message_t Sub_DB[SUB_SIZE];
    for (int i = 0; i < SUB_SIZE; i++) {
        Sub_DB[i].sem_id = NULL;
        Sub_DB[i].sensor_mask = 0;
        Sub_DB[i].sensor_state = 0;
    }

    uint8_t counter = 0; // Counts the index of the sensor array

    // Initialize timing
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        xStatus = xQueueReceive(xSubscribeQueue, &message, 0);

        if (xStatus == pdPASS) {
            // Local variable declaration
            counter = 0;

            // Search for an available slot in the subscription array
            // and copy the subscription into Sub_DB
            for (counter = 0; counter < SUB_SIZE; counter++) {
                if (Sub_DB[counter].sem_id == NULL) {
                    Sub_DB[counter] = *message;
                    break;
                }
            }
        }

        // Scan the array of registered subscriptions
        for (int i = 0; i < SUB_SIZE; i++) {
            // Check for existing subscriptions
            if (Sub_DB[i].sem_id != NULL) {
                // Check the sensor status to see if it matches the subscription
                if (FACTORY_IO_Sensors_Check(Sub_DB[i].sensor_mask, Sub_DB[i].sensor_state)) {
                    // If so, give the corresponding semaphore
                    xSemaphoreGive(*Sub_DB[i].sem_id);

                    // Reset the subscription slot
                    Sub_DB[i].sem_id = NULL;
                    Sub_DB[i].sensor_mask = 0;
                    Sub_DB[i].sensor_state = 0;
                }
            }
        }

        // Wait here for 100ms since the last wakeup
        vTaskDelayUntil(&xLastWakeTime, (100 / (portTICK_RATE_MS)));
    }
}
void vTask_Write(void *pvParameters) {
    command_message_t *message;

    uint8_t i = 0;

    // Set priority level 1 for DMA1 interrupt -> reading the sensor values is our highest priority
    NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);

    // Enable DMA1 interrupts
    NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

    while (1) {
        xQueueReceive(xPublishQueue, &message, portMAX_DELAY);
        // The message was taken as expected

        for (i = 0; i < 7; i++) {
            tx_dma_buffer[i] = (*message)[i];
        }

        // Set Memory Buffer size
        DMA1_Channel4->CNDTR = 7;

        // Enable DMA1 Channel 4
        DMA1_Channel4->CCR |= DMA_CCR_EN;

        // Enable USART2 DMA Request on TX
        USART2->CR3 |= USART_CR3_DMAT;

        xSemaphoreTake(xSem_DMA_TC, portMAX_DELAY);

        // Disable DMA1 Channel 4
        DMA1_Channel4->CCR &= ~DMA_CCR_EN;

        // Disable USART2 DMA Request on TX
        USART2->CR3 &= ~USART_CR3_DMAT;
    }
}

static void SystemClock_Config()
{
	uint32_t	HSE_Status;
	uint32_t	PLL_Status;
	uint32_t	SW_Status;
	uint32_t	timeout = 0;

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
	RCC->CFGR |= (0x08 <<RCC_CFGR_PLLSRC_Pos);

	// Set PLL PREDIV to /1
	RCC->CFGR2 = RCC_CFGR_HPRE_DIV8;

	// Set PLL MUL to x6
	RCC->CFGR &= ~RCC_CFGR_PLLMUL_Msk;
	RCC->CFGR |= (0x00 <<RCC_CFGR_PLLMUL_Pos);

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
	RCC->CFGR |= RCC_CFGR_HPRE_DIV8;

	//Set APB1 prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_PPRE_Msk;
	RCC->CFGR |= RCC_CFGR_PPRE_DIV1;

	// Enable FLASH Prefetch Buffer and set Flash Latency
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

	// Select the main PLL as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// Wait until PLL becomes main switch input
	do
	{
		SW_Status = (RCC->CFGR & RCC_CFGR_SWS_Msk);
		timeout--;
	} while ((SW_Status != RCC_CFGR_SWS_PLL) && (timeout > 0));

	// Update SystemCoreClock global variable
	SystemCoreClockUpdate();
}



