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

#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "types_util.h"

#ifdef __cplusplus
extern "C" {
#endif

#define COMMUNICATION_SUCCESS           (0)
#define COMMUNICATION_ERROR             (1)
#define BUFFER_MAX                      (128)


/***************************************************************
 *  Defines for i2c communication
 **************************************************************/
typedef enum {
    I2C_DEV_0 = 0,
    I2C_DEV_MAX,
} i2c_dev_t;
s32 i2c_open(i2c_dev_t i2c_dev);
s32 i2c_close(i2c_dev_t i2c_dev);
s32 i2c_set_slave_force(i2c_dev_t i2c_dev, u16 slaveAddr);
s32 i2c_read(i2c_dev_t i2c_dev, u16 slaveAddr, u8* writeData, u16 wlen, u8* readData, u16 rlen);
s32 i2c_write(i2c_dev_t i2c_dev, u16 slaveAddr, u8* writeData, u16 len);
s32 i2c_exists(i2c_dev_t i2c_dev, u16 slaveAddr, u16 id, u8 *searchPattern);

/***************************************************************
 *  Delay function for communication
 **************************************************************/
void delay_msec(u32 ms);
void delay_usec(u32 us);

#ifdef __cplusplus
}
#endif

#endif
