#include "servos.h"
#include <Arduino.h>

void servosTask(void * parameter)
{
    Servo servo1;
    Servo servo2;

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    servo1.setPeriodHertz(50);
    servo2.setPeriodHertz(50);

    // Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
    servo1.attach(32, 500, 2400);
    servo2.attach(33, 500, 2400);

    servo1.write(1500);
    servo2.write(1500);

    delay(20);

    for( ;; ) {
        TickType_t start = xTaskGetTickCount();

        servo1.write(2000);
        servo2.write(2000);

        vTaskDelay(100 / portTICK_PERIOD_MS - (xTaskGetTickCount() - start));
    }
}