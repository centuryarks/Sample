#!/bin/sh
cd /home/pi/demo
DATETIME=`date "+%Y%m%d_%H%M%S"`
FILENAME=still_${1}x${2}_${DATETIME}
libcamera-still --width ${1} --height ${2} -o ${FILENAME}.jpg --tuning-file ${3} ${4}
VERSION_CODENAME=$(cat /etc/os-release | grep VERSION_CODENAME | awk -F'=' '{print $2}'>&1)
if [ "${VERSION_CODENAME}" = "bookworm" ]; then
    eom ${FILENAME}.jpg &
else
    gpicview ${FILENAME}.jpg &
fi
