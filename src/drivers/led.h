#pragma once

#include <Arduino.h>

const uint8_t led_pin = 2;

#define LED0_ON digitalWrite(led_pin, 1);
#define LED0_OFF digitalWrite(led_pin, 0);
#define LED0_TOGGLE digitalWrite(led_pin, !digitalRead(led_pin));

#define LED1_ON digitalWrite(led_pin, 1);
#define LED1_OFF digitalWrite(led_pin, 0);
#define LED1_TOGGLE digitalWrite(led_pin, !digitalRead(led_pin));