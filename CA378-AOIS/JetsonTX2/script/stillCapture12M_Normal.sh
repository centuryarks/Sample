#!/bin/sh
sudo gst-launch-1.0 nvcamerasrc sensor-id=$1 num-buffers=1 ! 'video/x-raw(memory:NVMM),width=4056,height=3040,format=I420,framerate=30/1' ! nvvidconv flip-method=2 ! nvjpegenc ! filesink location=IMX378_4056x3040.jpg -e
geeqie IMX378_4056x3040.jpg
