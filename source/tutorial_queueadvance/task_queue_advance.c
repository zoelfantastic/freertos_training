#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

/*
* BRIEF MODE 1: 
* This example demonstrates creating a queue, sending data to the queue from multiple tasks, and receiving
* data from the queue. The queue is created to hold data items of type int32_t. The tasks that send to the
* queue do not specify a block time, whereas the task that receives from the queue does.
* The tasks that send to the queue have a lower priority than the task that receives from the queue. This means
* the queue should never contain more than one item because, as soon as data is sent to the queue the
* receiving task will unblock, pre-empt the sending task (because it has a higher priority), and remove the data,
* leaving the queue empty once again.
*/

/*
* BRIEF MODE 2: 
* This example is similiar to the previous one, but the sending tasks have a higher priority than the receiving task. This means
* the queue will contain more than one item because, as soon as data is sent to the queue the sending task will continue to run (because it has a higher priority than 
* the receiving task) and send more data to the queue until the queue is full. Once the queue is full the sending task will block, allowing the receiving task to run 
* and remove items from the queue until there is space in the queue for the sending task to unblock and start sending again.
* Also, the created queue holds structures rather than integers.
*/

/**
 * STRUCT MODE 2
 */
 /* Define an enumerated type used to identify the source of the data. */
 typedef enum
 {
    eSender1,
    eSender2
 } DataSource_t;

 /* Define the structure type that will be passed on the queue. */
 typedef struct 
 {
    uint8_t ucValue;
    DataSource_t eDataSource;
 } Data_t;

 /* Declare two variables of type Data_t that will be passed on the queue. */
static const Data_t xStructsToSend[ 2 ] = 
{
    {100, eSender1},
    {200, eSender2}
};

#define mainDELAY_LOOP_COUNT		( 0xffffff )
#define USED_MODE 2
/** 
 *  function that implement task
*/
static void vSenderTask( void *pvParams );
static void vReceiverTask( void *pvParams );


static QueueHandle_t xQueue;

int main( void ) {
    
    #if (USED_MODE == 1)
        /* The queue is created to hold a maximum of 5 values, each of which is
        large enough to hold a variable of type int32_t. */
        xQueue = xQueueCreate( 5, sizeof( int32_t ) );

           if( xQueue != NULL )
        {
            /* Create two instances of the task that will send to the queue. The
            task parameter is used to pass the value that the task will write
            to the queue, so one task will continuously write 100 to the queue
            while the other task will continuously write 200 to the queue. Both
            tasks are created at priority 1. */
            xTaskCreate( vSenderTask, "Sender1", 1000, ( void * ) 100, 1, NULL );
            xTaskCreate( vSenderTask, "Sender2", 1000, ( void * ) 200, 1, NULL );
            /* Create the task that will read from the queue. The task is created
            with priority 2, so above the priority of the sender tasks. */
            xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 0, NULL );
            /* Start the scheduler so the created tasks start executing. */
            vTaskStartScheduler();
        }
        else
        {
            /* The queue could not be created. */
        }

    #elif (USED_MODE == 2)
        xQueue = xQueueCreate( 3, sizeof( Data_t ) );

        if( xQueue != NULL )
        {
            /* Create two instances of the task that will write to the queue. The
            parameter is used to pass the structure that the task will write to
            the queue, so one task will continuously send xStructsToSend[ 0 ]
            to the queue while the other task will continuously send
            xStructsToSend[ 1 ]. Both tasks are created at priority 2, which is
            above the priority of the receiver. */
            xTaskCreate( vSenderTask, "Sender1", 1000, &( xStructsToSend[ 0 ] ), 2, NULL );
            xTaskCreate( vSenderTask, "Sender2", 1000, &( xStructsToSend[ 1 ] ), 2, NULL );
            /* Create the task that will read from the queue. The task is created with priority 1, so below the priority of the sender tasks. */
            xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 1, NULL );
            /* Start the scheduler so the created tasks start executing. */
            vTaskStartScheduler();
        }
        else
        {
            /* The queue could not be created. */
        }
    #endif

    /* If all is well then main() will never reach here as the scheduler will
    now be running the tasks. If main() does reach here then it is likely
    that there was insufficient FreeRTOS heap memory available for the idle
    task to be created. Chapter 3 provides more information on heap memory
    management. */
    for( ;; );

    /* Just to make the compiler happy. */
    return 0;
}

/** 
 *  function that implement task
*/
static void vSenderTask( void *pvParams ) {

    #if ( USED_MODE == 1 )
        /* Two instances of this task are created so the value that is sent to
        the queue is passed in via the task parameter - this way each instance
        can use a different value. The queue was created to hold values of type
        int32_t, so cast the parameter to the required type. */
        int32_t lValueToSend;
        lValueToSend = ( int32_t ) pvParams;
    #elif ( USED_MODE == 2 )
        const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
    #endif

    BaseType_t xStatus;
    
    for( ;; ) {
        #if ( USED_MODE == 1 )
            /* Send the value to the queue.
            The first parameter is the queue to which data is being sent. The
            queue was created before the scheduler was started, so before this
            task started to execute.The second parameter is the address of the data to be sent, in this
            case the address of lValueToSend.
            The third parameter is the Block time – the time the task should be
            kept in the Blocked state to wait for space to become available on
            the queue should the queue already be full. In this case a block
            time is not specified because the queue should never contain more
            than one item, and therefore never be full. */
            xStatus = xQueueSendBack( xQueue, &lValueToSend, 0 );
        #elif ( USED_MODE == 2 )
            /* Send to the queue.
            The second parameter is the address of the structure being sent. The
            address is passed in as the task parameter so pvParameters is used
            directly.
            The third parameter is the Block time - the time the task should be
            kept in the Blocked state to wait for space to become available on
            the queue if the queue is already full. A block time is specified
            because the sending tasks have a higher priority than the receiving
            task so the queue is expected to become full. The receiving task
            will remove items from the queue when both sending tasks are in the
            Blocked state. */
            xStatus = xQueueSendToBack( xQueue, pvParams, xTicksToWait );
        #endif
        if( xStatus != pdPASS )
        {
            /* The send operation could not complete because the queue was full-
            this must be an error as the queue should never contain more than
            one item! */
            // vPrintString( "Could not send to the queue.\r\n" );
            fprintf(stderr, "Could not send to the queue.\r\n" );
        }
    }
}

static void vReceiverTask( void *pvParams ) {

    #if ( USED_MODE == 1 )
        /* The queue was created to hold values of type int32_t, so declare
        a variable of that type to receive the values that are received from
        the queue. */
        int32_t lReceivedValue;
        const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
    #elif ( USED_MODE == 2 )
        Data_t xReceivedStructure;
    #endif

    BaseType_t xStatus;
    
    for ( ;; ) {
        #if ( USED_MODE == 1 )
            /* This call should always find the queue empty because this task will
            mmediately remove any data that is written to the queue. */
            f( uxQueueMessagesWaiting( xQueue ) != 0 )
            
              //vPrintString( "Queue should have been empty!\r\n" );
              fprintf(stderr, "Queue should have been empty!\r\n" );
        #elif ( USED_MODE == 2 )
            /* Because it has the lowest priority this task will only run when the
            sending tasks are in the Blocked state. The sending tasks will only
            enter the Blocked state when the queue is full so this task always
            expects the number of items in the queue to be equal to the queue
            length, which is 3 in this case. */
            if( uxQueueMessagesWaiting( xQueue ) != 3 )
            {
                fprintf(stderr, "Queue should have been full!\r\n" );
            }
        #endif
        

        #if ( USED_MODE == 1 )
            /* Receive data from the queue.
            The first parameter is the queue from which data is to be received.
            The queue is created before the scheduler is started, and therefore before this task runs for the first time.
            The second parameter is the buffer into which the received data will
            be placed. In this case the buffer is simply the address of a
            variable that has the required size to hold the received data.
            The last parameter is the block time – the maximum amount of time
            that the task will remain in the Blocked state to wait for data to
            be available should the queue already be empty. */
            xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );

            if( xStatus == pdPASS )
            {
                /* Data was successfully received from the queue, print out the
                received value. */
                //vPrintStringAndNumber( "Received = ", lReceivedValue );
                fprintf(stderr, "Value Received from the queue: %ld\r\n", lReceivedValue);
            }
            else
            {
                /* Data was not received from the queue even after waiting for
                100ms. This must be an error as the sending tasks are free
                running and will be continuously writing to the queue. */
                //vPrintString( "Could not receive from the queue.\r\n" );
                fprintf(stderr, "Could not receive from the queue.\r\n" );
            }
        #elif ( USED_MODE == 2 )
            /* Receive from the queue.
            The second parameter is the buffer into which the received data will
            be placed. In this case the buffer is simply the address of a
            variable that has the required size to hold the received structure.
            The last parameter is the block time - the maximum amount of time
            that the task will remain in the Blocked state to wait for data to
            be available if the queue is already empty. In this case a block
            time is not necessary because this task will only run when the
            queue is full. */
            xStatus = xQueueReceive( xQueue, &xReceivedStructure, 0 );

            if( xStatus == pdPASS )
            {
                /* Data was successfully received from the queue, print out the
                received value and the source of the value. */
                if( xReceivedStructure.eDataSource == eSender1 )
                {
                    fprintf(stderr, "From Sender 1 = %d\r\n", xReceivedStructure.ucValue);
                }
                else
                {
                    fprintf(stderr, "From Sender 2 = %d\r\n", xReceivedStructure.ucValue);
                }
                }
                else
                {
                    /* Nothing was received from the queue. This must be an error as
                    this task should only run when the queue is full. */
                    fprintf(stderr, "Could not receive from the queue.\r\n" );
                }
        #endif
    }
}

