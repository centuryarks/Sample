#!/bin/sh
cd /home/pi/demo
libcamera-hello -t 0 --viewfinder-width ${1} --viewfinder-height ${2} --framerate ${3} --tuning-file ${4} ${5}
