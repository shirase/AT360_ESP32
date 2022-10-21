#include "bus.h"

typedef struct spiHardware_s {
    uint8_t dev;
    SPIClass * instance;
} spiHardware_t;

static spiHardware_t spiHardwareMap[] = {
    {.dev = HSPI, .instance = NULL},
    {.dev = VSPI, .instance = NULL},
};

static const int spiClk = 1000000; // 1 MHz

void spiInit(SPIDevice device)
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
    SPIClass * spi = spiInstanceByDevice(dev->busdev.spi.spiBus);

    spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    digitalWrite(spi->pinSS(), LOW); //pull SS slow to prep other end for transfer

    spi->transfer(reg);
    *data = spi->transfer(0x00);

    digitalWrite(spi->pinSS(), HIGH); //pull ss high to signify end of data transfer
    spi->endTransaction();

    return true;
}

bool spiBusReadBuffer(const busDevice_t * dev, uint8_t reg, uint8_t * data, uint8_t length)
{
    SPIClass * spi = spiInstanceByDevice(dev->busdev.spi.spiBus);

    spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    digitalWrite(spi->pinSS(), LOW); //pull SS slow to prep other end for transfer

    spi->transfer(reg);
    spi->transferBytes(NULL, data, length);

    digitalWrite(spi->pinSS(), HIGH); //pull ss high to signify end of data transfer
    spi->endTransaction();

    return true;
}

bool spiBusWriteRegister(const busDevice_t * dev, uint8_t reg, uint8_t data)
{
    SPIClass * spi = spiInstanceByDevice(dev->busdev.spi.spiBus);

    spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    digitalWrite(spi->pinSS(), LOW); //pull SS slow to prep other end for transfer

    spi->transfer(reg);
    spi->transfer(data);

    digitalWrite(spi->pinSS(), HIGH); //pull ss high to signify end of data transfer
    spi->endTransaction();

    return true;
}

bool spiBusWriteBuffer(const busDevice_t * dev, uint8_t reg, const uint8_t * data, uint8_t length)
{
    SPIClass * spi = spiInstanceByDevice(dev->busdev.spi.spiBus);

    spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    digitalWrite(spi->pinSS(), LOW); //pull SS slow to prep other end for transfer

    spi->transfer(reg);
    spi->transferBytes(NULL, (uint8_t*)data, length);

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

void i2cInit(I2CDevice device)
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
    TwoWire * i2c = i2cInstanceByDevice(dev->busdev.i2c.i2cBus);

    i2c->beginTransmission(dev->busdev.i2c.address);
    i2c->write(reg);
    i2c->endTransmission();

    if (i2c->requestFrom(dev->busdev.i2c.address, (uint8_t)1)) {
        *data = i2c->read();
    } else {
        return false;
    }

    return true;
}

bool i2cBusReadBuffer(const busDevice_t * dev, uint8_t reg, uint8_t *data, uint8_t length)
{
    TwoWire * i2c = i2cInstanceByDevice(dev->busdev.i2c.i2cBus);

    i2c->beginTransmission(dev->busdev.i2c.address);
    i2c->write(reg);
    i2c->endTransmission();

    if (i2c->requestFrom(dev->busdev.i2c.address, length)) {
        i2c->readBytes(data, length);
    } else {
        return false;
    }

    return true;
}

bool i2cBusWriteRegister(const busDevice_t * dev, uint8_t reg, uint8_t data)
{    
    TwoWire * i2c = i2cInstanceByDevice(dev->busdev.i2c.i2cBus);

    i2c->beginTransmission(dev->busdev.i2c.address);
    i2c->write(reg);
    i2c->write(data);
    i2c->endTransmission();

    return true;
}

bool i2cBusWriteBuffer(const busDevice_t * dev, uint8_t reg, uint8_t *data, uint8_t length)
{
    TwoWire * i2c = i2cInstanceByDevice(dev->busdev.i2c.i2cBus);

    i2c->beginTransmission(dev->busdev.i2c.address);
    i2c->write(reg);
    for (uint8_t i = 0; i < length; i++) {
        i2c->write(data[i]);
    }
    i2c->endTransmission();

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

bool busWrite(const busDevice_t * dev, uint8_t reg, uint8_t data)
{
    switch (dev->busType) {
        case BUSTYPE_SPI:
#ifdef USE_SPI
            if (dev->flags & DEVFLAGS_USE_RAW_REGISTERS) {
                return spiBusWriteRegister(dev, reg, data);
            }
            else {
                return spiBusWriteRegister(dev, reg & 0x7F, data);
            }
#else
            return false;
#endif

        case BUSTYPE_I2C:
#ifdef USE_I2C
            return i2cBusWriteRegister(dev, reg, data);
#else
            return false;
#endif

        default:
            return false;
    }
}

bool busWriteBuf(const busDevice_t * dev, uint8_t reg, const uint8_t * data, uint8_t length)
{
    switch (dev->busType) {
        case BUSTYPE_SPI:
#ifdef USE_SPI
            if (dev->flags & DEVFLAGS_USE_RAW_REGISTERS) {
                return spiBusWriteBuffer(dev, reg, data, length);
            }
            else {
                return spiBusWriteBuffer(dev, reg | 0x80, data, length);
            }
#else
            return false;
#endif

        case BUSTYPE_I2C:
#ifdef USE_I2C
            return i2cBusWriteBuffer(dev, reg, (uint8_t*)data, length);
#else
            return false;
#endif

        default:
            return false;
    }
}

static busDevice_t busDeviceHardwareMap[] = {
    {.busType = BUSTYPE_I2C, .busdev = {.i2c = {.i2cBus = I2CDEV_1, .address = 0x1E}},}, // HMC5883
};

busDevice_t * busDeviceInit(busType_e bus, devHardwareType_e hw)
{
    switch (hw)
    {
    case DEVHW_HMC5883:
        busDevice_t * busDevice = &busDeviceHardwareMap[0];
        i2cInit(busDevice->busdev.i2c.i2cBus);
        return ;
        break;
    
    default:
        break;
    }

    return NULL;
}

void busDeviceDeInit(busDevice_t * dev)
{
    dev->busType = BUSTYPE_NONE;
}