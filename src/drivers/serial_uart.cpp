#include "serial_uart.h"

extern const struct serialPortVTable uartVTable;

static uartPort_t uartPorts[SERIAL_PORT_COUNT];
static const int uart_buffer_size = (1024 * 2);

serialPort_t *uartOpen(serialPortIdentifier_e USARTx, serialReceiveCallbackPtr rxCallback, void *rxCallbackData, uint32_t baudRate, portMode_t mode, portOptions_t options)
{
    uartPort_t *s = &uartPorts[USARTx];

    const uart_port_t uart_num = USARTx;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    s->port.vTable = &uartVTable;

    // common serial initialisation code should move to serialPort::init()
    /*s->port.rxBufferHead = s->port.rxBufferTail = 0;
    s->port.txBufferHead = s->port.txBufferTail = 0;*/
    // callback works for IRQ-based RX ONLY
    s->port.rxCallback = rxCallback;
    s->port.rxCallbackData = rxCallbackData;
    s->port.mode = mode;
    s->port.baudRate = baudRate;
    s->port.options = options;

    if (mode & MODE_RX) {
        
    }

    if (mode & MODE_TX) {
        
    }

    QueueHandle_t uart_queue;
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size, \
                                            uart_buffer_size, 10, &uart_queue, 0));

    return (serialPort_t *)s;
}

void uartWrite(serialPort_t *instance, uint8_t ch)
{
    uartPort_t *s = (uartPort_t *)instance;

    uart_write_bytes(s->USARTx, (const char*)&ch, 1);
}

uint32_t uartTotalRxBytesWaiting(const serialPort_t *instance)
{
    const uartPort_t *s = (const uartPort_t*)instance;

    int length = 0;
    uart_get_buffered_data_len(s->USARTx, (size_t*)&length);
    return length;
}

uint32_t uartTotalTxBytesFree(const serialPort_t *instance)
{
    uartPort_t *s = (uartPort_t *)instance;
    
    int length = 0;
    uart_get_tx_buffer_free_size(s->USARTx, (size_t*)&length);
    return length;
}

uint8_t uartRead(serialPort_t *instance)
{
    uint8_t ch;
    uartPort_t *s = (uartPort_t *)instance;

    int length = 0;
    ESP_ERROR_CHECK(uart_get_buffered_data_len(s->USARTx, (size_t*)&length));
    if (length) {
        length = uart_read_bytes(s->USARTx, &ch, 1, 100);
    }

    return ch;
}

void uartSetBaudRate(serialPort_t *instance, uint32_t baudRate)
{
    uartPort_t *uartPort = (uartPort_t *)instance;
    uartPort->port.baudRate = baudRate;
    uart_set_baudrate(uartPort->USARTx, baudRate);
}

bool isUartTransmitBufferEmpty(const serialPort_t *instance)
{
    const uartPort_t *s = (const uartPort_t *)instance;
    
    int length = 0;
    uart_get_tx_buffer_free_size(s->USARTx, (size_t*)&length);
    return uart_buffer_size - length == 0;
}

void uartSetMode(serialPort_t *instance, portMode_t mode)
{
    uartPort_t *uartPort = (uartPort_t *)instance;
    uartPort->port.mode = mode;
}

bool isUartIdle(serialPort_t *instance)
{
    return isUartTransmitBufferEmpty(instance);
}

const struct serialPortVTable uartVTable = {
    .serialWrite = uartWrite,
    .serialTotalRxWaiting = uartTotalRxBytesWaiting,
    .serialTotalTxFree = uartTotalTxBytesFree,
    .serialRead = uartRead,
    .serialSetBaudRate = uartSetBaudRate,
    .isSerialTransmitBufferEmpty = isUartTransmitBufferEmpty,
    .setMode = uartSetMode,
    .writeBuf = NULL,
    .isConnected = NULL,
    .isIdle = isUartIdle,
    .beginWrite = NULL,
    .endWrite = NULL,
};