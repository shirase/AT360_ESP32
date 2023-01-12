#include "position.h"
#include <Arduino.h>
#include "io/gps.h"

void positionTask(void * parameter)
{
    gpsInit();

    for( ;; ) {
        TickType_t start = xTaskGetTickCount();

        gpsUpdate();

        vTaskDelay(100 / portTICK_PERIOD_MS - (xTaskGetTickCount() - start));
    }
}