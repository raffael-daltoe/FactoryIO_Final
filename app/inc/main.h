#ifndef APP_INC_MAIN_H_
#define APP_INC_MAIN_H_

#include "stm32f0xx.h"
#include "bsp.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"
#include "stream_buffer.h"

// Synchronization flags for the EventGroup
#define SEND_BOXES      ( (EventBits_t)( 0x01 << 2) )
#define BOXES_SENDED        ( (EventBits_t)( 0x01 << 3) )
#define OPEN_TRAPE       ( (EventBits_t)( 0x01 << 4) )
#define BOXES_STOREDS      ( (EventBits_t)( 0x01 << 5) )
#define ARRIVED_PALETE      ( (EventBits_t)( 0x01 << 6) )
#define SEND_PALETE          ( (EventBits_t)( 0x01 << 7) )
#define CLOSE_PORT        ( (EventBits_t)( 0x01 << 8) )

#define SUB_SIZE 5 // Size of the subscription array: one subscription per task
#define TIMEBASE 10 // Simulation time in FactoryIO (default: 1) or tick divisor (default: 1000)
                    // Since the ticks are now at 100Hz instead of 1000Hz, we set the timebase to 10 for division.

// Structure for message queues
typedef struct {
    xSemaphoreHandle *sem_id;
    uint32_t sensor_mask; // Mask of sensors to monitor
    uint32_t sensor_state; // Desired sensor state
} subscribe_message_t;

typedef uint8_t command_message_t[7];

#endif /* APP_INC_MAIN_H_ */
