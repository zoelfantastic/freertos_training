/* standard includes*/
#include <stdio.h>
#include <unistd.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#define mainDELAY_LOOP_COUNT		( 0xfffffff )

/** Function that implements the task. */
static void prvTaskFunction1( void *pvParameters );
static void prvTaskFunction2( void *pvParameters );

/** Scheduling using cooperative scheduling.  Tasks must explicitly yield to allow other tasks to run. */

int main( void ) {
    BaseType_t xTaskCreationResult = pdFALSE;

    xTaskCreationResult = xTaskCreate( prvTaskFunction1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );

    configASSERT( xTaskCreationResult == pdPASS );

    xTaskCreationResult = xTaskCreate( prvTaskFunction2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

    configASSERT( xTaskCreationResult == pdPASS );

    vTaskStartScheduler();

     /* Should not reach here. */
    for( ;; )
    {
    }

    /* Just to make the compiler happy. */
    return 0;
}

void prvTaskFunction1(void *pvParameters)
{
    BaseType_t xTaskCreationResult = pdFALSE;
    (void) pvParameters;
    volatile unsigned long ulCount;

    for(;;) {
        
        fprintf(stderr, "Task 1 is running\n");
        taskYIELD();
        /* Delay for a period. */
        //vTaskDelay( pdMS_TO_TICKS( 100 ) ); /* Delay for 1000 ms. It also means , Task 1 is entering block state */
        // for( ulCount = 0; ulCount < 1; ulCount++ )
        // {
        //     if (ulCount == 5000000)
        //     {
        //         taskYIELD();
        //     }
        // }
        
    }
}

void prvTaskFunction2(void *pvParameters)
{
    (void) pvParameters;

    for(;;) {
        fprintf(stderr, "Task 2 continous with lower priority is running\n");
        /* Delay for a period. */
        vTaskDelay( 0 ); /* Delay for 1000 ms. */
        //taskYIELD();
    }
}

