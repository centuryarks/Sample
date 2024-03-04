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

#ifndef __AF_CONTROL_H__
#define __AF_CONTROL_H__

#include "slave_address.h"
#include "types_util.h"

/***************************************************************
 *  Defines for AF Control
 **************************************************************/
#define LINEBUF_MAX (256)
#define PDAF_WMAX   (16)
#define PDAF_HMAX   (12)
#define PDAF_PMAX   (PDAF_WMAX * PDAF_HMAX)
#define AREA_RATEX  (80)
#define AREA_RATEY  (80)

enum E_AF_DRIVER_ID
{
    AF_DRIVER_NONE = 0,
    AF_DRIVER_V1,
    AF_DRIVER_V2
};

typedef void (*DIRECT_MODE_FUNC)(void);
typedef void (*DIRECT_MOVE_FUNC)(int position);

/***************************************************************
 *  Extern function for I2C
 **************************************************************/
extern int __CCIRegReadBySlaveAddress(int CCISlaveAddress, int RegAddress, u8 *data);
extern int __CCIRegWriteBySlaveAddress(int CCISlaveAddress, int RegAddress, u8 data);
extern int __CCIRegReadMBySlaveAddress(int CCISlaveAddress, int RegAddress, u8 *data, int size);
extern int __CCIRegWriteMBySlaveAddress(int CCISlaveAddress, int RegAddress, u8 *data, int size);
extern int __CCIRegRead16bit(u16 addr, u16* data);
extern int __CCIRegWrite16bit(u16 addr, u16 data);

/***************************************************************
 *  Declare function for AF Control
 **************************************************************/
int GetI2cDev();
int GetSensID();
int GetWidth();
int GetHeight();
int GetFPS();
int GetPDAFWidth();
int GetPDAFHeight();
int GetAfPosition();
void SetParam(int argc, char *argv[]);
void AFControl(int pdafWidth, int pdafHeight);
void DirectModeV1();
void DirectModeV2();
void DirectMove(int position);
void DirectMoveV1(int position);
void DirectMoveV2(int position);
void SetPDAF(int width, int height, int div_x, int div_y);
double Q6_4(int data);
double CalcDCC(volatile u8 * data, int pdafWidth, int pdafHeight);
int ReadAFSettingFile();
void SetFocusControlMode(int mode, int useCenter4points);
void SetAfDriverId(int afDriver);
void SetFocusMode(int mode);
int GetStreaming();
void SetSensorSlaveId(int sensorSlaveId);
void SetRotation(int rotationIndex);

#endif
