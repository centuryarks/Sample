#!/bin/sh

export DISPLAY=:0.0 
media-ctl --set-v4l2 '"imx378 2-001a":0[fmt:SRGGB10_1X10/4032x3040]'
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SRGGB10_1X10/4032x3040]' #sink
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SRGGB10_1X10/4032x3040]' --set-v4l2 '"rkisp1-isp-subdev":0[crop:(0,0)/4032x3040]'
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:YUYV8_2X8/4032x3040]' #source
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:YUYV8_2X8/4032x3040]' --set-v4l2 '"rkisp1-isp-subdev":2[crop:(0,0)/4032x3040]'
gst-launch-1.0 rkcamsrc device=/dev/video1 io-mode=4 isp-mode=2A tuning-xml-path=./IMX378_1022.xml ! videoconvert ! video/x-raw,format=NV12,width=1280,height=960  ! rkximagesink
