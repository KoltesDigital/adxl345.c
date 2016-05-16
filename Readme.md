# ADXL345.c

Simple wrapper for ADXL345 accelerometer on I2C.

Tested on a Raspberry Pi 2.

## Build

Build, and optionally install, with [scons](http://scons.org/).

	scons
	sudo scons install

## Usage

All functions take a file handle as first parameter (obtained with `open` and released with `close`), and return `0` when successful or `-1` on failure.

Example:

	static int file;

	static int closeAndExit(int code)
	{
		close(file);
		return code;
	}

	int main(void)
	{
		file = open("/dev/i2c-1", O_RDWR);
		if (file < 0)
			return -1;

		if (ADXL345_Init(file, ADXL345_ID, true))
			return closeAndExit(-1);

	    struct ADXL345_DataFormat confDataFormat = {
	        .range = ADXL345_RANGE_2G,
	    };
	    if (ADXL345_ConfigureDataFormat(file, &confDataFormat))
			return closeAndExit(-1);

	    struct ADXL345_Power confPowerControl = {
	        .measurement = true,
	    };
		if (ADXL345_ConfigurePower(file, &confPowerControl))
			return closeAndExit(-1);

		short x, y, z;
		if (ADXL345_ReadData(file, &x, &y, &z))
			return closeAndExit(-1);

		return closeAndExit(0);
	}


### ADXL345_Init(int file, unsigned char id, bool check)

`id` is the device I2C address. It is usually `ADXL345_ID` (0x53).

If `check` is true, the function reads the device identification registers and checks against the expected values. This inherently checks that the device and the I2C bus are working.

## License

Copyright 2016 Bloutiouf

[MIT License](https://opensource.org/licenses/MIT)
