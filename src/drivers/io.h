#pragma once

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#define IO_t uint8_t

#include "bus.h"

void IOLo(IO_t io);
void IOHi(IO_t io);
