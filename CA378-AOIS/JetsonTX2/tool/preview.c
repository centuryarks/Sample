/*
 * RAW raw seq file viewer.
 *
 * Copyright (c) 2018, CenturyArks All rights reserved.
 * Copyright (c) 2018, Stream Technology Corporation All rights reserved.
 * Copyright (c) 2018, Vision Processing Community All rights reserved.
 *                     https://www.visionproc.org/
 *
 * Based on:
 * - SSP library viewer sample program
 *   highspeed camera shooting program
 *   https://www.visionproc.org/download.php
 *   + highspeed_20161219.zip
 *     + highspeed.c
 *     + preview.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define HAVE_OPENCV_VIDEOIO
#include <opencv/cv.h>
#include <opencv/highgui.h>
// sudo apt-get install libopencv-dev
// sudo apt-get install libgl1-mesa-dri


#define FRAME_MODE_RECORDING            (0)
#define FRAME_MODE_PREVIEW              (1)
#define FRAME_MODE_REALTIMEPREVIEW      (2)

#define INFO_SHOW_ENABLE                (1)
#define INFO_SHOW_DISABLE               (0)

#define KEY_CMD_QUIT                    'q'
#define KEY_CMD_RECORDING               'r'
#define KEY_CMD_REALTIMEPREVIEW         'v'
#define KEY_CMD_WRITEVIDEO              'w'
#define KEY_CMD_WRITEVIDEO_MJPG         'W'
#define KEY_CMD_WRITEVIDEO_MP4          'a'
#define KEY_CMD_PAUSE                   'p'
#define KEY_CMD_TOSTARTFRAME            's'
#define KEY_CMD_TOENDFRAME              'e'
#define KEY_CMD_SAVEFRAME               'z'
#define KEY_CMD_SAVEALLFRAME            'x'
#define KEY_CMD_SHOWINFO                't'
#define KEY_CMD_SLOWRATE_0              '0'
#define KEY_CMD_SLOWRATE_5              '5'
#define KEY_CMD_FRAMESTEP_5             '9'
#define KEY_CMD_FRAMESTEP_1             '8'
#define KEY_CMD_NEXTFRAME               ' '
#define KEY_CMD_PREVFRAME               'm'
#define KEY_CMD_NEXTFRAME_SPD           'f'
#define KEY_CMD_PREVFRAME_SPD           'b'
#define KEY_CMD_NEXTFRAME_HIGH          'F'
#define KEY_CMD_PREVFRAME_HIGH          'B'
#define KEY_CMD_GETBALANCE              'c'

#define PAUSE_MODE_PAUSED               (1)
#define PAUSE_MODE_RUNNING              (0)

#define FRAME_COUNT     (10000)         // specify how many frames you want to save
#define SLOW_RATE       (33)            // wait time
#define FPS_UPDATE_FREQUENCY            (100)
#define STEPS_SPD                       (100)
#define STEPS_HIGH                      (200) 
//#define RAW10THRU

// for recording, etc.
int g_frame_count = 0;
int g_pause_mode = PAUSE_MODE_RUNNING;
int g_slow_rate = 0;
int g_info_show = INFO_SHOW_ENABLE;
float g_balanceR = 1.f;
float g_balanceG = 1.f;
float g_balanceB = 1.f;
static int g_fno = 0;
static int g_stepval = 1;
int g_max_frame = FRAME_COUNT;
int g_save_frame = 0;

// for output text
CvFont g_font;
char g_text_buf[256];

// for writing video
CvVideoWriter *g_video_writer = NULL;
int g_video_write_flg = 0;
int g_specified_frames;

volatile int stop_process;  // volatile !!!

IplImage* BayerImg, *RGBImg;

// for fps calculation
struct timeval tv_start, tv_end;
struct timezone tz_local;
double fps = 0;

int g_raw_width  = 640;
int g_raw_height = 480;
int g_skipline = 0;
int g_bit = 10;
int g_flip = 0;

const char* g_raw_file;
unsigned char *g_frame_data_array[FRAME_COUNT];
unsigned short pixel[4096];

// allocate memory beforehand
void allocate_frame_data_array(int width, int height, char **argv)
{
  int i, w, h;
  int siz;
  FILE *fp;

  printf("%d, %d\n", width, height);

  for (i = 0; i < g_specified_frames; i++) {
    g_frame_data_array[i] = (unsigned char *)malloc(width*height); // 8bit
    if (g_frame_data_array[i] == NULL) {
      printf("fail: cannot malloc\n");
    }
  }

  g_save_frame = 0;
  int shift = 16 - g_bit;

  printf("[%05d] open: [%s] : ", g_save_frame, g_raw_file);
  fp = fopen(g_raw_file, "rb");
  if (fp != NULL) {

    fseek(fp, width*g_skipline*2, SEEK_SET);

    for (i = 0; i < g_specified_frames; i++) {
      siz = 0;
      for (h = 0; h < height; h++) {
        siz += fread(&pixel, 1, width*2, fp);
        for (w = 0; w < width; w++) {
          int val = (pixel[w] >> shift);
          if (val > 255)  val = 255;
          if (g_flip) {
            g_frame_data_array[g_save_frame][(height-1 - h) * width + (width-1 - w)] = val;
          } else {
            g_frame_data_array[g_save_frame][h * width + w] = val;
          }
        }
      }
      siz /= 2;

      if (siz != (width*height)) {
        printf("read failure\n");
      }
      else { 
        g_save_frame++;
        //printf("OK\n");
      }
    }

    fclose(fp);
  }
  else {
    printf("cannot open file\n");
  }
}

void discard_frame_data_array()
{
  int i;
  for (i = 0; i < FRAME_COUNT; i++) {
    if (g_frame_data_array[i] != 0) {
      free(g_frame_data_array[i]);
    }
  }
}

void process_parameters(int argc, char **argv)
{
  int i;
  int argChk = 0;

  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0) {
      if (i+1 < argc) {
        g_raw_file = argv[i+1];
        argChk++;
        printf("input:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-f") == 0) {
      if (i+1 < argc) {
        g_specified_frames = atoi(argv[i+1]);
        argChk++;
        printf("frames:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-w") == 0) {
      if (i+1 < argc) {
        g_raw_width = atoi(argv[i+1]);
        printf("width:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-h") == 0) {
      if (i+1 < argc) {
        g_raw_height = atoi(argv[i+1]);
        printf("height:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-skipline") == 0) {
      if (i+1 < argc) {
        g_skipline = atoi(argv[i+1]);
        printf("skipline:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-bit") == 0) {
      if (i+1 < argc) {
        g_bit = atoi(argv[i+1]);
        printf("bit:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-flip") == 0) {
      if (i+1 < argc) {
        g_flip = atoi(argv[i+1]);
        printf("flip:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-gain") == 0) {
      if (i+3 < argc) {
        g_balanceR = atof(argv[i+1]);
        g_balanceG = atof(argv[i+2]);
        g_balanceB = atof(argv[i+3]);
        printf("gain:%s %s %s\n", argv[i+1], argv[i+2], argv[i+3]);
      }
    }
  }

  if (argChk < 2) {
    fprintf (stderr, "Example: %s -i input.raw -f 100 -w 1920 -h 1080 -skipline 0 -bit 10 -flip 1 -gain 1.5 1.0 1.5\n", argv[0]);
    exit(1);
  }
}

static void signal_handler(int sig)
{
  printf("Terminated.\n");
  if(sig == SIGINT)
    stop_process = 1;
}


unsigned char* get_frame_data(int frameno)
{
  return g_frame_data_array[frameno];
}

void reset_fps_start_time()
{
  gettimeofday(&tv_start, &tz_local);
}

void get_time_passed()
{
  gettimeofday(&tv_end, &tz_local);
  double start_time = tv_start.tv_sec+(double)tv_start.tv_usec/1000000;
  double end_time = tv_end.tv_sec+(double)tv_end.tv_usec/1000000;
  fps = (double)g_frame_count/(end_time-start_time);
  printf("Completed.\n");
  printf("FrameRate = %f\n", fps);
  printf("Frame Count = %d\n", g_save_frame);
  gettimeofday(&tv_start, &tz_local);
}

void frame_valiables_init()
{
  g_pause_mode = PAUSE_MODE_RUNNING;
  g_fno = 0;
  g_slow_rate = 0;
  //g_save_frame = 0; 
  g_frame_count = 0;
  g_stepval = 1;
}

void frame_apply_easy_white_balance(unsigned char *frame_data)
{
  int i, j;
  float R, Gr, Gb, B;

  for (i=0;i<BayerImg->height;i+=2) {
    for (j=0;j<BayerImg->width;j+=2){
      R  = frame_data[(i  )*BayerImg->widthStep+j  ];
      R -= ((R >= 16)?16:R);
      R = R * g_balanceR;
      if (R > 255.f) R = 255;

      Gr = frame_data[(i  )*BayerImg->widthStep+j+1];
      Gr -= ((Gr >= 16)?16:Gr);

      Gb = frame_data[(i+1)*BayerImg->widthStep+j  ];
      Gb -= ((Gb >= 16)?16:Gb);

      B  = frame_data[(i+1)*BayerImg->widthStep+j+1];
      B -= ((B >= 16)?16:B);

      B = B * g_balanceB;
      if (B > 255.f) B = 255;

      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i  )))[j  ] = R;
      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i  )))[j+1] = Gr;
      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i+1)))[j  ] = Gb;
      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i+1)))[j+1] = B;
    }
  }
}

void frame_write_video()
{
  if (g_video_writer != NULL) {
    cvWriteFrame(g_video_writer, RGBImg);
    sprintf(g_text_buf, "[Writing...] frame=%05d", g_fno);
    cvPutText(RGBImg, g_text_buf, cvPoint(0,30), &g_font, CV_RGB(255, 0, 0));
    cvShowImage("RAW Viewer", RGBImg);
    cvWaitKey(1);
  }
  g_fno++;
  if (g_fno >= g_save_frame) {
    g_fno = 0;
    g_pause_mode = PAUSE_MODE_PAUSED;
    g_video_write_flg = 0;
    if (g_video_writer != NULL) {
      cvReleaseVideoWriter(&g_video_writer);
      g_video_writer = NULL;
    }
  }
}

void frame_preview_info_show()
{
  if (g_info_show == INFO_SHOW_ENABLE) {
    sprintf(g_text_buf, "[Preview Mode] frame=%05d", g_fno);
    cvPutText(RGBImg, g_text_buf, cvPoint(0,30), &g_font, CV_RGB(255, 0, 0));
    if (g_stepval != 0) {
      sprintf(g_text_buf, "step = %d", g_stepval);
      cvPutText(RGBImg, g_text_buf, cvPoint(0,60), &g_font, CV_RGB(255, 0, 0));
    }
  }
}

void frame_preview_writevideo_allframe(int fourcc, char *filename)
{
  fps = 30;
  printf("Write frame rate = %d\n", (int)fps);
  printf("width,height = %d, %d\n", g_raw_width, g_raw_height);

  g_video_writer = cvCreateVideoWriter(filename, fourcc, 
                                    (int)fps,
                                    cvSize(g_raw_width,
                                           g_raw_height), 1);
  if (g_video_writer == NULL) {
    printf("OpenCV Error: g_video_writer\n");
  } 
  else {
    g_pause_mode = 0;
    g_fno = 0;
    g_slow_rate = 0;
    g_video_write_flg = 1;
  }
}

void get_easy_white_balance_profile(unsigned char *frame_data)
{
  int sumR=0, sumGr=0, sumGb=0, sumB=0, aveR=0, aveG=0, aveB=0;
  int i, j;
  int Width = g_raw_width; 
  int Height = g_raw_height; 
    
  for (i=0;i<BayerImg->height; i+=2) {
    for (j=0;j<BayerImg->width; j+=2){
      sumR  = sumR  + frame_data[(i  )*BayerImg->widthStep+j  ];
      sumGr = sumGr + frame_data[(i  )*BayerImg->widthStep+j+1];
      sumGb = sumGb + frame_data[(i+1)*BayerImg->widthStep+j  ];
      sumB  = sumB  + frame_data[(i+1)*BayerImg->widthStep+j+1];
    }
  }
  aveR = sumR / (Width*Height/4);
  aveB = sumB / (Width*Height/4);
  aveG = (sumGr + sumGb) / (Width*Height/2);
  g_balanceR = (float)aveG / (float)aveR;  
  g_balanceG = 1.f;
  g_balanceB = (float)aveG / (float)aveB;  
  printf("aveR = %d\n", aveR);
  printf("aveG = %d\n", aveG);
  printf("aveB = %d\n", aveB);
  printf("g_balanceR = %f\n", g_balanceR);
  printf("g_balanceG = %f\n", g_balanceG);
  printf("g_balanceB = %f\n", g_balanceB);
}

void make_filename(char *filename, char *basename, char* extname)
{
  time_t timer;
  struct tm *local;

  timer = time(NULL);
  local = localtime(&timer);
  sprintf(filename, "%s%04d%02d%02d%02d%02d%02d.%s", 
          basename, 
          local->tm_year+1900,
          local->tm_mon +1,
          local->tm_mday,
          local->tm_hour,
          local->tm_min,
          local->tm_sec,
          extname);
}


void process_preview_mode_key(int key)
{
  char filename[PATH_MAX];

  if (key == KEY_CMD_PAUSE) {
    g_pause_mode ^= 1; // toggle pause mode
  }
  else if ((key >= KEY_CMD_SLOWRATE_0) && (key <= KEY_CMD_SLOWRATE_5)) {
    g_slow_rate = key - KEY_CMD_SLOWRATE_0;
  }
  else if (key == KEY_CMD_FRAMESTEP_1) {
    g_stepval = 1;
  }
  else if (key == KEY_CMD_FRAMESTEP_5) {
    g_stepval = 5;
  }
  else if (key == KEY_CMD_NEXTFRAME_SPD) {
    g_fno += STEPS_SPD;
    if (g_fno >= g_save_frame) {
      g_fno = g_save_frame - 1;
    }
  }
  else if (key == KEY_CMD_PREVFRAME_SPD) {
    if (g_fno >= STEPS_SPD) {
      g_fno -= STEPS_SPD;
    }
    else {
      g_fno = 0;
    }
  }
  else if (key == KEY_CMD_NEXTFRAME_HIGH) {
    g_fno += STEPS_HIGH;
    if (g_fno >= g_save_frame) {
      g_fno = g_save_frame - 1;
    }
  }
  else if (key == KEY_CMD_PREVFRAME_HIGH) {
    if (g_fno >= STEPS_HIGH) {
      g_fno -= STEPS_HIGH;
    }
    else {
      g_fno = 0;
    }
  }

  if (g_pause_mode == PAUSE_MODE_RUNNING) {
    g_fno += g_stepval;
    if (g_fno >= g_save_frame) {
      g_fno = 0;
    }
  }
  else {
    sprintf(g_text_buf,"[Pause]");
    if (g_info_show == INFO_SHOW_ENABLE) cvPutText(RGBImg, g_text_buf, cvPoint(0,90), &g_font, CV_RGB(255, 0, 0));

    if (key == KEY_CMD_NEXTFRAME) {
      g_fno += g_stepval;
      if (g_fno >= g_save_frame) {
        g_fno = 0;
      }
    }
    else if (key == KEY_CMD_PREVFRAME) {
      if (g_fno >= g_stepval) {
        g_fno -= g_stepval;
      }
    }
  }

  if (key == KEY_CMD_TOSTARTFRAME) {       // go to the first frame
    g_fno = 0;
    g_pause_mode = PAUSE_MODE_PAUSED;
  }
  else if (key == KEY_CMD_TOENDFRAME) {  // go to the end frame
    g_fno = g_save_frame - 1;
    g_pause_mode = PAUSE_MODE_PAUSED;
  }
  else if (key == KEY_CMD_SHOWINFO) {
     g_info_show ^= 1;  
  }
  else if (key == KEY_CMD_WRITEVIDEO) {  // RAW
    make_filename(filename, "capture_raw_", "avi");
    frame_preview_writevideo_allframe(0, filename);
  }
  else if (key == KEY_CMD_WRITEVIDEO_MJPG) {
    make_filename(filename, "capture_", "mjpg");
    frame_preview_writevideo_allframe(CV_FOURCC('M','J','P','G'), filename);
  }
  else if (key == KEY_CMD_WRITEVIDEO_MP4) {
    make_filename(filename, "capture_mp4_", "avi");
    frame_preview_writevideo_allframe(CV_FOURCC('F', 'M','P','4'), filename);
  }
  else if (key == KEY_CMD_GETBALANCE) {
    get_easy_white_balance_profile(get_frame_data(g_fno));
  }
  else if (key == KEY_CMD_QUIT) {
    stop_process = 1;
  }

  sprintf(g_text_buf,"Slow Rate = %d", g_slow_rate);
  if (g_info_show == INFO_SHOW_ENABLE) cvPutText(RGBImg, g_text_buf, cvPoint(0,120), &g_font, CV_RGB(255, 0, 0));
  if (g_slow_rate != 0) {
    usleep(g_slow_rate*SLOW_RATE*1000);
  }
}

double get_fps()
{
  gettimeofday(&tv_end, &tz_local);
  double start_time = tv_start.tv_sec+(double)tv_start.tv_usec/1000000;
  double end_time = tv_end.tv_sec+(double)tv_end.tv_usec/1000000;
  double fps = (double)g_frame_count/(end_time-start_time);

  return fps;
}

void update_fps_base()
{
  g_frame_count++;
  if (g_frame_count > FPS_UPDATE_FREQUENCY) { // reset offset time every FPS_UPDATE_FREQUENCY frames
    g_frame_count = 0;
    reset_fps_start_time();
  }
}

void cv_Gamma(IplImage* src, IplImage* dst, double gamma)
{
  int i;
  uchar LUT[256];

  //ガンマ補正テーブルの作成
  for (i = 0; i < 256; i++) {
    LUT[i] = (int)(pow((double)i / 255.0, 1.0 / gamma) * 255.0);
  }

  //CvMatへ変換  
  CvMat lut_mat = cvMat(1, 256, CV_8UC1, LUT);

  //ルックアップテーブル変換
  cvLUT(src, dst, &lut_mat);
}

void frame_preview()
{
  unsigned char *frame_data;

  do {
    frame_data = get_frame_data(g_fno);
    frame_apply_easy_white_balance(frame_data);
    if (g_flip) {
      cvCvtColor(BayerImg, RGBImg, CV_BayerBG2RGB);
    } else {
      cvCvtColor(BayerImg, RGBImg, CV_BayerRG2RGB);
    }
    //cv_Gamma(RGBImg, RGBImg, 2.2);
    if (g_video_write_flg == 1) {
      frame_write_video();
    }
  }
  while (g_video_write_flg == 1);

  frame_preview_info_show();
  int key = cvWaitKey(1) & 0xff;
  process_preview_mode_key(key);

  // get fps
  double fps = get_fps();

  // show fps
  sprintf(g_text_buf,"FrameRate = %f", fps);
  if (g_info_show == INFO_SHOW_ENABLE) cvPutText(RGBImg, g_text_buf, cvPoint(0,180), &g_font, CV_RGB(255, 0, 0));

  update_fps_base();

  cvShowImage("RAW Viewer", RGBImg);

}

int main(int argc, char **argv)
{
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    return 2;
  }

  g_frame_count = 0;

  // OpenCV Settings
  cvNamedWindow("RAW Viewer", CV_WINDOW_AUTOSIZE); 

  process_parameters(argc, argv);

  BayerImg = cvCreateImage(cvSize(g_raw_width, g_raw_height), IPL_DEPTH_8U, 1);
  RGBImg = cvCreateImage(cvSize(g_raw_width, g_raw_height),   IPL_DEPTH_8U, 3);
  cvInitFont(&g_font, CV_FONT_HERSHEY_SIMPLEX, 1.0f, 1.0f, 0, 1, 8);

  ////////////////////////////////////////
  allocate_frame_data_array(g_raw_width, g_raw_height, argv);

  frame_valiables_init();
  reset_fps_start_time();
  ////////////////////////////////////////

  cvWaitKey(2);

  while(1){
    if (stop_process==1) break;
    frame_preview();
  }

  ////////////////////////////////////////
  discard_frame_data_array();
  ////////////////////////////////////////

//finalize:

  if (g_video_writer != NULL) {
    cvReleaseVideoWriter(&g_video_writer);
  }
  cvReleaseImage(&RGBImg);
  cvReleaseImage(&BayerImg);
  cvDestroyWindow("RAW Viewer");
    
  printf("Finalize done.\n");fflush(stdout);

  return 1;
}

