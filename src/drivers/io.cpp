#include "io.h"
#include <Arduino.h>

void IOLo(IO_t io)
{
    digitalWrite(io, LOW);
}

void IOHi(IO_t io)
{
    digitalWrite(io, HIGH);
}
