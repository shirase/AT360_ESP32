#pragma once

#include "targets/target.h"

#include "io.h"
#include "common/memory.h"

#define BUS_SCRATCHPAD_MEMORY_SIZE      (20)

typedef enum {
    DEVHW_NONE = 0,
    DEVHW_HMC5883,
    DEVHW_MPU6000,
} devHardwareType_e;

typedef enum SPIDevice {
    SPIINVALID = -1,
    SPIDEV_1   = 0,
    SPIDEV_2,
    SPIDEV_COUNT
} SPIDevice;

typedef enum I2CDevice {
    I2CINVALID = -1,
    I2CDEV_1 = 0,
    I2CDEV_COUNT
} I2CDevice;

typedef enum {
    BUSTYPE_ANY  = 0,
    BUSTYPE_NONE = 0,
    BUSTYPE_I2C  = 1,
    BUSTYPE_SPI  = 2,
    BUSTYPE_SDIO = 3,
} busType_e;

typedef enum {
    DEVFLAGS_NONE                       = 0,
    DEVFLAGS_USE_RAW_REGISTERS          = (1 << 0),     // Don't manipulate MSB for R/W selection (SPI), allow using 0xFF register to raw i2c reads/writes

    // SPI-only
    DEVFLAGS_USE_MANUAL_DEVICE_SELECT   = (1 << 1),     // (SPI only) Don't automatically select/deselect device
    DEVFLAGS_SPI_MODE_0                 = (1 << 2),     // (SPI only) Use CPOL=0/CPHA=0 (if unset MODE3 is used - CPOL=1/CPHA=1)
} deviceFlags_e;

typedef struct busDevice_s {
    devHardwareType_e hardwareType;
    busType_e busType;              // Copy of busType to avoid additional pointer dereferencing
    uint32_t flags;                 // Copy of flags
    uint32_t param;                 // Copy of param
    union {
#ifdef USE_SPI
        struct {
            SPIDevice spiBus;       // SPI bus ID
            IO_t csnPin;            // IO for CS# pin
            uint32_t clock;
        } spi;
#endif
#ifdef USE_I2C
        struct {
            I2CDevice i2cBus;       // I2C bus ID
            uint8_t address;        // I2C bus device address
        } i2c;
#endif
    } busdev;
    IO_t irqPin;
    uint32_t * scratchpad;          // Memory where device driver can store persistent data. Zeroed out when initializing the device
                                    // for the first time. Useful when once device is shared between several sensors
                                    // (like MPU/ICM acc-gyro sensors)
} busDevice_t;

typedef enum {
    BUS_SPEED_INITIALIZATION = 0,
    BUS_SPEED_SLOW           = 1,
    BUS_SPEED_STANDARD       = 2,
    BUS_SPEED_FAST           = 3,
    BUS_SPEED_ULTRAFAST      = 4
} busSpeed_e;

busDevice_t * busDeviceInit(busType_e bus, devHardwareType_e hw);
void busDeviceDeInit(busDevice_t * dev);
busDevice_t * busDeviceOpen(busType_e bus, devHardwareType_e hw);

bool busRead(const busDevice_t * dev, uint8_t reg, uint8_t * data);
bool busReadBuf(const busDevice_t * dev, uint8_t reg, uint8_t * data, uint8_t length);
bool busWrite(const busDevice_t * dev, uint8_t reg, uint8_t data);
bool busWriteBuf(const busDevice_t * dev, uint8_t reg, const uint8_t * data, uint8_t length);

void busSetSpeed(const busDevice_t * dev, busSpeed_e speed);
uint32_t busDeviceReadScratchpad(const busDevice_t * dev);
void busDeviceWriteScratchpad(busDevice_t * dev, uint32_t value);
void * busDeviceGetScratchpadMemory(const busDevice_t * dev);
