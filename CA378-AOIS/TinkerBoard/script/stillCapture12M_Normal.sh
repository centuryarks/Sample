#!/bin/sh
cd /home/linaro/demo
#W=1024
#H=768
#media-ctl -d /dev/media0 --set-v4l2 '"imx378 2-001a":0[fmt:SBGGR10/1024x768]'
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10/1024x768]' #sink
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10/1024x768]' --set-v4l2 '"rkisp1-isp-subdev":0[crop:(0,0)/1024x768]'
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:SBGGR10/1024x768]' #source
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:SBGGR10/1024x768]' --set-v4l2 '"rkisp1-isp-subdev":2[crop:(0,0)/1024x768]'
#W=1920
#H=1080
#media-ctl -d /dev/media0 --set-v4l2 '"imx378 2-001a":0[fmt:SBGGR10/1920x1080]'
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10/1920x1080]' #sink
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10/1920x1080]' --set-v4l2 '"rkisp1-isp-subdev":0[crop:(0,0)/1920x1080]'
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:SBGGR10/1920x1080]' #source
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:SBGGR10/1920x1080]' --set-v4l2 '"rkisp1-isp-subdev":2[crop:(0,0)/1920x1080]'
#W=3840
#H=2160
#media-ctl -d /dev/media0 --set-v4l2 '"imx378 2-001a":0[fmt:SBGGR10/3840x2160]'
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10/3840x2160]' #sink
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10/3840x2160]' --set-v4l2 '"rkisp1-isp-subdev":0[crop:(0,0)/3840x2160]'
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:SBGGR10/3840x2160]' #source
#media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:SBGGR10/3840x2160]' --set-v4l2 '"rkisp1-isp-subdev":2[crop:(0,0)/3840x2160]'
W=4032
H=3040
media-ctl -d /dev/media0 --set-v4l2 '"imx378 2-001a":0[fmt:SBGGR10_1X10/4032x3040]'
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10_1X10/4032x3040]' #sink
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":0[fmt:SBGGR10_1X10/4032x3040]' --set-v4l2 '"rkisp1-isp-subdev":0[crop:(0,0)/4032x3040]'
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:SBGGR10_1X10/4032x3040]' #source
media-ctl -d /dev/media0 --set-v4l2 '"rkisp1-isp-subdev":2[fmt:SBGGR10_1X10/4032x3040]' --set-v4l2 '"rkisp1-isp-subdev":2[crop:(0,0)/4032x3040]'

filename=still_${W}x${H}_normal
v4l2-ctl -d /dev/video1 --set-ctrl exposure=${2} --set-ctrl gain=${3} --set-selection=target=crop,top=0,left=0,width=${W},height=${H} --set-fmt-video=width=${W},height=${H},pixelformat=BG10 --stream-mmap=8 --stream-to=$filename.raw --stream-count=1
./bin/raw2dng -i $filename.raw -o $filename.dng -w ${W} -h ${H} -bit 10 -gain 1.0 1.0 1.0
ufraw $filename.dng
