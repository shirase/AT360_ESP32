#pragma once

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct direction_s {
    bool    valid;
    int32_t roll; // decidegres
    int32_t pitch; // decidegres
    int32_t yaw; // decidegres
} direction_t;

void directionTask(void * parameter);
