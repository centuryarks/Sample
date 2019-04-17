#!/bin/sh
cd /home/nvidia/demo

WIDTH=1920
HEIGHT=1080
FPS=60
SLEEP_TIME=7

ID=0
while :
do
  if [ ${ID} = 5 ]; then
    sudo ./bin/DemoGUI -id ${ID} -w ${WIDTH} -h ${HEIGHT} -fps ${FPS}
    break
  else
    sudo ./bin/DemoGUI -id ${ID} -w ${WIDTH} -h ${HEIGHT} -fps ${FPS} &
  fi
  sleep ${SLEEP_TIME}
  ID=`expr ${ID} + 1`
done
