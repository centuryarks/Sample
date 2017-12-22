#!/bin/sh
cd /home/pi/demo
./bin/stillsampleRAW16 ./profile/IMX378_3968x3008_HDR.xml IMX378_3968x3008_HDR.raw $1
./bin/raw2hdr -i IMX378_3968x3008_HDR.raw -o IMX378_3968x3008_HDR -w 3968 -h 3008
./bin/raw2dng -i IMX378_3968x3008_HDR_L.raw -o IMX378_3968x3008_HDR_L.dng -w 3968 -h 3008 -bit 10 -gain 0.5 1.0 0.6
./bin/raw2dng -i IMX378_3968x3008_HDR_S.raw -o IMX378_3968x3008_HDR_S.dng -w 3968 -h 3008 -bit 10 -gain 0.5 1.0 0.6
ufraw-batch IMX378_3968x3008_HDR_L.dng --output=IMX378_3968x3008_HDR_L.ppm --overwrite --wb=camera
ufraw-batch IMX378_3968x3008_HDR_S.dng --output=IMX378_3968x3008_HDR_S.ppm --overwrite --wb=camera
python3 ./script/HDR.py
geeqie IMX378_3968x3008_HDR_fusion.ppm
