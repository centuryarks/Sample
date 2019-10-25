#!/bin/sh

rm /mnt/ram/VGA.yuv
rm /mnt/ram/VGA.mp4

export DISPLAY=:0.0 
media-ctl --set-v4l2 '"imx378 2-001a":0[fmt:SRGGB10_1X10/640x480]'

v4l2-ctl --set-ctrl gain=5120 --set-ctrl exposure=400

gst-launch-1.0 rkcamsrc device=/dev/video1 io-mode=4 isp-mode=0A tuning-xml-path=./IMX378_0424_at_RK_awb num-buffers=210 ! videoconvert ! video/x-raw,format=NV12,width=640,height=480 ! filesink location=/mnt/ram/VGA.yuv
gst-launch-1.0 filesrc location=/mnt/ram/VGA.yuv ! videoparse format=23 width=640 height=480 framerate=10 ! queue ! mpph264enc ! queue ! h264parse ! mp4mux ! filesink location=/mnt/ram/VGA.mp4
