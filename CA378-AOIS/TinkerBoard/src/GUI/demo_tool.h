#ifdef NONE_SSP
#include "communication.h"
#endif

#define IMX378_I2C_ADDR 0x1A
#define IMX219_I2C_ADDR 0x10
#define AFDRV_I2C_ADDR  0x3E
#define I2C_DEV         2
#define CCI_SLAVE_ADDR  IMX378_I2C_ADDR


int __CCIRegReadBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data)
{
	unsigned char writeAddr[2];

	writeAddr[0] = RegAddress >> 8;
	writeAddr[1] = RegAddress & 0xFF;
	i2c_write(I2C_DEV, CCISlaveAddress, writeAddr, 2);
	i2c_read(I2C_DEV, CCISlaveAddress, data, 1);

#if AF_TEST
	printf("R:%04X:", RegAddress);
	for (int i = 0; i < 1; i++)
	{
		printf("%02X ", data[i]);
	}
	printf("\r\n");
#endif

	return 1;
}

int __CCIRegWriteBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char data)
{
	unsigned char writeData[3];

	writeData[0] = RegAddress >> 8;
	writeData[1] = RegAddress & 0xFF;
	writeData[2] = data;
	i2c_write(I2C_DEV, CCISlaveAddress, writeData, 3);

#if AF_TEST
	printf("W:%04X:", RegAddress);
	for (int i = 0; i < 1; i++)
	{
		printf("%02X ", data);
	}
	printf("\r\n");
#endif

	return 1;
}

int __CCIRegReadMBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data, int size)
{
	unsigned char writeAddr[2];
	int i;

	if (CCISlaveAddress == CCI_SLAVE_ADDR ||
		CCISlaveAddress == AFDRV_I2C_ADDR)
	{
		writeAddr[0] = RegAddress >> 8;
		writeAddr[1] = RegAddress & 0xFF;
		i2c_write(I2C_DEV, CCISlaveAddress, writeAddr, 2);
		i2c_read(I2C_DEV, CCISlaveAddress, data, size);
	}
	else
	{
		for (i = 0; i < size; i++)
		{
			writeAddr[0] = (RegAddress + i) >> 8;
			writeAddr[1] = (RegAddress + i) & 0xFF;
			i2c_write(I2C_DEV, CCISlaveAddress, writeAddr, 2);
			i2c_read(I2C_DEV, CCISlaveAddress, &data[i], 1);
		}
	}

#if AF_TEST
	printf("R:%04X:", RegAddress);
	for (i = 0; i < size; i++)
	{
		printf("%02X ", data[i]);
	}
	printf("\r\n");
#endif

	return size;
}

int __CCIRegWriteMBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data, int size)
{
	unsigned char writeData[3];
	int i;

	for (i = 0; i < size; i++)
	{
		writeData[0] = (RegAddress + i) >> 8;
		writeData[1] = (RegAddress + i) & 0xFF;
		writeData[2] = data[i];
		i2c_write(I2C_DEV, CCISlaveAddress, writeData, 3);
	}

#if AF_TEST
	printf("R:%04X:", RegAddress);
	for (i = 0; i < size; i++)
	{
		printf("%02X ", data[i]);
	}
	printf("\r\n");
#endif

	return size;
}

int __RegRead1ByteAddress(int CCISlaveAddress, int RegAddress, unsigned char *data, int size)
{
	unsigned char writeAddr[1];
	int i;

	for (i = 0; i < size; i++)
	{
		writeAddr[0] = RegAddress + i;
		i2c_write(I2C_DEV, CCISlaveAddress, writeAddr, 1);
		i2c_read(I2C_DEV, CCISlaveAddress, &data[i], 1);
	}

	return size;
}
