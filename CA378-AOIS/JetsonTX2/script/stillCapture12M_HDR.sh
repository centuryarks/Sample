#!/bin/sh
W=4056
H=3040
FRAME_LENGTH=3200
ID=$1
EXPOSURE_LONG=$2
GAIN=$3
HDR_RATIO=$4
EXPOSURE_SHORT=`expr ${EXPOSURE_LONG} / ${HDR_RATIO}`
filename=still_${W}x${H}_HDR
v4l2-ctl -d /dev/video${ID} --set-fmt-video=width=${W},height=${H},pixelformat=RG10 \
 --set-ctrl frame_length=${FRAME_LENGTH} \
 --set-ctrl coarse_time=${EXPOSURE_LONG} \
 --set-ctrl coarse_time_short=${EXPOSURE_SHORT} \
 --set-ctrl hdr_enable=1 \
 --set-ctrl gain=${GAIN} \
 --set-ctrl bypass_mode=0 \
 --stream-mmap --stream-skip=1 --stream-count=1 --stream-to=$filename.raw --verbose
./tool/raw2hdr -i ${filename}.raw -o $filename -w 4096 -h 3040
./tool/raw2dng -i ${filename}_L.raw -o ${filename}_L.dng -w 4096 -h 3040 -bit 10 -flip 1 -gain 1.0 1.0 1.0
./tool/raw2dng -i ${filename}_S.raw -o ${filename}_S.dng -w 4096 -h 3040 -bit 10 -flip 1 -gain 1.0 1.0 1.0
ufraw-batch ${filename}_L.dng --output=${filename}_L.tif --overwrite --wb=camera
ufraw-batch ${filename}_S.dng --output=${filename}_S.tif --overwrite --wb=camera
python3 ./script/HDR.py
geeqie ${filename}_fusion.ppm
