#!/bin/sh

export DISPLAY=:0.0 
media-ctl --set-v4l2 '"imx378 2-001a":0[fmt:SBGGR10_1X10/1920x1080]'
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10_1X10/1920x1080]' #sink
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10_1X10/1920x1080]' --set-v4l2 '"rkisp1-isp-subdev":0[crop:(0,0)/1920x1080]'
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:YUYV8_2X8/1920x1080]' #source
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:YUYV8_2X8/1920x1080]' --set-v4l2 '"rkisp1-isp-subdev":2[crop:(0,0)/1920x1080]'
v4l2-ctl --set-ctrl=vertical_flip=1 --set-ctrl=horizontal_flip=1
gst-launch-1.0 rkcamsrc device=/dev/video1 io-mode=4 isp-mode=2A tuning-xml-path=./script/IMX378_1130_XGA_LSC.xml ! videoconvert ! video/x-raw,format=NV12,width=1280,height=720 ! rkximagesink
