#include "compass.h"
#include "common/utils.h"
#include "drivers/compass/compass_hmc5883l.h"

mag_t mag;

bool compassDetect(magDev_t *dev, magSensor_e magHardwareToUse)
{
    magSensor_e magHardware = MAG_NONE;

    dev->magAlign.useExternal = false;
    dev->magAlign.onBoard = ALIGN_DEFAULT;

    switch (magHardwareToUse) {
    case MAG_NONE:
        break;
    case MAG_AUTODETECT:
        FALLTHROUGH;
    case MAG_HMC5883:
#ifdef USE_MAG_HMC5883
        if (hmc5883lDetect(dev)) {
            magHardware = MAG_HMC5883;
            break;
        }
#endif
        /* If we are asked for a specific sensor - break out, otherwise - fall through and continue */
        if (magHardwareToUse != MAG_AUTODETECT) {
            break;
        }
        FALLTHROUGH;
    default:
        break;
    }

    if (magHardware == MAG_NONE) {
        return false;
    }

    return true;
}

bool compassInit(void)
{
    if (!compassDetect(&mag.dev, MAG_HMC5883)) {
        return false;
    }

    mag.dev.init(&mag.dev);

    return true;
}

void compassUpdate(void)
{
    if (!mag.dev.read(&mag.dev)) {
        mag.magADC[X] = 0;
        mag.magADC[Y] = 0;
        mag.magADC[Z] = 0;
        return;
    }

    for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
        mag.magADC[axis] = mag.dev.magADCRaw[axis];  // int32_t copy to work with
    }
}