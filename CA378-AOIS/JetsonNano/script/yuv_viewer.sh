#!/bin/bash

FILENAME=$1
WIDTH=$2
HEIGHT=$3
FPS=$4

gst-launch-1.0 filesrc location=${FILENAME} ! videoparse width=${WIDTH} height=${HEIGHT} framerate=${FPS}/1 format=23 ! videoconvert ! xvimagesink
