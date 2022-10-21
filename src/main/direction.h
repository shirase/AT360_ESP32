#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct direction_s {
    bool    valid;
    uint32_t roll; // decidegres
    uint32_t pitch; // decidegres
    uint32_t yaw; // decidegres
} direction_t;

void directionTask(void * parameter);
