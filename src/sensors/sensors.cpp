#include "sensors.h"

static uint32_t enabledSensors = 0;

bool sensors(uint32_t mask)
{
    return enabledSensors & mask;
}

void sensorsSet(uint32_t mask)
{
    enabledSensors |= mask;
}

void sensorsClear(uint32_t mask)
{
    enabledSensors &= ~(mask);
}

uint32_t sensorsMask(void)
{
    return enabledSensors;
}