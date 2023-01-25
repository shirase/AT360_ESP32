#include "accgyro.h"
#include <Arduino.h>
#include "drivers/accgyro/accgyro.h"

void accgyroTask(void * parameter)
{
    for( ;; ) {
        TickType_t start = xTaskGetTickCount();

        vTaskDelay(100 / portTICK_PERIOD_MS - (xTaskGetTickCount() - start));
    }
}