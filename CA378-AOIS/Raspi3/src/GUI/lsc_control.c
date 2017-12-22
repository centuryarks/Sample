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
#include <unistd.h>
#include <math.h>
#include "lsc_control.h"


/*******************************************************************************
 * @brief   Set Lens Shading Control (LSC)
 *
 * @param   enable  Enable LSC
 *
 * @return  void
 ******************************************************************************/
void SetLSC(int enable)
{
    unsigned char writeData[0x2];
    int LscTable[48];
    int addr;
    int x, y, i, color;
    int index = 0;
    double dis, theta, max, result;
    double cx = 3.5;
    double cy = 2.5;
    double knotPitch = 512;

    if (enable)
    {
        // calculate cos4 law
        dis = 0.5 * knotPitch * PIXEL_SIZE;
        theta = cos(atan(dis / FOCAL_LENGTH));
        max = pow(theta, LENS_FACTOR);
        for (y = 0; y < 6; y++)
        {
            for (x = 0; x < 8; x++)
            {
                dis = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) * knotPitch * PIXEL_SIZE;
                theta = cos(atan(dis / FOCAL_LENGTH));
                result = max / pow(theta, LENS_FACTOR);
                LscTable[index++] = (int)(result * 256);
            }
        }

        // Write LSC Table
        for (color = 0; color < 4; color++)
        {
            for (i = 0; i < 48; i++)
            {
                writeData[0] = (LscTable[i] >> 8) & 0x3;
                writeData[1] = LscTable[i] & 0xFF;
                addr = 0xab00 + (color * 48 + i) * 2;
                if (__CCIRegWriteMBySlaveAddress(CCI_SLAVE_ADDR, addr, writeData, 2) != 2)
                {
                    printf("error!\n");
                }
            }
        }

        // Shading Correction Enable
        addr = 0x0b00;
        writeData[0] = 1;
        if (__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, addr, writeData[0]) != 1)
        {
            printf("error!\n");
        }

        // Manual mode
        addr = 0x3804;
        writeData[0] = 3;
        if (__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, addr, writeData[0]) != 1)
        {
            printf("error!\n");
        }
    }
    else
    {
        // Shading Correction Disable
        addr = 0x0b00;
        writeData[0] = 0;
        if (__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, addr, writeData[0]) != 1)
        {
            printf("error!\n");
        }
    }
}
