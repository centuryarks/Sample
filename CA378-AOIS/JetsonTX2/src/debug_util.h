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

#ifndef __DEBUG_UTIL_H__
#define __DEBUG_UTIL_H__

#ifdef DEBUG_ENABLE

#ifdef DEBUG_LINUX      /*Debug for Linux*/
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#define DEBUG_PRINTF(lable,...) \
do{\
    {\
        struct tm *time_hms;\
        struct timeval time_ms;\
        gettimeofday(&time_ms,NULL);\
        time_hms = localtime(&(time_ms.tv_sec));\
        printf("[%s][%d:%d:%d.%d][%s]",\
                          lable,\
                          time_hms->tm_hour,\
                          time_hms->tm_min,\
                          time_hms->tm_sec,\
                          time_ms.tv_usec,\
                          __func__);\
        printf(__VA_ARGS__);\
    }\
}while(0)

#define DEBUG_INFO(...) DEBUG_PRINTF("INFO",__VA_ARGS__)
#define DEBUG_ERROR(...) DEBUG_PRINTF("ERROR",__VA_ARGS__)
#define DEBUG_FUNC_RETURN(ret) DEBUG_INFO("Return : %d\n",ret)

#elif defined DEBUG_ARDUINO
#include <Arduino.h>
#include <stdio.h>
#define DEBUG_PRINTF(lable,...) \
do{\
    {\
        char buf[256] = {0};\
        unsigned long time_millis;\
        unsigned int time_seconds,time_minutes,time_hours;\
        time_millis = millis();\
        time_seconds = (time_millis / 1000) % 60;\
        time_minutes = (time_millis / 60000) % 60;\
        time_hours = (time_millis / 3600000) % 24;\
        Serial.print("[");\
        Serial.print(lable);\
        Serial.print("]");\
        Serial.print("[");\
        Serial.print(time_hours,DEC);\
        Serial.print(":");\
        Serial.print(time_minutes,DEC);\
        Serial.print(":");\
        Serial.print(time_seconds,DEC);\
        Serial.print(".");\
        Serial.print(time_millis%1000,DEC);\
        Serial.print("]");\
        Serial.print("[");\
        Serial.print(__func__);\
        Serial.print("]");\
        sprintf(buf,__VA_ARGS__);\
        Serial.println(buf);\
    }\
}while(0)

#define DEBUG_INFO(...) DEBUG_PRINTF("INFO",__VA_ARGS__)
#define DEBUG_ERROR(...) DEBUG_PRINTF("ERROR",__VA_ARGS__)
#define DEBUG_FUNC_RETURN(ret) DEBUG_INFO("Return : %d\n",ret)

#elif defined DEBUG_BLE
#include <stdio.h>
#include "ble_dtm.h"
#define DEBUG_PRINTF(lable,...) \
do{\
    {\
        char buf[256] = {0};\
        uint32_t current_time;\
        uint32_t time_milisec,time_seconds,time_minutes,time_hours;\
        current_time = dtm_wait();\
        time_milisec = current_time % 10;\
        time_seconds = (current_time / 10) % 60;\
        time_minutes = (current_time / 600) % 60;\
        time_hours   = (current_time / 36000) % 24;\
        printf("[%s][%02lu:%02lu:%02lu.%lu][%s]",\
                        lable,\
                        time_hours,\
                        time_minutes,\
                        time_seconds,\
                        time_milisec,\
                        __func__);\
                        printf(__VA_ARGS__);\
        nrf_delay_ms(100);\
    }\
}while(0)

#define DEBUG_INFO(...) DEBUG_PRINTF("INFO",__VA_ARGS__)
#define DEBUG_ERROR(...) DEBUG_PRINTF("ERROR",__VA_ARGS__)
#define DEBUG_FUNC_RETURN(ret) DEBUG_INFO("Return : %d\n",ret)

#endif /*#ifdef DEBUG_BLE*/

#else /*#ifdef DEBUG_ENABLE*/

#define DEBUG_INFO(...)
#define DEBUG_ERROR(...)
#define DEBUG_FUNC_RETURN(ret)

#endif /*#ifdef DEBUG_ENABLE*/

#endif /*#ifdef __DEBUG_H__*/
