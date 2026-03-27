#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "FreeRTOS.h"
#include "task.h"

#define mainDELAY_LOOP_COUNT		( 0xffffff )
/** 
 *  function that implement task
*/
static void prvTaskFunction( void *pvParams );

int main( void ) {
    BaseType_t xTaskCreationResult = pdFALSE;

     /* TODO 1 - Create a FreeRTOS task using xTaskCreate API which
     * uses prvTaskFunction as the task function.
     *
     * Use the following values for xTaskCreate parameters:
     * pxTaskCode       prvTaskFunction
     * pcName           "Task"
     * usStackDepth     configMINIMAL_STACK_SIZE
     * pvParameters     NULL
     * uxPriority       tskIDLE_PRIORITY
     * pxCreatedTask    NULL
     *
     * Assign the return value to xTaskCreationResult.
     */

    xTaskCreationResult = xTaskCreate(
        prvTaskFunction,
        "Task",
        configMINIMAL_STACK_SIZE,
        "Task is Running from Task Function Parameter\n",
        tskIDLE_PRIORITY,
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
static void prvTaskFunction( void *pvParams ) {

    (void) pvParams; /* Just to prevent compiler warnings. */
    volatile unsigned long ul = 0; /* volatile to ensure ul is not optimized away. */

    char *pcTaskParameter; 
    pcTaskParameter = (char *) pvParams;
    for( ;; ) {
        fprintf(stderr, pcTaskParameter);
        //vPrintString(pcTaskParameter);

        for (ul  = 0; ul < mainDELAY_LOOP_COUNT; ul++)
        {
            /*
            * This loop is just a very crude delay implementation. There is
            * nothing to do in here. Later exercises will replace this crude
            * loop with a proper delay/sleep function.
            */
        }
    }

    
    
}

