/*
Copyright (c) 2022, CenturyArks
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
#include <wiringPi.h>
#include "gpio_control.h"

/***************************************************************
 *  Global variable for GPIO Control
 **************************************************************/
int trigger = 0;


/*******************************************************************************
 * @brief   Initialize GPIO
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void InitGPIO()
{
    if (wiringPiSetup() < 0)
    {
        printf("Error wiringPi setup.\n");
        return;
    }

    pinMode(TRIGGER_PIN, INPUT);
    trigger = 0;
}

/*******************************************************************************
 * @brief   V Sync Trigger
 *
 * @param   void
 *
 * @return  trigger
 ******************************************************************************/
int VSyncTrigger()
{
    /* VSync (hardware) */
    if (digitalRead(TRIGGER_PIN) == LOW) {
        if (trigger == 0) {
            trigger = 1;
        }
        else {
            return 0;
        }
    }
    else {
        if (trigger == 1) {
            trigger = 0;
            return 0;
        }
        else {
            return 0;
        }
    }
    
    return 1;
}
