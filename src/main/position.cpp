#include "position.h"

void positionTask(void * parameter)
{
    for( ;; ) {
        TickType_t start = xTaskGetTickCount();

        vTaskDelay(100 / portTICK_PERIOD_MS - (xTaskGetTickCount() - start));
    }
}