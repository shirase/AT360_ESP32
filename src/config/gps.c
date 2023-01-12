#include "gps.h"
#include <stdbool.h>

static gpsConfig_t _gpsConfig = {
    .provider = GPS_UBLOX,
    .sbasMode = SBAS_AUTO,
    .autoConfig = GPS_AUTOCONFIG_ON,
    .autoBaud = GPS_AUTOBAUD_ON,
    .dynModel = GPS_DYNMODEL_PEDESTRIAN,
    .ubloxUseGalileo = false,
    .gpsMinSats = 8
};

gpsConfig_t* gpsConfig()
{
    return &_gpsConfig;
}