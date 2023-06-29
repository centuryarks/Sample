/*
Copyright (c) 2017, CenturyArks
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

#ifndef __DEMO_CONTROL_H__
#define __DEMO_CONTROL_H__

#include "communication.h"
#include "af_control.h"
#include "ois_control.h"
#include "lsc_control.h"


/***************************************************************
 *  Defines for DEMO Control
 **************************************************************/
#define COMMAND_MAX     (1024)
#define OPTIONBUF_MAX   (256)
#define LINEBUF_MAX     (256)
#define FOCUS_WAIT      (200)
#define DEMO_PATH_MAX   (128)
#define UNUSED(x)       ((void)(x))

/***************************************************************
 *  Declare function for DEMO Control
 **************************************************************/
int DemoInit();
int DemoExit();
int DemoControl(char *commandLine);
int ReadSettingFile();
int WriteSettingFile();
int Capture(int mode);
int RestartPreview(int stop);
int MessageQueueSend(char *message);
int StartAFCThread();
void StopAFCThread();
void *AFCThread(void *arg);
void *CaptureThread(void *arg);
int WaitStreaming();
void GenerateOptions(char *options, int maxlength, int still);


/***************************************************************
 *  Property
 **************************************************************/
int GetShutter();
double GetGain();
int GetFocusPosition();
double GetSharpness();
double GetContrast();
double GetBrightness();
double GetSaturation();
double GetEvCompensation();
double GetRedGain();
double GetBlueGain();

/*******************************************************************************
 * @brief   Read CCI register by slave address
 *
 * @param   CCISlaveAddress     CCI Slave address
 * @param   RegAddress          Register address
 * @param   data                Read data
 *
 * @return  Read byte 1
 ******************************************************************************/
int __CCIRegReadBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data)
{
    unsigned char writeAddr[2];

    i2c_dev_t i2c_dev = GetI2cDev();

    writeAddr[0] = RegAddress >> 8;
    writeAddr[1] = RegAddress & 0xFF;
    i2c_read(i2c_dev, CCISlaveAddress, writeAddr, 2, data, 1);

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

/*******************************************************************************
 * @brief   Write CCI register by slave address
 *
 * @param   CCISlaveAddress     CCI Slave address
 * @param   RegAddress          Register address
 * @param   data                Write data
 *
 * @return  Write byte 1
 ******************************************************************************/
int __CCIRegWriteBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char data)
{
    unsigned char writeData[3];
    s32 ret;

    i2c_dev_t i2c_dev = GetI2cDev();

    writeData[0] = RegAddress >> 8;
    writeData[1] = RegAddress & 0xFF;
    writeData[2] = data;
    ret = i2c_write(i2c_dev, CCISlaveAddress, writeData, 3);
    if (ret != COMMUNICATION_SUCCESS)
    {
        return 0;
    }

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

/*******************************************************************************
 * @brief   Read multi bytes CCI register by slave address
 *
 * @param   CCISlaveAddress     CCI Slave address
 * @param   RegAddress          Register address
 * @param   data                Read data
 * @param   size                Read size
 *
 * @return  Read bytes
 ******************************************************************************/
int __CCIRegReadMBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data, int size)
{
    unsigned char writeAddr[2];
    int i;

    i2c_dev_t i2c_dev = GetI2cDev();

    if (CCISlaveAddress == CCI_SLAVE_ADDR ||
        CCISlaveAddress == AFDRV_I2C_ADDR)
    {
        writeAddr[0] = RegAddress >> 8;
        writeAddr[1] = RegAddress & 0xFF;
        i2c_read(i2c_dev, CCISlaveAddress, writeAddr, 2, data, size);
    }
    else
    {
        for (i = 0; i < size; i++)
        {
            writeAddr[0] = (RegAddress + i) >> 8;
            writeAddr[1] = (RegAddress + i) & 0xFF;
            i2c_read(i2c_dev, CCISlaveAddress, writeAddr, 2, &data[i], 1);
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

/*******************************************************************************
 * @brief   Write multi bytes CCI register by slave address
 *
 * @param   CCISlaveAddress     CCI Slave address
 * @param   RegAddress          Write address
 * @param   data                Write data
 * @param   size                Write size
 *
 * @return  Write bytes
 ******************************************************************************/
int __CCIRegWriteMBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data, int size)
{
    unsigned char writeData[3];
    int i;
    s32 ret;

    i2c_dev_t i2c_dev = GetI2cDev();

    for (i = 0; i < size; i++)
    {
        writeData[0] = (RegAddress + i) >> 8;
        writeData[1] = (RegAddress + i) & 0xFF;
        writeData[2] = data[i];
        ret = i2c_write(i2c_dev, CCISlaveAddress, writeData, 3);
        if (ret != COMMUNICATION_SUCCESS)
        {
            return 0;
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

/*******************************************************************************
 * @brief   Read 1 byte register address
 *
 * @param   SlaveAddress        Slave address
 * @param   RegAddress          Register address
 * @param   data                Read data
 * @param   size                Read size
 *
 * @return  Read bytes
 ******************************************************************************/
int __RegRead1ByteAddress(int SlaveAddress, int RegAddress, unsigned char *data, int size)
{
    unsigned char writeAddr[1];
    int i;

    i2c_dev_t i2c_dev = GetI2cDev();

    for (i = 0; i < size; i++)
    {
        writeAddr[0] = RegAddress + i;
        i2c_read(i2c_dev, SlaveAddress, writeAddr, 1, &data[i], 1);
    }

    return size;
}

#endif
