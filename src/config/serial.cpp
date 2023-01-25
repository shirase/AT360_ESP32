#include "serial.h"
#include "io/serial.h"
#include "driver/uart.h"

static serialConfig_t _serialConfig = {
    .portConfigs = {
        {
            .functionMask = FUNCTION_NONE,
            .identifier = UART_NUM_0,
            0,
            0,
            0,
            0
        },
        {
            .functionMask = FUNCTION_GPS,
            .identifier = UART_NUM_1,
            0,
            0,
            0,
            0
        },
        {
            .functionMask = FUNCTION_NONE,
            .identifier = UART_NUM_2,
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