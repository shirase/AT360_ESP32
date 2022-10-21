#include "compas.h"
#include "common/utils.h"
#include "drivers/compas/compass_hmc5883l.h"

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