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

enum E_LENS_ID
{
    LENS_V1 = 0,
    LENS_V2,
    LENS_V2_WIDE
};

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
void GetTuningFile(char *tuning_file);
int GetRotationIndex();
void GetProductName(char *product_name);
int ExistsAFDriver();
int IsAFDriverV1();
int IsAFDriverV2();


/*******************************************************************************
 * @brief   Read CCI register by slave address
 *
 * @param   CCISlaveAddress     CCI Slave address
 * @param   RegAddress          Register address
 * @param   data                Read data
 *
 * @return  Read byte 1
 ******************************************************************************/
int __CCIRegReadBySlaveAddress(int CCISlaveAddress, int RegAddress, u8 *data)
{
    u8 writeAddr[2];

    i2c_dev_t i2c_dev = GetI2cDev();

    writeAddr[0] = RegAddress >> 8;
    writeAddr[1] = RegAddress & 0xFF;
    i2c_read(i2c_dev, CCISlaveAddress, writeAddr, 2, data, 1);

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
int __CCIRegWriteBySlaveAddress(int CCISlaveAddress, int RegAddress, u8 data)
{
    u8 writeData[3];
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
int __CCIRegReadMBySlaveAddress(int CCISlaveAddress, int RegAddress, u8 *data, int size)
{
    u8 writeAddr[2];
    int i;

    i2c_dev_t i2c_dev = GetI2cDev();

    if (CCISlaveAddress == IMX378_I2C_ADDR ||
        CCISlaveAddress == IMX378_I2C_ADDR_V2WIDE ||
        CCISlaveAddress == AFDRV_I2C_ADDR_V1 ||
        CCISlaveAddress == AFDRV_I2C_ADDR_V2)
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
int __CCIRegWriteMBySlaveAddress(int CCISlaveAddress, int RegAddress, u8 *data, int size)
{
    u8 writeData[2+1024*2];
    int i;
    s32 ret;

    i2c_dev_t i2c_dev = GetI2cDev();

    if (CCISlaveAddress == AFDRV_I2C_ADDR_V2)
    {
        writeData[0] = RegAddress >> 8;
        writeData[1] = RegAddress & 0xFF;
        for (i = 0; i < size; i++)
        {
            writeData[2+i] = data[i];
        }
        ret = i2c_write(i2c_dev, CCISlaveAddress, writeData, 2+size);
        if (ret != COMMUNICATION_SUCCESS)
        {
            return 0;
        }
    }
    else
    {
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
    }

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
int __RegRead1ByteAddress(int SlaveAddress, int RegAddress, u8 *data, int size)
{
    u8 writeAddr[1];
    int i;

    i2c_dev_t i2c_dev = GetI2cDev();

    for (i = 0; i < size; i++)
    {
        writeAddr[0] = RegAddress + i;
        i2c_read(i2c_dev, SlaveAddress, writeAddr, 1, &data[i], 1);
    }

    return size;
}

/*******************************************************************************
 * @brief   Read 16 bit address / 16 bit data register
 *
 * @param   addr    Register address
 * @param   data    Read data
 *
 * @return  Read bytes
 ******************************************************************************/
int __CCIRegRead16bit(u16 addr, u16* data)
{
    int ret = 0;
    u16 r_dat;
    ret = __CCIRegReadMBySlaveAddress(AFDRV_I2C_ADDR_V2, addr, (u8 *) &r_dat, 2);
    *data = ((u8)(r_dat >> 8) & 0xff) + ((u8)(r_dat & 0xff) << 8);
    return ret;
}

/*******************************************************************************
 * @brief   Write 16 bit address / 16 bit data register
 *
 * @param   addr    Register address
 * @param   data    Write data
 *
 * @return  Write bytes
 ******************************************************************************/
int __CCIRegWrite16bit(u16 addr, u16 data)
{
    int ret = 0;
    u8 regdata_array[2] = { 0x00, 0x00 };
    regdata_array[0] = data >> 8;
    regdata_array[1] = data & 0xFF;
    ret = __CCIRegWriteMBySlaveAddress(AFDRV_I2C_ADDR_V2, addr, regdata_array, 2);
    return ret;
}

/*******************************************************************************
 * @brief   Block write 16 bit address / 16 bit data register
 *
 * @param   addr        Register address
 * @param   data        Write data
 * @param   datalen     Write data length
 *
 * @return  Write bytes
 ******************************************************************************/
int __CCIBlockWrite16bit(u16 addr, u16 *data, u16 datalen)
{
    int ret = 0;
    int i;
    union data_buffer {
        u8 cr[1024*2];
        u16 st[1024];
    } databuff;

    for (i = 0; i < datalen; i++) {
        databuff.st[i] = data[i];
    }

    ret = __CCIRegWriteMBySlaveAddress(AFDRV_I2C_ADDR_V2, addr, databuff.cr, datalen*2);
    return ret;
}

#endif
