#!/bin/sh
cd /home/pi/demo
./bin/stillsampleRAW16 ./profile/IMX378_3968x3008.xml IMX378_3968x3008.raw $1
./bin/raw2dng -i IMX378_3968x3008.raw -o IMX378_3968x3008.dng -w 3968 -h 3008 -bit 10 -gain 2.13 1.0 1.93
ufraw IMX378_3968x3008.dng --wb=camera
