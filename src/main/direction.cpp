#include <Arduino.h>
#include "direction.h"
#include "sensors/compass.h"
#include "drivers/compass/compass.h"

void directionTask(void * parameter)
{
    if (!compassDetect(&mag.dev, MAG_HMC5883)) {
        vTaskDelete(NULL);
        return;
    }

    for( ;; ) {
        TickType_t start = xTaskGetTickCount();

        vTaskDelay(100 / portTICK_PERIOD_MS - (xTaskGetTickCount() - start));
    }
}