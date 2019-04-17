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

source ./script/generate_framerate.sh

if [ ${WIDTH} = 4056 ] && [ ${HEIGHT} = 3040 ]; then
  FRAME_LENGTH=${FRAME_LENGTH_4K3K}
  ISO=${ISO[0]}
elif [ ${WIDTH} = 3840 ] && [ ${HEIGHT} = 2160 ]; then
  FRAME_LENGTH=${FRAME_LENGTH_4K2K}
  ISO=${ISO[1]}
elif [ ${WIDTH} = 1920 ] && [ ${HEIGHT} = 1080 ]; then
  FRAME_LENGTH=${FRAME_LENGTH_FHD}
  ISO=${ISO[2]}
elif [ ${WIDTH} = 640 ] && [ ${HEIGHT} = 480 ]; then
  FRAME_LENGTH=${FRAME_LENGTH_VGA}
  ISO=${ISO[3]}
else
  echo Not Supported Mode!
fi

COARSE_TIME=`expr ${FRAME_LENGTH} \- 20`
GAIN=`expr ${ISO} \* 256 / 100`

v4l2-ctl -d /dev/video${SENSOR_ID} \
 --set-ctrl frame_length=${FRAME_LENGTH} --set-ctrl coarse_time=${COARSE_TIME} --set-ctrl gain=${GAIN} \
 --set-fmt-video=width=${WIDTH},height=${HEIGHT},pixelformat=RG10 \
 --set-ctrl bypass_mode=0 --stream-mmap=8 --stream-skip=1 --stream-count=${CAPTURE_NUMBER} --stream-to=${FILENAME}${SENSOR_ID}.raw

./bin/preview -i ${FILENAME}${SENSOR_ID}.raw -f ${CAPTURE_NUMBER} -w ${WIDTH} -h ${HEIGHT} -skipline 0 -bit 10 -flip 1 -gain 1.0 1.0 1.0
