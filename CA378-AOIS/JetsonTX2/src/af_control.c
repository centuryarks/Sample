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
#include <math.h>
#include "af_control.h"


/***************************************************************
 *  Global variable for AF Control
 **************************************************************/
int TableCount[PDAF_PMAX];
int TableCOF[PDAF_PMAX];
double TablePD[PDAF_PMAX];
int afPosition                   = 0x200;
int lastAfPosition               = 0x200;
int dccCount                     = 0;
double dccAve                    = 0.0;
double posAve                    = 0.0;
double AutoFocusGain             = 2.0;
int AutoFocusMoveLimit           = 100;
int AutoFocusConfidenceThreshold = 10;
int AutoFocusAverageNum          = 1;
int SENS_ID                      = 0;
int WIDTH                        = 4056;
int HEIGHT                       = 3040;
int FPS                          = 30;
int PDAF_WIDTH                   = 16;
int PDAF_HEIGHT                  = 12;


/***************************************************************
 *  Property function for AF Control
 **************************************************************/
int GetI2cDev()     { return SENS_ID;        }
int GetSensID()     { return SENS_ID;        }
int GetWidth()      { return WIDTH;          }
int GetHeight()     { return HEIGHT;         }
int GetFPS()        { return FPS;            }
int GetPDAFWidth()  { return PDAF_WIDTH;     }
int GetPDAFHeight() { return PDAF_HEIGHT;    }
int GetAfPosition() { return lastAfPosition; }

/*******************************************************************************
 * @brief   Set parameters
 *
 * @param   argc    Number of arguments
 * @param   argv    Array of arguments
 *
 * @return  void
 ******************************************************************************/
void SetParam(int argc, char *argv[])
{
    int i;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-id") == 0) {
            if (i+1 < argc) {
                SENS_ID = atoi(argv[i+1]);
                printf("sensor-id:%s\n", argv[i+1]);
            }
        } else if (strcmp(argv[i], "-w") == 0) {
            if (i+1 < argc) {
                WIDTH = atoi(argv[i+1]);
                printf("width:%s\n", argv[i+1]);
            }
        } else if (strcmp(argv[i], "-h") == 0) {
            if (i+1 < argc) {
                HEIGHT = atoi(argv[i+1]);
                printf("height:%s\n", argv[i+1]);
            }
        } else if (strcmp(argv[i], "-fps") == 0) {
            if (i+1 < argc) {
                FPS = atoi(argv[i+1]);
                printf("framerate:%s\n", argv[i+1]);
            }
        } else if (strcmp(argv[i], "-pdaf") == 0) {
            if (i+2 < argc) {
                PDAF_WIDTH = atoi(argv[i+1]);
                PDAF_HEIGHT = atoi(argv[i+2]);
                printf("PDAF Area: %d x %d\n", PDAF_WIDTH, PDAF_HEIGHT);
            }
        }
    }
}

/*******************************************************************************
 * @brief   Control AF
 *
 * @param   pdafWidth   PDAF area width
 * @param   pdafHeight  PDAF area height
 *
 * @return  void
 ******************************************************************************/
void AFControl(int pdafWidth, int pdafHeight)
{
    unsigned char readData[0x1000];
    unsigned char writeData;
    double dcc = 0.0;
    double cof = 0.0;
    int w, h;

    if (dccCount == 0)
    {
        for (h = 0; h < pdafHeight; h++)
        {
            for (w = 0; w < pdafWidth; w++)
            {
                TablePD[h * pdafWidth + w] = 0;
                TableCOF[h * pdafWidth + w] = 0;
                TableCount[h * pdafWidth + w] = 0;
            }
        }
    }

    writeData = 1;
    if (__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, 0xe213, writeData) != 1)
    {
        printf("error!\n");
    }

    if (__CCIRegReadMBySlaveAddress(CCI_SLAVE_ADDR, 0x7c08, readData, 0x300) != 0x300)
    {
        printf("get error(PDAF data(0x7C08))!\n");
    }

    writeData = 0;
    if (__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, 0xe213, writeData) != 1)
    {
        printf("error!\n");
    }

    dcc = CalcDCC(readData, pdafWidth, pdafHeight);
    if (dcc == 0)
    {
        return;
    }

    dccAve += dcc;
    posAve += afPosition;
    dccCount++;

    if (dccCount == AutoFocusAverageNum)
    {
        posAve /= dccCount;
        dccAve /= dccCount;

        printf("Phase Difference:\n");
        for (h = 0; h < pdafHeight; h++)
        {
            for (w = 0; w < pdafWidth; w++)
            {
                if (TableCount[h * pdafWidth + w] == 0)
                {
                    printf("%7.2lf", 0.0);
                }
                else
                {
                    printf("%7.2lf", TablePD[h * pdafWidth + w]  / TableCount[h * pdafWidth + w]);
                }
            }
            printf("\n");
        }

        printf("Confidence:\n");
        cof = 0.0;
        for (h = 0; h < pdafHeight; h++)
        {
            for (w = 0; w < pdafWidth; w++)
            {
                if (TableCount[h * pdafWidth + w] == 0)
                {
                    printf(" %6d", 0);
                }
                else
                {
                    printf(" %6d", TableCOF[h * pdafWidth + w] / TableCount[h * pdafWidth + w]);
                    cof += TableCOF[h * pdafWidth + w] / TableCount[h * pdafWidth + w];
                }
            }
            printf("\n");
        }
        cof /= (pdafWidth * pdafHeight);
        printf("Average:\r\n  Position:%4.lf, Phase Difference:%6.2lf, Confidence:%6.2lf\r\n", posAve, dccAve, cof);

        dcc = dccAve * AutoFocusGain;
        if (dcc >  AutoFocusMoveLimit) dcc =  AutoFocusMoveLimit;
        if (dcc < -AutoFocusMoveLimit) dcc = -AutoFocusMoveLimit;

        afPosition -= dcc;
        if (afPosition < 0x000) afPosition = 0x000;
        if (afPosition > 0x3FF) afPosition = 0x3FF;
        DirectMove(afPosition);

        dccAve = 0.0;
        posAve = 0.0;
        dccCount = 0;
    }

    usleep(10 * 1000);
}

/*******************************************************************************
 * @brief   Focus direct mode
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void DirectMode()
{
    int mode_address = 0x60D6;
    int set_regdata = 0x04;

    // Direct
    if (__CCIRegWriteBySlaveAddress(AFDRV_I2C_ADDR, mode_address, set_regdata) == 0)
    {
        printf("set: error(Direct(0x60D6))!\n");
    }
}

/*******************************************************************************
 * @brief   Focus direct moving
 *
 * @param   position    Focus target position
 *
 * @return  void
 ******************************************************************************/
void DirectMove(int position)
{
    unsigned char direct_regdata[2];
    int direct_address = 0x60E4;

    lastAfPosition = position;

    direct_regdata[0] = (unsigned char)((position >> 8) & 0xFF);
    direct_regdata[1] = (unsigned char)( position       & 0xFF);

    if (__CCIRegWriteMBySlaveAddress(AFDRV_I2C_ADDR, direct_address, direct_regdata, 2) != 2)
    {
        printf("set: error(FocusMode Direct(0x60E4))!\n");
    }
}

/*******************************************************************************
 * @brief   Sett PDAF
 *
 * @param   width   Image width
 * @param   height  Image height
 * @param   div_x   X division number
 * @param   div_y   Y division number
 *
 * @return  void
 ******************************************************************************/
void SetPDAF(int width, int height, int div_x, int div_y)
{
    unsigned char writeData[2];
    unsigned char regdata;
    int addr;
    int div_width  = (int)(width  * AREA_RATEX / 100 / (div_x + 0.5) + 0.5);
    int div_height = (int)(height * AREA_RATEY / 100 / (div_y + 0.5) + 0.5);
    int offset_x = (width  - div_width  * div_x) / 2;
    int offset_y = (height - div_height * div_y) / 2;

    printf("Area:%dx%d, PD:%dx%d\n", width, height, div_x, div_y);

    addr = 0x38a4;
    writeData[0] = offset_x >> 8;
    writeData[1] = offset_x & 0xFF;
    if(__CCIRegWriteMBySlaveAddress(CCI_SLAVE_ADDR, addr, writeData, 2) != 2)
    {
        printf("Error.(%04X)\n", addr);
    }

    addr = 0x38a6;
    writeData[0] = offset_y >> 8;
    writeData[1] = offset_y & 0xFF;
    if(__CCIRegWriteMBySlaveAddress(CCI_SLAVE_ADDR, addr, writeData, 2) != 2)
    {
        printf("Error.(%04X)\n", addr);
    }

    addr = 0x38a8;
    writeData[0] = div_width >> 8;
    writeData[1] = div_width & 0xFF;
    if(__CCIRegWriteMBySlaveAddress(CCI_SLAVE_ADDR, addr, writeData, 2) != 2)
    {
        printf("Error.(%04X)\n", addr);
    }

    addr = 0x38aa;
    writeData[0] = div_height >> 8;
    writeData[1] = div_height & 0xFF;
    if(__CCIRegWriteMBySlaveAddress(CCI_SLAVE_ADDR, addr, writeData, 2) != 2)
    {
        printf("Error.(%04X)\n", addr);
    }

    addr = 0x38a3;

    // Select AF detect area mode
    // 0: fixed area ( 16 x 12 ), 1: fixed area ( 8 x 6 ), 2: free area, 3: prohibited
    if (div_x == 16 && div_y == 12)
    {
        regdata = 0x0;
    }
    else if (div_x == 8 && div_y == 6)
    {
        regdata = 0x1;
    }
    else
    {
        regdata = 0x2;
    }

    if(__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, addr, regdata) == 0)
    {
        printf("Error.(%04X)\n", addr);
    }

    addr = 0x3e37;
    regdata = 0x1;
    if(__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, addr, regdata) == 0)
    {
        printf("Error.(%04X)\n", addr);
    }
}

/*******************************************************************************
 * @brief   Calculate Q6.4
 *
 * @param   data  Input data
 *
 * @return  Calculated value
 ******************************************************************************/
double Q6_4(int data)
{
    double val;

    if (data & 0x400)
    {
        val = -(0x800 - data) / 16.0;
    }
    else
    {
        val = data / 16.0;
    }

    return val;
}

/*******************************************************************************
* @brief    Calculate Defocus Conversion Coefficient (DCC)
 *
 * @param data          Input data
 * @param pdafWidth     PDAF area width
 * @param pdafHeight    PDAF area height
 *
 * @return              Result value
 ******************************************************************************/
double CalcDCC(volatile unsigned char * data, int pdafWidth, int pdafHeight)
{
    int h, w;
    int count = 0;
    double sum = 0.0;
    double pd = 0.0;

    for(h = 0; h < pdafHeight; h++)
    {
        int index = 0x40 * h;
        for(w = 0; w < pdafWidth; w++)
        {
            int i = index + w * 4;
            int d0 = data[i + 0];
            int d1 = data[i + 1];
            int d2 = data[i + 2];
            int cof = (d0 << 3) + (d1 >> 5);
            int pda = ((d1 & 0x1F) << 6) + (d2 >> 2);

            if (cof >= AutoFocusConfidenceThreshold)
            {
                pd = Q6_4(pda);
                TablePD[h * pdafWidth + w] += pd;
                TableCOF[h * pdafWidth + w] += cof;
                TableCount[h * pdafWidth + w]++;
                sum += pd;
                count++;
            }
        }
    }

    if (count == 0)
    {
        return 0;
    }
    else
    {
        return sum / count;
    }
}

/*******************************************************************************
 * @brief   Read the setting file
 *
 * @param   void
 *
 * @return  Success 1
 ******************************************************************************/
int ReadAFSettingFile()
{
    FILE *fp;
    char buf[LINEBUF_MAX];
    char itemName[LINEBUF_MAX];

    if ((fp = fopen("./bin/demo.ini", "r")) != NULL) {
        while ( fgets(buf, LINEBUF_MAX, fp) != NULL ) {

            char *p = strtok(buf, "= \r\n");
            int n = 0;
            while (p)
            {
                switch (n)
                {
                    case 0:
                        strcpy(itemName, p);
                        break;
                    case 1:
                        if (strcmp(itemName, "autoFocusGain") == 0)
                        {
                            AutoFocusGain = atof(p);
                        }
                        else if (strcmp(itemName, "autoFocusConfidenceThreshold") == 0)
                        {
                            AutoFocusConfidenceThreshold = atoi(p);
                        }
                        else if (strcmp(itemName, "autoFocusMoveLimit") == 0)
                        {
                            AutoFocusMoveLimit = atoi(p);
                        }
                        else if (strcmp(itemName, "AutoFocusAverageNum") == 0)
                        {
                            AutoFocusMoveLimit = atoi(p);
                        }
                        break;
                    default:
                        break;
                }
                n++;
                p = strtok(NULL, "= \r\n");
            }
        }

        fclose(fp);
    }

    return 1;
}
