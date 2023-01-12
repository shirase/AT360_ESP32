#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct position_s {
    bool    valid;
    float   scale;
    int32_t lat;    // Lattitude * 1e+7
    int32_t lon;    // Longitude * 1e+7
    int32_t alt;    // Altitude in centimeters (meters * 100)
} position_t;

void positionTask(void * parameter);
