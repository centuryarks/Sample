#!/bin/sh
ID=$1
WIDTH=$2
HEIGHT=$3
FPS=$4
if [ $WIDTH = 4056 ] && [ $HEIGHT = 3040 ] && [ $FPS = 30 ]; then
  gst-launch-1.0 nvcamerasrc sensor-id=${ID} fpsRange="30.0 30.0" wbmode=9 wbManualMode=3 aeLock=true ! 'video/x-raw(memory:NVMM), width=(int)4056, height=(int)3040, framerate=(fraction)30/1' ! nvvidconv flip-method=2 !'video/x-raw, width=(int)1024, height=(int)768, framerate=(fraction)30/1' ! queue ! xvimagesink
elif [ $WIDTH = 3840 ] && [ $HEIGHT = 2160 ] && [ $FPS = 30 ]; then
  gst-launch-1.0 nvcamerasrc sensor-id=${ID} fpsRange="30.0 30.0" wbmode=9 wbManualMode=3 aeLock=true ! 'video/x-raw(memory:NVMM), width=(int)3840, height=(int)2160, framerate=(fraction)30/1' ! nvvidconv flip-method=2 !'video/x-raw, width=(int)1024, height=(int)576, framerate=(fraction)30/1' ! queue ! xvimagesink
elif [ $WIDTH = 1920 ] && [ $HEIGHT = 1080 ] && [ $FPS = 30 ]; then
  gst-launch-1.0 nvcamerasrc sensor-id=${ID} fpsRange="30.0 30.0" wbmode=9 wbManualMode=3 aeLock=true ! 'video/x-raw(memory:NVMM), width=(int)1920, height=(int)1080, framerate=(fraction)30/1' ! nvvidconv flip-method=2 !'video/x-raw, width=(int)1024, height=(int)576, framerate=(fraction)30/1' ! queue ! xvimagesink
elif [ $WIDTH = 1920 ] && [ $HEIGHT = 1080 ] && [ $FPS = 60 ]; then
  gst-launch-1.0 nvcamerasrc sensor-id=${ID} fpsRange="60.0 60.0" wbmode=9 wbManualMode=3 aeLock=true ! 'video/x-raw(memory:NVMM), width=(int)1920, height=(int)1080, framerate=(fraction)60/1' ! nvvidconv flip-method=2 !'video/x-raw, width=(int)1024, height=(int)576, framerate=(fraction)60/1' ! queue ! xvimagesink
elif [ $WIDTH = 1920 ] && [ $HEIGHT = 1080 ] && [ $FPS = 120 ]; then
  gst-launch-1.0 nvcamerasrc sensor-id=${ID} fpsRange="120.0 120.0" wbmode=9 wbManualMode=3 aeLock=true ! 'video/x-raw(memory:NVMM), width=(int)1920, height=(int)1080, framerate=(fraction)120/1' ! nvvidconv flip-method=2 !'video/x-raw, width=(int)1024, height=(int)576, framerate=(fraction)120/1' ! queue ! xvimagesink
elif [ $WIDTH = 640 ] && [ $HEIGHT = 480 ] && [ $FPS = 240 ]; then
  gst-launch-1.0 nvcamerasrc sensor-id=${ID} fpsRange="240.0 240.0" wbmode=9 wbManualMode=3 aeLock=true ! 'video/x-raw(memory:NVMM), width=(int)640, height=(int)480, framerate=(fraction)240/1' ! nvvidconv flip-method=2 !'video/x-raw, width=(int)640, height=(int)480, framerate=(fraction)240/1' ! queue ! xvimagesink
else
  echo Not Supported Mode!
fi
