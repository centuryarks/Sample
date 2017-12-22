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

#ifndef __LSC_CONTROL_H__
#define __LSC_CONTROL_H__

#include "slave_address.h"


/***************************************************************
 *  Defines for LSC Control
 **************************************************************/
#define FOCAL_LENGTH        (4.52)
#define PIXEL_SIZE          (0.00155)
#define LENS_FACTOR         (4.0)

/***************************************************************
 *  Extern function for LSC
 **************************************************************/
extern int __CCIRegReadBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data);
extern int __CCIRegWriteBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char data);
extern int __CCIRegReadMBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data, int size);
extern int __CCIRegWriteMBySlaveAddress(int CCISlaveAddress, int RegAddress, unsigned char *data, int size);

/***************************************************************
 *  Declare function for LSC Control
 **************************************************************/
void SetLSC(int enable);

#endif
