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
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <tiffio.h>
#include <errno.h>
#include "tools.h"


int main (int argc, char **argv)
{
    static const short CFARepeatPatternDim[] = { 2, 2 };

    // this color matrix is definitely inaccurate, TODO: calibrate
    static const float cam_xyz[] = {
        // R    G       B
        1.000,  0.000,  0.000,  // R
        0.000,  1.000,  0.000,  // G
        0.000,  0.000,  1.000   // B
    };

    static float neutral[] = { 1.0, 1.0, 1.0 }; // TODO calibrate
    unsigned short curve[0x10000];
    long white = 0xFFFF;
    long sub_offset = 0x0;

    int status = 0;
    int i, row, col;

    FILE *ifp = NULL;
    TIFF *tif = NULL;
    struct stat st;
    struct tm tm;
    char datetime[64];
    const char* inFilename  = NULL;
    const char* outFilename = NULL;
    unsigned short* pixel   = NULL;         // array holds 16 bits per pixel
    unsigned short* lineBuf = NULL;         // line buffer
    unsigned long fileLen;                  // number of bytes in file
    unsigned long offset;                   // offset into file to start reading pixel data
    unsigned char split;                    // single byte with 4 pairs of low-order bits
    int HPIXELS  = 640;                     // number of horizontal pixels
    int VPIXELS  = 480;                     // number of vertical pixels
    int SKIPLINE = 0;
    int BIT      = 10;
    int FLIP     = 0;
    int BLACK_LEVEL = 0;
    double R_GAIN = 2.0;
    double G_GAIN = 1.0;
    double B_GAIN = 2.0;

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
                printf("output:%s\n", argv[i+1]);
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
        } else if (strcmp(argv[i], "-bit") == 0) {
            if (i+1 < argc) {
                BIT = atoi(argv[i+1]);
                printf("bit:%s\n", argv[i+1]);
            }
        } else if (strcmp(argv[i], "-flip") == 0) {
            if (i+1 < argc) {
                FLIP = atoi(argv[i+1]);
                printf("flip:%s\n", argv[i+1]);
            }
        } else if (strcmp(argv[i], "-black") == 0) {
            if (i+1 < argc) {
                BLACK_LEVEL = atoi(argv[i+1]);
                printf("black:%s\n", argv[i+1]);
            }
        } else if (strcmp(argv[i], "-gain") == 0) {
            if (i+3 < argc) {
                R_GAIN = atof(argv[i+1]);
                G_GAIN = atof(argv[i+2]);
                B_GAIN = atof(argv[i+3]);
                printf("Gain:%s %s %s\n", argv[i+1], argv[i+2], argv[i+3]);
            }
        }
    }

    if (argChk < 2) {
        fprintf (stderr, "Example: %s -i input.raw -o output.dng -w 1920 -h 1080 -skipline 0 -bit 10 -flip 1 -gain 2.0 1.0 2.0 -black 256\n", argv[0]);
        return 1;
    }

    if (!(ifp = fopen (inFilename, "rb"))) {
        perror (inFilename);
        return 1;
    }
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

    lineBuf =(unsigned short *)malloc(HPIXELS * 2 + 1);
    if (!lineBuf) {
        fprintf(stderr, "Memory error!");
        status = ENOMEM;
        goto fail;
    }

    if (!(tif = TIFFOpen (outFilename, "w"))) goto fail;

    for (i = 0; i < 0x10000; i++) {
        curve[i] = i;
    }

    TIFFSetField (tif, TIFFTAG_DNGVERSION, "\01\01\00\00");
    TIFFSetField (tif, TIFFTAG_SUBFILETYPE, 0);
    TIFFSetField (tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField (tif, TIFFTAG_IMAGEWIDTH, HPIXELS);
    TIFFSetField (tif, TIFFTAG_IMAGELENGTH, VPIXELS);
    TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField (tif, TIFFTAG_ROWSPERSTRIP, 1);
    TIFFSetField (tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField (tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_CFA);
    TIFFSetField (tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField (tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    TIFFSetField (tif, TIFFTAG_CFAREPEATPATTERNDIM, CFARepeatPatternDim);
    if (FLIP) {
        TIFFSetField (tif, TIFFTAG_CFAPATTERN, "\02\01\01\00");
    } else {
        TIFFSetField (tif, TIFFTAG_CFAPATTERN, "\00\01\01\02");
    }
    TIFFSetField (tif, TIFFTAG_MAKE, "CenturyArks");
    TIFFSetField (tif, TIFFTAG_UNIQUECAMERAMODEL, "CA378-AOIS");
    TIFFSetField (tif, TIFFTAG_COLORMATRIX1, 9, cam_xyz);
    TIFFSetField (tif, TIFFTAG_ASSHOTNEUTRAL, 3, neutral);
    TIFFSetField (tif, TIFFTAG_CFALAYOUT, 1);
    TIFFSetField (tif, TIFFTAG_CFAPLANECOLOR, 3, "\00\01\02");
    TIFFSetField (tif, TIFFTAG_LINEARIZATIONTABLE, 0x10000, curve);
    TIFFSetField (tif, TIFFTAG_WHITELEVEL, 1, &white);
    TIFFSetField (tif, TIFFTAG_SUBIFD, 1, &sub_offset);

    fseek(ifp, 2 * HPIXELS * SKIPLINE, SEEK_SET);

    for (row = 0; row < VPIXELS; row++) {
        fread(lineBuf, 2 * HPIXELS, 1, ifp);
        for (col = 0; col < HPIXELS; col++) {
            if (FLIP) {
                pixel[(VPIXELS-1 - row) * HPIXELS + (HPIXELS-1 - col)] = lineBuf[col];
            } else {
                pixel[row * HPIXELS + col] = lineBuf[col];
            }
        }
    }

    int opb = (BLACK_LEVEL * 16) >> (12 - BIT);
    for (row = 0; row < VPIXELS; row++) {
        for (col = 0; col < HPIXELS; col++) {
            int val = pixel[row * HPIXELS + col];
            int pos = (col & 0x1) + (row & 0x1) * 2;
            if (pos == 0x0) {       // R pixel
                val = (val - opb) * R_GAIN + opb;
            }
            else if (pos == 0x3) {  // B pixel
                val = (val - opb) * B_GAIN + opb;
            }
            else {                  // G pixel
                val = (val - opb) * G_GAIN + opb;
            }
            if (val < 0) val = 0;
            val = ((val & 0xFFF0) << (12 - BIT));
            if (val > 0xFFFF) val = 0xFFFF;
            pixel[row * HPIXELS + col] = val;
        }
        TIFFWriteScanline (tif, &pixel[row * HPIXELS], row, 0);
    }

fail:
    if (pixel) {
        free(pixel); // free up that memory we allocated
    }

    if (lineBuf) {
        free(lineBuf);
    }

    if (tif) {
        TIFFClose (tif);
    }

    if (ifp) {
        fclose (ifp);
    }

    return status;
}
