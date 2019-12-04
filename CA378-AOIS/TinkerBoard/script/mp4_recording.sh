#!/bin/sh

FILENAME=$1
WIDTH=$2
HEIGHT=$3
FPS=$4
CAPTURE_NUMBER=$5

rm ${FILENAME}.yuv
rm ${FILENAME}.mp4

export DISPLAY=:0.0 
media-ctl --set-v4l2 "'imx378 2-001a':0[fmt:SBGGR10_1X10/${WIDTH}x${HEIGHT}]"
media-ctl -d /dev/media0 --set-v4l2 "'rkisp1-isp-subdev':0[fmt:SBGGR10_1X10/${WIDTH}x${HEIGHT}]" #sink
media-ctl -d /dev/media0 --set-v4l2 "'rkisp1-isp-subdev':0[fmt:SBGGR10_1X10/${WIDTH}x${HEIGHT}]" --set-v4l2 "'rkisp1-isp-subdev':0[crop:(0,0)/${WIDTH}x${HEIGHT}]"
media-ctl -d /dev/media0 --set-v4l2 "'rkisp1-isp-subdev':2[fmt:YUYV8_2X8/${WIDTH}x${HEIGHT}]" #source
media-ctl -d /dev/media0 --set-v4l2 "'rkisp1-isp-subdev':2[fmt:YUYV8_2X8/${WIDTH}x${HEIGHT}]" --set-v4l2 "'rkisp1-isp-subdev':2[crop:(0,0)/${WIDTH}x${HEIGHT}]"
v4l2-ctl --set-ctrl=vertical_flip=1 --set-ctrl=horizontal_flip=1

gst-launch-1.0 rkcamsrc device=/dev/video1 io-mode=4 isp-mode=2A tuning-xml-path=./IMX378_1130_XGA_LSC.xml num-buffers=${CAPTURE_NUMBER} ! videoconvert ! video/x-raw,format=NV12,width=${WIDTH},height=${HEIGHT} ! filesink location=${FILENAME}.yuv
gst-launch-1.0 filesrc location=${FILENAME}.yuv ! videoparse format=23 width=${WIDTH} height=${HEIGHT} framerate=${FPS} ! queue ! mpph264enc ! queue ! h264parse ! mp4mux ! filesink location=${FILENAME}.mp4
