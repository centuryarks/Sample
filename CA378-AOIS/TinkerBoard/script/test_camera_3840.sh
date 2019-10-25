#!/bin/sh

export DISPLAY=:0.0 
media-ctl --set-v4l2 '"imx378 2-001a":0[fmt:SRGGB10_1X10/3840x2160]'
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SRGGB10_1X10/3840x2160]' #sink
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SRGGB10_1X10/3840x2160]' --set-v4l2 '"rkisp1-isp-subdev":0[crop:(0,0)/3840x2160]'
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:YUYV8_2X8/3840x2160]' #source
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:YUYV8_2X8/3840x2160]' --set-v4l2 '"rkisp1-isp-subdev":2[crop:(0,0)/3840x2160]'
gst-launch-1.0 rkcamsrc device=/dev/video1 io-mode=4 isp-mode=0A tuning-xml-path=./IMX378_1022.xml ! videoconvert ! video/x-raw,format=NV12,width=1280,height=720  ! rkximagesink
