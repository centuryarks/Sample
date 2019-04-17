#!/bin/sh
./ramdisk.sh

FILENAME=/mnt/ram/test
SENSOR_NUMBER=6
WIDTH=640
HEIGHT=480
FPS=200
RECORD_NUMBER=1000

ID=0
while :
do
	if [ ${ID} = ${SENSOR_NUMBER} ]; then
		break;
	fi
	./yuv_capture.sh ${FILENAME} ${ID} ${WIDTH} ${HEIGHT} ${FPS} ${RECORD_NUMBER} &
	ID=`expr ${ID} + 1`
done
