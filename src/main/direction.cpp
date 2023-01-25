#include <Arduino.h>
#include "direction.h"
#include "sensors/compass.h"
#include "drivers/compass/compass.h"

void directionTask(void * parameter)
{
    if (!compassInit()) {
        vTaskDelete(NULL);
        return;
    }

    for( ;; ) {
        TickType_t start = xTaskGetTickCount();

        compassUpdate();

        vTaskDelay(100 / portTICK_PERIOD_MS - (xTaskGetTickCount() - start));
    }
}