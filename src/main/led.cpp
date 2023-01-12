#include "led.h"

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#define LED 2

void ledTask(void * parameter)
{
    pinMode(LED, OUTPUT);

    for( ;; ) {
        TickType_t start = xTaskGetTickCount();

        digitalWrite(LED, !digitalRead(LED));

        vTaskDelay(500 / portTICK_PERIOD_MS - (xTaskGetTickCount() - start));
    }
}