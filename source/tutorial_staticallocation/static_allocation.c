#include <stdio.h>
#include <unistd.h>

#include "FreeRTOS.h"
#include "task.h"

static void prvTaskFunction ( void *pvParams );

/**
 * @brief TCB and stack buffers for the task.
 */
static StaticTask_t xTaskTcbBuffer;
static StackType_t xTaskStackBuffer[ configMINIMAL_STACK_SIZE ];

int main( void ) {
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

     
}