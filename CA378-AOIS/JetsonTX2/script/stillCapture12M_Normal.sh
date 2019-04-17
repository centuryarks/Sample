#!/bin/sh
#sudo gst-launch-1.0 nvcamerasrc sensor-id=$1 num-buffers=1 ! 'video/x-raw(memory:NVMM),width=4056,height=3040,format=I420,framerate=30/1' ! nvvidconv flip-method=2 ! nvjpegenc ! filesink location=IMX378_4056x3040.jpg -e
#geeqie IMX378_4056x3040.jpg
W=4056
H=3040
FRAME_LENGTH=3200
ID=$1
EXPOSURE=$2
GAIN=$3
filename=still_${W}x${H}_normal
v4l2-ctl -d /dev/video${ID} --set-fmt-video=width=${W},height=${H},pixelformat=RG10 \
 --set-ctrl frame_length=${FRAME_LENGTH} \
 --set-ctrl coarse_time=${EXPOSURE} \
 --set-ctrl gain=${GAIN} \
 --set-ctrl bypass_mode=0 \
 --stream-mmap --stream-skip=1 --stream-count=1 --stream-to=$filename.raw --verbose
./tool/raw2dng -i $filename.raw -o $filename.dng -w 4096 -h 3040 -bit 10 -flip 1 -gain 1.0 1.0 1.0
ufraw $filename.dng
