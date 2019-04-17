#!/bin/sh
./ramdisk.sh

FILENAME=/mnt/ram/test
SENSOR_ID_1ST=0
SENSOR_ID_2ND=1
WIDTH=640
HEIGHT=480
FPS=240
RECORD_NUMBER=1000

./raw_capture.sh ${FILENAME} ${SENSOR_ID_1ST} ${WIDTH} ${HEIGHT} ${FPS} ${RECORD_NUMBER} &
./raw_capture.sh ${FILENAME} ${SENSOR_ID_2ND} ${WIDTH} ${HEIGHT} ${FPS} ${RECORD_NUMBER} &