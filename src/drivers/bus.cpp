#include "bus.h"

typedef struct spiHardware_s {
    uint8_t dev;
    SPIClass * instance;
} spiHardware_t;

static spiHardware_t spiHardwareMap[] = {
    {.dev = HSPI, .instance = NULL},
    {.dev = VSPI, .instance = NULL},
};

void spiDeviceInit(SPIDevice device)
{
    spiHardware_t spiHardware = spiHardwareMap[device];
    if (!spiHardware.instance) {
        spiHardware.instance = new SPIClass(spiHardware.dev);
        spiHardware.instance->begin();
        pinMode(spiHardware.instance->pinSS(), OUTPUT);
    }
}

SPIClass * spiInstanceByDevice(SPIDevice device)
{
    return spiHardwareMap[device].instance;
}

bool spiBusReadRegister(const busDevice_t * dev, uint8_t reg, uint8_t * data)
{
    static const int spiClk = 1000000; // 1 MHz

    SPIClass * spi = spiInstanceByDevice(dev->busdev.spi.spiBus);

    spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    digitalWrite(spi->pinSS(), LOW); //pull SS slow to prep other end for transfer
    spi->transfer(reg);
    *data = spi->transfer(0x00);
    digitalWrite(spi->pinSS(), HIGH); //pull ss high to signify end of data transfer
    spi->endTransaction();

    return true;
}

typedef struct i2cHardware_s {
    TwoWire *dev;
    TwoWire *instance;
} i2cHardware_t;

static i2cHardware_t i2cHardwareMap[] = {
    {.dev = &Wire, .instance = NULL},
    {.dev = &Wire1, .instance = NULL},
};

void i2cDeviceInit(I2CDevice device)
{
    i2cHardware_t i2cHardware = i2cHardwareMap[device];
    if (!i2cHardware.instance) {
        i2cHardware.instance = i2cHardware.dev;
        i2cHardware.instance->begin();
    }
}

TwoWire * i2cInstanceByDevice(I2CDevice device)
{
    return i2cHardwareMap[device].instance;
}

bool i2cBusReadRegister(const busDevice_t * dev, uint8_t reg, uint8_t *data)
{
    const bool allowRawAccess = (dev->flags & DEVFLAGS_USE_RAW_REGISTERS);
    
    TwoWire * i2c = i2cInstanceByDevice(dev->busdev.i2c.i2cBus);

    i2c->beginTransmition(dev->busdev.i2c.address);
    i2c->write(reg);
    i2c->endTransmition();

    if (i2c->requestFrom(dev->busdev.i2c.address, 1)) {
        *data = i2c->read();
    } else {
        return false;
    }

    return true;
}

bool busRead(const busDevice_t * dev, uint8_t reg, uint8_t * data)
{
    switch (dev->busType) {
        case BUSTYPE_SPI:
#ifdef USE_SPI
            if (dev->flags & DEVFLAGS_USE_RAW_REGISTERS) {
                return spiBusReadRegister(dev, reg, data);
            }
            else {
                return spiBusReadRegister(dev, reg | 0x80, data);
            }
#else
            return false;
#endif

        case BUSTYPE_I2C:
#ifdef USE_I2C
            return i2cBusReadRegister(dev, reg, data);
#else
            return false;
#endif
        default:
            return false;
    }
}

bool busReadBuf(const busDevice_t * dev, uint8_t reg, uint8_t * data, uint8_t length)
{
    switch (dev->busType) {
        case BUSTYPE_SPI:
#ifdef USE_SPI
            if (dev->flags & DEVFLAGS_USE_RAW_REGISTERS) {
                return spiBusReadBuffer(dev, reg, data, length);
            }
            else {
                return spiBusReadBuffer(dev, reg | 0x80, data, length);
            }
#else
            return false;
#endif

        case BUSTYPE_I2C:
#ifdef USE_I2C
            return i2cBusReadBuffer(dev, reg, data, length);
#else
            return false;
#endif

        default:
            return false;
    }
}