#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

#include "adxl345.h"

#define READ(N) \
	if (read(file, buffer, N) != N) \
		return -1;

#define WRITE(N) \
	if (write(file, buffer, N) != N) \
		return -1;

int ADXL345_Init(int file, unsigned char id, bool check)
{
    if (ioctl(file, I2C_SLAVE, id) < 0)
        return -1;

    if (check)
    {
        unsigned char buffer[1];

        buffer[0] = 0x00;
        WRITE(1);

        READ(1);
        if (buffer[0] != 0xE5)
            return -1;
    }

    return 0;
}

int ADXL345_ConfigureActivity(int file, const struct ADXL345_Activity *conf)
{
    unsigned char buffer[5];

    buffer[0] = 0x24;
    buffer[1] = conf->activityThreshold;
    buffer[2] = conf->inactivityThreshold;
    buffer[3] = conf->inactivityTime;
    buffer[4] = ((conf->activityACDC & 0x01) << 7) |
        ((conf->activityAxes & 0x07) << 4) |
        ((conf->inactivityACDC & 0x01) << 3) |
        (conf->inactivityAxes & 0x07);
    WRITE(5);

    return 0;
}

int ADXL345_ConfigureDataFormat(int file, const struct ADXL345_DataFormat *conf)
{
    unsigned char buffer[2];

    buffer[0] = 0x31;
    buffer[1] = (conf->selfTest ? 0x80 : 0x00) |
        (conf->SPI & 0x40) |
        (conf->intActive & 0x20) |
        (conf->resolution & 0x08) |
        (conf->justify & 0x04) |
        (conf->range & 0x03);
    WRITE(2);

    return 0;
}

int ADXL345_ConfigureInterrupts(int file, const struct ADXL345_Interrupts *conf)
{
    unsigned char buffer[3];

    buffer[0] = 0x2E;
    buffer[1] = conf->enable;
    buffer[2] = conf->map;
    WRITE(3);

    return 0;
}

int ADXL345_ConfigureFIFO(int file, const struct ADXL345_FIFO *conf)
{
    unsigned char buffer[2];

    buffer[0] = 0x38;
    buffer[1] = (conf->mode & 0xC0) |
        (conf->triggerMap ? 0x20 : 0x00) |
        (conf->samples & 0x1F);
    WRITE(2);

    return 0;
}

int ADXL345_ConfigureFreeFall(int file, const struct ADXL345_FreeFall *conf)
{
    unsigned char buffer[3];

    buffer[0] = 0x28;
    buffer[1] = conf->threshold;
    buffer[2] = conf->time;
    WRITE(3);

    return 0;
}

int ADXL345_ConfigureOutputRate(int file, const struct ADXL345_OutputRate *conf)
{
    unsigned char buffer[2];

    buffer[0] = 0x2C;
    buffer[1] = (conf->lowPower ? 0x10 : 0x00) |
        (conf->rate & 0x0F);
    WRITE(2);

    return 0;
}

int ADXL345_ConfigurePower(int file, const struct ADXL345_Power *conf)
{
    unsigned char buffer[2];

    buffer[0] = 0x2D;
    buffer[1] = (conf->linkActivity ? 0x20 : 0x00) |
        (conf->autoSleep ? 0x10 : 0x00) |
        (conf->measurement ? 0x08 : 0x00) |
        (conf->sleep ? 0x04 : 0x00) |
        (conf->wakeup & 0x03);
    WRITE(2);

    return 0;
}

int ADXL345_ConfigureTap(int file, const struct ADXL345_Tap *conf)
{
    unsigned char buffer[4];

    buffer[0] = 0x1D;
    buffer[1] = conf->threshold;
    WRITE(2);

    buffer[0] = 0x21;
    buffer[1] = conf->duration;
    buffer[2] = conf->latent;
    buffer[3] = conf->window;
    WRITE(4);

    buffer[0] = 0x2A;
    buffer[1] = (conf->suppress ? 0x80 : 0x00) |
        (conf->axes & 0x07);
    WRITE(2);

    return 0;
}

int ADXL345_ReadActivityTapSources(int file, enum ADXL345_Source *sources)
{
    unsigned char buffer[1];

    buffer[0] = 0x2B;
    WRITE(1);

    READ(1);
    *sources = buffer[0];

    return 0;
}

int ADXL345_ReadData(int file, short *x, short *y, short *z)
{
    unsigned char buffer[6];

    buffer[0] = 0x32;
    WRITE(1);

    READ(6);
    *x = ((buffer[1]) << 8) | buffer[0];
    *y = ((buffer[3]) << 8) | buffer[2];
    *z = ((buffer[5]) << 8) | buffer[4];

    return 0;
}

int ADXL345_ReadFIFOStatus(int file, struct ADXL345_FIFOStatus *status)
{
    unsigned char buffer[1];

    buffer[0] = 0x2B;
    WRITE(1);

    READ(1);
    status->triggered = ((buffer[0] & 0x80) != 0);
    status->entries = (buffer[0] & 0x3F);

    return 0;
}

int ADXL345_ReadInterruptSources(int file, enum ADXL345_Interrupt *sources)
{
    unsigned char buffer[1];

    buffer[0] = 0x30;
    WRITE(1);

    READ(1);
    *sources = buffer[0];

    return 0;
}

int ADXL345_WriteOffset(int file, char x, char y, char z)
{
    unsigned char buffer[4];

    buffer[0] = 0x1E;
    buffer[1] = x;
    buffer[2] = y;
    buffer[3] = z;
    WRITE(4);

    return 0;
}
