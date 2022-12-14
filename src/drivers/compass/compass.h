/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "targets/target.h"

#include "common/vector.h"
#include "common/axis.h"
#include "drivers/bus.h"
#include "drivers/sensor.h"

typedef struct magDev_s {
    busDevice_t * busDev;
    sensorMagInitFuncPtr init;  // initialize function
    sensorMagReadFuncPtr read;  // read 3 axis data function
    struct {
        bool useExternal;
        union {
            fpMat3_t externalRotation;
            sensor_align_e onBoard;
        };
    } magAlign;
    uint8_t magSensorToUse;
    int16_t magADCRaw[XYZ_AXIS_COUNT];
} magDev_t;
