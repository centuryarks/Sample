#!/bin/bash
cd /home/nvidia/demo

FILENAME=$1
SENSOR_ID=$2
WIDTH=$3
HEIGHT=$4
FPS=$5
CAPTURE_NUMBER=$6

FPS_4K3K=${FPS}
FPS_4K2K=${FPS}
FPS_FHD=${FPS}
FPS_VGA=${FPS}

if [ ${WIDTH} = 4032 ] && [ ${HEIGHT} = 3040 ]; then
  MODE = 0
elif [ ${WIDTH} = 3840 ] && [ ${HEIGHT} = 2160 ]; then
  MODE = 1
elif [ ${WIDTH} = 1920 ] && [ ${HEIGHT} = 1080 ]; then
  MODE = 2
elif [ ${WIDTH} = 640 ] && [ ${HEIGHT} = 480 ]; then
  MODE = 3
else
  echo Not Supported Mode!
fi

v4l2-ctl -d /dev/video${SENSOR_ID} \
 --set-ctrl sensor_mode=${MODE}
 --set-ctrl exposure=1000
 --set-ctrl gain=200 \
 --set-fmt-video=width=${WIDTH},height=${HEIGHT},pixelformat=RG10 \
 --set-ctrl bypass_mode=0 --stream-mmap=8 --stream-skip=1 --stream-count=${CAPTURE_NUMBER} --stream-to=${FILENAME}${SENSOR_ID}.raw

./bin/preview -i ${FILENAME}${SENSOR_ID}.raw -f ${CAPTURE_NUMBER} -w ${WIDTH} -h ${HEIGHT} -skipline 0 -bit 10 -flip 1 -gain 1.0 1.0 1.0
