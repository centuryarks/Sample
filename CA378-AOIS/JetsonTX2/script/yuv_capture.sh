#!/bin/sh
cd /home/nvidia/demo

FILENAME=$1
SENSOR_ID=$2
WIDTH=$3
HEIGHT=$4
FPS=$5
CAPTURE_NUMBER=$6

gst-launch-1.0 nvcamerasrc sensor-id=${SENSOR_ID} num-buffers=${CAPTURE_NUMBER} fpsRange="${FPS}.0 ${FPS}.0" wbmode=9 wbManualMode=3 aeLock=false \
 ! "video/x-raw(memory:NVMM), width=(int)${WIDTH}, height=(int)${HEIGHT}, framerate=(fraction)${FPS}/1" ! nvvidconv flip-method=2 \
 ! "video/x-raw, width=(int)${WIDTH}, height=(int)${HEIGHT}, framerate=(fraction)${FPS}/1" ! filesink location=${FILENAME}${SENSOR_ID}.yuv
