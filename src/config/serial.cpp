#include "serial.h"
#include "io/serial.h"

static serialConfig_t _serialConfig = {
    .portConfigs = {
        {
            .functionMask = FUNCTION_NONE,
            .identifier = SERIAL_PORT_USART1,
            0,
            0,
            0,
            0
        },
        {
            .functionMask = FUNCTION_GPS,
            .identifier = SERIAL_PORT_USART2,
            0,
            0,
            0,
            0
        },
        {
            .functionMask = FUNCTION_NONE,
            .identifier = SERIAL_PORT_USART3,
            0,
            0,
            0,
            0
        },
    },
    .reboot_character = 'R'
};

serialConfig_t* serialConfig()
{
    return &_serialConfig;
}