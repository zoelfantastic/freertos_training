#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#define STACK_SIZE 200

static void prvTaskFunction ( void *pvParams );

/**
 * @brief TCB and stack buffers for the task.
 */
static StaticTask_t xTaskBuffer;
static StackType_t xTaskStack[ configMINIMAL_STACK_SIZE ];

int main( void ) {

    /*** IMPORTANT  : for using static allocation, set both configSUPPORT_STATIC_ALLOCATION and configKERNEL_PROVIDED_STATIC_MEMORY to 1 */

    /* TODO 1 - Create a FreeRTOS task using xTaskCreateStatic API which
     * uses prvTaskFunction as the task function.
     *
     * Use the following values for xTaskCreateStatic parameters:
     * pxTaskCode       prvTaskFunction
     * pcName           "Task1"
     * ulStackDepth     configMINIMAL_STACK_SIZE
     * pvParameters     NULL
     * uxPriority       tskIDLE_PRIORITY
     * puxStackBuffer   &( xTaskStackBuffer[ 0 ] )
     * pxTaskBuffer     &( xTaskTcbBuffer )
     *
     * Assign the return value to xCreatedTaskHandle.
     */

     TaskHandle_t xCreatedTaskHandle = NULL;
     static int8_t taskParameter = 1; /* static, so it persists after the function returns , 
                                        Hindari melewatkan variabel lokal dari main jika variabel tersebut tidak lagi digunakan setelah vTaskStartScheduler(), 
                                        karena stack main bisa tertimpa. Gunakan static atau heap.*/

     xCreatedTaskHandle = xTaskCreateStatic(
        prvTaskFunction,            /* Function that implements the task. */
        "TaskWithStaticAllocation", /* Text name for the task. */
        configMINIMAL_STACK_SIZE,   /* Number of indexes in the xStack array. */
        &taskParameter,             /* Parameter passed into the task. */
        tskIDLE_PRIORITY,           /* Priority at which the task is created. */
        &(xTaskStack[0]),           /* Array to use as the task's stack. */
        &xTaskBuffer                /* Variable to hold the task's data structure. */
     );

    vTaskStartScheduler();

    return 0;
}

static void prvTaskFunction ( void *pvParams) 
{
     /* The parameter value is expected to be 1 as 1 is passed in the
           pvParameters value in the call to xTaskCreateStatic(). */
    int8_t *receivedParams = (int8_t *) pvParams;
    int8_t expectedValue = *receivedParams;
    for(;;) {
        fprintf(stderr, "Static Allocated Task is running, with parameter value: %d\n", expectedValue);
        vTaskDelay(1000);
    }
}