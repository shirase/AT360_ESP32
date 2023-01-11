#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "drivers/compass/compass.h"
#include "sensors.h"
#include "targets/target.h"

typedef struct mag_s {
    magDev_t dev;
    int32_t magADC[XYZ_AXIS_COUNT];
} mag_t;

extern mag_t mag;

typedef enum {
    MAG_NONE = 0,
    MAG_AUTODETECT = 1,
    MAG_HMC5883 = 2,
} magSensor_e;

typedef struct compassConfig_s {
    int16_t mag_declination;                // Get your magnetic declination from here : http://magnetic-declination.com/
                                            // For example, -6deg 37min, = -637 Japan, format is [sign]dddmm (degreesminutes) default is zero.
    sensor_align_e mag_align;               // on-board mag alignment. Ignored if externally aligned via *DeciDegrees.
    uint8_t mag_hardware;                   // Which mag hardware to use on boards with more than one device
    flightDynamicsTrims_t magZero;
    int16_t magGain[XYZ_AXIS_COUNT];
#ifdef USE_DUAL_MAG
    uint8_t mag_to_use;
#endif
    uint8_t magCalibrationTimeLimit;        // Time for compass calibration (seconds)
    int16_t rollDeciDegrees;                // Alignment for external mag on the roll (X) axis (0.1deg)
    int16_t pitchDeciDegrees;               // Alignment for external mag on the pitch (Y) axis (0.1deg)
    int16_t yawDeciDegrees;                 // Alignment for external mag on the yaw (Z) axis (0.1deg)
} compassConfig_t;

#ifdef USE_MAG

bool compassDetect(magDev_t *dev, magSensor_e magHardwareToUse);

#endif