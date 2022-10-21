#include "direction.h"

void directionTask(void * parameter)
{
    for( ;; ) {
        TickType_t start = xTaskGetTickCount();

        vTaskDelay(100 / portTICK_PERIOD_MS - (xTaskGetTickCount() - start));
    }
}