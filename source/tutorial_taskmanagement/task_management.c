#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/*
* BRIEF: 
* vTaskDelayUntil() is similar to vTaskDelay(). As just demonstrated, the vTaskDelay() parameter
* specifies the number of tick interrupts that should occur between a task calling vTaskDelay(), and the same
* task once again transitioning out of the Blocked state. The length of time the task remains in the blocked state
* is specified by the vTaskDelay() parameter, but the time at which the task leaves the blocked state is relative
* to the time at which vTaskDelay() was called.
* 
* The parameters to vTaskDelayUntil() specify, instead, the exact tick count value at which the calling task
* should be moved from the Blocked state into the Ready state. vTaskDelayUntil() is the API function to use
* when a fixed execution period is required (where you want your task to execute periodically with a fixed
* frequency), as the time at which the calling task is unblocked is absolute, rather than relative to when the
* function was called (as is the case with vTaskDelay()).
*
* using vTaskDelay() does not guarantee that the
* frequency at which they run is fixed, as the time at which the tasks leave the Blocked state is relative to when
* they call vTaskDelay(). Converting the tasks to use vTaskDelayUntil() instead of vTaskDelay() solves
* this potential problem.
*/

#define mainDELAY_LOOP_COUNT		( 0xffffff )
/** 
 *  function that implement task
*/
static void vContinuouosTask( void *pvParams );
static void vPeriodicTask( void *pvParams );

int main( void ) {
    BaseType_t xTaskCreationResult = pdFALSE;

    xTaskCreationResult = xTaskCreate(
        vContinuouosTask,
        "ContinuousTask1",
        configMINIMAL_STACK_SIZE,
        "Continuous task 1 is running\n",
        0,
        NULL
    );

    if (xTaskCreationResult != pdPASS) {
        fprintf(stderr, "Task creation failed\n");  
    }

    xTaskCreationResult = xTaskCreate(
        vContinuouosTask,
        "ContinuousTask2",
        configMINIMAL_STACK_SIZE,
        "Continuous task 2 is running\n",
        //"\n",
        0,
        NULL
    );

    if (xTaskCreationResult != pdPASS) {
        fprintf(stderr, "Task creation failed\n");  
    }

    xTaskCreationResult = xTaskCreate(
        vPeriodicTask,
        "PeriodicTask",
        configMINIMAL_STACK_SIZE,
        "Periodic task is running\n",
        2,
        NULL
    );

    if (xTaskCreationResult != pdPASS) {
        fprintf(stderr, "Task creation failed\n");  
    }

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
static void vContinuouosTask( void *pvParams ) {

    (void) pvParams; /* Just to prevent compiler warnings. */

    char *pcTaskParameter; 
    pcTaskParameter = (char *) pvParams;
    for( ;; ) {
        fprintf(stderr, pcTaskParameter);
        vTaskDelay( pdMS_TO_TICKS( 1 ) ); /* Delay for a period. */
    }    
    // vTaskDelete(NULL);
}

static void vPeriodicTask( void *pvParams ) {

    (void) pvParams; /* Just to prevent compiler warnings. */

    char *pcTaskParameter; 
    pcTaskParameter = (char *) pvParams;

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS( 3 ); /* 1000 ms. */

    /* Initialize the xLastWakeTime variable with the current time. */
    xLastWakeTime = xTaskGetTickCount();

    for( ;; ) {
        fprintf(stderr, pcTaskParameter);
        /* Wait for the next cycle. */
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }    
    // vTaskDelete(NULL);
}

