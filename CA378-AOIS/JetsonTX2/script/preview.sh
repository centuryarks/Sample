#!/bin/sh
ID=$1
WIDTH=$2
HEIGHT=$3
FPS=$4

# Display for 4:3
DISP43_W=1400
DISP43_H=`expr ${DISP43_W} \* 3 / 4`

# Display for 16:9
MONITOR_W=3840
SPLIT_WINDOW=6
DISP169_W=`expr \( ${MONITOR_W} - 64 \) / ${SPLIT_WINDOW} / 4 \* 4`
DISP169_H=`expr ${DISP169_W} \* 9 / 16`

if [ ${WIDTH} = 4032 ] && [ ${HEIGHT} = 3040 ]; then
  DISP_W=${DISP43_W}
  DISP_H=${DISP43_H}
elif [ ${WIDTH} = 3840 ] && [ ${HEIGHT} = 2160 ]; then
  DISP_W=${DISP169_W}
  DISP_H=${DISP169_H}
elif [ ${WIDTH} = 1920 ] && [ ${HEIGHT} = 1080 ]; then
  DISP_W=${DISP169_W}
  DISP_H=${DISP169_H}
elif [ ${WIDTH} = 640 ] && [ ${HEIGHT} = 480 ]; then
  DISP_W=${WIDTH}
  DISP_H=${HEIGHT}
else
  echo Not Supported Mode!
fi

gst-launch-1.0 nvarguscamerasrc sensor-id=${ID} maxperf=true wbmode=9 awblock=false aelock=true \
 ! "video/x-raw(memory:NVMM), width=(int)${WIDTH}, height=(int)${HEIGHT}, framerate=(fraction)${FPS}/1" ! nvvidconv flip-method=2 \
 ! "video/x-raw, width=(int)${DISP_W}, height=(int)${DISP_H}, framerate=(fraction)${FPS}/1" ! queue ! xvimagesink
