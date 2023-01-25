#pragma once

#include "targets/target.h"
#include <Arduino.h>
#include "drivers/serial.h"
#include "io/serial.h"
#include "driver/uart.h"

typedef struct {
    serialPort_t port;

#ifdef USE_HAL_DRIVER
    UART_HandleTypeDef Handle;
#endif

    serialPortIdentifier_e USARTx;
} uartPort_t;

serialPort_t *uartOpen(serialPortIdentifier_e USARTx, serialReceiveCallbackPtr rxCallback, void *rxCallbackData, uint32_t baudRate, portMode_t mode, portOptions_t options);
