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
pthread_t still_thread;
int run_afc_thread;
int run_still_thread;
int abort_afc_thread;
int g_div_x;
int g_div_y;
int g_width;
int g_height;
int g_sensor_id;
int g_fps;
int g_gain;
int g_exposure;
char PathPreview[PATH_MAX]      = "/home/pi/demo/script/preview.sh";
char PathStill12M_NML[PATH_MAX] = "/home/pi/demo/script/stillCapture12M_Normal.sh";
char PathStill12M_HDR[PATH_MAX] = "/home/pi/demo/script/stillCapture12M_HDR.sh";
char PathHighspeed[PATH_MAX]    = "/home/pi/demo/script/highspeed.sh";
int StillCaptureFrame = 10;


/***************************************************************
 *  Property
 **************************************************************/
int GetExposure() { return g_exposure;  }
int GetGain()     { return g_gain;      }

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

    char command[COMMAND_MAX];

    g_sensor_id = GetSensID();

    i2c_open(g_sensor_id);

    g_width  = GetWidth();
    g_height = GetHeight();
    g_div_x  = GetPDAFWidth();
    g_div_y  = GetPDAFHeight();
    g_fps    = GetFPS();

    ReadSettingFile();

    sprintf(command, "%s %d %d &", PathPreview, g_gain, g_exposure);
    system(command);
    WaitStreaming();

    printf("Demo Start\n");
    return 1;
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
    MessageQueueSend("q");

    usleep(1000 * 1000);

    StopStreaming();

    WriteSettingFile();

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
    int ret;
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
        printf("af on\n");
    }
    else if (strcmp(commandLine, "af 0") == 0)
    {
        run_afc_thread = 0;
        while (abort_afc_thread)
        {
            usleep(500 * 1000);
        }
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
    else if (strstr(commandLine, "exposure/gain") != 0)
    {
        sscanf(commandLine, "%s %d %d", command, &g_exposure, &g_gain);

        printf("Exposure:%d, Gain:%d\n", g_exposure, g_gain);

        MessageQueueSend("q");
        usleep(1000 * 1000);

        sprintf(command, "%s %d %d &", PathPreview, g_gain, g_exposure);
        system(command);
        WaitStreaming();
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
                        if (strcmp(itemName, "preview") == 0)
                        {
                            strcpy(PathPreview, p);
                        }
                        else if (strcmp(itemName, "stillCapture12M_Normal") == 0)
                        {
                            strcpy(PathStill12M_NML, p);
                        }
                        else if (strcmp(itemName, "stillCapture12M_HDR") == 0)
                        {
                            strcpy(PathStill12M_HDR, p);
                        }
                        else if (strcmp(itemName, "stillCaptureFrame") == 0)
                        {
                            StillCaptureFrame = atoi(p);
                        }
                        else if (strcmp(itemName, "gain") == 0)
                        {
                            g_gain = atoi(p);
                        }
                        else if (strcmp(itemName, "exposure") == 0)
                        {
                            g_exposure = atoi(p);
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

    if ((fp = fopen("./bin/demo.ini", "r")) != NULL) {
        while ( fgets(&buf[LINEBUF_MAX * lineCount], LINEBUF_MAX, fp) != NULL ) {
            lineCount++;
        }
        fclose(fp);
    }

    if ((fp = fopen("./bin/demo.ini", "w")) != NULL) {
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
                        if (strcmp(itemName, "exposure") == 0)
                        {
                            sprintf(&buf[LINEBUF_MAX * i], "exposure=%d\n", g_exposure);
                            printf("Exposure:%d\n", g_exposure);
                        }
                        else if (strcmp(itemName, "gain") == 0)
                        {
                            sprintf(&buf[LINEBUF_MAX * i], "gain=%d\n", g_gain);
                            printf("Gain:%d\n", g_gain);
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
 * @brief   Still image capturing
 *
 * @param   mode    Mode0:Normal, Mode1:HDR
 *
 * @return  Success 1
 ******************************************************************************/
int StillCapture(int mode)
{
    char command[COMMAND_MAX];
    int ret;

    MessageQueueSend("q");
    usleep(1000 * 1000);

    ret = pthread_create(&still_thread, NULL, StillThread, &mode);
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

    while (run_still_thread)
    {
        usleep(500 * 1000);
    }
    printf("Finish StillCapture\n");

finish:
    sprintf(command, "lxterminal --command=\"%s %d %d\"", PathPreview, g_gain, g_exposure);
    //sprintf(command, "lxterminal --command=%s", PathPreview);
    system(command);
    WaitStreaming();
    printf("Focus Position:%d\n", GetAfPosition());
    usleep(FOCUS_WAIT * 1000);
    DirectMode();
    usleep(FOCUS_WAIT * 1000);
    DirectMove(GetAfPosition());

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
    mqd_t mqd;
    char buf[COMMAND_MAX] = { "" };

    mqd = mq_open("/mq_demo", O_WRONLY);
    if (mqd == -1)
    {
        perror("mq_open");
        return -1;
    }

    strcpy(buf, message);

    mq_send(mqd, buf, strlen(buf), 10);
    mq_close(mqd);

    return 1;
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
    printf("AFControl PD:%dx%d\n", g_div_x, g_div_y);

    ReadAFSettingFile();

    DirectMode();
    DirectMove(0x200);

    while (run_afc_thread)
    {
        AFControl(g_div_x, g_div_y);
    }

    abort_afc_thread = 0;
    pthread_exit(NULL);
}

/*******************************************************************************
 * @brief   Still image capturing thread
 *
 * @param   arg     Argument
 *
 * @return  void
 ******************************************************************************/
void *StillThread(void *arg)
{
    printf("StillThread start\n");

    char command[COMMAND_MAX];
    int mode = *(int*)arg;

    run_still_thread = 1;

    switch (mode)
    {
        case 0:
            sprintf(command, "%s %d %d %d", PathStill12M_NML, StillCaptureFrame, g_gain, g_exposure);
            system(command);
            break;
        case 1:
            sprintf(command, "%s %d", PathStill12M_HDR, StillCaptureFrame);
            system(command);
            break;
        default:
            break;
    }

    run_still_thread = 0;
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
