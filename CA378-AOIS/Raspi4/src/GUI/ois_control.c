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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ois_control.h"
#include "DW9784_Temp_FW_V0403_D1022.h"         // fw file

/***************************************************************
 *  Global variable for OIS Control
 **************************************************************/
double GyroGainRateX = 1.0;
double GyroGainRateY = 1.0;
FirmwareContex g_firmwareContext;
ParametersContex g_parametersContext;
u16 g_downloadByForce;
u16 g_updateFw;
u8 ProductName[PRODUCT_NAME_NUM+1];

/*******************************************************************************
 * @brief   Initialize OIS
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void OIS_InitNone()
{
    // read product name from EEPROM
    memset(ProductName, 0, PRODUCT_NAME_NUM+1);
    int product_name = ReadProductNameFromEEPROM_V2(ProductName);
    if (product_name > 0) {
        printf("Product Name: %s\n", ProductName);
    }

    // read production date from EEPROM
    u8 ProductionDate[PRODUCTION_DATE_NUM];
    int production_date = ReadProductionDateFromEEPROM_V2(ProductionDate);
    if (production_date == PRODUCTION_DATE_NUM) {
        printf("Production Date: %02x%02x/%02x/%02x\n", ProductionDate[0], ProductionDate[1], ProductionDate[2], ProductionDate[3]);
    }
}

/*******************************************************************************
 * @brief   Initialize OIS
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void OIS_InitV1()
{
    printf("OIS Initilize\n");

    int i;
    int slave_address = AFDRV_I2C_ADDR_V1;
    int sensor_slave_address = IMX378_I2C_ADDR;

    // read Calib data from EEPROM
    u8 CalibData[CALIB_DATA_NUM];
    int calib = ReadCalibFromEEPROM(CalibData);

    // read product name from EEPROM
    memset(ProductName, 0, PRODUCT_NAME_NUM+1);
    int product_name = ReadProductNameFromEEPROM_V1(ProductName);
    if (product_name > 0) {
        printf("Product Name: %s\n", ProductName);
    }

    // read production date from EEPROM
    u8 ProductionDate[PRODUCTION_DATE_NUM];
    int production_date = ReadProductionDateFromEEPROM_V1(ProductionDate);
    if (production_date == PRODUCTION_DATE_NUM) {
        printf("Production Date: %02x%02x/%02x/%02x\n", ProductionDate[0], ProductionDate[1], ProductionDate[2], ProductionDate[3]);
    }

    // read recalibration date from EEPROM
    u8 RecalibDate[RECALIB_DATE_NUM];
    int recalibration_date = ReadReCalibDateFromEEPROM_V1(RecalibDate);
    u8 RecalibVersion[RECALIB_VERSION_NUM];
    int recalibration_version = ReadReCalibVersionFromEEPROM_V1(RecalibVersion);
    if ((recalibration_date == RECALIB_DATE_NUM) && (recalibration_version == RECALIB_VERSION_NUM)) {
        printf("Recalibration Date(Version): %02x%02x/%02x/%02x(%x.%x)\n", RecalibDate[0], RecalibDate[1], RecalibDate[2], RecalibDate[3], RecalibVersion[0], RecalibVersion[1]);
    }

    // OIS init
    int ois_init_address = 0x30AC;
    int ois_init_regdata = 0x01;
    if (__CCIRegWriteBySlaveAddress(sensor_slave_address, ois_init_address, ois_init_regdata) > 0) {
        printf("set: [Init:OIS] slave=%02x, address=%04x, RegData = %02x\n", sensor_slave_address, ois_init_address, ois_init_regdata);
    } else {
        printf("set: error(Init:OIS)!\n");
    }

    // Gyro Activation
    printf("[Init:i2cdat_gyro]\n");
    for (i = 0; i < 65535; i++) {
        if (i2cdat_gyro_[i].adr == 0xffff) {
            break;
        }
        if (__CCIRegWriteBySlaveAddress(slave_address, i2cdat_gyro_[i].adr, i2cdat_gyro_[i].dat) > 0) {
            //printf("set: [Init:i2cdat_gyro] slave=%02x, address=%04x, RegData = %02x\n", slave_address, i2cdat_gyro_[i].adr, i2cdat_gyro_[i].dat);
        } else {
            printf("set: error(Init:i2cdat_gyro)!\n");
        }
    }

    // Start download
    int start_address = 0xF010;
    int start_regdata = 0x00;
    if (__CCIRegWriteBySlaveAddress(slave_address, start_address, start_regdata) > 0) {
        printf("set: [Init:Start download] slave=%02x, address=%04x, RegData = %02x\n", slave_address, start_address, start_regdata);
    } else {
        printf("set: error(Init:Start download)!\n");
    }

    // DownloadProgram1
    printf("[Init:DownloadProgram1]");
    RegWriteBurst(slave_address, i2cdat_dp1_);

    // DownloadProgram2
    printf("[Init:DownloadProgram2]");
    RegWriteBurst(slave_address, i2cdat_dp2_);

    // Calibration
    printf("[Init:Calibration]\n");
    for (i = 0; i < 65535; i++) {
        if (i2cdat_calb_[i].adr == 0xffff) {
            break;
        }
        if (calib == 0) {
            if (__CCIRegWriteBySlaveAddress(slave_address, i2cdat_calb_[i].adr, i2cdat_calb_[i].dat) > 0) {
            } else {
                printf("set: error(Init:Calibration)!\n");
            }
        } else {
            if (__CCIRegWriteBySlaveAddress(slave_address, i2cdat_calb_[i].adr, CalibData[i]) > 0) {
            } else {
                printf("set: error(Init:Calibration)!\n");
            }
        }
    }

    // Set OIS complete DL
    int ois_comp_address = 0xF006;
    int ois_com_regdata = 0x00;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_comp_address, ois_com_regdata) > 0) {
        printf("set: [Init:Set OIS complete DL] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_comp_address, ois_com_regdata);
    } else {
        printf("set: error(Init:Set OIS complete DL)!\n");
    }

    // PWM frequency adjustment setting - About 4.5MHz
    printf("[Init:PWM frequency]");
    for (i = 0; i < 65535; i++) {
        if (i2cdat_pwmfrequency_[i].adr == 0xffff) {
            break;
        }
        if (__CCIRegWriteBySlaveAddress(slave_address, i2cdat_pwmfrequency_[i].adr, i2cdat_pwmfrequency_[i].dat) > 0) {
        } else {
            printf("set: error(Init:PWM frequency)!\n");
        }
        if ((i & 0x3F) == 0x3F) {
            printf(".");
            fflush(stdout);
        }
    }
    printf("\n");

    // Servo on
    int servo_address = 0x6020;
    int servo_regdata = 0x01;
    if (__CCIRegWriteBySlaveAddress(slave_address, servo_address, servo_regdata) > 0) {
        printf("set: [Init:Servo on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, servo_address, servo_regdata);
    } else {
        printf("set: error(Init:Servo on)!\n");
    }

    int temp_address;
    int temp_data;
    // Gyro On
    temp_address = 0x6023;
    temp_data = 0x02;
    if (__CCIRegWriteBySlaveAddress(slave_address, temp_address, temp_data) > 0) {
        printf("set: [Init:Gyro on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, temp_address, temp_data);
    } else {
        printf("set: error(Init:Gyro on)!\n");
    }

    temp_address = 0x602C;
    temp_data = 0x1B;
    if (__CCIRegWriteBySlaveAddress(slave_address, temp_address, temp_data) > 0) {
        printf("set: [Init:Gyro on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, temp_address, temp_data);
    } else {
        printf("set: error(Init:Gyro on)!\n");
    }

    temp_address = 0x602D;
    temp_data = 0x10;
    if (__CCIRegWriteBySlaveAddress(slave_address, temp_address, temp_data) > 0) {
        printf("set: [Init:Gyro on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, temp_address, temp_data);
    } else {
        printf("set: error(Init:Gyro on)!\n");
    }

    // Gyro on
    int gyro_address = 0x6023;
    int gyro_regdata = 0x00;
    if (__CCIRegWriteBySlaveAddress(slave_address, gyro_address, gyro_regdata) > 0) {
        printf("set: [Init:Gyro on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, gyro_address, gyro_regdata);
    } else {
        printf("set: error(Init:Gyro on)!\n");
    }

    // GPO to Hsync
    printf("[Init:GPO]");
    for (i = 0; i < 65535; i++) {
        if (i2cdat_gpo_[i].adr == 0xffff) {
            break;
        }
        if (__CCIRegWriteBySlaveAddress(sensor_slave_address, i2cdat_gpo_[i].adr, i2cdat_gpo_[i].dat) > 0) {
        } else {
            printf("set: error(Init:GPO)!\n");
        }
        if ((i & 0x3F) == 0x3F) {
            printf(".");
            fflush(stdout);
        }
    }
    printf("\n");
}

/*******************************************************************************
 * @brief   Initialize OIS
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void OIS_InitV2()
{
    printf("OIS Initilize\n");

    int sensor_slave_address = IMX378_I2C_ADDR;

    // read product name from EEPROM
    memset(ProductName, 0, PRODUCT_NAME_NUM+1);
    int product_name = ReadProductNameFromEEPROM_V2(ProductName);
    if (product_name > 0) {
        printf("Product Name: %s\n", ProductName);
    }

    // read production date from EEPROM
    u8 ProductionDate[PRODUCTION_DATE_NUM];
    int production_date = ReadProductionDateFromEEPROM_V2(ProductionDate);
    if (production_date == PRODUCTION_DATE_NUM) {
        printf("Production Date: %02x%02x/%02x/%02x\n", ProductionDate[0], ProductionDate[1], ProductionDate[2], ProductionDate[3]);
    }

    // OIS init
    int ois_init_address = 0x30AC;
    int ois_init_regdata = 0x01;
    if (__CCIRegWriteBySlaveAddress(sensor_slave_address, ois_init_address, ois_init_regdata) > 0) {
        printf("set: [Init:OIS] slave=%02x, address=%04x, RegData = %02x\n", sensor_slave_address, ois_init_address, ois_init_regdata);
    } else {
        printf("set: error(Init:OIS)!\n");
    }

    if (FWDownload() != EOK) {
        printf("FWDownload: error!\n"); return;
    }

    if (ServoOn() == FUNC_FAIL) {
        printf("ServoOn: error!\n"); return;
    }
}

/*******************************************************************************
 * @brief   OIS mode settings
 *
 * @param   mode    0: OFF
 *                  1: Zero shutter
 *                  2: Exposure/Shake eval.
 *                  3: Movie
 *                  4: High SR Movie
 *                  5: View Finder
 *
 * @return  void
 ******************************************************************************/
void OIS_ModeV1(int mode)
{
    printf("OIS Mode\n");

    int serection_value;
    int slave_address = (0x7c) >> 1;

    int off_address = 0x6020;
    int off_regdata = 0x01;

    serection_value = mode;
    if (serection_value == 0) {
        // OFF
        if (__CCIRegWriteBySlaveAddress(slave_address, off_address, off_regdata) > 0) {
            printf("set: [OIS OFF] slave=%02x, address=%04x, RegData = %02x\n", slave_address, off_address, off_regdata);
        } else {
            printf("set: error(OIS OFF)!\n");
        }
    } else {
        // OFF
        if (__CCIRegWriteBySlaveAddress(slave_address, off_address, off_regdata) > 0) {
            printf("set: [1.OIS OFF] slave=%02x, address=%04x, RegData = %02x\n", slave_address, off_address, off_regdata);
        } else {
            printf("set: error(1.OIS OFF)!\n");
        }

        int mode_address = 0x6021;
        u8 mode_regdata = mode;

        switch (mode)
        {
        case 1:
            // Zero shutter
            mode_regdata = 0x6B;
            break;
        case 2:
            // Exposure/Shake eval.
            mode_regdata = 0x03;
            break;
        case 3:
            // Movie
            mode_regdata = 0x61;
            break;
        case 4:
            // High SR Movie
            mode_regdata = 0x63;
            break;
        case 5:
            // View Finder
            mode_regdata = 0x79;
            break;
        default:
            break;
        }

        if (__CCIRegWriteBySlaveAddress(slave_address, mode_address, mode_regdata) > 0) {
            printf("set: [2.Mode setting] slave=%02x, address=%04x, RegData = %02x\n", slave_address, mode_address, mode_regdata);
            if (__CCIRegReadBySlaveAddress(slave_address, mode_address, &mode_regdata) > 0) {
                printf("read: [2.Mode setting] slave=%02x, address=%04x, RegData = %02x\n", slave_address, mode_address, mode_regdata);
            }
        } else {
            printf("set: error(2.Mode setting)!\n");
        }

        // ON
        int on_address = 0x6020;
        int on_regdata = 0x02;
        if (__CCIRegWriteBySlaveAddress(slave_address, on_address, on_regdata) > 0) {
            printf("set: [3.OIS ON] slave=%02x, address=%04x, RegData = %02x\n", slave_address, on_address, on_regdata);
        } else {
            printf("set: error(3.OIS ON)!\n");
        }
    }
}

/*******************************************************************************
 * @brief   OIS mode settings
 *
 * @param   mode    0: OFF
 *                  1: ON
 *
 * @return  void
 ******************************************************************************/
void OIS_ModeV2(int mode)
{
    char logbuff[256];
    u16 active = 0x0000;
    u16 status = 0x0000;

    if (mode == 0) {
        sprintf(logbuff, "%s", "OIS off(Servo on)");
        active = 0x0001;
        status = 0x1001;
    } else {
        sprintf(logbuff, "%s", "OIS on");
        active = 0x0000;
        status = 0x1000;
    }

    // Set control mode
    if (__CCIRegWrite16bit(0x7012, 0x0001) == 0) {
        printf("set: error(Set control mode)!\n");
    }
    delay_msec(1);
    // Set active
    if (__CCIRegWrite16bit(0x7011, active) == 0) {
        printf("set: error(%s)!\n", logbuff);
    }
    delay_msec(1);
    // Read status
    if (WaitCheckRegister(0x7010, status) == FUNC_PASS) {  // busy check
        printf("%s\n", logbuff);
    } else {
        printf("Read status error!\n");
    }
}

/*******************************************************************************
 * @brief
 *
 * @param
 *
 * @return
 ******************************************************************************/
int RegWriteBurst(int slave_address, const T_I2CRegWriteData *I2CRegWriteData)
{
    u8 data[BURST_SIZE];
    int i;
    int addr;
    int lastAddr;
    int size = 0;

    for (i = 0; i < 0xFFFF; i++) {
        if (I2CRegWriteData[i].adr == 0xFFFF) {
            break;
        }

        if (size == 0) {
            lastAddr = addr = I2CRegWriteData[i].adr;
            data[size++] = I2CRegWriteData[i].dat;
        } else {
            if (lastAddr + 1 == I2CRegWriteData[i].adr) {
                data[size] = I2CRegWriteData[i].dat;
                size++;
            } else {
                if (__CCIRegWriteMBySlaveAddress(slave_address, addr, data, size) == size) {
                    printf(".");
                    fflush(stdout);
                } else {
                    printf("RegWriteBurst error!\n");
                }
                size = 0;
                lastAddr = addr = I2CRegWriteData[i].adr;
                data[size++] = I2CRegWriteData[i].dat;
            }
        }

        if (size == BURST_SIZE) {
            if (__CCIRegWriteMBySlaveAddress(slave_address, addr, data, size) == size) {
                printf(".");
                fflush(stdout);
            } else {
                printf("RegWriteBurst error!\n");
            }
            size = 0;
        }

        lastAddr = I2CRegWriteData[i].adr;
    }

    if (size != 0) {
        if (__CCIRegWriteMBySlaveAddress(slave_address, addr, data, size) == size) {
            printf(".");
            fflush(stdout);
        } else {
            printf("RegWriteBurst error!\n");
        }
    }

    printf("\n");

    return 1;
}

/*******************************************************************************
 * @brief
 *
 * @param
 *
 * @return
 ******************************************************************************/
int ReadGyroGain()
{
    FILE *fp;
    char buf[256];
    char itemName[256];

    if ((fp = fopen("./bin/demo.ini", "r")) != NULL) {
        while ( fgets(buf, 256, fp) != NULL ) {

            char *p = (char *)strtok(buf, "= \r\n");
            int n = 0;
            while (p)
            {
                switch (n)
                {
                    case 0:
                        strcpy(itemName, p);
                        break;
                    case 1:
                        if (strcmp(itemName, "gyroGainRateX") == 0)
                        {
                            GyroGainRateX = atof(p);
                        }
                        else if (strcmp(itemName, "gyroGainRateY") == 0)
                        {
                            GyroGainRateY = atof(p);
                        }
                        break;
                    default:
                        break;
                }
                n++;
                p = (char *)strtok(NULL, "= \r\n");
            }
        }

        fclose(fp);
    }

    return 1;
}

/*******************************************************************************
 * @brief   Read calibration data from EEPROM
 *
 * @param   CalibData    Calibration data
 *
 * @return  Read bytes
 ******************************************************************************/
int ReadCalibFromEEPROM(u8 *CalibData)
{
    int readBytes = 0;
    int gyroGainX, gyroGainY;
    u8 data[CALIB_DATA_NUM];
    int i;

    ReadGyroGain();

    __RegRead1ByteAddress(EEPROM_I2C_ADDR, 0x40, data, CALIB_DATA_NUM);

    if (data[0] != 0xFF)
    {
        // manual adjustment for gyro gain
        gyroGainX = ((data[0x16] << 8) + data[0x17]) * GyroGainRateX;
        gyroGainY = ((data[0x18] << 8) + data[0x19]) * GyroGainRateY;
        data[0x16] = gyroGainX >> 8;    // OIS Gyro gain X Hi
        data[0x17] = gyroGainX & 0xFF;  // OIS Gyro gain X Lo
        data[0x18] = gyroGainY >> 8;    // OIS Gyro gain Y Hi
        data[0x19] = gyroGainY & 0xFF;  // OIS Gyro gain Y Lo

        printf("EEPROM Read:\n  ");
        for (i = 0; i < CALIB_DATA_NUM; i++)
        {
            CalibData[i] = data[i];
            readBytes++;
            printf("%2.2X ", data[i]);
            if ((i & 0xF) == 0xF) printf("\n  ");
        }

        printf("Gyro gain:%d(x%.2lf), %d(x%.2lf)\n", gyroGainX, GyroGainRateX, gyroGainY, GyroGainRateY);
    }

    return readBytes;
}

/*******************************************************************************
 * @brief   Read product name from EEPROM
 *
 * @param   ProductName     Product name
 *
 * @return  Read bytes
 ******************************************************************************/
int ReadProductNameFromEEPROM_V1(u8 *ProductName)
{
    int readBytes = 0;
    u8 data[PRODUCT_NAME_NUM];
    int i;

    __RegRead1ByteAddress(EEPROM_I2C_ADDR, 0x00, data, PRODUCT_NAME_NUM);

    if (data[0] != 0xFF)
    {
        for (i = 0; i < PRODUCT_NAME_NUM; i++)
        {
            if (data[i] == 0x00) {  // terminating character
                break;
            }
            if (data[i] != 0x20) {  // remove space
                ProductName[i] = data[i];
                readBytes++;
            }
        }
    }
    return readBytes;
}

/*******************************************************************************
 * @brief   Read product name from EEPROM
 *
 * @param   ProductName     Product name
 *
 * @return  Read bytes
 ******************************************************************************/
int ReadProductNameFromEEPROM_V2(u8 *ProductName)
{
    int readBytes = 0;
    u8 data[PRODUCT_NAME_NUM];
    int i;

    __CCIRegReadMBySlaveAddress(EEPROM_I2C_ADDR, 0x0000, data, PRODUCT_NAME_NUM);

    if (data[0] != 0xFF)
    {
        for (i = 0; i < PRODUCT_NAME_NUM; i++)
        {
            if (data[i] == 0x00) {  // terminating character
                break;
            }
            if (data[i] != 0x20) {  // remove space
                ProductName[i] = data[i];
                readBytes++;
            }
        }
    }
    return readBytes;
}

/*******************************************************************************
 * @brief   Read production date from EEPROM
 *
 * @param   ProductionDate     Production date
 *
 * @return  Read bytes
 ******************************************************************************/
int ReadProductionDateFromEEPROM_V1(u8 *ProductionDate)
{
    int readBytes = 0;
    u8 data[PRODUCTION_DATE_NUM];
    int i;
    int allZero = 1;

    __RegRead1ByteAddress(EEPROM_I2C_ADDR, 0x28, data, PRODUCTION_DATE_NUM);

    if (data[0] != 0xFF)
    {
        for (i = 0; i < PRODUCTION_DATE_NUM; i++)
        {
            ProductionDate[i] = data[i];
            readBytes++;
            if (data[i] != 0x00) {
                allZero = 0;
            }
        }
    }

    if (allZero) {
        readBytes = 0;
    }
    return readBytes;
}

/*******************************************************************************
 * @brief   Read production date from EEPROM
 *
 * @param   ProductionDate     Production date
 *
 * @return  Read bytes
 ******************************************************************************/
int ReadProductionDateFromEEPROM_V2(u8 *ProductionDate)
{
    int readBytes = 0;
    u8 data[PRODUCTION_DATE_NUM];
    int i;
    int allZero = 1;

    __CCIRegReadMBySlaveAddress(EEPROM_I2C_ADDR, 0x0028, data, PRODUCTION_DATE_NUM);

    if (data[0] != 0xFF)
    {
        for (i = 0; i < PRODUCTION_DATE_NUM; i++)
        {
            ProductionDate[i] = data[i];
            readBytes++;
            if (data[i] != 0x00) {
                allZero = 0;
            }
        }
    }

    if (allZero) {
        readBytes = 0;
    }
    return readBytes;
}

/*******************************************************************************
 * @brief   Read recalibration date from EEPROM
 *
 * @param   RecalibDate     Recalibration date
 *
 * @return  Read bytes
 ******************************************************************************/
int ReadReCalibDateFromEEPROM_V1(u8 *RecalibDate)
{
    int readBytes = 0;
    u8 data[RECALIB_DATE_NUM];
    int i;
    int allZero = 1;

    __RegRead1ByteAddress(EEPROM_I2C_ADDR, 0x2e, data, RECALIB_DATE_NUM);

    if (data[0] != 0xFF)
    {
        for (i = 0; i < RECALIB_DATE_NUM; i++)
        {
            RecalibDate[i] = data[i];
            readBytes++;
            if (data[i] != 0x00) {
                allZero = 0;
            }
        }
    }

    if (allZero) {
        readBytes = 0;
    }
    return readBytes;
}

/*******************************************************************************
 * @brief   Read recalibration version from EEPROM
 *
 * @param   RecalibVersion     Recalibration version
 *
 * @return  Read bytes
 ******************************************************************************/
int ReadReCalibVersionFromEEPROM_V1(u8 *RecalibVersion)
{
    int readBytes = 0;
    u8 data[RECALIB_VERSION_NUM];
    int i;
    int allZero = 1;

    __RegRead1ByteAddress(EEPROM_I2C_ADDR, 0x2c, data, RECALIB_VERSION_NUM);

    if (data[0] != 0xFF)
    {
        for (i = 0; i < RECALIB_VERSION_NUM; i++)
        {
            RecalibVersion[i] = data[i];
            readBytes++;
            if (data[i] != 0x00) {
                allZero = 0;
            }
        }
    }

    if (allZero) {
        readBytes = 0;
    }
    return readBytes;
}

/*******************************************************************************
 * @brief   Set product name
 *
 * @param   pName       Product name
 *
 * @return  void
 ******************************************************************************/
void SetProductName(u8 *pName)
{
    int i;
    for (i = 0; i < PRODUCT_NAME_NUM; i++)
    {
        pName[i] = ProductName[i];
    }
}

/*******************************************************************************
 * @brief   Generate firmware contexts
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void GenerateFirmwareContexts(void)
{
    g_firmwareContext.version = DW9784_FW_VERSION;
    g_firmwareContext.driverIc = 0x9784;
    g_firmwareContext.fwContentPtr = DW9784_FW;
    g_downloadByForce = 0;
    g_updateFw = 0;
}

/*******************************************************************************
 * @brief   Firmware download
 *
 * @param   void
 *
 * @return  Success 0
 ******************************************************************************/
int FWDownload(void)
{
    int result = 0;
    u16 pre_module_state = 0; // 0x0000: normal, 0xFFFF: abnormal

    GenerateFirmwareContexts();

    result = VerifySecondChipId();
    if (result == ERROR_SECOND_ID) {
        printf("Failed to check the second_id(0x0020) inside the ic.\n");
        printf("Stop the ic boot operation.\n");
        return ERROR_SECOND_ID;
    }

    if (ReadChipId() == DW9784_CHIP_ID) {
        printf("Chip id check pass.\n");
    } else {
        g_downloadByForce = 1;
        printf("Chip id check failed.\n");
        printf("Force to recovery firmware.\n");
    }

    if (VerifyFirmwareChecksum() == ERROR_FW_CHECKSUM) {
        g_downloadByForce = 1;
        printf("The firmware checksum check of the flash memory failed.\n");
        printf("Force to recovery firmware.\n");
    }

    if (ReadFirmwareVersion() != DW9784_FW_VERSION) {
        g_updateFw = 1; 
        printf("Update fw to the latest version.\n");
    } else {
        printf("The firmware version is up to date, ver: 0x%04X.\n", DW9784_FW_VERSION);
    }

    if (g_downloadByForce == 1) {
        pre_module_state = 0xFFFF; // abnormal state
    } else if (g_updateFw == 1) {
        pre_module_state = 0x0000; // normal state
    }

    if (g_downloadByForce || g_updateFw) {
        printf("Start downloading the latest version firmware, ver: 0x%04X.\n", DW9784_FW_VERSION);
        if (DownloadFirmware(pre_module_state) == EOK) {
            // fw download success
            printf("Complete fw download.\n");
            return EOK;
        } else {
            pre_module_state = 0x0001; // Firmware download failed once
            printf("1st fw download failed, retry 2nd fw download.\n");
            __CCIRegWrite16bit(0xD002, 0x0001); // logic reset
            delay_msec(4);

            if (DownloadFirmware(pre_module_state) == EOK) {
                // fw download success
                printf("Complete 2nd fw download.\n");
                return EOK;
            } else {
                // fw download failed
                EnterShutdownMode();
                printf("2nd fw download failed, enter ic shutdown(sleep) mode.\n");
                return ERROR_FW_DOWN_FAIL;
            }
        }
    }

    return EOK;
}

/*******************************************************************************
 * @brief   Download firmware
 *
 * @param   module_state        Module state
 *
 * @return  Success 0
 ******************************************************************************/
int DownloadFirmware(int module_state)
{
    u16 i;
    u16 addr;
    u16 FMC;

    __CCIRegWrite16bit(0xD002, 0x0001);     // Logic reset
    delay_msec(4);

    // Select FW sector
    ReleaseAllProtection();                 // All protection release
    delay_msec(1);

    __CCIRegWrite16bit(0xD001, 0x0000);     // DSP off (Standby mode)
    delay_msec(5);

    __CCIRegWrite16bit(0xDE01, 0x0000);     // FMC block FW select
    delay_msec(5);
    __CCIRegRead16bit(0xDE01, &FMC);        // FMC register check
    if (FMC != 0) {
        printf("FMC register value 1st warning : %04X\n", FMC);
        __CCIRegWrite16bit(0xDE01, 0x0000);     // FMC block FW select
        delay_msec(5);
        FMC = 0; // initialize FMC value
        __CCIRegRead16bit(0xDE01, &FMC);        // FMC register check
        if (FMC != 0) {
            printf("2nd FMC register value 2nd warning : %04X\n", FMC);
            printf("Stop FW download.\n");
            return ERROR_FW_DOWN_FMC;
        }
    }

    // Erase FW sector
    EraseFlashFW();

    // Write FW data(20KB)
    printf("Start firmware download.\n");
    *(g_firmwareContext.fwContentPtr + MCS_SIZE_W -1) = module_state;
    for (i = 0; i < MCS_SIZE_W; i += DATPKT_SIZE) {
        addr = MCS_START_ADDRESS + i;
        __CCIBlockWrite16bit(addr, g_firmwareContext.fwContentPtr + i, DATPKT_SIZE);
    }

    // Select IF sector
    __CCIRegWrite16bit(0xDE01, 0x1000);     // FMC block IF select
    delay_msec(5);
    __CCIRegRead16bit(0xDE01, &FMC);        // FMC register check
    if (FMC != 0x1000) {
        printf("IF FMC register value 1st warning : %04X\n", FMC);
        __CCIRegWrite16bit(0xDE01, 0x1000);     // FMC block IF select
        delay_msec(5);
        FMC = 0; // initialize FMC value
        __CCIRegRead16bit(0xDE01, &FMC);        // FMC register check
        if (FMC != 0x1000) {
            printf("2nd IF FMC register value 2nd fail : %04X\n", FMC);
            printf("Stop firmware download.\n");
            return ERROR_FW_DOWN_FMC;
        }
    }

    // Erase IF sector
    EraseFlashIF();

    // Write FW data(512B)
    printf("Start firmware/pid download.\n");
    for (i = 0; i < PID_SIZE_W; i += DATPKT_SIZE) {
        addr = IF_START_ADDRESS + i;
        __CCIBlockWrite16bit(addr, g_firmwareContext.fwContentPtr + MCS_SIZE_W + i, DATPKT_SIZE);
    }

    // OIS reset
    OISReset();

    // Checksum verification
    if (VerifyFirmwareChecksum() != EOK) {
        printf("Firmware download failed.\n");
        return ERROR_FW_CHECKSUM;
    }

    printf("Firmware download success.\n");
    return EOK;
}

/*******************************************************************************
 * @brief   Verify second chip id
 *
 * @param   void
 *
 * @return  Success 0
 ******************************************************************************/
int VerifySecondChipId(void)
{
    u16 sec_chip_id;
    __CCIRegWrite16bit(0xD000, 0x0001); // chip enable
    delay_msec(4);
    __CCIRegWrite16bit(0xD002, 0x0001); // logic reset
    delay_msec(4);

    ReleaseAllProtection(); // All protection

    __CCIRegRead16bit(0xD060, &sec_chip_id); // 2nd chip id
    printf("sec_chip_id : 0x%04X\n", sec_chip_id);
    if (sec_chip_id != 0x0020) {
        printf("second_chip_id check fail : 0x%04X\n", sec_chip_id);
        EnterShutdownMode();
        return ERROR_SECOND_ID;
    }

    OISReset(); /* ois reset */
    return EOK;
}

/*******************************************************************************
 * @brief   Read chip id
 *
 * @param   void
 *
 * @return  Chip id
 ******************************************************************************/
u16 ReadChipId(void)
{
    u16 chip_id;
    __CCIRegRead16bit(DW9784_CHIP_ID_ADDRESS, &chip_id);

    printf("chip id : 0x%04X\n", chip_id);
    return chip_id;
}

/*******************************************************************************
 * @brief   Read firmware version
 *
 * @param   void
 *
 * @return  Firmware version
 ******************************************************************************/
u16 ReadFirmwareVersion(void)
{
    u16 fw_ver;
    u16 fw_date;

    __CCIRegRead16bit(0x7001, &fw_ver);
    __CCIRegRead16bit(0x7002, &fw_date);

    printf("fw version: 0x%04X\n", fw_ver);
    printf("fw date   : 0x%04X\n", fw_date);

    return fw_ver;
}

/*******************************************************************************
 * @brief   Release all protection
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void ReleaseAllProtection(void)
{
    __CCIRegWrite16bit(0xFAFA, 0x98AC);
    delay_msec(1);
    __CCIRegWrite16bit(0xF053, 0x70BD);
    delay_msec(1);
}

/*******************************************************************************
 * @brief   Erase flash IF
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void EraseFlashIF (void)
{
    __CCIRegWrite16bit(0xDE03, 0x0000);     // IF0 select
    __CCIRegWrite16bit(0xDE04, 0x0008);     // 512byte page erase start
    delay_msec(10);
}

/*******************************************************************************
 * @brief   Erase flash FW
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void EraseFlashFW(void)
{
    __CCIRegWrite16bit(0xDE03, 0x0000);     // 4K sector 0
    __CCIRegWrite16bit(0xDE04, 0x0002);     // 4K sector erase start
    delay_msec(10);

    __CCIRegWrite16bit(0xDE03, 0x0008);     // 4K sector 1
    __CCIRegWrite16bit(0xDE04, 0x0002);     // 4K sector erase start
    delay_msec(10);

    __CCIRegWrite16bit(0xDE03, 0x0010);     // 4K sector 2
    __CCIRegWrite16bit(0xDE04, 0x0002);     // 4K sector erase start
    delay_msec(10);

    __CCIRegWrite16bit(0xDE03, 0x0018);     // 4K sector 3
    __CCIRegWrite16bit(0xDE04, 0x0002);     // 4K sector erase start
    delay_msec(10);

    __CCIRegWrite16bit(0xDE03, 0x0020);     // 4K sector 4
    __CCIRegWrite16bit(0xDE04, 0x0002);     // 4K sector erase start
    delay_msec(10);
}

/*******************************************************************************
 * @brief   Enter shutdown mode
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void EnterShutdownMode(void)
{
    printf("Enter ic shutdown mode.\n");
    __CCIRegWrite16bit(0xD000, 0x0000);
    delay_msec(1);
}

/*******************************************************************************
 * @brief   Reset OIS
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void OISReset(void)
{
    printf("OIS reset.\n");
    delay_msec(10);
    __CCIRegWrite16bit(0xD002, 0x0001);     // Logic reset
    delay_msec(4);
    __CCIRegWrite16bit(0xD001, 0x0001);     // DSP on (Active mode)
    delay_msec(25);
    __CCIRegWrite16bit(0xEBF1, 0x56FA);     // User protection release
}

/*******************************************************************************
 * @brief   Change to servo on mode
 *
 * @param   void
 *
 * @return  Success 0
 ******************************************************************************/
int ServoOn(void)
{
    printf("Servo on mode change.\n");
    __CCIRegWrite16bit(0x7012, 0x0001);    // set control mode
    delay_msec(1);
    __CCIRegWrite16bit(0x7011, 0x0001);    // servo on
    delay_msec(1);
    if (WaitCheckRegister(0x7010, 0x1001) == FUNC_PASS) {  // busy check
        printf("Servo on success.\n");
        return FUNC_PASS;
    }

    return FUNC_FAIL;
}

/*******************************************************************************
 * @brief   Change to servo off mode
 *
 * @param   void
 *
 * @return  Success 0
 ******************************************************************************/
int ServoOff(void)
{
    printf("Servo off mode change.\n");
    __CCIRegWrite16bit(0x7012, 0x0001);    // set control mode
    delay_msec(1);
    __CCIRegWrite16bit(0x7011, 0x0002);    // servo off
    delay_msec(1);

    if (WaitCheckRegister(0x7010, 0x1002) == FUNC_PASS) {  // busy check
        printf("Servo off success.\n");
        return FUNC_PASS;
    }

    return FUNC_FAIL;
}

/*******************************************************************************
 * @brief   Verify firmware checksum
 *
 * @param   void
 *
 * @return  Success 0
 ******************************************************************************/
int VerifyFirmwareChecksum(void)
{
    u16 reg_fw_checksum;            // 0x700C
    u16 reg_checksum_status;        // 0x700D

    __CCIRegRead16bit(0x700C, &reg_fw_checksum);
    __CCIRegRead16bit(0x700D, &reg_checksum_status);

    printf("reg_checksum_status : 0x%04X\n", reg_checksum_status);
    printf("ref_fw_checksum : 0x%04X, reg_fw_checksum : 0x%04X\n", DW9784_FW_CHECKSUM, reg_fw_checksum);

    // Bit [0]: FW checksum error
    if (reg_checksum_status & 0x0001) {
        printf("FW checksum error.\n");
        return ERROR_FW_CHECKSUM;
    }

    printf("FW checksum pass.\n");
    return EOK;
}

/*******************************************************************************
 * @brief   Wait check register
 *
 * @param   reg         Read target register
 * @param   ref         Compare reference data
 *
 * @return  Success 0
 ******************************************************************************/
int WaitCheckRegister(u16 reg, u16 ref)
{
    u16 r_data;
    int i=0;

    for (i = 0; i < LOOP_A; i++) {
        __CCIRegRead16bit(reg, &r_data); //Read status
        if (r_data == ref) {
            break;
        } else {
            if (i >= LOOP_B) {
                printf("Verify fail: 0x%04X\n", r_data);
                return FUNC_FAIL;
            }
        }
        delay_msec(WAIT_TIME);
    }

    return FUNC_PASS;
}
