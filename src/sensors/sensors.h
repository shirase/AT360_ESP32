#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    SENSOR_INDEX_GYRO = 0,
    SENSOR_INDEX_ACC,
    SENSOR_INDEX_MAG,
    SENSOR_INDEX_COUNT
} sensorIndex_e;

typedef struct int16_flightDynamicsTrims_s {
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
} flightDynamicsTrims_def_t;

typedef union flightDynamicsTrims_u {
    int16_t raw[3];
    flightDynamicsTrims_def_t values;
} flightDynamicsTrims_t;

typedef enum {
    SENSOR_GYRO = 1 << 0, // always present
    SENSOR_ACC = 1 << 1,
    SENSOR_BARO = 1 << 2,
    SENSOR_MAG = 1 << 3,
    SENSOR_RANGEFINDER = 1 << 4,
    SENSOR_PITOT = 1 << 5,
    SENSOR_OPFLOW = 1 << 6,
    SENSOR_GPS = 1 << 7,
    SENSOR_GPSMAG = 1 << 8,
    SENSOR_TEMP = 1 << 9
} sensors_e;

bool sensors(uint32_t mask);
void sensorsSet(uint32_t mask);
void sensorsClear(uint32_t mask);
uint32_t sensorsMask(void);
