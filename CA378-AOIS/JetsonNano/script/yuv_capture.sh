#!/bin/sh
cd /home/nvidia/demo

FILENAME=$1
SENSOR_ID=$2
WIDTH=$3
HEIGHT=$4
FPS=$5
CAPTURE_NUMBER=$6

gst-launch-1.0 nvarguscamerasrc sensor-id=${SENSOR_ID} num-buffers=${CAPTURE_NUMBER} wbmode=9 awblock=false aelock=true \
 ! "video/x-raw(memory:NVMM), width=(int)${WIDTH}, height=(int)${HEIGHT}, framerate=(fraction)${FPS}/1" ! nvvidconv flip-method=2 \
 ! "video/x-raw, width=(int)${WIDTH}, height=(int)${HEIGHT}, framerate=(fraction)${FPS}/1" ! filesink location=${FILENAME}${SENSOR_ID}.yuv
