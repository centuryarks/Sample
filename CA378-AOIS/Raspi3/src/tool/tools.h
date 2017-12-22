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

void GetPixelSize(int pixelNum, int *HPIXELS, int *VPIXELS)
{
    int i, w;
    int pixelTable[128];
    int pixelIdx = 0;
    double d, d1, d2, d3;
    double min = 0xFFFF;

    for (i = 128; i < 8192; i += 4)
    {
        if (pixelNum % i == 0)
        {
            pixelTable[pixelIdx] = i;
            pixelIdx++;
        }
    }

    for (i = 0; i < pixelIdx; i++)
    {
        w = pixelNum / pixelTable[i];
        d1 = fabs(w * 9.0 / pixelTable[i] - 16.0);  // 16:9
        d2 = fabs(w * 9.0 / pixelTable[i] - 12.0);  // 4:3
        d3 = fabs(w * 18.0 / pixelTable[i] - 12.0); // HDR

        if (d1 < d2 && d1 < d3)
        {
            d = d1;
        }
        else if (d2 < d1 && d2 < d3)
        {
            d = d2;
        }
        else
        {
            d = d3;
        }

        if (min > d)
        {
            min = d;
            *HPIXELS = w;
            *VPIXELS = pixelTable[i];
        }
    }

    printf("RAW:%d x %d\n", *HPIXELS, *VPIXELS);
}
