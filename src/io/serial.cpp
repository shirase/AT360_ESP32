#include "serial.h"
#include "stdlib.h"
#include "config/serial.h"
#include "io/serial.h"
#include "common/utils.h"

const uint32_t baudRates[] = { 0, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 250000,
        460800, 921600, 1000000, 1500000, 2000000, 2470000 }; // see baudRate_e

typedef struct findSerialPortConfigState_s {
    uint8_t lastIndex;
} findSerialPortConfigState_t;

static findSerialPortConfigState_t findSerialPortConfigState;

static serialPortUsage_t serialPortUsageList[SERIAL_PORT_COUNT];

serialPortConfig_t *findNextSerialPortConfig(serialPortFunction_e function)
{
    while (findSerialPortConfigState.lastIndex < SERIAL_PORT_COUNT) {
        serialPortConfig_t *candidate = &serialConfig()->portConfigs[findSerialPortConfigState.lastIndex++];

        if (candidate->functionMask & function) {
            return candidate;
        }
    }
    return NULL;
}

serialPortConfig_t *findSerialPortConfig(serialPortFunction_e function)
{
    memset(&findSerialPortConfigState, 0, sizeof(findSerialPortConfigState));

    return findNextSerialPortConfig(function);
}

void waitForSerialPortToFinishTransmitting(serialPort_t *serialPort)
{
    while (!isSerialTransmitBufferEmpty(serialPort)) {
        delay(10);
    };
}

serialPortUsage_t *findSerialPortUsageByIdentifier(serialPortIdentifier_e identifier)
{
    uint8_t index;
    for (index = 0; index < SERIAL_PORT_COUNT; index++) {
        serialPortUsage_t *candidate = &serialPortUsageList[index];
        if (candidate->identifier == identifier) {
            return candidate;
        }
    }
    return NULL;
}

serialPort_t *openSerialPort(
    serialPortIdentifier_e identifier,
    serialPortFunction_e function,
    serialReceiveCallbackPtr rxCallback,
    void *rxCallbackData,
    uint32_t baudRate,
    portMode_t mode,
    portOptions_t options)
{
#if (!defined(USE_VCP) && !defined(USE_UART1) && !defined(USE_UART2) && !defined(USE_UART3) && !defined(USE_SOFTSERIAL1) && !defined(USE_SOFTSERIAL2))
    UNUSED(rxCallback);
    UNUSED(rxCallbackData);
    UNUSED(baudRate);
    UNUSED(mode);
    UNUSED(options);
#endif

    serialPortUsage_t *serialPortUsage = findSerialPortUsageByIdentifier(identifier);
    if (!serialPortUsage || serialPortUsage->function != FUNCTION_NONE) {
        // not available / already in use
        return NULL;
    }

    serialPort_t *serialPort = NULL;

    switch (identifier) {
#ifdef USE_VCP
        case SERIAL_PORT_USB_VCP:
            serialPort = usbVcpOpen();
            break;
#endif
#ifdef USE_UART1
        case SERIAL_PORT_USART1:
            serialPort = uartOpen(USART1, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_UART2
        case SERIAL_PORT_USART2:
            serialPort = uartOpen(USART2, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_UART3
        case SERIAL_PORT_USART3:
            serialPort = uartOpen(USART3, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_UART4
        case SERIAL_PORT_USART4:
            serialPort = uartOpen(UART4, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_UART5
        case SERIAL_PORT_USART5:
            serialPort = uartOpen(UART5, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_UART6
        case SERIAL_PORT_USART6:
            serialPort = uartOpen(USART6, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_UART7
        case SERIAL_PORT_USART7:
            serialPort = uartOpen(UART7, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_UART8
        case SERIAL_PORT_USART8:
            serialPort = uartOpen(UART8, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_SOFTSERIAL1
        case SERIAL_PORT_SOFTSERIAL1:
            serialPort = openSoftSerial(SOFTSERIAL1, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
#ifdef USE_SOFTSERIAL2
        case SERIAL_PORT_SOFTSERIAL2:
            serialPort = openSoftSerial(SOFTSERIAL2, rxCallback, rxCallbackData, baudRate, mode, options);
            break;
#endif
        default:
            break;
    }

    if (!serialPort) {
        return NULL;
    }

    serialPort->identifier = identifier;

    serialPortUsage->function = function;
    serialPortUsage->serialPort = serialPort;

    return serialPort;
}