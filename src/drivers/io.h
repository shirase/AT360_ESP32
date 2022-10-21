#pragma once

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#include "bus.h"

typedef uint8_t IO_t; 

void IOLo(IO_t io);
void IOHi(IO_t io);
