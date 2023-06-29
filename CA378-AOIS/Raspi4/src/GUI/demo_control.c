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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <linux/limits.h>
#include "demo_control.h"

/***************************************************************
 *  Global variable for DEMO Control
 **************************************************************/
pthread_t afc_thread;
pthread_t capture_thread;
int run_afc_thread;
int run_capture_thread;
int abort_afc_thread;
int g_div_x;
int g_div_y;
int g_width;
int g_height;
int g_sensor_id;
int g_fps;
double g_gain;
int g_shutter;
int g_aec_agc;
int g_raw;
char PathPreview[DEMO_PATH_MAX]     = "/home/pi/demo/script/preview.sh";
char PathStill[DEMO_PATH_MAX]       = "/home/pi/demo/script/stillCapture.sh";
char PathIniFile[DEMO_PATH_MAX]     = "./bin/demo.ini";
char TuningFile[DEMO_PATH_MAX]      = "/usr/share/libcamera/ipa/raspberrypi/imx378.json";
int g_focus_control_mode = 0;
int g_use_center_4points_of_pd_data = 0;
double g_sharpness = 1.0;
double g_contrast = 1.0;
double g_brightness = 0.0;
double g_saturation = 1.0;
double g_ev_compensation = 0.0;
double g_red_gain = 1.0;
double g_blue_gain = 1.0;
int g_metering_mode = 0;
int g_denoise_mode = 0;
int g_awb_mode = 0;

/***************************************************************
 *  Property
 **************************************************************/
int GetShutter() { return g_shutter;  }
double GetGain()  { return g_gain;      }
int GetFocusPosition() { return GetAfPosition(); }
double GetSharpness()  { return g_sharpness; }
double GetContrast() { return g_contrast; }
double GetBrightness() { return g_brightness; }
double GetSaturation() { return g_saturation; }
double GetEvCompensation() { return g_ev_compensation; }
double GetRedGain() { return g_red_gain; }
double GetBlueGain() { return g_blue_gain; }

/*******************************************************************************
 * @brief   Initialize Demo software
 *
 * @param   void
 *
 * @return  Success 1
 ******************************************************************************/
int DemoInit()
{
    printf("Demo Initialize\n");

    int ret;

    g_sensor_id = GetSensID();

    i2c_open(g_sensor_id);

    g_width  = GetWidth();
    g_height = GetHeight();
    g_div_x  = GetPDAFWidth();
    g_div_y  = GetPDAFHeight();
    g_fps    = GetFPS();

    ReadSettingFile();

    g_aec_agc = 1;  // auto
    g_raw = 0;  // do not save raw file

    SetFocusControlMode(g_focus_control_mode, g_use_center_4points_of_pd_data);

    ret = RestartPreview(0);
    printf("Demo Start\n");
    return ret;
}

/*******************************************************************************
 * @brief   Exit Demo software
 *
 * @param   void
 *
 * @return  Success 1
 ******************************************************************************/
int DemoExit()
{
    StopAFCThread();

    MessageQueueSend("quit");

    WriteSettingFile();

    i2c_close(g_sensor_id);

    return 1;
}

/*******************************************************************************
 * @brief   Control Demo software
 *
 * @param   commandLine     Command line
 *
 * @return  Success 1, Failure -1
 ******************************************************************************/
int DemoControl(char *commandLine)
{
    char command[COMMAND_MAX];
    int mode = 0;
    int afPosition = 0x200;

    if (strcmp(commandLine, "lsc 1") == 0)
    {
        SetLSC(1);
        printf("lsc on\n");
    }
    else if (strcmp(commandLine, "lsc 0") == 0)
    {
        SetLSC(0);
        printf("lsc off\n");
    }
    else if (strstr(commandLine, "af 1") != 0)
    {
        SetFocusMode(1);
        printf("af on\n");
    }
    else if (strcmp(commandLine, "af 0") == 0)
    {
        SetFocusMode(0);
        printf("af off\n");
    }
    else if (strstr(commandLine, "focus") != 0)
    {
        sscanf(commandLine, "%s %d", command, &afPosition);

        printf("Focus Position:%d\n", afPosition);
        if (afPosition > 0x03ff || afPosition < 0)
        {
            printf("write: invalid data\n");
        }
        else
        {
            usleep(FOCUS_WAIT * 1000);
            DirectMode();
            usleep(FOCUS_WAIT * 1000);
            DirectMove(afPosition);
        }
    }
    else if (strstr(commandLine, "ois") != 0)
    {
        sscanf(commandLine, "%s %d", command, &mode);

        OIS_Mode(mode);
    }
    else if (strstr(commandLine, "shutter/gain") != 0)
    {
        sscanf(commandLine, "%s %d %lf", command, &g_shutter, &g_gain);
        printf("Shutter:%d, Gain:%2.1f\n", g_shutter, g_gain);

        RestartPreview(1);
    }
    else if (strstr(commandLine, "aec/agc") != 0)
    {
        sscanf(commandLine, "%s %d", command, &g_aec_agc);

        RestartPreview(1);
    }
    else if (strstr(commandLine, "raw") != 0)
    {
        sscanf(commandLine, "%s %d", command, &g_raw);
    }
    else if (strstr(commandLine, "metering") != 0)
    {
        sscanf(commandLine, "%s %d", command, &g_metering_mode);
    }
    else if (strstr(commandLine, "cameracontrol") != 0)
    {
        sscanf(commandLine, "%s %lf %lf %lf %lf %lf %d %d %lf %lf", 
            command,
            &g_sharpness,
            &g_contrast,
            &g_brightness,
            &g_saturation,
            &g_ev_compensation,
            &g_denoise_mode,
            &g_awb_mode, &g_red_gain, &g_blue_gain);

        RestartPreview(1);
    }
    else
    {
        printf("bad command!\n");
        return -1;
    }

    return 1;
}

/*******************************************************************************
 * @brief   Read setting file
 *
 * @param   void
 *
 * @return  Success 1
 ******************************************************************************/
int ReadSettingFile()
{
    FILE *fp;
    char buf[LINEBUF_MAX];
    char itemName[LINEBUF_MAX];

    if ((fp = fopen(PathIniFile, "r")) != NULL) {
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
                        if (strcmp(itemName, "preview") == 0)
                        {
                            strcpy(PathPreview, p);
                        }
                        else if (strcmp(itemName, "stillCapture") == 0)
                        {
                            strcpy(PathStill, p);
                        }
                        else if (strcmp(itemName, "gain") == 0)
                        {
                            g_gain = atof(p);
                        }
                        else if (strcmp(itemName, "shutter") == 0)
                        {
                            g_shutter = atoi(p);
                        }
                        else if (strcmp(itemName, "FocusControlMode") == 0)
                        {
                            g_focus_control_mode = atoi(p);
                            printf("Focus Control Mode:%d\n", g_focus_control_mode);
                        }
                        else if (strcmp(itemName, "UseCenter4PointsOfPDdata") == 0)
                        {
                            g_use_center_4points_of_pd_data = atoi(p);
                            printf("Use Center 4 Points Of PD data:%d\n", g_use_center_4points_of_pd_data);
                        }
                        else if (strcmp(itemName, "tuning-file") == 0)
                        {
                            strcpy(TuningFile, p);
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

/*******************************************************************************
 * @brief   Write setting file
 *
 * @param   void
 *
 * @return  Success 1
 ******************************************************************************/
int WriteSettingFile()
{
    FILE *fp;
    char buf[LINEBUF_MAX*LINEBUF_MAX];
    char lineBuf[LINEBUF_MAX];
    char itemName[LINEBUF_MAX];
    int i, lineCount = 0;

    printf("WriteSettingFile\n");

    if ((fp = fopen(PathIniFile, "r")) != NULL) {
        while ( fgets(&buf[LINEBUF_MAX * lineCount], LINEBUF_MAX, fp) != NULL ) {
            lineCount++;
        }
        fclose(fp);
    }

    if ((fp = fopen(PathIniFile, "w")) != NULL) {
        for (i = 0; i < lineCount; i++) {
            strcpy(lineBuf, &buf[LINEBUF_MAX * i]);
            char *p = strtok(lineBuf, "= \r\n");
            int n = 0;
            while (p)
            {
                switch (n)
                {
                    case 0:
                        strcpy(itemName, p);
                        break;
                    case 1:
                        if (strcmp(itemName, "shutter") == 0)
                        {
                            sprintf(&buf[LINEBUF_MAX * i], "shutter=%d\n", g_shutter);
                            printf("Shutter:%d\n", g_shutter);
                        }
                        else if (strcmp(itemName, "gain") == 0)
                        {
                            sprintf(&buf[LINEBUF_MAX * i], "gain=%2.1f\n", g_gain);
                            printf("Gain:%2.1f\n", g_gain);
                        }
                        break;
                    default:
                        break;
                }
                n++;
                p = strtok(NULL, "= \r\n");
            }
            fprintf(fp, "%s", &buf[LINEBUF_MAX * i]);
        }
        fclose(fp);
    }

    return 1;
}

/*******************************************************************************
 * @brief   image capturing
 *
 * @param   mode    Mode0:Still
 *
 * @return  Success 1
 ******************************************************************************/
int Capture(int mode)
{
    int ret;

    StopAFCThread();
    MessageQueueSend("quit");
    usleep(1000 * 1000);

    ret = pthread_create(&capture_thread, NULL, CaptureThread, &mode);
    if (ret)
    {
        printf("ERROR; return code from pthread_create() is %d\n", ret);
        goto finish;
    }

    WaitStreaming();

    printf("Focus Position:%d\n", GetAfPosition());
    usleep(FOCUS_WAIT * 1000);
    DirectMode();
    usleep(FOCUS_WAIT * 1000);
    DirectMove(GetAfPosition());

    while (run_capture_thread)
    {
        usleep(500 * 1000);
    }
    printf("Finish Capture\n");

finish:
    ret = RestartPreview(0);

    return ret;
}

/*******************************************************************************
 * @brief   Restart Preview
 *
 * @param   stop    stop preview
 *
 * @return  Success 1
 ******************************************************************************/
int RestartPreview(int stop)
{
    char command[COMMAND_MAX];
    char options[OPTIONBUF_MAX];
    int ret;

    if (stop)
    {
        StopAFCThread();
        MessageQueueSend("quit");
        usleep(1000 * 1000);
    }

    GenerateOptions(options, OPTIONBUF_MAX, 0);
    snprintf(command, COMMAND_MAX, "%s %d %d %d %s '%s' &", PathPreview, g_width, g_height, g_fps, TuningFile, options);
    ret = system(command);
    if (ret != 0)
    {
        printf("[Error]: %s (ret=%d)\n", command, WEXITSTATUS(ret));
    }

    WaitStreaming();

    printf("Focus Position:%d\n", GetAfPosition());
    usleep(FOCUS_WAIT * 1000);
    DirectMode();
    usleep(FOCUS_WAIT * 1000);
    DirectMove(GetAfPosition());

    ret = StartAFCThread();
    if (ret == -1) return 0;

    return 1;
}

/*******************************************************************************
 * @brief   Send queue message
 *
 * @param   message     Input message
 *
 * @return  Success 1
 ******************************************************************************/
int MessageQueueSend(char *message)
{
    char command[COMMAND_MAX];
    int ret;

    if (strcmp(message, "quit") == 0)
    {
        snprintf(command, COMMAND_MAX, "pgrep -f 'libcamera-hello' | xargs sudo kill -9");
        ret = system(command);
        if (ret != 9)
        {
            printf("[error]fail to kill libcamera-hello (ret=%d)\n", ret);
        }
    }

    return 1;
}

/*******************************************************************************
 * @brief   Start AF control thread
 *
 * @param   void
 *
 * @return  Success 1, Failure -1
 ******************************************************************************/
int StartAFCThread()
{
    printf("StartAFCThread\n");
    int ret;

    SetPDAF(g_width, g_height, g_div_x, g_div_y);

    usleep(FOCUS_WAIT * 1000);
    DirectMode();
    usleep(FOCUS_WAIT * 1000);

    run_afc_thread = 1;
    ret = pthread_create(&afc_thread, NULL, AFCThread, NULL);
    if (ret)
    {
        printf("ERROR; return code from pthread_create() is %d\n", ret);
        return -1;
    }

    return 1;
}

/*******************************************************************************
 * @brief   Stop AF control thread
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void StopAFCThread()
{
    printf("StopAFCThread\n");
    abort_afc_thread = 1;
    run_afc_thread = 0;
    while (abort_afc_thread)
    {
        usleep(500 * 1000);
    }
}

/*******************************************************************************
 * @brief   AF control thread
 *
 * @param   arg     Argument
 *
 * @return  void
 ******************************************************************************/
void *AFCThread(void *arg)
{
	UNUSED(arg);
    printf("AFControl PD:%dx%d\n", g_div_x, g_div_y);

    ReadAFSettingFile();

    while (run_afc_thread)
    {
        AFControl(g_div_x, g_div_y);
    }

    abort_afc_thread = 0;
    pthread_exit(NULL);
}

/*******************************************************************************
 * @brief   Capturing thread
 *
 * @param   arg     Argument
 *
 * @return  void
 ******************************************************************************/
void *CaptureThread(void *arg)
{
    printf("CaptureThread start\n");

    char command[COMMAND_MAX];
    char options[OPTIONBUF_MAX];
    int mode = *(int*)arg;

    run_capture_thread = 1;

    switch (mode)
    {
        case 0:
            GenerateOptions(options, OPTIONBUF_MAX, 1);
            snprintf(command, COMMAND_MAX, "%s %d %d %s '%s'", PathStill, g_width, g_height, TuningFile, options);
            system(command);
            break;
        default:
            break;
    }

    run_capture_thread = 0;
    pthread_exit(NULL);
}

/*******************************************************************************
 * @brief   WaitStreaming
 *
 * @param   void
 *
 * @return  Success 1
 ******************************************************************************/
int WaitStreaming()
{
    printf("Wait for streaming\n");

    int streamingInit = 0;

    while (1)
    {
        if (GetStreaming() == 1)
        {
            if (streamingInit == 1)
            {
                break;
            }
        }
        else
        {
            streamingInit = 1;
        }
        usleep(100 * 1000);
    }

    OIS_Init();

    return 1;
}

/*******************************************************************************
 * @brief   Generate option sting
 *
 * @param   options     Option sting
 * @param   maxlength   Option sting max length
 * @param   still       0: preview / 1: still
 *
 * @return  void
 ******************************************************************************/
void GenerateOptions(char *options, int maxlength, int still)
{
    int len = 0;

    memset(options, 0, maxlength);

    if (still)
    {
        if (g_raw != 0)
        {
            len += snprintf(options + len, maxlength - len, " --raw");
        }
    }

    if (g_aec_agc == 0)
    {
        len += snprintf(options + len, maxlength - len, " --shutter %d", g_shutter);
        len += snprintf(options + len, maxlength - len, " --gain %2.1f", g_gain);
    }

    const char* METERING_MODE[] = {
        "centre",
        "spot",
        "average",
    };
    int metering_mode_num = sizeof(METERING_MODE) / sizeof(char*);
    if (g_metering_mode < metering_mode_num)
    {
        len += snprintf(options + len, maxlength - len, " --metering %s", METERING_MODE[g_metering_mode]);
    }

    len += snprintf(options + len, maxlength - len, " --sharpness %2.1f", g_sharpness);
    len += snprintf(options + len, maxlength - len, " --contrast %2.1f", g_contrast);
    len += snprintf(options + len, maxlength - len, " --brightness %2.1f", g_brightness);
    len += snprintf(options + len, maxlength - len, " --saturation %2.1f", g_saturation);
    len += snprintf(options + len, maxlength - len, " --ev %2.1f", g_ev_compensation);

    const char* AWB_MODE[] = {
        "auto",
        "incandescent",
        "tungsten",
        "fluorescent",
        "indoor",
        "daylight",
        "cloudy",
    };
    int awb_mode_num = sizeof(AWB_MODE) / sizeof(char*);
    if (g_awb_mode < awb_mode_num)
    {
        len += snprintf(options + len, maxlength - len, " --awb %s", AWB_MODE[g_awb_mode]);
    }
    else
    {
        len += snprintf(options + len, maxlength - len, " --awbgains %2.1f,%2.1f", g_red_gain, g_blue_gain);
    }

    const char* DENOISE_MODE[] = {
        "auto",
        "off",
        "cdn_off",
        "cdn_fast",
        "cdn_hq",
    };
    int denoise_mode_num = sizeof(DENOISE_MODE) / sizeof(char*);
    if (g_denoise_mode < denoise_mode_num)
    {
        len += snprintf(options + len, maxlength - len, " --denoise %s", DENOISE_MODE[g_denoise_mode]);
    }
}