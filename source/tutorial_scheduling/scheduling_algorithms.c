/* standard includes*/
#include <stdio.h>
#include <unistd.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

#define mainDELAY_LOOP_COUNT		( 0xffffff )

/** Function that implements the task. */
static void prvTaskFunction1( void *pvParameters );
static void prvTaskFunction2( void *pvParameters );
static void prvTaskFunction3( void *pvParameters );

/** Scheduling using preemptive with time slicing 
 *  CASE 1 - Two tasks with same priority
 *  CASE 2 - Two tasks with different priorities
 * 
*/

int main( void ) {
    BaseType_t xTaskCreationResult = pdFALSE;

    xTaskCreationResult = xTaskCreate( prvTaskFunction1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

    if( xTaskCreationResult == pdFALSE )
    {
        fprintf(stderr, "Task 1 creation failed\n");
    }

    xTaskCreationResult = xTaskCreate( prvTaskFunction2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

    if( xTaskCreationResult == pdFALSE )
    {
        fprintf(stderr, "Task 2 creation failed\n");
    }

    // xTaskCreationResult = xTaskCreate( prvTaskFunction3, "Task3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

    // if( xTaskCreationResult == pdFALSE )
    // {
    //     fprintf(stderr, "Task 3 creation failed\n");
    // }

    vTaskStartScheduler();

     /* Should not reach here. */
    for( ;; )
    {
        fprintf(stderr, "Scheduler failed to start\n");
    }

    /* Just to make the compiler happy. */
    return 0;
}

void prvTaskFunction1(void *pvParameters)
{
    (void) pvParameters;

    /* ulCount is declared volatile to ensure it is not optimized out. */
    volatile unsigned long ulCount;

    for(;;) {
        fprintf(stderr, "Task 1 is running\n");
        /* Delay for a period. */
        for( ulCount = 0; ulCount < mainDELAY_LOOP_COUNT; ulCount++ )
        {
        /*
        * This loop is just a very crude delay implementation. There is
        * nothing to do in here. Later examples will replace this crude
        * loop with a proper delay/sleep function.
        */
        }
    }
}

void prvTaskFunction2(void *pvParameters)
{
    (void) pvParameters;

    /* ulCount is declared volatile to ensure it is not optimized out. */
    volatile unsigned long ulCount;

    for(;;) {
        fprintf(stderr, "Task 2 is running\n");
        /* Delay for a period. */
        for( ulCount = 0; ulCount < mainDELAY_LOOP_COUNT; ulCount++ )
        {
        /*
        * This loop is just a very crude delay implementation. There is
        * nothing to do in here. Later examples will replace this crude
        * loop with a proper delay/sleep function.
        */
        }
    }
}

void prvTaskFunction3(void *pvParameters)
{
     (void) pvParameters;

    /* ulCount is declared volatile to ensure it is not optimized out. */
    volatile unsigned long ulCount;

    for(;;) {
        fprintf(stderr, "Task 3 is running\n");
        /* Delay for a period. */
        for( ulCount = 0; ulCount < mainDELAY_LOOP_COUNT; ulCount++ )
        {
        /*
        * This loop is just a very crude delay implementation. There is
        * nothing to do in here. Later examples will replace this crude
        * loop with a proper delay/sleep function.
        */
        }
    }
}
