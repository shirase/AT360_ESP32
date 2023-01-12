#pragma once

#include <Arduino.h>

typedef enum {
    BAUD_AUTO = 0,
    BAUD_1200,
    BAUD_2400,
    BAUD_4800,
    BAUD_9600,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200,
    BAUD_230400,
    BAUD_250000,
    BAUD_460800,
    BAUD_921600,
    BAUD_1000000,
    BAUD_1500000,
    BAUD_2000000,
    BAUD_2470000,

    BAUD_MIN = BAUD_AUTO,
    BAUD_MAX = BAUD_2470000,
} baudRate_e;

extern const uint32_t baudRates[];

typedef enum {
    FUNCTION_NONE                       = 0,
    FUNCTION_MSP                        = (1 << 0), // 1
    FUNCTION_GPS                        = (1 << 1), // 2
    FUNCTION_TELEMETRY_FRSKY            = (1 << 2), // 4
    FUNCTION_TELEMETRY_HOTT             = (1 << 3), // 8
    FUNCTION_TELEMETRY_LTM              = (1 << 4), // 16
    FUNCTION_TELEMETRY_SMARTPORT        = (1 << 5), // 32
    FUNCTION_RX_SERIAL                  = (1 << 6), // 64
    FUNCTION_BLACKBOX                   = (1 << 7), // 128
    FUNCTION_TELEMETRY_MAVLINK          = (1 << 8), // 256
    FUNCTION_TELEMETRY_IBUS             = (1 << 9), // 512
    FUNCTION_RCDEVICE                   = (1 << 10), // 1024
    FUNCTION_VTX_SMARTAUDIO             = (1 << 11), // 2048
    FUNCTION_VTX_TRAMP                  = (1 << 12), // 4096
    FUNCTION_UNUSED_1                   = (1 << 13), // 8192: former\ UAV_INTERCONNECT
    FUNCTION_OPTICAL_FLOW               = (1 << 14), // 16384
    FUNCTION_LOG                        = (1 << 15), // 32768
    FUNCTION_RANGEFINDER                = (1 << 16), // 65536
    FUNCTION_VTX_FFPV                   = (1 << 17), // 131072
    FUNCTION_ESCSERIAL                  = (1 << 18), // 262144: this is used for both SERIALSHOT and ESC_SENSOR telemetry
    FUNCTION_TELEMETRY_SIM              = (1 << 19), // 524288
    FUNCTION_FRSKY_OSD                  = (1 << 20), // 1048576
    FUNCTION_DJI_HD_OSD                 = (1 << 21), // 2097152
    FUNCTION_SERVO_SERIAL               = (1 << 22), // 4194304
    FUNCTION_TELEMETRY_SMARTPORT_MASTER = (1 << 23), // 8388608
    FUNCTION_IMU2                       = (1 << 24), // 16777216
	FUNCTION_HDZERO_OSD                 = (1 << 25), // 33554432
} serialPortFunction_e;

typedef enum {
    SERIAL_PORT_NONE = -1,
    SERIAL_PORT_USART1 = 0,
    SERIAL_PORT_USART2,
    SERIAL_PORT_USART3,
    SERIAL_PORT_IDENTIFIER_MAX = SERIAL_PORT_USART3
} serialPortIdentifier_e;

typedef struct serialPortConfig_s {
    uint32_t functionMask;
    serialPortIdentifier_e identifier;
    uint8_t msp_baudrateIndex;
    uint8_t gps_baudrateIndex;
    uint8_t peripheral_baudrateIndex;
    uint8_t telemetry_baudrateIndex; // not used for all telemetry systems, e.g. HoTT only works at 19200.
} serialPortConfig_t;

#define SERIAL_PORT_COUNT 3

typedef struct serialConfig_s {
    serialPortConfig_t portConfigs[SERIAL_PORT_COUNT];
    uint8_t reboot_character;               // which byte is used to reboot. Default 'R', could be changed carefully to something else.
} serialConfig_t;

serialPortConfig_t *findSerialPortConfig(serialPortFunction_e function);
