#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

/*
* BRIEF: 
* 
*/

#define mainDELAY_LOOP_COUNT		( 0xffffff )
/** 
 *  function that implement task
*/
static void prvSenderTask( void *pvParams );
static void prvReceiverTask( void *pvParams );


static QueueHandle_t xQueue;

int main( void ) {
    BaseType_t xTaskCreationResult = pdFALSE;

    xTaskCreationResult = xTaskCreate(
        prvSenderTask,
        "Sender",
        configMINIMAL_STACK_SIZE,
        "Sender task is running\n",
        tskIDLE_PRIORITY + 1,
        NULL
    );

    configASSERT( xTaskCreationResult == pdPASS );

     xTaskCreationResult = xTaskCreate(
        prvReceiverTask,
        "Receiver",
        configMINIMAL_STACK_SIZE,
        "Receiver task is running\n",
        tskIDLE_PRIORITY,
        NULL
    );

    configASSERT( xTaskCreationResult == pdPASS );

    /* TODO 1 - Create a queue capable of holding 5 UBaseType_t using
     * xQueueCreate API.
     *
     * Use the following values for xQueueCreate parameters:
     * uxQueueLength    5
     * uxItemSize       sizeof( UBaseType_t )
     *
     * Assign the return value to xQueue.
     */

    xQueue = xQueueCreate( 5, sizeof( UBaseType_t ) );

    configASSERT( xQueue != NULL );

    /* TODO 2 - Call vTaskStartScheduler to start the scheduler. */
    vTaskStartScheduler();

    /* Should not reach here. */
    for( ;; )
    {
        fprintf(stderr, "Scheduler failed to start\n");
    }

    /* Just to make the compiler happy. */
    return 0;
}

/** 
 *  function that implement task
*/
static void prvSenderTask( void *pvParams ) {

    BaseType_t xQueueSendResult;
    UBaseType_t uxValueToSend = 0;

    (void) pvParams; /* Just to prevent compiler warnings. */

    char *pcTaskParameter; 
    pcTaskParameter = (char *) pvParams;
    for( ;; ) {
        
        fprintf(stderr, pcTaskParameter);
        uxValueToSend++;
        /* TODO 1 - Send an unsigned long value to the queue using xQueueSend API
         * Use the following parameters for xQueueSend:
         * xQueue          xQueue
         * pvItemToQueue   &uxValueToSend
         * xTicksToWait    0
         * Return value should be assigned to xQueueSendResult.
         */
        xQueueSendResult = xQueueSend( xQueue, &uxValueToSend, 0 );

        configASSERT( xQueueSendResult == pdPASS );

        vTaskDelay( pdMS_TO_TICKS( 1000 ) ); /* Delay for a period. */
    }
}

static void prvReceiverTask( void *pvParams ) {

    UBaseType_t uxReceivedValue;

    (void) pvParams; /* Just to prevent compiler warnings. */

    char *pcTaskParameter; 
    pcTaskParameter = (char *) pvParams;

    for( ;; ) {
        
        fprintf(stderr, pcTaskParameter);

        /* TODO 1 - Receive from    the queue using xQueueReceive API   
         * Use the following parameters for xQueueReceive:
         * xQueue          xQueue
         * pvBuffer        &uxReceivedValue
         * xTicksToWait    portMAX_DELAY
         * Return value should be assigned to xQueueReceiveResult.
         */
        configASSERT( xQueueReceive( xQueue, &uxReceivedValue, portMAX_DELAY ) == pdPASS );

        
        fprintf(stderr, "Value Received from the queue: %lu\r\n", uxReceivedValue);
        fprintf(stderr, "Number of items in the queue: %lu\r\n", uxQueueMessagesWaiting( xQueue ) );
    }    
    // vTaskDelete(NULL);
}

