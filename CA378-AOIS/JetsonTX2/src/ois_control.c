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

/***************************************************************
 *  Global variable for OIS Control
 **************************************************************/
double GyroGainRateX = 1.0;
double GyroGainRateY = 1.0;


/*******************************************************************************
 * @brief	Initialize OIS
 *
 * @param	void
 *
 * @return	void
 ******************************************************************************/
void OIS_Init()
{
    printf("OIS Initilize\n");

    int i;
    int slave_address = (0x7c) >> 1;
    int sensor_slave_address = (0x34) >> 1;

    // read Calib data from EEPROM
    unsigned char CalibData[CALIB_DATA_NUM];
    int calib = ReadCalibFromEEPROM(CalibData);

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
        if(calib == 0) {
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

    // Servo on
    int servo_address = 0x6020;
    int servo_regdata = 0x01;
    if (__CCIRegWriteBySlaveAddress(slave_address, servo_address, servo_regdata) > 0) {
         printf("set: [Init:Servo on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, servo_address, servo_regdata);
    }
    else {
        printf("set: error(Init:Servo on)!\n");
    }

    int temp_address;
    int temp_data;
    // Gyro On
    temp_address = 0x6023;
    temp_data = 0x02;
    if (__CCIRegWriteBySlaveAddress(slave_address, temp_address, temp_data) > 0) {
         printf("set: [Init:Gyro on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, temp_address, temp_data);
    }
    else {
        printf("set: error(Init:Gyro on)!\n");
    }

    temp_address = 0x602C;
    temp_data = 0x1B;
    if (__CCIRegWriteBySlaveAddress(slave_address, temp_address, temp_data) > 0) {
         printf("set: [Init:Gyro on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, temp_address, temp_data);
    }
    else {
        printf("set: error(Init:Gyro on)!\n");
    }

    temp_address = 0x602D;
    temp_data = 0x10;
    if (__CCIRegWriteBySlaveAddress(slave_address, temp_address, temp_data) > 0) {
         printf("set: [Init:Gyro on] slave=%02x, address=%04x, RegData = %02x\n", slave_address, temp_address, temp_data);
    }
    else {
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

    // EEPROM to DriverRegister
    int regdata = 0x00;
    int regdata1 = 0x00;
    // OIS Hall current X DataWidthDetai:8bit EEPROM:1DC1h Source:60A2h
    int ois_hall_cur_x_source = 0x60A2;
    regdata = 0x37;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_hall_cur_x_source, regdata) > 0) {
         printf("set: [OIS Hall current X] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_hall_cur_x_source, regdata);
    }

    // OIS Hall current Y DataWidthDetai:8bit EEPROM:1DC3h Source:60A3h
    int ois_hall_cur_y_source = 0x60A3;
    regdata = 0x3C;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_hall_cur_y_source, regdata) > 0) {
         printf("set: [OIS Hall current Y] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_hall_cur_y_source, regdata);
    }

    // OIS Hall offset X DataWidthDetai:10bit EEPROM:1DC4h Source:60A4h
    int ois_hall_off_x_source = 0x60A4;
    int ois_hall_off_x_source1 = 0x60A5;
    regdata = 0x02;
    regdata1 = 0x18;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_hall_off_x_source, regdata) > 0) {
         printf("set: [OIS Hall offset X] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_hall_off_x_source, regdata);
    }
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_hall_off_x_source1, regdata1) > 0) {
         printf("set: [OIS Hall offset X] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_hall_off_x_source1, regdata1);
    }

    // OIS Hall offset Y DataWidthDetai:10bit EEPROM:1DC6h Source:60A6h
    int ois_hall_off_y_source = 0x60A6;
    int ois_hall_off_y_source1 = 0x60A7;
    regdata = 0x01;
    regdata1 = 0xB4;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_hall_off_y_source, regdata) > 0) {
         printf("set: [OIS Hall offset Y] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_hall_off_y_source, regdata);
    }
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_hall_off_y_source1, regdata1) > 0) {
         printf("set: [OIS Hall offset Y] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_hall_off_y_source1, regdata1);
    }

    // OIS Gyro gain X DataWidthDetai:16bit EEPROM:1DD6h Source:607Ah
    int ois_gyro_gain_x_source = 0x607A;
    int ois_gyro_gain_x_source1 = 0x607B;
    regdata = 0x17;
    regdata1 = 0x76;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_gyro_gain_x_source, regdata) > 0) {
         printf("set: [OIS Gyro gain X] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_gyro_gain_x_source, regdata);
    }
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_gyro_gain_x_source1, regdata1) > 0) {
         printf("set: [OIS Gyro gain X] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_gyro_gain_x_source1, regdata1);
    }

    // OIS Gyro gain Y DataWidthDetai:16bit EEPROM:1DD8h Source:607Ch
    int ois_gyro_gain_y_source = 0x607C;
    int ois_gyro_gain_y_source1 = 0x607D;
    regdata = 0x18;
    regdata1 = 0x27;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_gyro_gain_y_source, regdata) > 0) {
         printf("set: [OIS Gyro gain Y] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_gyro_gain_y_source, regdata);
    }
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_gyro_gain_y_source1, regdata1) > 0) {
         printf("set: [OIS Gyro gain Y] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_gyro_gain_y_source1, regdata1);
    }

    // OIS Gyro offset X DataWidthDetai:16bit EEPROM:1DDAh Source:6076h
    int ois_gyro_off_x_source = 0x6076;
    int ois_gyro_off_x_source1 = 0x6077;
    regdata = 0x29;
    regdata1 = 0x24;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_gyro_off_x_source, regdata) > 0) {
         printf("set: [OIS Gyro offset X] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_gyro_off_x_source, regdata);
    }
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_gyro_off_x_source1, regdata1) > 0) {
         printf("set: [OIS Gyro offset X] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_gyro_off_x_source1, regdata1);
    }

    // OIS Gyro offset Y DataWidthDetai:16bit EEPROM:1DDCh Source:6078h
    int ois_gyro_off_y_source = 0x6078;
    int ois_gyro_off_y_source1 = 0x6079;
    regdata = 0xD6;
    regdata1 = 0xDC;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_gyro_off_y_source, regdata) > 0) {
         printf("set: [OIS Gyro offset Y] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_gyro_off_y_source, regdata);
    }
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_gyro_off_y_source1, regdata1) > 0) {
         printf("set: [OIS Gyro offset Y] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_gyro_off_y_source1, regdata1);
    }

    // CLAF Hall current DataWidthDetai:8bit unsigned EEPROM:1DE7h Source:610Ah
    int ois_claf_hall_cur_source = 0x610A;
    regdata = 0x4E;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_claf_hall_cur_source, regdata) > 0) {
         printf("set: [CLAF Hall current] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_claf_hall_cur_source, regdata);
    }

    // CLAF Hall offset DataWidthDetai:10bit EEPROM:1DE8h Source:60E0h
    int ois_claf_hall_off_source = 0x60E0;
    int ois_claf_hall_off_source1 = 0x60E1;
    regdata = 0x01;
    regdata1 = 0x8D;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_claf_hall_off_source, regdata) > 0) {
         printf("set: [CLAF Hall offset] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_claf_hall_off_source, regdata);
    }
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_claf_hall_off_source1, regdata1) > 0) {
         printf("set: [CLAF Hall offset] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_claf_hall_off_source1, regdata1);
    }

    // CLAF Servo loop gain DataWidthDetai:16bit EEPROM:1DEAh Source:60E2h
    int ois_claf_servo_gain_source = 0x60E2;
    int ois_claf_servo_gain_source1 = 0x60E3;
    regdata = 0x00;
    regdata1 = 0x00;
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_claf_servo_gain_source, regdata) > 0) {
         printf("set: [CLAF Servo loop gain] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_claf_servo_gain_source, regdata);
    }
    if (__CCIRegWriteBySlaveAddress(slave_address, ois_claf_servo_gain_source1, regdata1) > 0) {
         printf("set: [CLAF Servo loop gain] slave=%02x, address=%04x, RegData = %02x\n", slave_address, ois_claf_servo_gain_source1, regdata1);
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
void OIS_Mode(int mode)
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
        unsigned char mode_regdata = mode;

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
 * @brief
 *
 * @param
 *
 * @return
 ******************************************************************************/
int RegWriteBurst(int slave_address, const T_I2CRegWriteData *I2CRegWriteData)
{
    unsigned char data[BURST_SIZE];
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
 * @brief	Read calibration data from EEPROM
 *
 * @param	CalibData	Calibration data
 *
 * @return	Read bytes
 ******************************************************************************/
int ReadCalibFromEEPROM(unsigned char *CalibData)
{
    int readBytes = 0;
    int gyroGainX, gyroGainY;
    unsigned char data[CALIB_DATA_NUM];
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
