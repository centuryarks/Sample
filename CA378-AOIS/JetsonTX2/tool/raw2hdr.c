/* Read in jpeg from Raspberry Pi camera captured using 'raspistill --raw'
   and extract raw file with 10-bit values stored left-justified at 16 bpp
   in Adobe DNG (TIFF-EP) format, convert with 'ufraw out.dng' for example

   John Beale  26 May 2013
   and others
   
   Contains code written by Dave Coffin for Berkeley Engineering and Research.

   Free for all uses.


   Requires LibTIFF 3.8.0 plus a patch, see http://www.cybercom.net/~dcoffin/dcraw/

   Compile with:
   gcc -o raspi_dng raspi_dng.c -O4 -Wall -lm -ljpeg -ltiff
   
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <tiffio.h>
#include <errno.h>
#include "tools.h"


int main (int argc, char **argv)
{
  int status = 0;
  int i, row, col;
  FILE *ifp = NULL;
#if 0
  FILE *ofp = NULL;
#endif
  FILE *fwL = NULL;
  FILE *fwS = NULL;
  struct stat st;
  struct tm tm;
  char datetime[64];
  char strFilename[256];
  const char*   inFilename  = NULL;
  const char*   outFilename = NULL;
  unsigned short* pixel  = NULL;    // array holds 16 bits per pixel
  unsigned short* pixelL = NULL;    // array holds 16 bits per pixel
  unsigned short* pixelS = NULL;    // array holds 16 bits per pixel
  unsigned long fileLen;            // number of bytes in file
  unsigned long offset;             // offset into file to start reading pixel data
  unsigned char split;              // single byte with 4 pairs of low-order bits
  int HPIXELS  = 640;               // number of horizontal pixels
  int VPIXELS  = 480;               // number of vertical pixels
  int SKIPLINE = 0;

  int argChk = 0;
  int sizChk = 0;
  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0) {
      if (i+1 < argc) {
        inFilename = argv[i+1];
        argChk++;
        printf("input:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-o") == 0) {
      if (i+1 < argc) {
        outFilename = argv[i+1];
        argChk++;
        printf("output:%s_L.raw %s_S.raw\n", argv[i+1], argv[i+1]);
      }
    } else if (strcmp(argv[i], "-w") == 0) {
      if (i+1 < argc) {
        HPIXELS = atoi(argv[i+1]);
        sizChk++;
        printf("width:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-h") == 0) {
      if (i+1 < argc) {
        VPIXELS = atoi(argv[i+1]);
        sizChk++;
        printf("height:%s\n", argv[i+1]);
      }
    } else if (strcmp(argv[i], "-skipline") == 0) {
      if (i+1 < argc) {
        SKIPLINE = atoi(argv[i+1]);
        printf("skipline:%s\n", argv[i+1]);
      }
    }
  }

  if (argChk < 2) {
    fprintf (stderr, "Example: %s -i input.raw -o output -w 1920 -h 1080 -skipline 0\n", argv[0]);
    return 1;
  }

  if (!(ifp = fopen (inFilename, "rb"))) {
    perror (inFilename);
    return 1;
  }

  sprintf(strFilename, "%s_L.raw", outFilename);
  if (!(fwL = fopen(strFilename, "wb"))) {
    perror (outFilename);
    return 1;
  }

  sprintf(strFilename, "%s_S.raw", outFilename);
  if (!(fwS = fopen(strFilename, "wb"))) {
    perror (outFilename);
    return 1;
  }

#if 0
  if (!(ofp = fopen (outFilename, "wb"))) {
    perror (outFilename);
    return 1;
  }
#endif

  stat (inFilename, &st);
  gmtime_r (&st.st_mtime, &tm);
  sprintf (datetime, "%04d:%02d:%02d %02d:%02d:%02d",
    tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);

  // Get file length
  fseek(ifp, 0, SEEK_END);
  fileLen = ftell(ifp);
  fseek(ifp, 0, SEEK_SET);

  if (sizChk < 2) {
    // Get pixel size
    GetPixelSize(fileLen / 2, &HPIXELS, &VPIXELS);
  }

  // Allocate memory for one line of pixel data
  pixel =(unsigned short *)malloc(HPIXELS * VPIXELS * 2 + 1);
  if (!pixel) {
    fprintf(stderr, "Memory error!");
    status = ENOMEM;
    goto fail;
  }
  pixelL =(unsigned short *)malloc(HPIXELS * VPIXELS * 2 + 1);
  if (!pixelL) {
    fprintf(stderr, "Memory error!");
    status = ENOMEM;
    goto fail;
  }
  pixelS =(unsigned short *)malloc(HPIXELS * VPIXELS * 2 + 1);
  if (!pixelS) {
    fprintf(stderr, "Memory error!");
    status = ENOMEM;
    goto fail;
  }

  fseek(ifp, 2 * HPIXELS * SKIPLINE, SEEK_SET);
  fread(pixel, 2 * HPIXELS * VPIXELS, 1, ifp);

  //   0 1 2 3
  // 0|R|G|r|G|R|G|r|G|
  // 1|g|b|g|B|g|b|g|B|
  // 2|r|G|R|G|r|G|g|R|
  // 3|g|B|g|b|g|B|g|b|
  //
  // LONG: R G B
  // SHORT:r g b
  //
  for (row = 2; row < VPIXELS - 2; row++) {
    for (col = 2; col < HPIXELS - 2; col++) {
      //pixelL[row * HPIXELS + col] =   pixel[row * HPIXELS + col];
      int type = (col & 0x3) * 0x10 + (row & 0x3);
      switch (type)
      {
      case 0x00:
      case 0x22:
      case 0x13:
      case 0x31:
        pixelL[row * HPIXELS + col] =   pixel[row * HPIXELS + col];
        pixelS[row * HPIXELS + col] =  (pixel[row * HPIXELS + (col - 2)] +
                        pixel[row * HPIXELS + (col + 2)] +
                        pixel[(row - 2) * HPIXELS + col] +
                        pixel[(row + 2) * HPIXELS + col]) / 4;
        break;
      case 0x10:
      case 0x30:
      case 0x12:
      case 0x32:
        pixelL[row * HPIXELS + col] =   pixel[row * HPIXELS + col];
        pixelS[row * HPIXELS + col] =  (pixel[(row - 1) * HPIXELS + (col - 1)] +
                        pixel[(row - 1) * HPIXELS + (col + 1)] +
                        pixel[(row + 1) * HPIXELS + (col - 1)] +
                        pixel[(row + 1) * HPIXELS + (col + 1)]) / 4;
        break;
      case 0x02:
      case 0x20:
      case 0x11:
      case 0x33:
        pixelS[row * HPIXELS + col] =   pixel[row * HPIXELS + col];
        pixelL[row * HPIXELS + col] =  (pixel[row * HPIXELS + (col - 2)] +
                        pixel[row * HPIXELS + (col + 2)] +
                        pixel[(row - 2) * HPIXELS + col] +
                        pixel[(row + 2) * HPIXELS + col]) / 4;
        break;
      case 0x01:
      case 0x21:
      case 0x03:
      case 0x23:
        pixelS[row * HPIXELS + col] =   pixel[row * HPIXELS + col];
        pixelL[row * HPIXELS + col] =  (pixel[(row - 1) * HPIXELS + (col - 1)] +
                        pixel[(row - 1) * HPIXELS + (col + 1)] +
                        pixel[(row + 1) * HPIXELS + (col - 1)] +
                        pixel[(row + 1) * HPIXELS + (col + 1)]) / 4;
        break;
      default:
        printf("error col=%d row=%d\n", col, row);
        break;
      }
    }
  }

  fwrite(pixelL, 2 * HPIXELS * VPIXELS, 1, fwL);
  fwrite(pixelS, 2 * HPIXELS * VPIXELS, 1, fwS);

#if 0
  fwrite(pixelL, 2 * HPIXELS * VPIXELS, 1, ofp);
  fwrite(pixelS, 2 * HPIXELS * VPIXELS, 1, ofp);
#endif

fail:
  if (pixel) {
    free(pixel); // free up that memory we allocated
  }

  if (pixelL) {
    free(pixelL); // free up that memory we allocated
  }

  if (pixelS) {
    free(pixelS); // free up that memory we allocated
  }

  if (ifp) {
    fclose (ifp);
  }

#if 0
  if (ofp) {
    fclose (ofp);
  }
#endif

  if (fwL) {
    fclose (fwL);
  }

  if (fwS) {
    fclose (fwS);
  }

  return status;
}
