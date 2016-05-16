// ADXL345 accelerometer
// Copyright 2016 Bloutiouf
// https://opensource.org/licenses/MIT

#ifndef ADXL345_H
#define ADXL345_H

#include <stdbool.h>

#define ADXL345_ID 0x53

enum ADXL345_ACDC
{
    ADXL345_ACDC_DC = 0x00,
    ADXL345_ACDC_AC = 0x01,
};

// bitfield
enum ADXL345_Axis
{
    ADXL345_AXIS_Z = 0x01,
    ADXL345_AXIS_Y = 0x02,
    ADXL345_AXIS_X = 0x04,
};

enum ADXL345_FIFOMode
{
    ADXL345_FIFOMODE_BYPASS = 0x00,
    ADXL345_FIFOMODE_FIFO = 0x40,
    ADXL345_FIFOMODE_STREAM = 0x80,
    ADXL345_FIFOMODE_TRIGGER = 0xC0,
};

// bitfield
enum ADXL345_Interrupt
{
    ADXL345_INTERRUPT_OVERRUN = 0x01,
    ADXL345_INTERRUPT_WATERMARK = 0x02,
    ADXL345_INTERRUPT_FREE_FALL = 0x04,
    ADXL345_INTERRUPT_INACTIVITY = 0x08,
    ADXL345_INTERRUPT_ACTIVITY = 0x10,
    ADXL345_INTERRUPT_DOUBLE_TAP = 0x20,
    ADXL345_INTERRUPT_SINGLE_TAP = 0x40,
    ADXL345_INTERRUPT_DATA_READY = 0x80,
};

enum ADXL345_InterruptActive
{
    ADXL345_INTERRUPTACTIVE_HIGH = 0x00,
    ADXL345_INTERRUPTACTIVE_LOW = 0x20,
};

enum ADXL345_Justify
{
    ADXL345_JUSTIFY_RIGHT = 0x00,
    ADXL345_JUSTIFY_LEFT = 0x04,
};

enum ADXL345_Range
{
    ADXL345_RANGE_2G = 0x00,
    ADXL345_RANGE_4G = 0x01,
    ADXL345_RANGE_8G = 0x02,
    ADXL345_RANGE_16G = 0x03,
};

enum ADXL345_Rate
{
    ADXL345_RATE_0_10 = 0x00,
    ADXL345_RATE_0_20 = 0x01,
    ADXL345_RATE_0_39 = 0x02,
    ADXL345_RATE_0_78 = 0x03,
    ADXL345_RATE_1_56 = 0x04,
    ADXL345_RATE_3_13 = 0x05,
    ADXL345_RATE_6_25 = 0x06,
    ADXL345_RATE_12_5 = 0x07,
    ADXL345_RATE_25 = 0x08,
    ADXL345_RATE_50 = 0x09,
    ADXL345_RATE_100 = 0x0A,
    ADXL345_RATE_200 = 0x0B,
    ADXL345_RATE_400 = 0x0C,
    ADXL345_RATE_800 = 0x0D,
    ADXL345_RATE_1600 = 0x0E,
    ADXL345_RATE_3200 = 0x0F,
};

enum ADXL345_Resolution
{
    ADXL345_RESOLUTION_RANGE = 0x00,
    ADXL345_RESOLUTION_FULL = 0x08,
};

// bitfield
enum ADXL345_Source
{
    ADXL345_SOURCE_TAP_Z = 0x01,
    ADXL345_SOURCE_TAP_Y = 0x02,
    ADXL345_SOURCE_TAP_X = 0x04,
    ADXL345_SOURCE_ASLEEP = 0x08,
    ADXL345_SOURCE_ACTIVITY_Z = 0x10,
    ADXL345_SOURCE_ACTIVITY_Y = 0x20,
    ADXL345_SOURCE_ACTIVITY_X = 0x40,
};

enum ADXL345_SPI
{
    ADXL345_SPI_4 = 0x00,
    ADXL345_SPI_3 = 0x40,
};

enum ADXL345_Wakeup
{
    ADXL345_WAKEUP_8 = 0x00,
    ADXL345_WAKEUP_4 = 0x01,
    ADXL345_WAKEUP_2 = 0x02,
    ADXL345_WAKEUP_1 = 0x03,
};

struct ADXL345_Activity
{
    enum ADXL345_ACDC activityACDC;
    enum ADXL345_Axis activityAxes;
    unsigned char activityThreshold; // 62.5 mg/LSB
    enum ADXL345_ACDC inactivityACDC;
    enum ADXL345_Axis inactivityAxes;
    unsigned char inactivityThreshold; // 62.5 mg/LSB
    unsigned char inactivityTime; // 1 s/LSB
};

struct ADXL345_DataFormat
{
    enum ADXL345_InterruptActive intActive;
    enum ADXL345_Justify justify;
    enum ADXL345_Range range;
    enum ADXL345_Resolution resolution;
    bool selfTest;
    enum ADXL345_SPI SPI;
};

struct ADXL345_Interrupts
{
    enum ADXL345_Interrupt enable;
    enum ADXL345_Interrupt map;
};

struct ADXL345_FIFO
{
    enum ADXL345_FIFOMode mode;
    unsigned char samples;
    bool triggerMap;
};

struct ADXL345_FIFOStatus
{
    unsigned char entries;
    bool triggered;
};

struct ADXL345_FreeFall
{
    unsigned char threshold; // 62.5 mg/LSB
    unsigned char time; // 5 ms/LSB
};

struct ADXL345_OutputRate
{
    bool lowPower;
    enum ADXL345_Rate rate;
};

struct ADXL345_Power
{
    bool autoSleep;
    bool linkActivity;
    bool measurement;
    bool sleep;
    enum ADXL345_Wakeup wakeup;
};

struct ADXL345_Tap
{
    enum ADXL345_Axis axes;
    unsigned char duration; // 625 �s/LSB
    unsigned char latent; // 1.25 ms/LSB
    bool suppress;
    unsigned char threshold; // 62.5 mg/LSB
    unsigned char window; // 1.25 ms/LSB
};

int ADXL345_Init(int file, unsigned char id, bool check);

int ADXL345_ConfigureActivity(int file, const struct ADXL345_Activity *conf);
int ADXL345_ConfigureDataFormat(int file, const struct ADXL345_DataFormat *conf);
int ADXL345_ConfigureInterrupts(int file, const struct ADXL345_Interrupts *conf);
int ADXL345_ConfigureFIFO(int file, const struct ADXL345_FIFO *conf);
int ADXL345_ConfigureFreeFall(int file, const struct ADXL345_FreeFall *conf);
int ADXL345_ConfigureOutputRate(int file, const struct ADXL345_OutputRate *conf);
int ADXL345_ConfigurePower(int file, const struct ADXL345_Power *conf);
int ADXL345_ConfigureTap(int file, const struct ADXL345_Tap *conf);

int ADXL345_ReadActivityTapSources(int file, enum ADXL345_Source *sources);
int ADXL345_ReadData(int file, short *x, short *y, short *z);
int ADXL345_ReadFIFOStatus(int file, struct ADXL345_FIFOStatus *status);
int ADXL345_ReadInterruptSources(int file, enum ADXL345_Interrupt *sources);

int ADXL345_WriteOffset(int file, char x, char y, char z); // 15.6 mg/LSB

#endif
